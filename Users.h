//
// Created by nicol on 16/05/2023.
//

#ifndef WET1_USERS_H
#define WET1_USERS_H
#include "AvlDana.h"
#include "StreamingDBa1.h"

class User{
private:
    int userId;
    bool isVip;
    int inGroupId;
    int genreCount[4];
    
public:
    User(int id, bool vip) : userId(id), isVip(vip), inGroupId(0), genreCount{0,0,0,0} {}
    explicit User(int id) : userId(id), isVip(false),inGroupId(0),genreCount{0,0,0,0} {}
    ~User() = default;

    int getUserId() const {
        return userId;
    }
    bool getIsVip() const{
        return isVip;
    }
    int getInGroupId() const {
        return inGroupId;
    }
    int genreCountIdx(int num) const {
        return genreCount[num];
    }

    int getGenreCount(Genre genre) const{
        return genreCount[static_cast<int>(genre)];
    }

    void setInGroupId(int GroupId) {
        inGroupId = GroupId;
    }

    void incGenreCount(Genre genre) {
        genreCount[int(genre)]++;
    }




    friend bool operator<(const User& user1, const User& user2) {
        return (user1.userId < user2.userId);
    }
    friend bool operator>(const User& user1, const User& user2) {
        return (user1.userId > user2.userId);
    }
    friend bool operator==(const User& user1, const User& user2) {
        return (user1.userId == user2.userId);
    }
    friend ostream& operator<<(ostream& os, const User& user)
    {
        os << "User id is " << user.userId << " ";
        os << "User is " << user.isVip << '\n';
        return os;
    }

    void printComedyWatches()
    {
        cout << "user watched" << this->genreCount[0] << '\n';
    }
};

class Group{
private:
    int groupId;
    int groupIsVip;
    int numOfMembers;

public:
    AvlTree<User> members;
    explicit Group(int id) : groupId(id), groupIsVip(false), numOfMembers(0) { members = AvlTree<User>(); }
    ~Group() = default;


    int getGroupId() const {
        return groupId;
    }

    int getNumOfMembers() const {
        return numOfMembers;
    }

    bool getGroupIsVip() const {
        return groupIsVip;
    }

    void setGroupId(int groupId) {
        Group::groupId = groupId;
    }

    void setGroupIsVip(bool groupIsVip) {
        Group::groupIsVip = groupIsVip;
    }

    void setNumOfMembers(int numOfMembers) {
        Group::numOfMembers = numOfMembers;
    }


    friend bool operator<(const Group& g1, const Group& g2) {
        return (g1.groupId < g2.groupId);
    }
    friend bool operator>(const Group& g1, const Group& g2) {
        return (g1.groupId > g2.groupId);
    }
    friend bool operator==(const Group& g1, const Group& g2) {
        return (g1.groupId == g2.groupId);
    }

    void printMembers()
    {
        members.inorderPrint();
    }



//    bool groupStillVip(int id)
//    {
//
//    }
};
#endif //WET1_USERS_H
