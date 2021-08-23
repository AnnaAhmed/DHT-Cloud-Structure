

#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

template<typename T,typename U>
class AVLtree {
private:
	struct node {
		node* left;
		node* right;
		T ID;
		U filePath;
		T lineNum;
	};

	node* root;
	
	node* linkedlist_Head;	//Head of the Singly Linked List created from AVL Tree for certain calculations 
	
	T insertion_check;//It is used as return value to machine to confirm whether data has been inserted in AVL tree or not
	
	//temp and height are being used to calculate height in avl
	T temp;
	T height;
public:
	AVLtree()
	{
		root = new node;
		root = NULL;
		linkedlist_Head = NULL;
		insertion_check = 0;
		temp = 0;
		height = 0;
	}

	//This function calculates height of the node in AVL Tree 
	void getheight(node* tmp) {
		if (tmp == NULL) {
			return;
		}
		if (tmp->left != NULL) {
			temp++;
			getheight(tmp->left);
		}
		if (tmp->right != NULL) {
			temp++;
			getheight(tmp->right);
		}
		if (height < temp) {
			height = temp;
		}
		temp--;
		return;
	}

	//Calculates the height difference between left and right sides of a node
	//It is used for balancing the tree 
	T height_diff(node* tmp) {
		getheight(tmp->left);
		T l = height;
		temp = 0;
		height = 0;

		getheight(tmp->right);
		T r = height;
		temp = 0;
		height = 0;

		T height_difference = l - r;
		return height_difference;
	}

	//Rotation Functions for balancing the AVL tree
	node* rotateRight(node* previous) {
		node* tmp;
		tmp = previous->left;
		previous->left = tmp->right;
		tmp->right = previous;
		return tmp;
	}

	node* rotateLeft(node* previous) {
		node* tmp;
		tmp = previous->right;
		previous->right = tmp->left;
		tmp->left = previous;
		return tmp;
	}

	//Balancing function called during insertion and deletion to balance the AVL tree
	node* balance_tree(node* tmp) {
		T balance_factor = height_diff(tmp);
		if (balance_factor > 1) {
			T left_balance_factor = height_diff(tmp->left);
			if (left_balance_factor > 0) {
				tmp = rotateRight(tmp);
			}
			else {
				tmp->left = rotateLeft(tmp->left);
				tmp = rotateRight(tmp);
			}

		}
		else if (balance_factor < -1) {
			T right_balance_factor = height_diff(tmp->right);
			if (right_balance_factor < 0) {
				tmp = rotateLeft(tmp);
			}
			else {
				tmp->right = rotateRight(tmp->right);
				tmp = rotateLeft(tmp);

			}
		}
		return tmp;
	}

	//It inserts a node in an AVL tree and each node has ID, FilePath and Line Number
	node* insert_avltree(node*& tmp1, T ID, string path, T line) {

		if (tmp1 == NULL) {
			node* tmp = new node;
			tmp->left = NULL;
			tmp->right = NULL;
			tmp->ID = ID;
			tmp->filePath = path;
			tmp->lineNum = line;
			tmp1 = tmp;
		}
		else if (ID < tmp1->ID) {
			tmp1->left = insert_avltree(tmp1->left, ID, path, line);
			tmp1 = balance_tree(tmp1);
		}
		else if (ID > tmp1->ID) {
			tmp1->right = insert_avltree(tmp1->right, ID, path, line);
			tmp1 = balance_tree(tmp1);
		}
		else {
			cout << "DUPLICATE NOT ALLOWED" << endl;
			insertion_check = 1;
		}
		return tmp1;
	}

	//It deletes a node from an AVL Tree on the basis of the given ID 
	node* delete_value(node*& tmp, T ID) {
		if (tmp == NULL) {
			return tmp;
		}
		else if (ID < tmp->ID) {
			tmp->left = delete_value(tmp->left, ID);
		}
		else if (ID > tmp->ID) {
			tmp->right = delete_value(tmp->right, ID);
		}
		else {
			if (tmp->left == NULL && tmp->right == NULL) {
				tmp = NULL;
				delete tmp;
			}
			else if (tmp->left == NULL) {
				node* tmp2 = tmp;
				tmp = tmp->right;
				delete tmp2;

			}
			else if (tmp->right == NULL) {
				node* tmp2 = tmp;
				tmp = tmp->left;
				delete tmp2;
			}
			else {
				node* tmp2 = tmp->right;
				while (tmp2->left != NULL) {
					tmp2 = tmp2->left;
				}
				tmp->ID = tmp2->ID;
				tmp->filePath = tmp2->filePath;
				tmp->lineNum = tmp2->lineNum;
				tmp->right = delete_value(tmp->right, tmp2->ID);
			}
		}

		if (tmp == NULL) {
			return tmp;
		}
		else {
			tmp = balance_tree(tmp);
		}
		return tmp;
	}

	//It retrieves a node's data on the basis of the ID given 
	//It is a recursive function
	node* retrieve_rec(node*& ptr, T ID) {
		if (ptr == NULL) {
			return NULL;
		}
		if (ptr->ID == ID) {
			return ptr;
		}
		else {
			if (ID < ptr->ID) {
				retrieve_rec(ptr->left, ID);
			}
			else {
				retrieve_rec(ptr->right, ID);
			}
		}
	}

	
	//It traverses the AVL tree in inorder 
	/*The flag decides whether inorder traversal is being used to print AVL OR 
	It is being used to insert in the singly linked list of the AVL */
	void inorder_traversal(node* tmp, T flag) {
		if (tmp == NULL) {
			return;
		}
		inorder_traversal(tmp->left, flag);
		if (flag == 1)
		{
			cout << "ID= " << tmp->ID << " " << endl;
			cout << "File Path= " << tmp->filePath << " " << endl;
			cout << "Line Number= " << tmp->lineNum << " " << endl;
			cout << endl;
		}
		else if (flag == 0)
		{
			insert_linked_list(tmp->ID, tmp->filePath, tmp->lineNum);
		}
		inorder_traversal(tmp->right, flag);
	}

	//It inserts a node in a singly linked list havin data as ID, filePath and Line Number
	//It uses the right pointer as next to create the singly linked list 
	void insert_linked_list(T ID, U path, T line)
	{
		node* tmp = new node;
		tmp->right = NULL;
		tmp->left = NULL;
		tmp->ID = ID;
		tmp->filePath = path;
		tmp->lineNum = line;
		if (linkedlist_Head == NULL)
		{
			linkedlist_Head = tmp;
		}
		else
		{
			node* tmp2 = linkedlist_Head;
			while (tmp2->right != NULL)
			{
				tmp2 = tmp2->right;
			}
			tmp2->right = tmp;
		}
	}

	//this searches for a node in the AVL TREE based on ID
	void search(T id, U& path, T& line) {
		node* ptr = new node;
		ptr = retrieve_rec(root, id);
		if (ptr == NULL) {
			path = "";
			line = -1;
		}
		else {
			path = ptr->filePath;
			line = ptr->lineNum;
		}
		return;
	}

	//this function creates the linked List using inorder traversal and the value of flag ==0(which causes insertion)
	void create_linked_list()
	{
		inorder_traversal(root, 0);
	}

	// searching the singly linked list by index provided
	void search_list_byIndex(T index, T& ID, U& path, T& line)
	{
		node* tmp = new node;
		tmp = linkedlist_Head;
		T i = 0;
		while (i < index)
		{
			tmp = tmp->right;
			i++;
		}
		ID = tmp->ID;
		path = tmp->filePath;
		line = tmp->lineNum;
	}

	//displays the singly linked list
	void display_linkedlist()
	{
		node* tmp = new node;
		tmp = linkedlist_Head;
		while (tmp != NULL)
		{
			cout << "ID of node is : " << tmp->ID << endl;
			tmp = tmp->right;
		}
	}

	//deletes the linked list entirely
	void delete_linkedlist()
	{
		node* tmp = linkedlist_Head;
		node* tmp2 = NULL;
		while (tmp != NULL)
		{
			tmp2 = tmp->right;
			tmp = NULL;
			delete tmp;
			tmp = tmp2;
		}
		linkedlist_Head = NULL;
	}

	//this function display the AVL tree by using inorder traversal and flag ==1(which causes inorder function to display)
	void display_avl() {
		if (root == NULL)
		{
			cout << "Tree is empty" << endl;
			return;
		}
		else
		{
			inorder_traversal(root, 1);
		}
	}

	//takes input of an ID,path and line number to input in the tree
	int insert_in_tree(T ID, U path, T line) {
		insertion_check = 0;
		root = insert_avltree(root, ID, path, line);
		return insertion_check;
	}

	//deletes a certain ID node in the tree
	void delete_in_tree(T ID) {
		root = delete_value(root, ID);
	}

	//this deletes the entire AVL tree
	void delete_tree(node*& tmp)
	{
		if (tmp == NULL)
		{
			return;
		}
		delete_tree(tmp->left);
		delete_tree(tmp->right);
		delete tmp;
	}

	//the deconstructor calls the delete functions to erase the tree and linked list
	~AVLtree()
	{
		delete_tree(root);
		delete_linkedlist();
	}

};

//forward declaration of FingerTable class
template <typename T>
class FingerTable;


template <typename T,typename U>
class Machine
{
private:
	T id;						//ID of machine
	T lineNo;	//This variable is used to allocate lineNo to values in files and then insert them in AVL Tree
	FingerTable <T>* table;		//This is the FingerTable for the machine
	Machine* next;				//This is used to point to next node in DHT
	AVLtree<T,U>* tree;			//This is the AVL Tree of a machine node 
	T valueCount;				//This tells the total number of values stored in the machine
	T noOfFiles;				//This tells the total number of files present in the machine
	double file_check;			//This variable is used during insertion to maintain 100 values per file
public:
	//Default Constructor
	Machine() {
		id = -1;
		table = NULL;
		next = NULL;
		tree = new AVLtree<T,U>;
		lineNo = 1;
		valueCount = 0;
		noOfFiles = 1;
		file_check = 10.0;
	}
	
	//It sets the machine ID of a machine
	void set_machineid(T x) {
		id = x;
	}
	
	//It returns the machine iD of a machine
	T get_machineid() {
		return id;
	}

	//It sets the next pointer of a machine 
	void set_next(Machine<T,U>* tmp) {
		next = tmp;
	}

	//It returns the next pointer of a machine
	Machine<T,U>* get_next() {
		return next;
	}

	//It returns the AVL tree of a machine
	AVLtree<T,U>* get_tree() {
		return tree;
	}
	
	/*The FingerTable is created in DHT and then this function is called to 
	link the machine to its FingerTable	*/
	void link_table(FingerTable <T>* tmp) {
		table = tmp;
	}

	//This returns the FingerTable of a machine
	FingerTable<T>* get_table() {
		return table;
	}

	//This function inserts the value in a file and stores the ID,FilePath and Line Number in the AVL Tree
	void machine_insertdata(T ID, U value) {

		valueCount++;

		//For maintaining 100 values in 1 file 
		double ans = 0.0;
		ans = double(valueCount / 10.0);
		if (ans > file_check)
		{
			noOfFiles++;
			lineNo = 1;
			file_check = file_check + 10;
		}

		
		ofstream output;
		U strID = to_string(id);
		//Standard format for File Name (depends on the file number)
		U filename = "machine" + strID + "(" + to_string(noOfFiles) + ")" + ".txt";	

		/*file_insert receives an int value from insert_in_tree which decides
		whether the data was inserted in tree */
		T file_insert = tree->insert_in_tree(ID, filename, lineNo);
		
		//If data is successfully inserted in tree, then the value is inserted in the file
		if (file_insert == 0)
		{
			output.open(filename, ios::out | ios::app);

			output << value << endl;

			output.close();
			lineNo++;
		}
		else
		{
			valueCount--;
		}
	}

	//This function helps to delete empty files
	void delete_fileCheck()
	{
		//Checking if file has no values 
		if (valueCount == 0)
		{
			delete tree;
			for (int i = 0; i < noOfFiles; i++)
			{
				U filename = "machine" + to_string(id) + "(" + to_string(i + 1) + ")" + ".txt";
				remove(filename.c_str());	//the file is removed
			}
			//Machine variables are reset
			tree = new AVLtree<T,U>;
			lineNo = 1;
			valueCount = 0;
			noOfFiles = 1;
			file_check = 10;
		}
	}

	//This function deletes data from the AVL Tree and value is deleted from the file
	T delete_Machinedata(T id) {
		U inputLine;
		U value;
		U path;
		T line;
		//Given ID is searched from the Tree and path and line are sent by reference
		tree->search(id, path, line);
		if (path == "") {
			cout << "ID NOT FOUND" << endl;
			return 0;
		}
		else {
			bool found = false;
			ifstream input;
			ofstream output;
			output.open("tmp.txt");	//A temporary file created 
			input.open(path);
			
			T count = 1;
			T check = 1;
			//The value to be deleted is not copied in the new file
			while (!input.eof()) {
				getline(input, inputLine);
				if (line == count) {	
					output << endl;	//Blank Line is placed from where value is deleted
					found = true;
				}
				else {
					output << inputLine;
					if (check != valueCount) {
						output << endl;
					}
					check++;
				}
				count++;
			}
			output.close();
			input.close();
			if (found == true) {
				remove(path.c_str());				//previous file is deleted
				rename("tmp.txt", path.c_str());	//temp file is renamed 
				tree->delete_in_tree(id);			//ID deleted from Tree
				valueCount--;
				delete_fileCheck();					//Checking if file is empty
				return 1;
			}
			else {
				return 0;
			}

		}

	}

	//This function searches the data from the AVL Tree and then gets the value from the File
	void search_Machinedata(T id) {
		U inputLine;
		U value;
		U path;
		T line;
		//Given ID is searched from the Tree and path and line are sent by reference
		tree->search(id, path, line);
		if (path == "") {
			cout << "ID NOT FOUND" << endl;
		}
		else {
			bool found = false;
			ifstream input;
			input.open(path);
			T count = 1;
			//Finding Value from the File with received Line Number and Path
			while (!input.eof()) {
				getline(input, inputLine);
				if (line == count) {
					found = true;
					break;
				}
				count++;
			}
			input.close();
			if (found == false) {
				cout << "Data not found in file" << endl;
				return;
			}

			U value = inputLine;
			cout << "ID = " << id << endl;
			cout << "Value= " << value << endl;

		}
	}

	/* This function is called from DHT when a new machine is inserted into the Ring. The function 
	 is called by the successor of the Machine and the new machine is passed as a parameter.
	 Data from the successor is distributed to the new machine based on the IDs which are within
	the range of the new machine */
	void distribute_onAddition(Machine*& ptr) {
		T prevID = ptr->get_machineid();
		T circular_check = 0;
		
		/*Circular Check checks whether the successor machine ID is smaller than the new machien ID.
		Therefore efficiently storing data in the DHT */
		if (id < prevID)
		{
			circular_check = 1;
		}
		tree->delete_linkedlist();	
		tree->create_linked_list();	
		//Linked List of the AVL Tree is created in ascending order of IDs 
		T tmpValueCount = valueCount;
		for (int i = 0; i < tmpValueCount ; i++) {
			T ID;
			U path;
			T line;
			U inputLine;
			bool found = false;
			tree->search_list_byIndex(i, ID, path, line);

			//Now the Following code distributes the data from the successor to the new machine
			if(circular_check==1){
				
				if (ID <= prevID)
				{
					if (ID > id)
					{
						U inputLine;
						bool found = false;
						ifstream input;
						input.open(path);
						T count = 1;
						while (!input.eof()) {
							getline(input, inputLine);
							if (line == count) {
								found = true;
								break;
							}
							count++;
						}
						input.close();
						if (found == false) {
							cout << "Data not found in file" << endl;
							return;
						}
						else
						{
							U value = inputLine;
							ptr->machine_insertdata(ID, value);
							delete_Machinedata(ID);
						}
					}
				}
			}
			else {
				if (ID <= prevID) {
					U inputLine;
					bool found = false;
					ifstream input;
					input.open(path);
					T count = 1;
					while (!input.eof()) {
						getline(input, inputLine);
						if (line == count) {
							found = true;
							break;
						}
						count++;
					}
					input.close();
					if (found == false) {
						cout << "Data not found in file" << endl;
						return;
					}
					else
					{
						U value = inputLine;
						ptr->machine_insertdata(ID, value);
						delete_Machinedata(ID);
					}
				}
			}
		}
	}

	/* This function is called in the DHT by the machine to be removed and the successor machine is passed
	as the parameter. The data from the machine to be removed is distributed to the successor machine. */
	void distribute_onRemoval(Machine*& ptr)
	{
		tree->delete_linkedlist();
		tree->create_linked_list();
		//Linked List of the AVL Tree is created in ascending order of IDs 

		//Now the Following code distributes the data from the machine to the successor
		for (int i = 0; i < valueCount ; i++)
		{
			T ID;
			U path;
			T line;
			U inputLine;
			bool found = false;
			tree->search_list_byIndex(i, ID, path, line);
	
			ifstream input;
			input.open(path);
			T count = 1;
			while (!input.eof()) {
				getline(input, inputLine);
				if (line == count) {
					found = true;
					break;
				}
				count++;
			}
			input.close();
			if (found == false) {
				cout << "Data not found in file" << endl;
				return;
			}
			else
			{
				U value = inputLine;
				ptr->machine_insertdata(ID, value);
			}
		}
	}

	//This function displays the AVL tree of the machine
	void display_tree() {
		cout << "TREE of Machine " << id << ": " << endl;
		tree->display_avl();
	}

	//Destructor of Machine
	~Machine() {
		//deletes the tree
		delete tree;
		delete table;
		//deletes the files of the machine
		for (int i = 0; i < noOfFiles; i++)
		{
			U filename = "machine" + to_string(id) + "(" + to_string(i + 1) + ")" + ".txt";
			remove(filename.c_str());
		}
	}

};










