//
// Created by Dana Awad on 5/19/23.
//

#ifndef WET1_MOVIEHandler_H
#define WET1_MOVIEHandler_H
#include "Movie.h"

template <class T>
class Handler{
private:
    int key;
    T* element;

public:
    explicit Handler<T>(int key = -1, T* element = nullptr) : key(key), element(element) {} ;

    Handler(const Handler& other) : key(other.key), element(nullptr) {
        if (other.element != nullptr) {
            element = new T(*other.element);
        }
    }

    ~Handler()
    {
        delete element;
    }

    Handler<T>& operator=(const Handler& handler)
    {
        if(this == &handler)
            return *this;
        delete element;
        key = handler.getKey();
        element = handler.getElement();
        return *this;
    }


    friend bool operator<(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key < Handler2.key);
    }
    friend bool operator>(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key > Handler2.key);
    }
    friend bool operator==(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key == Handler2.key);
    }



    T* getElement() const
    {
        return this->element;
    }

    int getKey() const {
        return key;
    }
    friend ostream& operator<<(ostream& os, const Handler<T>& handler)
    {
        os << "Movie id is " << handler.getKey() << " ";
        return os;
    }
};

#endif //WET1_MOVIEHandler_H