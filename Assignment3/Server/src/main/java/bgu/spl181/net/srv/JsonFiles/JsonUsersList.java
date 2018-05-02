package bgu.spl181.net.srv.JsonFiles;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;
import java.util.List;

public class JsonUsersList {

    @SerializedName("users")
    @Expose
    private List<JsonUser> users;

    public List<JsonUser> getUsers() {
        return users;
    }

    public void setUsers(List<JsonUser> users) {
        this.users = users;
    }

    public boolean addUser(JsonUser jsonUser){
        if(!users.contains(jsonUser)){
            users.add(jsonUser);
            return true;
        }
        return false;
    }
}
