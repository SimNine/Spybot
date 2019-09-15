#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdlib.h>
#include <stdio.h>

template<class T>
class LinkedList
{
    public:
        LinkedList(T obj)
        {
            contents = obj;
            next = NULL;
        };
        ~LinkedList()
        {

        };
        T getContents()
        {
            return contents;
        };
        LinkedList* getNext()
        {
            return next;
        };
        void setNext(LinkedList* n)
        {
            next = n;
        };
    protected:
    private:
        T contents;
        LinkedList* next;
};

// adds an object to the end of this list
template <class T> void LL_addObject(LinkedList<T>* l, T obj)
{
    LinkedList<T>* currNode = l;
    while (currNode->getNext() != NULL)
    {
        currNode = currNode->getNext();
    }
    currNode->setNext(new LinkedList<T>(obj));
}

// appends a list of objects to this list of objects
template <class T> void LL_addAllObjects(LinkedList<T>* l, LinkedList<T>* l2)
{
    LinkedList<T>* currNode = l;
    while (currNode->getNext() != NULL)
    {
        currNode = currNode->getNext();
    }
    currNode->setNext(l2);
}

// gets the number of nodes in this list
template <class T> int LL_getLength(LinkedList<T>* l)
{
    int num = 0;
    while (l != NULL)
    {
        num++;
        l = l->getNext();
    }
    return num;
}

// gets the (first) index of this item in this list
// returns -1 if not in this list
template <class T> int LL_getIndexOf(LinkedList<T>* l, T obj)
{
    if (obj == NULL)
    {
        printf("err: tried to find index of a null object\n");
        return -1;
    }

    int num = 0;
    while (l != NULL)
    {
        if (l->getContents() == obj)
        {
            return num;
        }
        num++;
        l = l->getNext();
    }
    return -1;
}

// frees this list, but NOT its contents
template <class T> void LL_freeList(LinkedList<T>* l)
{
    LinkedList<T>* temp = NULL;
    while (l != NULL)
    {
        temp = l;
        l = l->getNext();
        delete temp;
    }
}

// frees this list AND its contents
template <class T> void LL_freeListAndContents(LinkedList<T>* l)
{
    LinkedList<T>* temp = NULL;
    while (l != NULL)
    {
        temp = l;
        l = l->getNext();
        delete temp->getContents();
        delete temp;
    }
}

// gets the object at a given index
template <class T> T LL_getObjectAt(LinkedList<T>* l, int i)
{
    if (i < 0 || i >= LL_getLength(l))
    {
        printf("err: searching for an out of bounds index\n");
        return NULL;
    }
    if (l == NULL)
    {
        printf("err: list is null\n");
        return NULL;
    }

    while (i > 0)
    {
        i--;
        l = l->getNext();
    }
    return l->getContents();
}

#endif // LINKEDLIST_H
