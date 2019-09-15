#ifndef ITERATOR_H_INCLUDED
#define ITERATOR_H_INCLUDED

template<class T>
class Iterator
{
public:
    Iterator(ListNode<T>* n)
    {
        currNode = n;
    };
    ~Iterator()
    {

    }
    bool hasNext()
    {
        if (currNode != NULL) return true;
        else return false;
    }
    T next()
    {
        T temp = currNode->contents;
        currNode = currNode->next;
        return temp;
    }
protected:
private:
    ListNode<T>* currNode;
};

#endif // ITERATOR_H_INCLUDED
