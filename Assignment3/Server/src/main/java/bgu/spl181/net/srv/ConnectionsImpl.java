package bgu.spl181.net.srv;

import bgu.spl181.net.api.bidi.Connections;
import bgu.spl181.net.srv.bidi.ConnectionHandler;
import java.util.HashMap;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;


public class ConnectionsImpl <T> implements Connections { // TODO - who holds this object?

    private ConcurrentHashMap<Integer, ConnectionHandler> connections;

    public ConnectionsImpl(){
        connections = new ConcurrentHashMap<>();
    }

    @Override
    public synchronized boolean send(int connectionId, Object msg) {
        if(connections.containsKey(connectionId)) {
            connections.get(connectionId).send(msg);
            return true;
        }
        else{
            return false;
        }

    }

    @Override
    public synchronized void broadcast(Object msg) {
        for (Map.Entry<Integer, ConnectionHandler> id : connections.entrySet()) {
            id.getValue().send(msg);
        }
    }

    @Override
    public synchronized void disconnect(int connectionId) {
        if(connections.containsKey(connectionId)) {
            connections.remove(connectionId);
        }

    }

    public synchronized ConcurrentHashMap<Integer, ConnectionHandler> getConnections() {
        return connections;
    }

    public synchronized void connect (Integer id, ConnectionHandler<T> connection){
        if(!connections.containsKey(id)){
            connections.put(id, connection);
        }
    }

    public ConnectionHandler getClient(Integer id){
        return connections.get(id);
    }
}
