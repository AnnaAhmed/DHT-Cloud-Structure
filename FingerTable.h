
#pragma once
#include "Machine.h"
#include <iostream>
#include<cmath>
using namespace std;

//Node data for Doubly Linked List of FingerTable
template <typename T>
struct doublynode {
	doublynode<T>* next;
	doublynode<T>* previous;
	T machineID = -1;						//This will store the ID of the machine pointer in FingerTable
	Machine<T,string>* ptr = new Machine<T, string>;			//each node of FingerTable points to a machine 
};


template <typename T>
class FingerTable {
private:
	T bitsize;				
	T totalmachines;
	doublynode<T>* head;
public:
	FingerTable() {
		bitsize = 0;
		totalmachines = 0;
		head = NULL;
	}

	//Insertion Code for Doubly Linked List
	void insert(T id, Machine<T,string>*& tmp) {
		doublynode<T>* current_node = head;
		doublynode<T>* newNode = new doublynode<T>;
		newNode->next = NULL;
		newNode->previous = NULL;
		newNode->machineID = id;

		/*Now we are using the initial node to traverse over the DHT to find the node with
		the same id as passed in the function*/
		while (1) {
			if (tmp->get_machineid() == id) {
				newNode->ptr = tmp;
				break;
			}
			else {
				tmp = tmp->get_next();
			}
		}

		if (current_node == NULL)	//first node insertion
		{
			head = newNode;
		}
		else          //insertion at end 
		{
			while (current_node->next != NULL)
			{
				current_node = current_node->next;
			}
			current_node->next = newNode;
			newNode->previous = current_node;
		}
	}

	//To find successor of an ID 
	T successor(T I, T* arr)
	{
		T totalvalues = pow(2, bitsize);
		//if ID is greater than totalvalues, totalvalues are subtracted so it can wrap around the circular DHT
		if (I > totalvalues) {      
			I = I - totalvalues;
		}
		T ans = 0;
		T loop = 0;
		for (int j = 0; j < totalmachines; j++)  
		{
			ans = arr[loop];

			if (ans < I)
			{
				loop++;
			}
			else if (ans == I)
			{
				return ans;
			}
			else if (ans > I)
			{
				return ans;
			}
		}
		ans = arr[0];
		return ans;
	}

	/*Creates FingerTable of a machine node by using a dynamically allocated array of all 
	the machine IDs in DHT (The array is updated in DHT) */
	void createtable(Machine<T,string>* ptr, T* arr, T bits, T totalm)
	{
		bitsize = bits;
		totalmachines = totalm;
		T machineId = ptr->get_machineid();

		T id = 0;
		T id2 = 0;
		
		//Finds successor of ID and inserts that in the list 
		for (int i = 1; i <= bitsize; i++)   
		{
			id = machineId + (pow(2, i - 1));  //calculated using FTp[i]=succ(p+2^i-1)
			id2 = successor(id, arr);
			insert(id2, ptr);
		}
	}

	//It searches a doubly list of Fingertable and returns a machine pointer at the index given
	Machine<T,string>* search_byindex(T index){
		doublynode<T>* temp = new doublynode<T>;
		temp = head;
		T loop = 0;
		while (temp != NULL)
		{
			if (loop == index)
			{
				return temp->ptr;
			}
			temp = temp->next;
			loop++;
		}
		return NULL;
	}

	//Displaying List of a FingerTable
	void DisplayList()
	{
		doublynode<T>* temp = new doublynode<T>;
		temp = head;
		while (temp != NULL)
		{
			cout << temp->ptr->get_machineid();
			cout << endl;
			temp = temp->next;
		}
	}

	~FingerTable()
	{
		//Deleting doubly linked list 
		doublynode<T>* tmp = head;
		doublynode<T>* tmp2 = NULL;
		while (tmp != NULL)
		{
			tmp2 = tmp->next;
			delete tmp;
			tmp = tmp2;
		}
		head = NULL;
	}
};
