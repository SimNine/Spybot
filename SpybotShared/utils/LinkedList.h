#pragma once

#include "ListNode.h"
#include "Iterator.h"

template<class T> class LinkedList {
public:
	LinkedList() {
		length = 0;
		firstNode = NULL;
		lastNode = NULL;
	};

	~LinkedList() {
		ListNode<T>* currNode = firstNode;
		while (currNode != NULL) {
			ListNode<T>* temp = currNode;
			currNode = currNode->next_;
			delete temp;
		}
	};

	void addLast(T obj) {
		ListNode<T>* n = new ListNode<T>(obj);
		if (lastNode == NULL) {
			firstNode = n;
			lastNode = n;
		} else {
			n->prev_ = lastNode;
			lastNode->next_ = n;
			lastNode = n;
		}
		length++;
	};

	void addFirst(T obj) {
		ListNode<T>* n = new ListNode<T>(obj);
		if (firstNode == NULL) {
			firstNode = n;
			lastNode = n;
		} else {
			n->next_ = firstNode;
			firstNode->prev_ = n;
			firstNode = n;
		}
		length++;
	};

	void addAllLast(LinkedList<T>* l2) {
		ListNode<T>* currNode = l2->firstNode;
		while (currNode != NULL) {
			addLast(currNode->contents_);
			currNode = currNode->next_;
		}
	};

	void addAllFirst(LinkedList<T>* l2) {
		ListNode<T>* currNode = l2->lastNode;
		while (currNode != NULL) {
			addFirst(currNode->contents_);
			currNode = currNode->prev_;
		}
	};

	T getFirst() {
		if (firstNode == NULL) return NULL;
		else return firstNode->contents_;
	}

	T getLast() {
		if (lastNode == NULL) return NULL;
		else return lastNode->contents_;
	}

	T removeFirst() {
		if (length == 0) return NULL;

		ListNode<T>* tempNode = firstNode;
		if (firstNode->next_ == NULL) {
			firstNode = NULL;
			lastNode = NULL;
		} else {
			firstNode = firstNode->next_;
			firstNode->prev_ = NULL;
		}

		length--;
		T temp = tempNode->contents_;
		delete tempNode;
		return temp;
	}

	T removeLast() {
		if (length == 0) return NULL;

		ListNode<T>* tempNode = lastNode;
		if (lastNode->prev_ == NULL) {
			firstNode = NULL;
			lastNode = NULL;
		} else {
			lastNode = lastNode->prev_;
			lastNode->next_ = NULL;
		}

		length--;
		T temp = tempNode->contents_;
		delete tempNode;
		return temp;
	}

	// checks if an object is contained in this list
	bool contains(T obj) {
		if (getIndexOf(obj) == -1) return false;
		else return true;
	}

	/* returns an object at a certain index */
	T getObjectAt(int index) {
		ListNode<T>* curr = firstNode;
		int counter = 0;

		while (curr != NULL) {
			if (counter == index) return curr->contents_;
			counter++;
			curr = curr->next_;
		}
		return NULL;
	}

	/* gets the index of the given item, or returns -1 if not in the list */
	int getIndexOf(T obj) {
		if (obj == NULL) {
			log("err: tried to find index of a null object\n");
			return -1;
		}

		int num = 0;
		ListNode<T>* curr = firstNode;
		while (curr != NULL) {
			if (curr->contents_ == obj) {
				return num;
			}
			num++;
			curr = curr->next_;
		}
		return -1;
	}
	int getLength() {
		return length;
	};

	// stack method wrappers
	T poll() {
		return removeFirst();
	}

	void push(T obj) {
		addFirst(obj);
	}

	/* returns true and removes the passed object if it is in the list,
		otherwise if the object is not in the list, returns false */
	bool remove(T obj) {
		ListNode<T>* currNode = firstNode;
		while (currNode != NULL) {
			if (currNode->contents_ == obj) {
				if (currNode == firstNode && currNode == lastNode) {
					firstNode = NULL;
					lastNode = NULL;
				} else if (currNode == lastNode) {
					lastNode = currNode->prev_;
					lastNode->next_ = NULL;
				} else if (currNode == firstNode) {
					firstNode = currNode->next_;
					firstNode->prev_ = NULL;
				} else {
					ListNode<T>* nxt = currNode->next_;
					ListNode<T>* prv = currNode->prev_;

					nxt->prev_ = prv;
					prv->next_ = nxt;
				}

				delete currNode;
				length--;
				return true;
			}
			currNode = currNode->next_;
		}
		return false;
	}

	// removes the item at a specific index
	T removeObjectAt(int i) {
		if (i < 0 || i >= length)
			return NULL;

		T temp = getObjectAt(i);
		remove(temp);
		return temp;
	}

	// replaces the object at the given index with a new object
	T replaceObjectAt(int i, T obj) {
		if (i < 0 || i >= length)
			return NULL;

		ListNode<T>* curr = firstNode;
		int counter = 0;
		while (curr != NULL) {
			if (counter == i)
				break;
			counter++;
			curr = curr->next_;
		}
		if (curr == NULL)
			return NULL;

		T temp = curr->contents_;
		curr->contents_ = obj;
		return temp;
	}

	// removes every item from the list
	void removeAll() {
		while (length > 0) {
			poll();
		}
	}

	void forEach(void(*func)(T)) {
		ListNode<T>* currNode = firstNode;
		while (currNode != NULL) {
			func(currNode->contents_);
			currNode = currNode->next_;
		}
	}

	void forEachBackwards(void(*func)(T)) {
		ListNode<T>* currNode = lastNode;
		while (currNode != NULL) {
			func(currNode->contents_);
			currNode = currNode->prev_;
		}
	}

	/* returns an iterator over this list */
	Iterator<T> getIterator() {
		return Iterator<T>(firstNode);
	}
protected:
private:
	int length;
	ListNode<T>* firstNode;
	ListNode<T>* lastNode;
};