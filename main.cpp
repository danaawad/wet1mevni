#include <iostream>
#include "AvlDana.h"
#include "StreamingDBa1.h"

int main() {
    streaming_database database;
//    StatusType status1 = database.add_user(1,true);
//    cout << static_cast<int>(status1) << endl;
//    StatusType status2 = database.add_user(2,true);
//    database.printUsersTree();
//    cout << static_cast<int>(status2) << "         ";
    database.add_user(1,true);
    database.add_user(2,true);
    database.add_user(3,false);
    database.add_user(4,false);
    database.add_user(5,false);


//    database.remove_user(1);

    database.add_group(10);
//    database.add_group(20);
//    database.remove_group(20);
//    StatusType movie1 = database.add_movie(1, Genre::COMEDY, 0, 1);
//    StatusType movie2 = database.add_movie(2, Genre::COMEDY, 0, 1);
    StatusType a = database.add_movie(1, Genre::COMEDY, 3, true);
    StatusType b = database.add_movie(2, Genre::COMEDY, 0, true);
    StatusType c = database.add_movie(3, Genre::COMEDY, 0, true);
    StatusType d = database.add_movie(4, Genre::ACTION, 0, true);
//    StatusType e = database.add_movie(5, Genre::COMEDY, 0, true);
    database.add_user_to_group(2,10);
    database.add_user_to_group(3,10);
//    StatusType watch1 = database.user_watch(1, 1);
    database.add_user_to_group(4,10);
    database.group_watch(10, 1);
    database.user_watch(2, 2);
    database.user_watch(2, 2);
    database.user_watch(2, 2);
    database.user_watch(2, 2);
    database.user_watch(2, 2);
    database.printGroupTree();
//    database.remove_user(2);
    database.rate_movie(1, 1, 10);
    database.rate_movie(2, 2, 70);
    database.rate_movie(1, 3, 50);
    database.rate_movie(2, 4, 70);
    database.rate_movie(2, 5, 70);
    database.rate_movie(2, 4, 100);
    int output[4]{0,0,0,0};
    database.get_all_movies(Genre::NONE, output);
    for(int i = 0; i < 4; i++)
    {
        cout << output[i] << "  ";
    }
    cout << endl;
//    output_t<int> status = database.get_num_views(2, Genre::ACTION);
//    cout << "status =" << static_cast<int>(status.ans()) << endl;
//    output_t<int> status1 = database.get_all_movies_count(Genre::COMEDY);
//    cout << "status1 =" << static_cast<int>(status1.ans()) << endl;
//    output_t<int> status2 = database.get_group_recommendation(10);
//    cout << "status2 =" << static_cast<int>(status2.ans()) << endl;
//    cout << static_cast<int>(watch1) << endl;
//    database.add_user_to_group(4,10);


//    for(int i=0; i<3; i++) {
//        User *user = database.usersTree.root->obj;
//        int id = (user->getUserId());
//        bool vip = user->getIsVip();
//        database.usersTree.remove(user);
//        std::cout << "push it push it like a pony!" << std::endl;
//        cout << id << " " << vip << endl;
//    }

////    StatusType movie3 = database.remove_movie(8);
//    StatusType watch1 = database.user_watch(1, 1);

    database.printMovieTree();
//    database.printGroupTree();
//    database.printHandlerTree();
//    database.printUsersTree();
////    cout << static_cast<int>(movie3) << endl;
////    printf("%d hi ", database.getMoviesInDatabase());
//    cout << "status =" << static_cast<int>(a) << endl;
//    cout << "status =" << static_cast<int>(b) << endl;
//    cout << "status =" << static_cast<int>(c) << endl;
//    cout << "status =" << static_cast<int>(d) << endl;
//    cout << "status =" << static_cast<int>(e) << endl;

//    AvlTree<Handler<Movie>> hand;
//    Handler<Movie> m1(1);
//    Handler<Movie> m2(2);
//    Handler<Movie> m3(3);
//
//
//    hand.insert(&m1);
//    hand.insert(&m2);
//    hand.insert(&m3);
//    hand.inorderPrint();
//    hand.findThroughKey(1);
//    hand.remove(&m1);
//
//    tree.inorderPrint();

//    database.~streaming_database();
//    AvlTree<Movie> tree;
//    Movie movie1(1, Genre::COMEDY, 3, 1, 10);
//    Movie movie2(2, Genre::COMEDY, 0, 1, 50);
//    Movie movie3(3, Genre::COMEDY, 0, 1, 70);
//    Movie movie4(4, Genre::COMEDY, 0, 1, 50);
//    Movie movie5(5, Genre::COMEDY, 0, 1, 100);
//
//    tree.insert(&movie2, 0);
//    tree.insert(&movie1, 0);
//    tree.insert(&movie3, 0);
//    tree.insert(&movie4, 0);
//
//
//
////    tree.remove(&movie1);
//    tree.remove(&movie2);
//    movie2.setRating(90);
//    tree.insert(&movie2, 0);
//    tree.insert(&movie5, 0);
//    cout << "the root is " << *((tree.root)->obj) << '\n';
//    tree.inorderPrint();

    return 0;
}

