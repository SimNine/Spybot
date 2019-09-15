#pragma once

template<class T>
class ListNode {
public:
	ListNode(T obj) {
		contents = obj;
		next = NULL;
		prev = NULL;
	};
	~ListNode() {

	};
	T contents;
	ListNode* next;
	ListNode* prev;
protected:
private:
};