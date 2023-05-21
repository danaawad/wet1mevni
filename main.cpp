#include <iostream>
#include "AvlDana.h"
#include "StreamingDBa1.h"

int main() {
    std::cout << "push it push it like a pony!" << std::endl;
    streaming_database database;
    StatusType status1 = database.add_user(1,true);
    cout << static_cast<int>(status1) << endl;
    StatusType status2 = database.add_user(2,true);
    cout << static_cast<int>(status2) << "         ";
    database.add_user(3,true);
    database.add_user(1,false);

    for(int i=0; i<3; i++) {
        User *user = database.usersTree.root->obj;
        int id = (user->getUserId());
        bool vip = user->getIsVip();
        database.usersTree.remove(user);
        std::cout << "push it push it like a pony!" << std::endl;
        cout << id << " " << vip << endl;
    }

    printf("hi dear");
//    AvlTree<int> tree;
//    AvlTree<Handler<Movie>> hand;
//    Handler<Movie> m1(1);
//    Handler<Movie> m2(2);
//    Handler<Movie> m3(3);
//
//
//    hand.insert(&m1);
//    hand.insert(&m2);
//    hand.insert(&m3);
////    hand.findThroughKey(1);
////    hand.remove(&m1);
//
//    tree.inorderPrint();

    database.~streaming_database();
    return 0;
}

