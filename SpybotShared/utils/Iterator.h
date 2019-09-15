#pragma once

template<class T> class Iterator {
public:
	Iterator(ListNode<T>* n) {
		currNode_ = n;
	};

	~Iterator() {

	}

	bool hasNext() {
		if (currNode_ != NULL)
			return true;
		else
			return false;
	}

	T next() {
		T temp = currNode_->contents_;
		currNode_ = currNode_->next_;
		return temp;
	}

protected:
private:
	ListNode<T>* currNode_;
};