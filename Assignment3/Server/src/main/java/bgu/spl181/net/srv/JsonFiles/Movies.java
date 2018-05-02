package bgu.spl181.net.srv.JsonFiles;

import com.google.gson.Gson;
import com.google.gson.GsonBuilder;

import java.io.*;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;

public class Movies {

    // In every method we approached the database files, we used ReadLock.
    // If the approach used to change data in the files - we used WriteLock instead.

    private static final String databaseInput = "Database/Movies.json";
    private ReadWriteLock readWriteLock = new ReentrantReadWriteLock();
    private final Lock readLock = readWriteLock.readLock();
    private final Lock writeLock = readWriteLock.writeLock();

    public JsonMoviesList getJsonMoviesList() throws IOException {
        readLock.lock();
        JsonMoviesList movies;
        try {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            Reader reader = new FileReader(databaseInput);
            movies = gson.fromJson(reader, JsonMoviesList.class);
        }finally {
            readLock.unlock();
        }
        return movies;
    }

    public void setMovies(JsonMoviesList jsonMoviesList) throws IOException {
        writeLock.lock();
        try {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            Writer writer = new FileWriter(databaseInput);
            gson.toJson(jsonMoviesList, writer);
            writer.close();
        } finally {
            writeLock.unlock();
        }
    }

    public String addMovie(Users users, String userName, JsonMovie movie) throws IOException {
        writeLock.lock();
        String msgToSend = null;
        try {
            JsonUsersList jsonUsersList = users.getJsonUsersList();
            JsonUser jsonUser = users.getJsonUser(jsonUsersList, userName);
            JsonMoviesList movies = getJsonMoviesList();

            if (jsonUser!=null && jsonUser.getUserType().equals("admin") && movie.getMoviePrice()>0 && movie.getTotalAmount()>0 && !listContainsMovieByName(movies.getMovies(), movie.getMovieName())) {
                movies.getMovies().add(movie);
                setMovies(movies);
                msgToSend ="BROADCAST movie " + '"' + movie.getMovieName() + '"' + " " + movie.getAvailableAmount() + " " + movie.getMoviePrice();
            }
        }finally {
            writeLock.unlock();
        }

        return msgToSend;
    }

    public String remmovieRequest (Users users, String userName, String movie) throws IOException {
        writeLock.lock();
        String msgToSend = null;
        try {
            JsonUsersList jsonUsersList = users.getJsonUsersList();
            JsonUser jsonUser = users.getJsonUser(jsonUsersList, userName);
            JsonMoviesList movies = getJsonMoviesList();
            JsonMovie jsonMovie = getJsonMovie(movies, movie);

            if (jsonUser!=null && jsonMovie!=null && jsonUser.getUserType().equals("admin") &&  movies.getMovies().contains(jsonMovie) && !userHasMovie(jsonUsersList,movie)) {
                movies.getMovies().remove(jsonMovie);
                setMovies(movies);
                msgToSend ="BROADCAST movie " + '"' + movie + '"' + " removed";
            }
        }finally {
            writeLock.unlock();
        }

        return msgToSend;
    }


public String infoCmd(){
        readLock.lock();
        String output="";
        try {
            JsonMoviesList jsonMoviesList = getJsonMoviesList();
            for (JsonMovie jsonMovie: jsonMoviesList.getMovies()) {
                output = output + '"' + jsonMovie.getMovieName() + '"' + " ";
            }
            if(output.length()!=0)
                output=output.substring(0, output.length()-1);
        } catch (IOException e) {
            e.printStackTrace();
        }
        readLock.unlock();
        return output;
    }

    public String changePriceRequest(Users users, String userName, String movieName, Integer price) throws IOException {
        writeLock.lock();
        String msgToSend = null;
        try {
            JsonUsersList jsonUsersList = users.getJsonUsersList();
            JsonUser jsonUser = users.getJsonUser(jsonUsersList, userName);
            JsonMoviesList movies = getJsonMoviesList();
            JsonMovie jsonMovie = getJsonMovie(movies, movieName);

            if (jsonMovie!=null && jsonUser.getUserType().equals("admin") && price>0) {
                jsonMovie.setMoviePrice(price);
                setMovies(movies);
                msgToSend ="BROADCAST movie " + '"' + movieName + '"' + " " + jsonMovie.getAvailableAmount() + " " + price;
            }
        }finally {
            writeLock.unlock();
        }
        return msgToSend;
    }

    private boolean userHasMovie(JsonUsersList jsonUsersList, String movieName){
        readLock.lock();
        for (JsonUser jsonUser:jsonUsersList.getUsers()) {
            for (JsonMovie movie : jsonUser.getUserMovies()) {
                if (movie.getMovieName().equals(movieName)) {
                    readLock.unlock();
                    return true;
                }
            }
        }
        readLock.unlock();
        return false;
    }

    private boolean listContainsMovieByName(List<JsonMovie> movies, String movie){
        readLock.lock();
        for (JsonMovie jsonMovie:movies) {
            if(jsonMovie.getMovieName().equals(movie)) {
                readLock.unlock();
                return true;
            }
        }
        readLock.unlock();
        return false;
    }


    public String rentRequest(Users users, String movieName, String userName){
        writeLock.lock();
        String msgToBroadcast=null;
        try {
            JsonMoviesList jsonMoviesList = getJsonMoviesList();
            JsonUsersList jsonUsersList = users.getJsonUsersList();
            JsonMovie jsonMovie = getJsonMovie(jsonMoviesList, movieName);
            JsonUser jsonUser = users.getJsonUser(jsonUsersList, userName);

            if (jsonMovie != null && jsonUser.getUserBalance() >= jsonMovie.getMoviePrice() && jsonMovie.getAvailableAmount() > 0 &&
                    !jsonMovie.getCountries().contains(jsonUser.getUserCountry()) &&
                    !listContainsMovieByName(jsonUser.getUserMovies(), movieName)) {
                jsonMovie.setAvailbleAmount(jsonMovie.getAvailableAmount() - 1);
                jsonUser.setUserBalance(jsonUser.getUserBalance() - jsonMovie.getMoviePrice());
                jsonUser.getUserMovies().add(new JsonMovie(jsonMovie.getMovieId(), movieName));
                setMovies(jsonMoviesList);
                users.setUsers(jsonUsersList);
                msgToBroadcast = "BROADCAST movie " + '"' + movieName + '"' + " " + jsonMovie.getAvailableAmount() + " " + jsonMovie.getMoviePrice();

            }
        }catch (IOException e) {
            e.printStackTrace();
        } finally {
            writeLock.unlock();
        }
        return msgToBroadcast;
    }


    public String returnRequest(Users users, String movieName, String userName){
        writeLock.lock();
        String msgToBroadcast = null;
        try {

            JsonMoviesList jsonMoviesList = getJsonMoviesList();
            JsonMovie jsonMovie= getJsonMovie(jsonMoviesList, movieName);
            JsonUsersList jsonUsersList = users.getJsonUsersList();
            JsonUser jsonUser = users.getJsonUser(jsonUsersList, userName);

            if(jsonMovie!=null && listContainsMovieByName(jsonUser.getUserMovies(),movieName)  ) {
                jsonMovie.setAvailbleAmount(jsonMovie.getAvailableAmount()+1);
                jsonUser.removeMovieByName(movieName);
                users.setUsers(jsonUsersList);
                setMovies(jsonMoviesList);
                msgToBroadcast = "BROADCAST movie " + '"' + movieName + '"' + " " + jsonMovie.getAvailableAmount() +" "+ jsonMovie.getMoviePrice();
            }
        } catch (IOException e) {
            e.printStackTrace();
        } finally {
            writeLock.unlock();
        }
        return msgToBroadcast;
    }


    public JsonMovie getJsonMovie(JsonMoviesList jsonMoviesList, String movieName){
        readLock.lock();
        JsonMovie output = null;
            for (JsonMovie jsonMovie: jsonMoviesList.getMovies()) {
                if(jsonMovie.getMovieName().equals(movieName)){
                    output = jsonMovie;
                    readLock.unlock();
                    return output;
                }
            }
            readLock.unlock();
        return output;
    }
}
