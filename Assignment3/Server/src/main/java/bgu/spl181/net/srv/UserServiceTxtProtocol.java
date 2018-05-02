package bgu.spl181.net.srv;

import bgu.spl181.net.api.bidi.BidiMessagingProtocol;
import bgu.spl181.net.api.bidi.Connections;

import java.util.ArrayList;
import java.util.concurrent.atomic.AtomicBoolean;

public class UserServiceTxtProtocol implements BidiMessagingProtocol {

    protected Integer connectionId;
    protected ConnectionsImpl connections;
    protected AtomicBoolean terminate = new AtomicBoolean(false);
    protected ClientsInfo clients;
    protected AtomicBoolean isLogged = new AtomicBoolean(false);

    public UserServiceTxtProtocol(ClientsInfo clients){
        this.clients = clients;
    }

    @Override
    public void start(int connectionId, Connections connections) {
        this.connectionId = connectionId;
        this.connections=(ConnectionsImpl)connections;
    }

    @Override
    public void process(Object message) {

        String cmdWord;
        String msg = (String) message;

        if(msg.equals("SIGNOUT"))
            cmdWord=msg;
        else
            cmdWord = (msg).substring(0, (msg.indexOf(' ')));

        switch (cmdWord){
            case "LOGIN" : {
                loginCmd(msg);
                break;
            }
            case "SIGNOUT" : {
                signoutCmd(msg);
                break;
            }
            default : {
                connections.send(connectionId, "ERROR unknown command");
            }
        }
    }

    /**
     * @return true if the connection should be terminated
     */
    @Override
    public boolean shouldTerminate() {
        if(terminate.get()){
            connections.disconnect(connectionId);

        }
        return terminate.get();
    }



   private void loginCmd(String msg){
       String[] parsedInput = msg.split(" ");
       if(parsedInput.length>2) {
           String username = parsedInput[1];
           String password = parsedInput[2];

           if (!isLogged.get() && !clients.alreadyLogged(username) && clients.checkMatchPw(username, password)) {
               isLogged.set(true);
               clients.addLoggedClient(connectionId,username);
               connections.send(connectionId, "ACK login succeeded");
           } else {
               connections.send(connectionId, "ERROR login failed");
           }
       }
        else{
           connections.send(connectionId, "ERROR login failed");
           }
   }


    private void signoutCmd(String msg){

        String username = clients.getLoggedClients().get(connectionId);

            if (username!=null && clients.alreadyLogged(username)) {
                isLogged.set(false);
                clients.removeLoggedClient(connectionId, username);
                connections.send(connectionId, "ACK signout succeeded");
                terminate.set(true);

            } else {
                connections.send(connectionId, "ERROR signout failed");
            }
        }
    }



