#ifndef __FIBONACCI_H
#define __FIBONACCI_H

#include<vector>
#include <iostream>
using namespace std;


class FibHeap {
public:

	FibHeap() :
	_size(0) {} 
	
	FibHeap(int elem)  {
		_minimum = new Node(elem);
		_rightmost = _minimum;
		_leftmost = _minimum;
		_size = 1;
	}

	~FibHeap() {}

	
	class Node {
	public:

		Node() :
			_elem(0),
			_left(this),
			_right(this),
			_parent(nullptr),
			_son(nullptr),
			_mark(false),
			_deg(0) {}

		Node(int elem) :
			_elem(elem),
			_left(this),
			_right(this),
			_parent(nullptr),
			_son(nullptr),
			_mark(false),
			_deg(0) {}

		~Node() {
			_left = nullptr;
			_right = nullptr;
			_parent = nullptr;
			_son = nullptr;
		}

	 /* Node Fields */
		int _elem;

		//Pointers for the linked lists
		Node* _left;
		Node* _right;
		Node* _parent;
		Node* _son;

		//Mark. True if it has been cut, false otherwise.
		bool _mark;

		//Degree of the node (as in Binomial Heaps).
		int _deg;
	
	};
//FibHeap fields
private:

	Node *_minimum;
	Node *_leftmost;
	Node *_rightmost;
	int _size;

public:

	Node* insert(int val);
	void merge(FibHeap *h2); //Joins other to the actual FibHeap
	int get_min();
	int get_size();
	void decrease_key(Node *n, int k); //Decrease the key of the node n by one
	int extract_min();
	void extract_elem(Node *n);

//Auxiliar methods
private: 

	void consolidate();
	void cut(Node *son, Node *parent);
	void cascading_cut(Node *son);
	void link(Node *x, Node *y);
	void erase_min();
};

#endif