#ifndef PAIR_H
#define PAIR_H

template <class T>
class Pair
{
    public:
        Pair(T first, T second)
        {
            a = first;
            b = second;
        };
        virtual ~Pair()
        {

        };
        T a;
        T b;
    protected:
    private:
};

#endif // PAIR_H
