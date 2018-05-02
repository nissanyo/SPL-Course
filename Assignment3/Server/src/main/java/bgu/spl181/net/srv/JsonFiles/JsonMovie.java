package bgu.spl181.net.srv.JsonFiles;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.List;

public class JsonMovie {


    public JsonMovie(Integer id, String movieName, Integer moviePrice, List<String> bannedCounries, Integer totalAmount){
        this.movieId = id;
        this.movieName=movieName;
        this.moviePrice=moviePrice;
        this.bannedCountries=bannedCounries;
        this.totalAmount=totalAmount;
        this.availableAmount=totalAmount;
    }

    public JsonMovie(Integer id, String movieName){
        this.movieId = id;
        this.movieName=movieName;
    }

    @SerializedName("id")
    @Expose
    private Integer movieId;

    @SerializedName("name")
    @Expose
    private String movieName;

    @SerializedName("price")
    @Expose
    private Integer moviePrice;

    @SerializedName("bannedCountries")
    @Expose
    private List<String> bannedCountries;

    @SerializedName("availableAmount")
    @Expose
    private Integer availableAmount;

    @SerializedName("totalAmount")
    @Expose
    private Integer totalAmount;


    // setters and getters

    public Integer getMovieId() {
        return movieId;
    }

    public void setMovieId(Integer movieId) {
        this.movieId = movieId;
    }

    public String getMovieName() {
        return movieName;
    }

    public void setMovieName(String movieName) {
        this.movieName = movieName;
    }

    public Integer getMoviePrice() {
        return moviePrice;
    }

    public void setMoviePrice(Integer moviePrice) {
        this.moviePrice = moviePrice;
    }

    public List<String> getCountries() {
        return bannedCountries;
    }

    public void setCountries(List<String> countries) {
        this.bannedCountries = countries;
    }

    public Integer getAvailableAmount() {
        return availableAmount;
    }

    public void setAvailbleAmount(Integer availbleAmount) {
        this.availableAmount = availbleAmount;
    }

    public Integer getTotalAmount() {
        return totalAmount;
    }

    public void setTotalAmount(Integer totalAmount) {
        this.totalAmount = totalAmount;
    }





}
