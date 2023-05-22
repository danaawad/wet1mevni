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
    Handler<T>& operator=(const Handler& handler)
    {
        if(this == &handler)
            return *this;
        delete element;
        key = handler.getKey();
        element = handler.getElement();
        return *this;
    }
    ~Handler() = default;

    friend bool operator<(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key < Handler2.key && Handler1.element != nullptr &&  Handler2.element != nullptr);
    }
    friend bool operator>(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key > Handler2.key && Handler1.element != nullptr &&  Handler2.element != nullptr);
    }
    friend bool operator==(const Handler& Handler1, const Handler& Handler2)
    {
        return (Handler1.key == Handler2.key && Handler1.element != nullptr &&  Handler2.element != nullptr);
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