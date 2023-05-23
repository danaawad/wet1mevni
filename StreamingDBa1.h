//
// 234218 Data Structures 1.
// Semester: 2023B (spring).
// Wet Exercise #1.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef STREAMINGDBA1_H_
#define STREAMINGDBA1_H_

#include "wet1util.h"
#include "AvlDana.h"
#include "Users.h"
#include "Movie.h"
#include "MOVIEHandler.h"

class streaming_database {
private:

    void modifyUsers(Node<User>* node, Genre genre)
    {
            if (node == nullptr)
            {
                return;
            }
        modifyUsers(node->left, genre);
        node->obj->incGenreCount(genre);
        modifyUsers(node->right, genre);

    }

    void getUsersViews(Node<User>* node, int genre, int countArray[4])
    {
        if (node == nullptr)
        {
            return;
        }
        getUsersViews(node->left, genre, countArray);
        countArray[genre] += node->obj->getGenreCount(static_cast<Genre>(genre));
        getUsersViews(node->right, genre, countArray);
    }

    void setOutput(Node<Movie>* node, int *const output, int& i)
    {
        if (node == nullptr)
        {
            return;
        }
        setOutput(node->right, output, i);
        output[i++] = node->obj->getMovieId();
        setOutput(node->left, output, i);
    }

protected:
    AvlTree<Movie> dramaTree;
    AvlTree<Movie> actionTree;
    AvlTree<Movie> fantasyTree;
    AvlTree<Group> groupsTree;
    AvlTree<Movie> comedyTree;
    AvlTree<Movie> noneTree;
    AvlTree<Handler<Movie>> allMovies;

    AvlTree<Movie>* genrePtrs[5]{};
    int groupsInDatabase;
    int usersInDatabase;
    int moviesInDatabase[4]{};
public:

    const int *getMoviesInDatabase() const;
    void printMovieTree();
    void printUsersTree();
    void printHandlerTree();
    void printGroupTree();

	// <DO-NOT-MODIFY> {
	
	streaming_database();

	virtual ~streaming_database() = default;
	
	StatusType add_movie(int movieId, Genre genre, int views, bool vipOnly);
	
	StatusType remove_movie(int movieId);
	
	StatusType add_user(int userId, bool isVip);
	
	StatusType remove_user(int userId);
	
	StatusType add_group(int groupId);
	
	StatusType remove_group(int groupId);

	StatusType add_user_to_group(int userId, int groupId);
	
	StatusType user_watch(int userId, int movieId);

    StatusType group_watch(int groupId,int movieId);

    output_t<int> get_all_movies_count(Genre genre);

    StatusType get_all_movies(Genre genre, int *const output);
	
	output_t<int> get_num_views(int userId, Genre genre);

    StatusType rate_movie(int userId, int movieId, int rating);
	
	output_t<int> get_group_recommendation(int groupId);
	
	// } </DO-NOT-MODIFY>
    AvlTree<User> usersTree;
};



#endif // STREAMINGDBA1_H_
