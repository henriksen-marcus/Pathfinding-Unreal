// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * \brief Data container for use as an element inside a double linked list.
 * \tparam T The datatype to store.
 * \remark Strings will not work here.
 */
template<class T>
class Node 
{
public:
	T data;
	Node* next;
	Node* prev;

	Node()
	{
		next = nullptr;
		prev = nullptr;
		data = NULL;
	}

	Node(T& data_)
	{
		next = nullptr;
		prev = nullptr;
		data = data_;
	}
};

class DLL_Base
{
public:
	enum direction	{ forward, backwards };
	enum fillType	{ rand, asc, desc };
};
template<class T>
/**
 * \brief Double linked list that works like an array.
 * \tparam T Datatype to store.
 * \remarks Able to traverse back and fourth.
 */
class PATHFINDING_API DLL : DLL_Base
{
public:
	DLL();
	/**
	 * \brief Copy constructor.
	 * \param obj The old list.
	 * \remarks cursor_ and index_ will not be copied.
	 */
	DLL(const DLL<T>& obj);
	/* Iteratively delete all elements in the linked list. */
	~DLL();

	int size() { return size_; }

	/**
	 * \param i The index to return.
	 * \return Pointer to the node at the index.
	 * \remarks Pointer safety: If the given index exceeds the
	 * list size, the tail will be returned.
	 */
	Node<T>* at(const int &i);
	
	/**
	 * \brief Adds the specified element to the end of the list.
	 * \return The new list size.
	 */
	int add(T data);
	
	/**
	 * \brief Adds the array of elements to the end of the list.
	 * \param size The size of the incoming array.
	 * \return The new list size.
	 */
	int add(T arr[], const int& size);

	/**
	 * \brief Adds the specified element to the start of the list.
	 * \return The new list size.
	 */
	int addFront(T data) { return insert(data, 0); }

	int addFront(T arr[], const int& size);

	/**
	 * \brief Fills the list starting at the next available slot
	 * with the specified number of elements.
	 * \param type What fill pattern to use.
	 * \param min The smallest possible number in the fill pattern.
	 * \param max The largest possible number in the fill pattern.
	 * \param amount The number of elements to add.
	 * \return The new list size.
	 * \remarks If no amount is given the entire range from
	 * min-max will be printed.
	 */
	int fill(fillType type, int min, int max, int amount = -1);

	/** \brief Empties the list. */
	void clear();
	
	/**
	 * \brief Deletes the element at given index and cleans up pointers.
	 * \remarks If no argument is given the last element will be removed.
	 */
	int remove(const int index = -1);

	/** \brief Deletes the given element and cleans up pointers. */
	int remove(Node<T>* node);

	/**
	 * \brief Deletes the last element in the list.
	 * \return The new list size;
	 */
	int pop() { return remove(); }

	/**
	 * \brief Deletes the first element in the list.
	 * \return The new list size.
	 */
	int popFront() { return remove(0); }

	/**
	 * \brief Inserts a node with the given data into the specifed index.
	 * Moves the node at the index one slot to the right.
	 * \return The new list size.
	 * \remarks Cannot perform the same action as add(). Will always
	 * move the current element at the given index.
	 */
	int insert(T data, const int index);

	/**
	* \brief Inserts the given node into the specified index.
	* Moves the node at the index one slot to the right.
	* \return The new list size.
	* \remarks Cannot perform the same action as add(). Will always
	* move the element at the given index.
	*/
	int insert(Node<T>* node, const int index);

	int clearDuplicates();

	/** \brief Swaps the value of two elements. */
	void swap(int a, int b)
	{ 
		T temp = (*this)[a];
		(*this)[a] = (*this)[b];
		(*this)[b] = temp;
	}
	
	/**
	 * \brief Prints the data of each element in the specified direction.
	 * \param dir The direction to print in (start at head or tail).
	 */
	void print(direction dir = forward);

	Node<T>* head;
	Node<T>* tail;
	int operations;
	int operations_saved = 0;

private:
	/**
	 * \brief The size of the list.
	 * \remarks We don't want users to be able to edit this directly
	 */
	int size_;
	int index_;
	Node<T>* cursor_;

	/** \return The positive difference between a and b. */
	int diff(const int& a, const int& b) const { return abs(a - b); }

public:
	/** Allows for use of the standard [] array operator */
	T& operator [] (const int &i)
	{
		if (!i) return head->data;
		
		Node<T>* CurrentNode = head;
		int j{};

		// Check if we can minimize operations by starting off
		// closer to the desired index/node.
		if (i > diff(index_, i))
		{
			CurrentNode = cursor_;
			j = index_;
			operations_saved += i - diff(index_, i);
		}

		if (i > j)
		{
			while (CurrentNode && j++ != i)
			{
				CurrentNode = CurrentNode->next;
				cursor_ = CurrentNode;
				index_ = j;
				operations++;
			}
		}
		else
		{
			while (CurrentNode && j-- != i)
			{
				CurrentNode = CurrentNode->prev;
				cursor_ = CurrentNode;
				index_ = j;
				operations++;
			}
		}
		return CurrentNode->data;
	}
};


template<class T>
DLL<T>::DLL()
{
	srand(time(0));
	size_ = 0;
	head = nullptr;
	tail = head;
	cursor_ = head;
	index_ = 0;
	operations = 0;
}


template <class T>
DLL<T>::DLL(const DLL<T>& obj)
{
	Node<T>* CurrentNode = obj.head;

	// Add head
	auto* newNode = new Node<T>(CurrentNode->data);
	head = newNode;
	CurrentNode = CurrentNode->next;

	if (obj.size() > 1)
	{
		// Add middle
        	while (CurrentNode)
        	{
        		add(new Node<T>(CurrentNode->data));
        		CurrentNode = CurrentNode->next; // Belongs to obj
        	}
        
        	// Add tail
        	newNode = new Node<T>(CurrentNode->data); // Belongs to us
        	add(newNode);
        	tail = newNode; // Belongs to us
	}
}


template<class T>
DLL<T>::~DLL()
{
	printf("Deleting DLL. Operations performed: %d\nOperations saved: %d\n",
		operations, operations_saved);
	
	if (!head) return;
	
	Node<T>* PrevNode = head;
	while (head)
	{
		PrevNode = head;
		head = head->next;
		delete PrevNode;
	}
}


template <class T>
Node<T>* DLL<T>::at(const int &i)
{
	Node<T>* CurrentNode = head;
	int j{};
	
	while (CurrentNode && j++ != i)
		CurrentNode = CurrentNode->next;

	if (CurrentNode) return CurrentNode;
	return tail;
}


template<class T>
int DLL<T>::add(T data)
{
	auto* NewNode = new Node<T>;
	NewNode->data = data;
	NewNode->next = nullptr;

	if (!head)
	{
		head = NewNode;
	}
	if (tail)
	{
		tail->next = NewNode;
		NewNode->prev = tail;
	}
	tail = NewNode;
	
	return ++size_;
}


template <class T>
int DLL<T>::add(T arr[], const int &size)
{
	if (!arr || !size) return size_;
	
	// Use our existing add function for space efficiency
	for (int i{}; i < size; i++)
	{
		add(arr[i]);
	}

	/* We don't need any math here because the list size
	 * is taken care of in the add() function. */
	return size_;
}


template <class T>
int DLL<T>::addFront(T arr[], const int& size)
{
	// We iterate backwards to insert the array in the
	// correct order
	for (int i = size; i >= 0; i--)
	{
		insert(arr[i], 0);
	}
	return size_;
}


template <class T>
int DLL<T>::fill(fillType type, int min, int max, int amount)
{
	if (!amount) return size_;
	if (amount == -1) amount = max - min + 1;

	// Convert to float for decimal temp calculations.
	const float diff = max - min;
	
	switch (type)
	{
	case rand:
		for (int i{}; i < amount; i++)
		{
			add(FMath::Rand() % max + min);
		}
		break;
	case asc:
		add(min);
		if (amount > 1)
		{
			for (int i = 1; i < amount - 1; i++)
			{
				// Get proper spacing in case diff between min/max
				// is greater than amount.
				add(diff / (amount - 1) * i);
			}
			add(max);
		}
		break;
	case desc:
		add(max);
		if (amount > 1)
		{	// I have no idea how or why this works
			for (int i = amount - 2; i > 0; i--)
			{
				// Get proper spacing in case diff between min/max
				// is greater than amount.
				add(diff / (amount - 1) * i);
			}
			add(min);
		}
		break;
	}
	return size_;
}

template <class T>
void DLL<T>::clear()
{
	if (!head) return;
	
	Node<T>* PrevNode = head;
	while (head)
	{
		PrevNode = head;
		head = head->next;
		delete PrevNode;
	}
	head = nullptr;
	tail = head;
	size_ = 0;
	cursor_ = head;
	index_ = 0;
}


template<class T>
int DLL<T>::remove(const int index)
{
	if (index >= size_) return size_;
	
	// Delete last element if no argument is given
	if (index == -1 || index == size_ - 1)
	{
		tail = tail->prev;
		delete tail->next;
		return --size_;
	}
	
	Node<T>* CurrentNode = head;

	// Iterate through list until we find the desired index
	for (int i{}; i < index; i++)
	{
		CurrentNode = CurrentNode->next;
	}

	// Clean up pointers
	if (CurrentNode->prev) CurrentNode->prev->next = CurrentNode->next;
	CurrentNode->next->prev = CurrentNode->prev;
	delete CurrentNode;

	// Make sure our cursor doesn't go out of scope
	if (index_ >= size_ - 1)
	{
		index_ = size_ - 1;
		cursor_ = tail;
	}
	
	return --size_;
}

template <class T>
int DLL<T>::remove(Node<T>* node)
{
	// Clean up pointers
	if (node->prev) node->prev->next = node->next;
	if (node->next) node->next->prev = node->prev;
	delete node;

	// Make sure our cursor doesn't go out of scope
	if (index_ >= size_ - 1)
	{
		index_ = size_ - 1;
		cursor_ = tail;
	}
	
	return --size_;
}


template<class T>
int DLL<T>::insert(T data, const int index)
{
	Node<T>* IndexNode = this->at(index);
	// Since the user only has the data, we create a node to insert
	auto* NewNode = new Node<T>();
	
	// Prep new node
	NewNode->data = data;
	NewNode->next = IndexNode;
	
	// Clean up pointers
	// Incase the indexnode is the head, we check for nullptr on prev
	if (IndexNode->prev)
	{
		NewNode->prev = IndexNode->prev;
		IndexNode->prev->next = NewNode;
		IndexNode->prev = NewNode;
	} else
	{
		head = NewNode;
	}
	
	return ++size_;
}


template<class T>
int DLL<T>::insert(Node<T>* node, const int index)
{
	Node<T>* IndexNode = this->at(index);

	// Since the user already has a node, we just work with
	// that off the bat. 
	node->next = IndexNode;

	// Clean up pointers
	// Incase the indexnode is the head, we check for nullptr on prev
	if (IndexNode->prev)
	{
		node->prev = IndexNode->prev;
		IndexNode->prev->next = node;
		IndexNode->prev = node;
	} else
	{
		head = node;
	}
	
	return ++size_;
}


template <class T>
int DLL<T>::clearDuplicates()
{
	int beforesize = size_;
	auto* CurrentNode = head;
	while (CurrentNode->next)
	{
		if (CurrentNode->next->data == CurrentNode->data)
		{
			remove(CurrentNode->next);
		}
		else
		{
			CurrentNode = CurrentNode->next;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("Removed %d dupes."), beforesize - size_);
	return size_;
}


template<class T>
void DLL<T>::print(const direction dir)
{
	Node<T>* CurrentNode = head;
	int i{};
	UE_LOG(LogTemp, Warning, TEXT("[ "));
	switch (dir)
	{
	case forward:
		CurrentNode = head;
		while (CurrentNode)
		{
			if (i++ >= size_) return;
			UE_LOG(LogTemp, Warning, TEXT("%d "), CurrentNode->data);
			CurrentNode = CurrentNode->next;
		}
		break;
	case backwards:
		CurrentNode = tail;
		while (CurrentNode)
		{
			if (i++ >= size_) return;
			UE_LOG(LogTemp, Warning, TEXT("%d "), CurrentNode->data);
			CurrentNode = CurrentNode->prev;
		}
		break;
	}
	UE_LOG(LogTemp, Warning, TEXT("]"), CurrentNode->data);
}