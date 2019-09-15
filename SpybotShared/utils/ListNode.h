#pragma once

template<class T>
class ListNode {
public:
	ListNode(T obj) {
		contents_ = obj;
		next_ = NULL;
		prev_ = NULL;
	};
	~ListNode() {

	};
	T contents_;
	ListNode* next_;
	ListNode* prev_;
protected:
private:
};