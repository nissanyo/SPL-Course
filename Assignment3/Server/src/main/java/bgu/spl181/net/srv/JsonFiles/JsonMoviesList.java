package bgu.spl181.net.srv.JsonFiles;
import com.google.gson.annotations.Expose;
import com.google.gson.annotations.SerializedName;

import java.util.List;

public class JsonMoviesList {

    @SerializedName("movies")
    @Expose
    private List<JsonMovie> movies;

    public List<JsonMovie> getMovies() {
        return movies;
    }

}
