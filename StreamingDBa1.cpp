#include "StreamingDBa1.h"

streaming_database::streaming_database() : usersTree(), groupsTree(), allMovies(),
        comedyTree(), dramaTree(), actionTree(), fantasyTree()
{
    usersInDatabase = 0;
    groupsInDatabase = 0;
    for(int i=0; i<4; i++)
    {
        moviesInDatabase[i] = 0;
    }
    genrePtrs[0] = &(comedyTree);
    genrePtrs[1]= &(dramaTree);
    genrePtrs[2]= &(actionTree);
    genrePtrs[3]= &(fantasyTree);
}

streaming_database::~streaming_database()
{
    for(int i=0; i<4; i++)
    {
        delete genrePtrs[i];
    }
    usersTree.~AvlTree();
    groupsTree.~AvlTree();
    comedyTree.~AvlTree();
    dramaTree.~AvlTree();
    actionTree.~AvlTree();
    fantasyTree.~AvlTree();
    allMovies.~AvlTree();
}


StatusType streaming_database::add_movie(int movieId, Genre genre, int views, bool vipOnly)
{
    if(movieId <= 0 || genre == Genre::NONE || views < 0) {
        return StatusType::INVALID_INPUT;
    }
    try {
        Movie movie(movieId, genre, views, vipOnly, 0);
        Handler<Movie> movieHandler(movieId, &movie);

        int gen = static_cast<int>(genre);
        if (genrePtrs[gen]->find(&movie) != nullptr)
        {
            return StatusType::FAILURE;
        }
        genrePtrs[gen]->insert(&movie);
        allMovies.insert(&movieHandler);

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
        Handler<Movie> *movieHandler = allMovies.findThroughKey(movieId)->obj;
        Movie *thisMovie = movieHandler->getElement();
        if (thisMovie == nullptr) {
            return StatusType::FAILURE;
        }
        int genre = static_cast<int>(thisMovie->getGenre());
        genrePtrs[genre]->remove(thisMovie);
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
        User newUser(userId, isVip);
        if(usersTree.find(&newUser) != nullptr)
        {
            newUser.~User();
            return StatusType::FAILURE;
        }
        usersTree.insert(&newUser);
    } catch(...){ return StatusType::ALLOCATION_ERROR; }
    usersInDatabase++;
    return StatusType::SUCCESS;
}

StatusType streaming_database::remove_user(int userId)
{
    if(userId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    User tmpUser(userId);
    if(usersTree.find(&tmpUser) == nullptr)
    {
        tmpUser.User::~User();
        return StatusType::FAILURE;
    }
    User *userToRemove = usersTree.find(&tmpUser)->obj;
    try {
        usersTree.remove(userToRemove);
        int groupId = userToRemove->getInGroupId();
        //if user was in group, remove him from it
        if(groupId != 0)
        {
            Group tmpGroup(groupId);
            Group *group = groupsTree.find(&tmpGroup)->obj;
            group->members.remove(userToRemove);
            group->setNumOfMembers(group->getNumOfMembers()-1);
            //check if group remains vip
            if(userToRemove->getIsVip())
            {
                group->setGroupIsVip(false);
                for (ReverseInorderIterator<User> it = reverse_inorder_begin(group->members.root);
                     it != reverse_inorder_end<User>(); ++it)
                {
                    if(it.getCurrent()->obj->getIsVip())
                    {
                        group->setGroupIsVip(true);
                        break;
                    }
                }
            }
            tmpGroup.~Group();
        }

        userToRemove->~User();
        tmpUser.~User();
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
    Group newGroup(groupId);
    if(groupsTree.find(&newGroup) != nullptr)
    {
        newGroup.~Group();
        return StatusType::FAILURE;
    }
    try {
        groupsTree.insert(&newGroup);
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
    Group tmpGroup(groupId);
    if(groupsTree.find(&tmpGroup) != nullptr)
    {
        tmpGroup.~Group();
        return StatusType::FAILURE;
    }
    Group *group = groupsTree.find(&tmpGroup)->obj;
    Node<User> *currentUser = group->members.getFirstInOrder();
    tmpGroup.~Group();

    //setting user.inGroupId = 0 to all members in the group and deleting members tree
    try {
        while (currentUser != nullptr) {
            currentUser->obj->setInGroupId(0);
            group->members.remove(currentUser->obj);
            currentUser = group->members.getFirstInOrder();
        }
        groupsTree.remove(group);
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
    User tmpUser(userId);
    Group tmpGroup(groupId);
    if(usersTree.find(&tmpUser) == nullptr || groupsTree.find(&tmpGroup) == nullptr)
    {
        return StatusType::FAILURE; //user or group not in database
    }

    User *user = usersTree.find(&tmpUser)->obj;
    Group *group = groupsTree.find(&tmpGroup)->obj;

    if(user->getInGroupId() != 0)
    {
        return StatusType::FAILURE; // user already in other group
    }

    if(user->getIsVip())
    {
        group->setGroupIsVip(true);
    }

    try {
        group->members.insert(user);
        tmpUser.~User();
        tmpGroup.~Group();
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    user->setInGroupId(groupId);
    group->setNumOfMembers(group->getNumOfMembers()+1);
    return StatusType::SUCCESS;
}

StatusType streaming_database::user_watch(int userId, int movieId)
{
    if(userId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }
    //find movie and user in database
    User tmpUser(userId);
//    Movie tmpMovie(movieId);
    if(usersTree.find(&tmpUser) == nullptr || allMovies.findThroughKey(movieId) == nullptr)
    {
        return StatusType::FAILURE; //user or movie not in database
    }

    User *user = usersTree.find(&tmpUser)->obj;
    Movie *movie = allMovies.findThroughKey(movieId)->obj->getElement();

    if( movie->isVipOnly() && !(user->getIsVip()) )
    {
        return StatusType::FAILURE;
    }

    movie->setViews(movie->getViews()+1);
    tmpUser.~User();

    return StatusType::SUCCESS;
}

StatusType streaming_database::group_watch(int groupId,int movieId)
{
    if(groupId <= 0 || movieId <= 0)
    {
        return StatusType::INVALID_INPUT;
    }

    Group tmpGroup(groupId);

    if(groupsTree.find(&tmpGroup) == nullptr || allMovies.findThroughKey(movieId) == nullptr)
    {
        return StatusType::FAILURE;
    }

    try {
        Group *group = groupsTree.find(&tmpGroup)->obj;
        Movie *movie = allMovies.findThroughKey(movieId)->obj->getElement();
        Genre currGenre = movie->getGenre();

        if (movie->isVipOnly() && !(group->getGroupIsVip())) {
            return StatusType::FAILURE;
        }

        tmpGroup = *group;     /// or use the reverse inorder iterator
        Node<User> *currentUser = tmpGroup.members.getFirstInOrder();
        while (currentUser != nullptr) {
            currentUser->obj->incGenreCount(currGenre);
            tmpGroup.members.remove(currentUser->obj);
            currentUser = group->members.getFirstInOrder();
        }

        //remove movie from genre tree then insert again, so the genre tree remains balanced
        AvlTree<Movie> *genreTree = (genrePtrs[int(currGenre)]);
        genreTree->remove(movie);

        movie->setViews(movie->getViews() + group->getNumOfMembers());

        genreTree->insert(movie);

    } catch (std::bad_alloc &e) { return StatusType::ALLOCATION_ERROR;}

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
    if(genre != Genre::NONE)
    {
        if(sumMovies == 0)
        {
            return StatusType::FAILURE;
        }
        try {
            AvlTree<Movie> *genTree = genrePtrs[gen];
            int curr = 0;
            for (ReverseInorderIterator<Movie> it = reverse_inorder_begin(genTree->root);
                 it != reverse_inorder_end<Movie>(); ++it) {
                output[curr] = it.getCurrent()->obj->getMovieId();
                curr++;
            }
        } catch(std::bad_alloc &e) { return StatusType::ALLOCATION_ERROR; }
        return StatusType::SUCCESS;
    }
    else {
        if (sumMovies == 0) {
            return StatusType::FAILURE;
        }
        try {
            Movie **arr = new Movie *[4];
            for (int i = 0; i < 4; i++) {
                int size = get_all_movies_count(static_cast<Genre>(i)).ans();
                arr[i] = *new Movie *[size];

                AvlTree<Movie> *currTree = genrePtrs[i];
                // Assign the movie ids to the array
                int j = 0;
                for (ReverseInorderIterator<Movie> it = reverse_inorder_begin(currTree->root);
                     it != reverse_inorder_end<Movie>(); ++it) {
                    arr[i][j] = *(it.getCurrent()->obj);
                    j++;
                }
            }


            //merge the 4 arrays
            for (int i = 0; i < sumMovies; i++) {
                int comedy = 0, action = 0, drama = 0, fantasy = 0;
                if (arr[0][comedy] < arr[1][action]
                    && arr[0][comedy] < arr[2][drama]
                    && arr[0][comedy] < arr[3][fantasy]) {
                    output[i] = arr[0][comedy].getMovieId();
                    comedy++;
                }
                if (arr[1][action] < arr[0][comedy]
                    && arr[1][action] < arr[2][drama]
                    && arr[1][action] < arr[3][fantasy]) {
                    output[i] = arr[1][action].getMovieId();
                    action++;
                }
                if (arr[2][drama] < arr[0][comedy]
                    && arr[2][drama] < arr[1][action]
                    && arr[2][drama] < arr[3][fantasy]) {
                    output[i] = arr[2][drama].getMovieId();
                    drama++;
                }
                if (arr[3][fantasy] < arr[0][comedy]
                    && arr[3][fantasy] < arr[1][action]
                    && arr[3][fantasy] < arr[2][drama]) {
                    output[i] = arr[3][fantasy].getMovieId();
                    fantasy++;
                }
            }
            //output array is loaded


            for (int i = 0; i < 4; i++) {
                delete[] arr[i];
            }
            delete[] arr;
        } catch (std::bad_alloc &e) { return StatusType::ALLOCATION_ERROR; }
    }
    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_num_views(int userId, Genre genre)
{
    if(userId <= 0)
    {
        return output_t<int>(StatusType::INVALID_INPUT);
    }
	User tmpUser(userId);
    if(usersTree.find(&tmpUser) == nullptr)
    {
        tmpUser.~User();
        return output_t<int>(StatusType::FAILURE);
    }
    User *thisUser = usersTree.find(&tmpUser)->obj;
    int views = thisUser->getGenreCount(genre);
    tmpUser.~User();
    return output_t<int>(views);
}

StatusType streaming_database::rate_movie(int userId, int movieId, int rating)
{
    if(userId <= 0 || movieId <= 0 || rating < 0 || rating > 100)
    {
        return StatusType::INVALID_INPUT;
    }
    User tmpUser(userId);
    Handler<Movie> movieHandler(movieId);
    if(usersTree.find(&tmpUser) == nullptr || allMovies.findThroughKey(movieId) == nullptr)
    {
        return StatusType::FAILURE;
    }
    User *thisUser = usersTree.find(&tmpUser)->obj;
    Movie *thisMovie = allMovies.findThroughKey(movieId)->obj->getElement();
    if (thisMovie->isVipOnly() && !(thisUser->getIsVip()) )
    {
        return StatusType::FAILURE;
    }

    //rating is ok, user and movie exist
    try {
        int gen = static_cast<int>(thisMovie->getGenre());
        genrePtrs[gen]->remove(thisMovie);

        int num = thisMovie->getTimesRated();
        int newAverage = ((thisMovie->getRating() * num) + rating) / (num + 1);
        thisMovie->setRating(newAverage);
        thisMovie->incTimesRated(1);

        genrePtrs[gen]->insert(thisMovie);
    } catch(...) { return StatusType::ALLOCATION_ERROR; }

    return StatusType::SUCCESS;
}

output_t<int> streaming_database::get_group_recommendation(int groupId)
{
    if(groupId <= 0)
    {
        return output_t<int>(StatusType::INVALID_INPUT);
    }

    Group tmpGroup(groupId);
    if(groupsTree.find(&tmpGroup) == nullptr)
    {
        tmpGroup.~Group();
        return output_t<int>(StatusType::FAILURE);
    }

    Group *thisGroup = groupsTree.find(&tmpGroup)->obj;
    if(thisGroup->members.root == nullptr)
    {
        tmpGroup.~Group();
        return output_t<int>(StatusType::FAILURE);
    }

    int countArray[]{0,0,0,0};
    int val = 0;
    for (ReverseInorderIterator<User> it = reverse_inorder_begin(thisGroup->members.root);
         it != reverse_inorder_end<User>(); ++it)
    {
        for(int i=0; i<4; i++)
        {
            val = it.getCurrent()->obj->genreCountIdx(i);
            countArray[i] += val;
        }
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
    ReverseInorderIterator<Movie> resultMovie = reverse_inorder_begin(tree->root);
    int res = resultMovie.getCurrent()->obj->getMovieId();

    return output_t<int>(res);

//    static int i = 0;
//    return (i++==0) ? 11 : 2;
}


