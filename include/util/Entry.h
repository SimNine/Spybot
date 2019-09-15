#ifndef ENTRY_H
#define ENTRY_H

template <class T, class U>
class Entry
{
    public:
        Entry(T first, U second)
        {
            a = first;
            b = second;
        };
        virtual ~Entry()
        {

        };
        T a;
        U b;
    protected:
    private:
};

#endif // ENTRY_H
