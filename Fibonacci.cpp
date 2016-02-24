#include "Fibonacci.hpp"

FibHeap::Node* FibHeap::insert(int val) {
	//Create a new Node with value val
	Node *insertNode = new Node(val);
	//In the special case that the heap is empty we just point null, rightmost and leftmost to the node and also its left and right pointer to itself.
	if (_minimum == nullptr) {
		_minimum = insertNode;
		_leftmost = insertNode;
		_rightmost = insertNode;
	}
	//In the other case
	else {
		if (_minimum == _leftmost) {
			_leftmost = insertNode;
		}
		//Insert the new node in the existin list
		_minimum->_left->_right = insertNode;
		insertNode->_left = _minimum->_left;
		_minimum->_left = insertNode;
		insertNode->_right = _minimum;

		//If the new Node is lower than the minimum we uodate the pointer to the minimum
		if (insertNode->_elem < _minimum->_elem){
			_minimum = insertNode;
		}
	}
	_size++;

	return insertNode;
}

//In this case I just merge the second heap to the first because is faster than joinning 2
void FibHeap::merge(FibHeap *h2){
	//Concatenate both lists.
	_rightmost->_right = h2->_leftmost;
	h2->_leftmost->_left = _rightmost;
	_rightmost = h2->_rightmost;
	_leftmost->_left = h2->_rightmost;
	h2->_rightmost->_right = _leftmost;

	//If the first heap has no minimum of the new one is lower update the minimum
	if ((_minimum == nullptr) || ((h2->_minimum != nullptr) && (h2->_minimum->_elem < _minimum->_elem))) {
		_minimum = h2->_minimum;
	}
	//update the size of the heap and delete the other one after finishing
	_size += h2->get_size();
}

int FibHeap::get_min() {
	return _minimum->_elem;
}

int FibHeap::get_size() {
	return _size;
}

void FibHeap::erase_min() {
	Node *min, *son, *next;
	Node ** childList;

	min = _minimum;
	//If the heap is not empty
	if (min != nullptr)
	{
		//We get the pointer son of the minimum that we are gonna use to get all the minimum sons
		son = min->_son;
		if (son != nullptr)
		{
			//We create a list with all the sons of the minimum
			childList = new Node*[min->_deg];
			next = son;
			for (int i = 0; i < (int)min->_deg; i++)
			{
				childList[i] = next;
				next = next->_right;
			}
			//Then we add them to the root list
			for (int i = 0; i < (int)min->_deg; i++)
			{
				son = childList[i];
				_minimum->_left->_right = son;
				son->_left = _minimum->_left;
				_minimum->_left = son;
				son->_right = _minimum;
				son->_parent = nullptr;
			}
			//We update leftmost and rightmost if needed
			if (_minimum == _leftmost) {
				_leftmost = childList[0];
			}
			if (_minimum == _rightmost) {
				_rightmost = childList[min->_deg];
			}
			delete[] childList;
		}
		//Finally we delete the minimum from the list
		min->_left->_right = min->_right;
		min->_right->_left = min->_left;
		//If the list is empty
		if (min == min->_right)
		{
			_minimum = nullptr;
		}
		//If not we get a provisional minimum and we consolidate.
		else
		{
			_minimum = min->_right;
			consolidate();
		}
		_size--;
	}
}

void FibHeap::decrease_key(Node *n, int k) {
	//We check if the value that we want to assign is lower than the actual
	if (k > n->_elem) {
		cout << "Error the key is greater than the one existing in the node" << endl;
	}
	else {
		//We update the value of the Node n and we get its parent
		n->_elem = k;
		Node *father = n->_parent;
		if (father != nullptr && n->_elem < father->_elem) {
			//Then if the new value is lower than the father one we apply recursively cut and cascading cut to be able to position the node in its new position
			cut(n, father);
			cascading_cut(father);
		}
		//We update the minimum
		if (n->_elem < _minimum->_elem) {
			_minimum = n;
		}
	}
}

void FibHeap::consolidate() {
	Node* w, *next, *x, *y, *temp;
	Node** Array, ** rootList;
	int d, rootSize;
	int max_degree = static_cast<int>(floor(log(static_cast<double>(_size)) / log(static_cast<double>(1 + sqrt(static_cast<double>(5))) / 2)));

	//We create an array of pointers to Nodes and we fill it with NULL values
	Array = new Node*[max_degree + 2];
	for (int k = 0; k < max_degree + 2; k++) {
		Array[k] = nullptr;
	}
	//We calculate the size of the root list
	w = _minimum;
	rootSize = 0;
	next = w;
	do
	{
		rootSize++;
		next = next->_right;
	} while (next != w);
	//We create a list with only the Nodes in the root
	next = _leftmost;
	rootList = new Node*[rootSize];
	for (int i = 0; i < rootSize; i++)
	{
		rootList[i] = next;
		next = next->_right;
	}
	//For each node in the list we get it and link it to the Node in the vector Array[] that has it same degree but respecting always that the key of the father is lower than the key of the son
	for (int i = 0; i < rootSize; i++)
	{
		w = rootList[i];
		x = w;
		d = x->_deg;
		while (Array[d] != nullptr)
		{
			y = Array[d];
			if (x->_elem > y->_elem)
			{
				temp = x;
				x = y;
				y = temp;
			}
			link(y, x);
			Array[d] = nullptr;
			d++;
		}
		Array[d] = x;
	}
	delete[] rootList;
	_minimum = nullptr;
	//We traverse the vector Array[] adding all the nodes in it to the root list of the FibHeap and updating the _minimum _leftmost and _rightmost when necessary
	for (int i = 0; i < max_degree + 2; i++)
	{
		if (Array[i] != nullptr)
		{
			if (_minimum == nullptr)
			{
				_minimum =_leftmost = _rightmost = Array[i]->_left = Array[i]->_right = Array[i];
			}
			else
			{
				_minimum->_left->_right = Array[i];
				Array[i]->_left = _minimum->_left;
				_minimum->_left = Array[i];
				Array[i]->_right = _minimum;
				if (_minimum == _rightmost) {
					_rightmost = Array[i];
				}
				if (Array[i]->_elem < _minimum->_elem)
				{
					_minimum = Array[i];
				}
			}
		}
	}
	delete[] Array;
}

void FibHeap::cut(Node *son, Node *parent) {
	//delete son from the list of parent decrementing degree of parent
	if (son == son->_right) {
		parent->_son = nullptr;
	}
	else {
		son->_right->_left = son->_left;
		son->_left->_right = son->_right;
		if (parent->_son == son) {
			parent->_son = son->_right;
		}
	}
	parent->_deg--;

	//add son to the root list
	_minimum->_left->_right = son;
	son->_left = _minimum->_left;
	_minimum->_left = son;
	son->_right = _minimum;

	if (_minimum == _leftmost) {
		_leftmost = son;
	}
	//set parent of the previous son to Null and the mark to false
	son->_parent = nullptr;
	son->_mark = false;
}

void FibHeap::cascading_cut(Node *son) {
	//We get the parent of the node son and check if his mark is true or false
	Node *parent = son->_parent;
	if (parent != nullptr) {
		//If is false we just make it true
		if (!son->_mark) {
			son->_mark = true;
		}
		//Else we cut the son and go on with the cascading cut
		else {
			cut(son, parent);
			cascading_cut(parent);
		}
	}
}

int FibHeap::extract_min() {
	int min = get_min();
	erase_min();
	return min;
}

void FibHeap::extract_elem(Node *n) {
	decrease_key(n, INT_MIN);
	erase_min();
}

void FibHeap::link(Node *y, Node *x) {
	//remove y from the root list of the heap
	y->_left->_right = y->_right;
	y->_right->_left = y->_left;

	//make y a child of x and increment the degree of x
	if (x->_son == nullptr) {
		x->_son = y;
		y->_right = y;
		y->_left = y;
	}
	else {
		y->_left = x->_son->_left;
		x->_son->_left->_right = y;
		x->_son->_left = y;
		y->_right = x->_son;
		x->_son = y;
	}
	y->_parent = x;
	x->_deg++;

	//make the mark of y false
	y->_mark = false;
}