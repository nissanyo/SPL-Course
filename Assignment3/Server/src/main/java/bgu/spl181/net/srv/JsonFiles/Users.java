package bgu.spl181.net.srv.JsonFiles;
import com.google.gson.Gson;
import com.google.gson.GsonBuilder;
import java.io.*;
import java.util.List;
import java.util.concurrent.locks.Lock;
import java.util.concurrent.locks.ReadWriteLock;
import java.util.concurrent.locks.ReentrantReadWriteLock;



public class  Users {

    // In every method we approached the database files, we used ReadLock.
    // If the approach used to change data in the files - we used WriteLock instead.


    private static final String databaseInput = "Database/Users.json";
    private ReadWriteLock readWriteLock = new ReentrantReadWriteLock();
    private final Lock readLock = readWriteLock.readLock();
    private final Lock writeLock = readWriteLock.writeLock();

    private JsonUsersList jsonUsersList;

    public Users() throws FileNotFoundException {
        Gson gson = new GsonBuilder().setPrettyPrinting().create();
        Reader reader = new FileReader(databaseInput);
        this.jsonUsersList = gson.fromJson(reader, JsonUsersList.class);
    }


    public JsonUsersList getJsonUsersList() throws IOException {
        readLock.lock();
        JsonUsersList users;
        try {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            Reader reader = new FileReader(databaseInput);
            users = gson.fromJson(reader, JsonUsersList.class);
        } finally {
            readLock.unlock();
        }
        return users;
    }

    public void setUsers(JsonUsersList jsonUsersList) throws IOException {
        writeLock.lock();
        try {
            Gson gson = new GsonBuilder().setPrettyPrinting().create();
            Writer writer = new FileWriter(databaseInput);
            gson.toJson(jsonUsersList, writer);
            writer.close();
        } finally {
            writeLock.unlock();
        }
    }

    public void addUser(JsonUser user) throws IOException {
        writeLock.lock();
        try {
            JsonUsersList users = getJsonUsersList();
            if (users.addUser(user)) {
                setUsers(users);
            }
        } finally {
            writeLock.unlock();
        }
    }

    public Integer setUserBalance(String userName, Integer balanceToAdd) throws IOException {
        writeLock.lock();
        Integer newBalance;
        try {
            JsonUsersList jsonUsersList = getJsonUsersList();
            JsonUser output = getJsonUser(jsonUsersList, userName);

            newBalance = balanceToAdd + output.getUserBalance();
            output.setUserBalance(newBalance);
            setUsers(jsonUsersList);

        } finally {
            writeLock.unlock();
        }
        return newBalance;
    }

    public JsonUser getJsonUser(JsonUsersList jsonUsersList, String userName) {
        readLock.lock();
        JsonUser output = null;
        for (JsonUser jsonUser : jsonUsersList.getUsers()) {
            if (jsonUser.getUserUsername().equals(userName)) {
                output = jsonUser;
            }
        }
        readLock.unlock();
        return output;
    }

}
