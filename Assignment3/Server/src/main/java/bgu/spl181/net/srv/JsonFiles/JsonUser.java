package bgu.spl181.net.srv.JsonFiles;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.HashMap;
import java.util.LinkedList;
import java.util.List;

public class JsonUser {


    public JsonUser (String userUsername, String userPassword, String userCountry){
        this.userUsername = userUsername;
        this.userPassword=userPassword;
        this.userCountry=userCountry;
        this.userBalance = 0;
        this.userType = "normal";
        this.userMovies = new LinkedList<>();
    }

    @SerializedName("username")
    @Expose
    private String userUsername;

    @SerializedName("type")
    @Expose
    private String userType;

    @SerializedName("password")
    @Expose
    private String userPassword;

    @SerializedName("country")
    @Expose
    private String userCountry;

    @SerializedName("movies")
    @Expose
    private List<JsonMovie> userMovies;

    @SerializedName("balance")
    @Expose
    private Integer userBalance;


    // setters and getters


    public String getUserUsername() {
        return userUsername;
    }

    public void setUserUsername(String userUsername) {
        this.userUsername = userUsername;
    }

    public String getUserType() {
        return userType;
    }

    public void setUserType(String userType) {
        this.userType = userType;
    }

    public String getUserPassword() {
        return userPassword;
    }

    public String getUserCountry() {
        return userCountry;
    }

    public void setUserCountry(String userCountry) {
        this.userCountry = userCountry;
    }

    public List<JsonMovie> getUserMovies() {
        return userMovies;
    }

    public Integer getUserBalance() {
        return userBalance;
    }

    public void setUserBalance(Integer userBalance) {
        this.userBalance = userBalance;
    }

    public void removeMovieByName(String movieName){

        for (JsonMovie jsonMovie : userMovies) {
            if (jsonMovie.getMovieName().equals(movieName)) {
                userMovies.remove(jsonMovie);
                return;
            }
        }
    }
}
