#ifndef MAP_H_INCLUDED
#define MAP_H_INCLUDED

template<class T, class U> class Map
{
public:
    Map()
    {
        keys_ = new LinkedList<T>*();
        vals_ = new LinkedList<U>*();
    };
    ~LinkedList()
    {
        while (keys_->getLength() > 0)
            delete keys_->poll();
        delete keys_;
        while (vals_->getLength() > 0)
            delete vals_->poll();
        delete vals_;
    };

    void put(T key, U val)
    {
        if (keys_->contains(key))
        {
            int keyIndex = keys_->getIndexOf(key);
            vals_->replaceObjectAt(keyIndex, val);
        }
        else
        {
            keys_->addFirst(key);
            vals_->addFirst(val);
        }
    }

    T getKey(U val)
    {
        if (vals_->contains(val))
        {
            int valIndex = vals_->getIndexOf(val);
            keys_->getObjectAt(valIndex);
        }
        else
            return NULL;
    }

    T getVal(T key)
    {
        if (keys_->contains(key))
        {
            int keyIndex = keys_->getIndexOf(key);
            vals_->getObjectAt(keyIndex);
        }
        else
            return NULL;
    }

    int getLength()
    {
        return keys_->getLength();
    }
private:
    LinkedList<T>* keys_;
    LinkedList<U>* vals_;
}

#endif // MAP_H_INCLUDED
