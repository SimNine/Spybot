#ifndef LISTNODE_H_INCLUDED
#define LISTNODE_H_INCLUDED

template<class T>
class ListNode
{
public:
    ListNode(T obj)
    {
        contents = obj;
        next = NULL;
        prev = NULL;
    };
    ~ListNode()
    {

    };
    T contents;
    ListNode* next;
    ListNode* prev;
protected:
private:
};

#endif // LISTNODE_H_INCLUDED
