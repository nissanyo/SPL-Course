package bgu.spl181.net.srv;

import bgu.spl181.net.srv.JsonFiles.*;

import java.io.IOException;
import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;
import java.util.concurrent.ConcurrentHashMap;

public class ClientsInfo {

//     In every method we usded synchronize,it was to get the right current data.

    private HashMap<String, String> regClients;
    private HashMap<Integer, String> loggedClients;
    private Integer movieID;

    private Users users;
    private Movies movies;

    public ClientsInfo(){
        this.regClients = new HashMap<>();
        this.loggedClients = new HashMap<>();
        try {
            this.users = new Users();
        } catch (IOException e){
            e.printStackTrace();
        }
        this.movies = new Movies();
        try {
            this.movieID=getHighestId(movies.getJsonMoviesList());
            initDatabase(getUsers().getJsonUsersList());
        } catch (IOException e) {
            e.printStackTrace();
        }
    }

    private void initDatabase (JsonUsersList listOfUsers){
        for (JsonUser jsonUser: listOfUsers.getUsers())
            regClients.put(jsonUser.getUserUsername(), jsonUser.getUserPassword());
    }

    private Integer getHighestId(JsonMoviesList jsonMoviesList){
        Integer id=1;

        for (JsonMovie jsonMovie:jsonMoviesList.getMovies()) {
            if(id<jsonMovie.getMovieId())
                id=jsonMovie.getMovieId()+1;
        }
        return id;
    }


    public synchronized HashMap<String, String> getRegClients() {
        return regClients;
    }

    public synchronized HashMap<Integer, String> getLoggedClients() {
        return loggedClients;
    }

    public synchronized boolean addRegClient(String username, String password){

        if(!regClients.containsKey(username)) {
            regClients.put(username,password);
            return true;
        }
        else{
            return false;
        }
    }

    public synchronized boolean addLoggedClient(Integer connectionId, String username){

        if(!loggedClients.containsKey(connectionId)) {
            loggedClients.put(connectionId,username);
            return true;
        }
        else{
            return false;
        }
    }


    public synchronized void removeLoggedClient(Integer connectionId, String username){

        if(loggedClients.containsValue(username)) {
            loggedClients.remove(connectionId);
        }
    }

    //return true if username match password
    public synchronized boolean checkMatchPw (String username, String password){
        if(regClients.containsKey(username) && regClients.get(username).equals(password)){
            return true;
        }
        return false;
    }

    public synchronized Users getUsers() {
        return users;
    }

    public  synchronized Movies getMovies() {
        return movies;
    }

    public synchronized boolean alreadyLogged (String username){
        return loggedClients.containsValue(username);
    }

    public synchronized boolean alreadyRegistered (String username){
        return regClients.containsKey(username);
    }

    public Integer getMovieID() {
        return movieID;
    }

    public void setMovieID(Integer movieID) {
        this.movieID = movieID;
    }
}
