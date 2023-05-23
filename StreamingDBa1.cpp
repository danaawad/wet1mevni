#include "StreamingDBa1.h"

streaming_database::streaming_database() : usersTree(), groupsTree(), allMovies(),
        comedyTree(), dramaTree(), actionTree(), fantasyTree(), noneTree()
{
    usersInDatabase = 0;
    groupsInDatabase = 0;
    for(int i=0; i<4; i++)
    {
        moviesInDatabase[i] = 0;
    }
    genrePtrs[0] = &comedyTree;
    genrePtrs[1]= &dramaTree;
    genrePtrs[2]= &actionTree;
    genrePtrs[3]= &fantasyTree;
    genrePtrs[4]= &noneTree;

}

StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(movieId <= 0 || genre == Genre::NONE || views < 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Movie* movie = new Movie(movieId, genre, views, vipOnly);
        Handler<Movie>* movieHandler = new Handler<Movie>(movieId, movie);
        int gen = static_cast<int>(genre);
        if (genrePtrs[gen]->find(movie) != nullptr)
        {
            delete movie;
            delete movieHandler;
            return StatusType::FAILURE;
        }
        genrePtrs[gen]->insert(movie);
        noneTree.insert(movie);
        allMovies.insert(movieHandler);
        moviesInDatabase[gen]++;
    } catch (...) { return StatusType::ALLOCATION_ERROR; }

	return StatusType::SUCCESS;
}

StatusType streaming_database::remove_movie(int movieId)
{
    if(movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
       Handler<Movie>* movieHandler = new Handler<Movie>(movieId);
       if(allMovies.find(movieHandler) == nullptr)
       {
           delete movieHandler;
           return StatusType::FAILURE;
       }
        Handler<Movie>* handlerToRemove = allMovies.find(movieHandler)->obj;
        Movie *thisMovie = handlerToRemove->getElement();
        int genre = static_cast<int>(thisMovie->getGenre());
        genrePtrs[genre]->remove(thisMovie);
        noneTree.remove(thisMovie);
        allMovies.remove(movieHandler);
        moviesInDatabase[genre]--;
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

	return StatusType::SUCCESS;
}

StatusType streaming_database::add_user(int userId, bool isVip)
{
    if(userId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        User *newUser = new User(userId, isVip);
        if(usersTree.find(newUser) != nullptr)
        {
            delete newUser;
            return StatusType::FAILURE;
        }
        usersTree.insert(newUser);
    } catch(...) { return StatusType::ALLOCATION_ERROR; }
    usersInDatabase++;
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    try {
        User *tmpUser = new User(userId);
        if(usersTree.find(tmpUser) == nullptr)
        {
            delete tmpUser;
            return StatusType::FAILURE;
        }

        User *userToRemove = usersTree.find(tmpUser)->obj;
        usersTree.remove(userToRemove);

        /// if user was in group, remove him from it
        int groupId = userToRemove->getInGroupId();
        if(groupId != 0)
        {
            Group *tmpGroup = new Group(groupId);
            Group *group = groupsTree.find(tmpGroup)->obj;
            group->members.remove(userToRemove);
            group->setNumOfMembers(group->getNumOfMembers()-1);
            //check if group remains vip
            if(userToRemove->getIsVip())
                group->setGroupIsVip(group->getGroupIsVip() - 1);
            delete tmpUser;
            delete tmpGroup;
        }

//        userToRemove->~User();
    } catch(...){ return StatusType::ALLOCATION_ERROR; }

    usersInDatabase--;
    return StatusType::SUCCESS;
}

StatusType streaming_database::add_group(int groupId)
{
    if(groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Group *newGroup = new Group(groupId);
    if(groupsTree.find(newGroup) != nullptr)
    {
        delete newGroup;
        return StatusType::FAILURE;
    }
    try {
        groupsTree.insert(newGroup);
    } catch(...){ return StatusType::ALLOCATION_ERROR; }
    groupsInDatabase++;
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_group(int groupId)
{
    if(groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    Group *group = new Group(groupId);
    if(groupsTree.find(group) == nullptr)
    {
        delete group;
        return StatusType::FAILURE;
    }
    Group *groupToRemove = groupsTree.find(group)->obj;
    try {
//        User *currentUser = group->members.getFirstInOrder()->obj;

        //setting user.inGroupId = 0 to all members in the group and deleting members tree

        while (group->members.getFirstInOrder() != nullptr) {
            User *currentUser = groupToRemove->members.getFirstInOrder()->obj;
            currentUser->setInGroupId(0);
            groupToRemove->members.remove(currentUser);
            delete currentUser;
        }
        groupsTree.remove(groupToRemove);
        delete group;
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    groupsInDatabase--;
    return StatusType::SUCCESS;
}


StatusType streaming_database::add_user_to_group(int userId, int groupId)
{
    if(userId <= 0 || groupId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    try {
        User *tmpUser = new User(userId);
        Group *tmpGroup = new Group(groupId);
        if(usersTree.find(tmpUser) == nullptr || groupsTree.find(tmpGroup) == nullptr)
        {
            delete tmpUser;
            delete tmpGroup;
            return StatusType::FAILURE; //user or group not in database
        }

        User *user = usersTree.find(tmpUser)->obj;
        Group *group = groupsTree.find(tmpGroup)->obj;

        if(user->getInGroupId() != 0)
        {
            return StatusType::FAILURE; // user already in other group
        }

        if(user->getIsVip())
        {
            group->setGroupIsVip(group->getGroupIsVip()+1);
        }

        group->members.insert(user);
        user->setInGroupId(groupId);
        group->setNumOfMembers(group->getNumOfMembers()+1);

        delete tmpUser;
        delete tmpGroup;

    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if(userId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    //find movie and user in database
    User* tmpUser = new User(userId);
    Handler<Movie>* tmpHandler = new Handler<Movie>(movieId);
//    Movie tmpMovie(movieId);
    if(usersTree.find(tmpUser) == nullptr || allMovies.find(tmpHandler) == nullptr)
    {
        delete tmpUser;
        delete tmpHandler;
        return StatusType::FAILURE; //user or movie not in database
    }

    User *user = usersTree.find(tmpUser)->obj;
    Movie *movie = allMovies.find(tmpHandler)->obj->getElement();

    if( movie->isVipOnly() && !(user->getIsVip()) )
    {
        delete tmpUser;
        delete tmpHandler;
        return StatusType::FAILURE;
    }
    Genre genre = movie->getGenre();
    user->incGenreCount(genre);


    AvlTree<Movie>* genTree = genrePtrs[static_cast<int>(genre)];
    genTree->remove(movie);
    noneTree.remove(movie);

    movie->setViews(movie->getViews()+1);

    genTree->insert(movie);
    noneTree.insert(movie);


    delete tmpUser;
    delete tmpHandler;
    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if(groupId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Group* tmpGroup = new Group(groupId);
    Handler<Movie>* tmpHandler = new Handler<Movie>(movieId);
    if(groupsTree.find(tmpGroup) == nullptr || allMovies.find(tmpHandler) == nullptr)
    {
        delete tmpGroup;
        delete tmpHandler;
        return StatusType::FAILURE;
    }
    try {
        Group *group = groupsTree.find(tmpGroup)->obj;
        Movie *movie = allMovies.find(tmpHandler)->obj->getElement();
        Genre currGenre = movie->getGenre();
        if (movie->isVipOnly() && !(group->getGroupIsVip())) {
            delete tmpGroup;
            delete tmpHandler;
            return StatusType::FAILURE;
        }
        Node<User>* root = group->members.root;
        modifyUsers(root, currGenre);

        //remove movie from genre tree then insert again, so the genre tree remains balanced
        genrePtrs[int(currGenre)]->remove(movie);
        noneTree.remove(movie);

        movie->setViews(movie->getViews() + group->getNumOfMembers());

        genrePtrs[int(currGenre)]->insert(movie);
        noneTree.insert(movie);

    } catch (std::bad_alloc &e) { return StatusType::ALLOCATION_ERROR;}

    delete tmpGroup;
    delete tmpHandler;
	return StatusType::SUCCESS;
}




output_t<int> streaming_database::get_all_movies_count(Genre genre)
{
    int ans = 0;
    int gen = static_cast<int>(genre);
    if(genre != Genre::NONE)
    {
        ans = moviesInDatabase[gen];
    }
    else {    // genre is NONE
        for (int i = 0; i < 4; i++)
        {
            ans += moviesInDatabase[i];
        }
    }
    return output_t<int>(ans);
//    static int i = 0;
//    return (i++==0) ? 11 : 2;
}

StatusType streaming_database::get_all_movies(Genre genre, int *const output)
{
    if(output == NULL)
    {
        return StatusType::INVALID_INPUT;
    }
    int gen = static_cast<int>(genre);
    int sumMovies = (get_all_movies_count(genre)).ans();

    if(sumMovies == 0)
    {
        return StatusType::FAILURE;
    }
    try {
        AvlTree<Movie> *genTree = genrePtrs[gen];
        Node<Movie>* root = genTree->root;
        int i = 0;
        setOutput(root, output, i);
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if(userId <= 0)
    {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
    User *tmpUser = new User(userId);
    if(usersTree.find(tmpUser) == nullptr)
    {
        delete tmpUser;
        return output_t<int>(StatusType::FAILURE);
    }
    User *thisUser = usersTree.find(tmpUser)->obj;
    int views = thisUser->getGenreCount(genre);
    delete tmpUser;
    return output_t<int>(views);
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
    {
        return StatusType::INVALID_INPUT;
    }
    User *tmpUser = new User(userId);
    Handler<Movie> *movieHandler = new Handler<Movie>(movieId);
    if(usersTree.find(tmpUser) == nullptr || allMovies.find(movieHandler) == nullptr)
    {
        delete tmpUser;
        delete movieHandler;
        return StatusType::FAILURE;
    }
    User *thisUser = usersTree.find(tmpUser)->obj;
    Handler<Movie> *thisHandler = allMovies.find(movieHandler)->obj;
    Movie *thisMovie = thisHandler->getElement();
    if (thisMovie->isVipOnly() && !(thisUser->getIsVip()) )
    {
        delete tmpUser;
        delete movieHandler;
        return StatusType::FAILURE;
    }

    //rating is ok, user and movie exist
    try {
        int gen = static_cast<int>(thisMovie->getGenre());

        genrePtrs[gen]->remove(thisMovie);
        noneTree.remove(thisMovie);


        Movie *newMovie = thisHandler->getElement();
        int num = newMovie->getTimesRated();
        int newAverage = ((newMovie->getRating() * num) + rating) / (num + 1);
        newMovie->setRating(newAverage);
        newMovie->incTimesRated(1);

        genrePtrs[gen]->insert(newMovie);
        noneTree.insert(newMovie);

        delete tmpUser;
        delete movieHandler;
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}
output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if(groupId <= 0)
    {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Group *tmpGroup = new Group(groupId);
    if(groupsTree.find(tmpGroup) == nullptr)
    {
        delete tmpGroup;
        return output_t<int>(StatusType::FAILURE);
    }

    Group *thisGroup = groupsTree.find(tmpGroup)->obj;
    if(thisGroup->members.root == nullptr)
    {
        delete tmpGroup;
        return output_t<int>(StatusType::FAILURE); ///group is empty
    }
    int countArray[]{0,0,0,0};

    Node<User>* root = thisGroup->members.root;
    for(int i=0; i<4; i++)
    {
        getUsersViews(root, i, countArray);
    }
    int max = countArray[0];
    int idx = 0;
    for(int i=0; i<4; ++i)
    {
        if (countArray[i] > max)
        {
            max = countArray[i];
            idx = i;
        }
    }

    AvlTree<Movie>* tree = genrePtrs[idx];
    Movie* resultMovie = tree->getLastInOrder()->obj;
    int res = resultMovie->getMovieId();

    return output_t<int>(res);
}

const int *streaming_database::getMoviesInDatabase() const {
    return moviesInDatabase;
}

void streaming_database::printMovieTree()
{
    comedyTree.inorderPrint();
    cout << '\n' << moviesInDatabase[0] << endl;

    return;
}

void streaming_database::printUsersTree()
{
    cout << "hello" << endl;
    usersTree.inorderPrint();
    cout << usersInDatabase << endl;
    return;
//    usersTree.root->obj->printComedyWatches();
}

void streaming_database::printHandlerTree()
{
    allMovies.inorderPrint();
}

void streaming_database::printGroupTree()
{
    groupsTree.root->obj->printMembers();
}