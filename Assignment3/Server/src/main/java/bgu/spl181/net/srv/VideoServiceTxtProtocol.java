package bgu.spl181.net.srv;

import bgu.spl181.net.api.bidi.Connections;
import bgu.spl181.net.srv.JsonFiles.JsonMovie;
import bgu.spl181.net.srv.JsonFiles.JsonUser;
import bgu.spl181.net.srv.bidi.ConnectionHandler;

import java.io.IOException;
import java.util.LinkedList;
import java.util.List;
import java.util.Map;

public class VideoServiceTxtProtocol extends UserServiceTxtProtocol {


    public VideoServiceTxtProtocol(ClientsInfo clients) {
        super(clients);
    }

    @Override
    public void start(int connectionId, Connections connections) {
        super.start(connectionId, connections);
    }


    @Override
    public void process(Object message) {

        String cmdWord;
        String msg = (String) message;

        if(msg.equals("SIGNOUT"))
            cmdWord=msg;
        else
            cmdWord = (msg).substring(0, (msg.indexOf(' ')));


        switch (cmdWord) {
            case "LOGIN": {
                super.process(message);
                break;
            }
            case "SIGNOUT": {
                super.process(message);
                break;
            }
            case "REGISTER": {
                registerCmd(msg);
                break;
            }
            case "REQUEST": {
                requestCmd(msg);
                break;
            }
            default: {
                connections.send(connectionId, "ERROR unknown command");
            }
        }
    }

    private void registerCmd(String msg) {

        String[] parsedInput = msg.split(" ");
        if (parsedInput.length > 3 && parsedInput[3].substring(0,8).equals("country=")) {
            String username = parsedInput[1];
            String password = parsedInput[2];
            String country = parsedInput[3].substring(9, parsedInput[3].length());
            for (int i=4; i<parsedInput.length; i++){
                country=country + " " + parsedInput[i];
            }
            country = country.substring(0, country.length()-1);

            if (!clients.alreadyLogged(username) && !clients.alreadyRegistered(username)) {
                clients.addRegClient(username, password);
                JsonUser newUser = new JsonUser(username, password, country);
                try {
                    clients.getUsers().addUser(newUser);
                } catch (IOException e) {
                    e.printStackTrace();
                }
                connections.send(connectionId, "ACK registration succeeded");
            } else {
                connections.send(connectionId, "ERROR registration failed");
            }
        } else {
            connections.send(connectionId, "ERROR registration failed");
        }
    }

    private void requestCmd(String msg){

        String[] parsedInput = msg.split(" ");
        String serviceName = parsedInput[1];
        String userName = clients.getLoggedClients().get(connectionId);
         switch (serviceName) {
             case "balance" : {
                 balanceRequest(parsedInput, userName);
                 break;
             }
             case "info" : {
                 infoRequest(parsedInput, userName);
                 break;
             }
             case "rent" : {
                 rentRequest(parsedInput, userName);
                 break;
             }
             case "return" : {
                 returnRequest(parsedInput, userName);
                 break;
             }
             case "addmovie" : {
                 addmovieRequest(msg, parsedInput, userName);
                 break;
             }
             case "remmovie" : {
                 remmovieRequest(parsedInput,userName);
                 break;
             }
             case "changeprice" : {
                 changePriceRequest(msg, parsedInput,userName);
                 break;
             }
             default : {
                 connections.send(connectionId, "ERROR unknown command"); 
             }

         }
    }

    //normal users requessts

    private void balanceRequest(String[] parsedInput, String userName){
        String balanceType = parsedInput[2];
        if(balanceType.equals("info")){
            JsonUser jsonUser = getJsonUser(userName);
            if(canMakeRequest(userName) && jsonUser != null){
                connections.send(connectionId, "ACK balance " + jsonUser.getUserBalance());
            }else{
                connections.send(connectionId, "ERROR request balance failed");
            }
        }
        else{
		Integer balanceToAdd= 0;
		if(parsedInput.length>3){
        	    balanceToAdd = Integer.valueOf(parsedInput[3]);
		}
            if(canMakeRequest(userName) && balanceToAdd>0){
                try {
                    Integer newBalance = clients.getUsers().setUserBalance(userName, balanceToAdd);
                    connections.send(connectionId, "ACK balance " + newBalance +  " added " + balanceToAdd);
                }catch (IOException e){
                    e.printStackTrace();
                }
            }else{
                connections.send(connectionId, "ERROR request balance failed");
            }
        }
    }

    private void infoRequest (String [] parsedInput, String userName){
        if (canMakeRequest(userName)){
            if(parsedInput.length>2){
                String movieName = "";
                for (int i=2; i<parsedInput.length; i++){
                    movieName=movieName+parsedInput[i] + " ";
                }
                movieName=movieName.substring(1, movieName.length()-2);
                JsonMovie jsonMovie = getJsonMovie(movieName);
                if(jsonMovie!=null) {
                    String output = "";
                    output = output + '"' + movieName + '"' + " " + jsonMovie.getAvailableAmount() + " " + jsonMovie.getMoviePrice() + " ";
                    for (String country:jsonMovie.getCountries()) {
                        output=output + '"' + country + '"' + " ";
                    }
                    if(output.length()!=0)
                        output=output.substring(0, output.length()-1);
                    connections.send(connectionId, "ACK info " + output);
                }
                else{
                    connections.send(connectionId, "ERROR request info failed");
                }
            }else{

                String msg = clients.getMovies().infoCmd();
                connections.send(connectionId, "ACK info " + msg);
            }
        }else{
            connections.send(connectionId, "ERROR request info failed");
        }
    }

    private void rentRequest(String [] parsedInput, String userName){

        String movieName = "";
        for (int i=2; i<parsedInput.length; i++){
            movieName=movieName+parsedInput[i] + " ";
        }
        movieName=movieName.substring(1, movieName.length()-2);
        String msgToSend = clients.getMovies().rentRequest(clients.getUsers(), movieName, userName);

        if(canMakeRequest(userName) && msgToSend!=null){
            connections.send(connectionId, "ACK rent "  + '"' + movieName + '"' + " success");
            sendBroadcast(msgToSend);
        }
        else{
            connections.send(connectionId, "ERROR request rent failed");
        }
    }

    private void returnRequest(String [] parsedInput, String userName){
        String movieName = "";
        for (int i=2; i<parsedInput.length; i++){
            movieName=movieName+parsedInput[i] + " ";
        }
        movieName=movieName.substring(1, movieName.length()-2);

        String msgToSend = clients.getMovies().returnRequest(clients.getUsers(), movieName, userName);

        if(canMakeRequest(userName) && msgToSend!=null){
            connections.send(connectionId, "ACK return "  + '"' + movieName + '"' + " success");
            sendBroadcast(msgToSend);
        }
        else {
            connections.send(connectionId, "ERROR request return failed");

        }
    }

    //admin user requests
    private void addmovieRequest(String msg, String [] parsedInput, String userName){

        String [] parsedByGersh = msg.split("\"");
        String movieName = parsedByGersh[1];

        Integer movieNumOfWords = movieName.split(" ").length;
        Integer numOfCopies = Integer.valueOf(parsedInput[movieNumOfWords + 2]);
        Integer price = Integer.valueOf(parsedInput[movieNumOfWords + 3]);
        List<String> bannedCountries = new LinkedList<>();

        for(int i=3; i<parsedByGersh.length; i=i+2){
            bannedCountries.add(parsedByGersh[i].substring(0, parsedByGersh[i].length()));

        }

        Integer movieId = clients.getMovieID();
        clients.setMovieID(movieId+1);

        JsonMovie jsonMovie = new JsonMovie(movieId, movieName, price, bannedCountries, numOfCopies);

        String msgToSend = null;
        try {
            msgToSend = clients.getMovies().addMovie(clients.getUsers(), userName, jsonMovie);
            if (canMakeRequest(userName) && msgToSend!=null){
                connections.send(connectionId, "ACK addmovie "  + '"' + movieName + '"' + " success");
                sendBroadcast(msgToSend);
            }
            else{
                connections.send(connectionId, "ERROR request addmovie failed");
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    private void remmovieRequest(String [] parsedInput, String userName){
        String movieName = parsedInput[2];
        movieName = movieName.substring(1,movieName.length());

        for (int i=3; i< parsedInput.length; i++)
            movieName=movieName+ " " + parsedInput[i];

        movieName = movieName.substring(0, movieName.length()-1);

        String msgToSend = null;
        try {
            msgToSend = clients.getMovies().remmovieRequest(clients.getUsers(), userName, movieName);
            if (canMakeRequest(userName) && msgToSend!=null){
                connections.send(connectionId, "ACK remmovie "  + '"' + movieName + '"' + " success");
                sendBroadcast(msgToSend);
            }
            else{
                connections.send(connectionId, "ERROR request remmovie failed");
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }

    private void changePriceRequest(String msg, String [] parsedInput, String userName){

        String [] parsedByGersh = msg.split("\"");
        String movieName = parsedByGersh[1];
        Integer movieNumOfWords = movieName.split(" ").length;
        Integer price = Integer.valueOf(parsedInput[movieNumOfWords + 2]);

        String msgToSend = null;
        try {
            msgToSend = clients.getMovies().changePriceRequest(clients.getUsers(), userName, movieName, price);
            if (canMakeRequest(userName) && msgToSend!=null){
                connections.send(connectionId, "ACK changeprice "  + '"' + movieName + '"' + " success");
                sendBroadcast(msgToSend);
            }
            else{
                connections.send(connectionId, "ERROR request changeprice failed");
            }
        }catch (IOException e){
            e.printStackTrace();
        }
    }


    // assist functions

    private synchronized void sendBroadcast(String msg){
        for (Map.Entry<Integer, String> loggedClientId : clients.getLoggedClients().entrySet()) {
            connections.send(loggedClientId.getKey(), msg);
        }
    }

    private synchronized JsonUser getJsonUser(String userName){
        JsonUser output = null;
        try {
            for (JsonUser jsonUser: clients.getUsers().getJsonUsersList().getUsers()) {
                if(jsonUser.getUserUsername().equals(userName)){
                    output = jsonUser;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return output;
    }

    private synchronized JsonMovie getJsonMovie(String movieName){
        JsonMovie output = null;
        try {
            for (JsonMovie jsonMovie: clients.getMovies().getJsonMoviesList().getMovies()) {
                if(jsonMovie.getMovieName().equals(movieName)){
                    output = jsonMovie;
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        return output;
    }

    private synchronized boolean canMakeRequest(String userName){
        return (clients.alreadyRegistered(userName) && clients.alreadyLogged(userName));
    }
}

