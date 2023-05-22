#include <iostream>
#include "AvlDana.h"
#include "StreamingDBa1.h"

int main() {
    streaming_database database;
    StatusType status1 = database.add_user(1,true);
//    cout << static_cast<int>(status1) << endl;
    StatusType status2 = database.add_user(1,true);
    database.printUsersTree();
//    cout << static_cast<int>(status2) << "         ";
//    database.add_user(3,true);
//    database.add_user(1,false);

//    for(int i=0; i<3; i++) {
//        User *user = database.usersTree.root->obj;
//        int id = (user->getUserId());
//        bool vip = user->getIsVip();
//        database.usersTree.remove(user);
//        std::cout << "push it push it like a pony!" << std::endl;
//        cout << id << " " << vip << endl;
//    }
//    StatusType movie1 = database.add_movie(1, Genre::COMEDY, 0, 1);
//    StatusType movie2 = database.add_movie(1, Genre::COMEDY, 0, 1);
//    Movie movie1(1, Genre::COMEDY, 3, 1, 10);
//    database.comedyTree.insert(&movie1, 1);
//    database.printMovieTree();
//    printf("%d hi ", database.getMoviesInDatabase());

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
//
//    tree.insert(&movie2, 0);
//    tree.insert(&movie1, 0);
//    tree.insert(&movie3, 0);
//    tree.insert(&movie4, 0);
//    cout << "the root is " << *((tree.root)->obj) << '\n';
//    tree.inorderPrint();

    return 0;
}

