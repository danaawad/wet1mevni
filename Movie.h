// Created by Dana Awad on 5/16/23.
//

#ifndef WET1_MOVIE_H
#define WET1_MOVIE_H
#include "StreamingDBa1.h"

class Movie{
private:
    int movieId;
    Genre genre;
    int views;
    bool vipOnly;
    int rating;
    int timesRated;

public:
//    explicit Movie(int id) : movieId(id), genre(Genre::COMEDY), views(0), vipOnly(false), rating(0) {}
    Movie(int movieId, Genre genre, int views, bool vipOnly, int rating =0, int timesRated = 0) : movieId(movieId), genre(genre),
                                                                              views(views), vipOnly(vipOnly),
                                                                              rating(rating), timesRated(timesRated) {}

    ~Movie() = default;
    friend bool operator<(const Movie& movie1, const Movie& movie2)
    {
        if(movie1.rating > movie2.rating) return false;
        if(movie1.rating < movie2.rating) return true;
        if(movie1.rating == movie2.rating)
        {
            if(movie1.views > movie2.views) return false;
            if(movie1.views < movie2.views) return true;
            if(movie1.views == movie2.views) return (movie1.movieId > movie2.movieId);
        }
        return false;
    }
    friend bool operator==(const Movie& movie1, const Movie& movie2)
    {
        return (movie1.movieId == movie2.movieId);
    }
    friend bool operator>(const Movie& movie1, const Movie& movie2)
    {
        if(movie1.rating < movie2.rating) return false;
        if(movie1.rating > movie2.rating) return true;
        if(movie1.rating == movie2.rating)
        {
            if(movie1.views < movie2.views) return false;
            if(movie1.views > movie2.views) return true;
            if(movie1.views == movie2.views) return (movie1.movieId < movie2.movieId);
        }
        return false;
    }

    friend bool operator!=(const Movie& movie1, const Movie& movie2)
    {
        if(movie1.movieId == movie2.movieId) return false;
        return true;
    }


    int getMovieId() const
    {
        return movieId;
    }

    Genre getGenre() const
    {
        return genre;
    }

    int getViews() const
    {
        return views;
    }

    bool isVipOnly() const
    {
        return vipOnly;
    }

    int getRating() const
    {
        return rating;
    }

    int getTimesRated() const
    {
        return timesRated;
    }

    void incTimesRated(int times)
    {
        Movie::timesRated += times;
    }

    void setRating(int rating)
    {
        Movie::rating = rating;
    }

    void setViews(int views) {
        Movie::views = views;
    }


    friend ostream& operator<<(ostream& os, const Movie& movie)
    {
        os << "Movie id is " << movie.movieId << " ";
        os << "num of views is " << movie.views << " ";
        os << "rating is " << movie.rating << " " << '\n';
        return os;
    }
};
#endif //WET1_MOVIE_H
