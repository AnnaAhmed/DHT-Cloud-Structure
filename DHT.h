
#pragma once
#include "FingerTable.h"
#include <functional> //to implement hash func
#include <string>
#include <cmath>
#pragma once

//An object from the class hash is created to hash the string input
template <typename T>
int hashingfunc(T input, int bitsize) {
	int totalSize = pow(2, bitsize);
	hash<T> createhash;
	unsigned int tmp = createhash(input);
	int output = tmp % totalSize;
	return output;
}


template <typename T,typename U>
class DHT {
private:
	T bitsize;			//bitsize of the ring DHT
	T totalmachines;	//number of machines
	Machine<T,U>* head;	//head of the DHT List
	Machine<T,U>* tail;	//tail of the DHT List
	T* arr;	//Array stores all the machine Ids dynamically during the execution and according to the changes made in the DHT
public:
	//Default Constructor
	DHT() {
		head = new Machine<T, U>;
		head = NULL;
		tail = new Machine<T, U>;
		tail = NULL;
		bitsize = 0;
		totalmachines = 0;
		arr = NULL;
	}
	
	//Parametrized Constructor for Auto Machine assigning
	DHT(T input1, T input2, U* tmparr) {

		cout << "Auto Machine Assigning" << endl;

		head = new Machine<T, U>;
		tail = new Machine<T, U>;

		head = NULL;
		tail = NULL;
		bitsize = input1;
		totalmachines = input2;

		//removeDupArray allows unique machine ids to be inserted
		T* removeDupArray = new T[totalmachines];
		for (int i = 0; i < totalmachines; i++)
		{
			removeDupArray[i] = -1;
		}

		//Unique machine IDs are entered in the DHT by the following code
		for (int i = 0; i < totalmachines; i++)
		{
			cout << "Enter machine " << i + 1 << ": " << endl;
			cin >> tmparr[i];

			T tmp = hashingfunc(tmparr[i], bitsize);

			bool check = true; 
			for (int j = 0; j < totalmachines; j++)
			{
				T tmp2 = removeDupArray[j];
				if (tmp2 == tmp) {
					check = false;
					cout << "Machine with id " << tmp2 << " already present enter again !" << endl;
					i--;
					break;
				}

				if (tmp2 == -1) {
					removeDupArray[j] = tmp;
					break;
				}

			}

			if (check == true) {
				DHT_list_insert(tmp);
			}
		}

		make_all_tables();	//All the FingerTables of the machines are made
		functionality();	//Now the program proceeds to all the functionalities

	}

	//Paramterized Constructor for Manual Machine Assigning
	DHT(T input1, T input2) {

		cout << "Manual Machine Assigning " << endl;

		head = new Machine<T, U>;
		tail = new Machine<T, U>;
		head = NULL;
		tail = NULL;
		bitsize = input1;
		totalmachines = input2;

		T totalSize = pow(2, bitsize); //converting bit size into total possibilites

		//removeDupArray allows unique machine ids to be inserted
		T* removeDupArray = new T[totalmachines];
		for (int i = 0; i < totalmachines; i++)
		{
			removeDupArray[i] = -1;
		}

		//Unique machine IDs are entered in the DHT by the following code
		for (int i = 0; i < totalmachines; i++)
		{
			T tmp;
			while (1) {
				while (1)
				{
					cout << "Enter Machine ID " << i + 1 << " : ";
					cin >> tmp;

					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}
				if (tmp >= totalSize) {
					cout << "Invalid ID!" << endl;
				}
				else {
					break;
				}

			}

			bool check = true; 
			
			for (int j = 0; j < totalmachines; j++)
			{
				T tmp2 = removeDupArray[j];
				if (tmp2 == tmp) {
					check = false;
					cout << "ID already present" << endl;
					i--;
					break;
				}

				if (tmp2 == -1) {
					removeDupArray[j] = tmp;
					break;
				}

			}

			if (check == true) {
				DHT_list_insert(tmp);
			}

		}
		make_all_tables();	//All the FingerTables of the machines are made
		functionality();	//Now the program proceeds to all the functionalities
	}

	//Main Menu of the program which gives options to run all the functionalities of the DHT
	void functionality() {
		while (1) {

			T input;
			cout << "Enter 1 to insert data " << endl;
			cout << "Enter 2 to remove data " << endl;
			cout << "Enter 3 to print any machine's routing table " << endl;
			cout << "Enter 4 to print any machine's AVL TREE " << endl;
			cout << "Enter 5 to search and retrieve value" << endl;
			cout << "Enter 6 to insert a new machine in DHT" << endl;
			cout << "Enter 7 to remove a machine from DHT" << endl;
			cout << "Enter 8 to display all machines" << endl;
			cout << "Enter anything else to quit" << endl;

			while (1)
			{
				cout << "Enter : ";
				cin >> input;

				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}

			if (input == 1) {
				insert_data();
			}
			else if (input == 2) {
				delete_data();
			}
			else if (input == 3) {
				print_routingTable();
			}
			else if (input == 4) {
				print_avl();
			}
			else if (input == 5) {
				search_value();
			}
			else if (input == 6) {
				Insert_Machine();
			}
			else if (input == 7)
			{
				Remove_Machine();
			}
			else if (input == 8)
			{
				display_machines();
			}
			else {
				return;
			}

		}

	}

	//This returns the bitsize of the DHT
	T get_bitsize() {
		return bitsize;
	}

	//This returns the number of totalmachines in the DHT 
	T get_totalmachines() {
		return totalmachines;
	}

	//This function adds the machine node in the Ring DHT (Singly Circular Linked List) in ascending order
	void DHT_list_insert(T id) {
		Machine<T, U>* tmp = new Machine<T, U>;
		tmp->set_machineid(id);

		if (head == NULL) {
			head = tmp;
			head->set_next(tmp);
			tail = head;
		}
		else {
			Machine<T, U>* tmp2 = head;
			T index = 0;
			do {
				if (id > tmp2->get_machineid()) {
					index++;
				}
				tmp2 = tmp2->get_next();
			} while (tmp2 != head);


			tmp2 = head;
			if (index == 0) {
				tmp->set_next(tmp2);
				while (tmp2->get_next() != head) {
					tmp2 = tmp2->get_next();
				}
				tmp2->set_next(tmp);
				head = tmp;
			}
			else {
				T i = 0;
				while (i < index - 1) {
					tmp2 = tmp2->get_next();
					i++;
				}
				tmp->set_next(tmp2->get_next());
				tmp2->set_next(tmp);

			}
			tmp2 = head;
			do
			{
				tmp2 = tmp2->get_next();
			} while (tmp2->get_next() != head);
			tail = tmp2;
		}

	}

	//This displays all the available machines in the Ring DHT
	void display_machines() {
		if (head == NULL)
		{
			cout << "No machines available" << endl;
			return;
		}
		cout << "Available machines : " << endl;
		Machine<T, U>* tmp = head;
		do
		{
			T output = tmp->get_machineid();
			cout << output << endl;
			tmp = tmp->get_next();
		} while (tmp != head);

	}

	//This function creates all the FingerTables for all the machine nodes
	void make_all_tables() {
		delete arr;
		arr = new int[totalmachines];
		Machine<T, U>* tmp = head;
		
		for (int i = 0; i < totalmachines; i++)
		{
			T ans = tmp->get_machineid();
			arr[i] = ans;
			tmp = tmp->get_next();
		}

		cout << endl;

		tmp = head;
		//First the FingerTable is created and then the machine node is linked to the FingerTable accordingly
		do {
			FingerTable<T>* obj = new FingerTable<T>;
			obj->createtable(tmp, arr, bitsize, totalmachines);
			tmp->link_table(obj);
			tmp = tmp->get_next();

		} while (tmp != head);

	}


	//This function searches for the appropriate machine using the FingerTables hence reduces the Time Complexity
	//Flag determines whether the path is to be printed or not
	Machine<T, U>* searching_algorithm(T input, Machine<T, U>*& ptr, Machine<T, U>*& ptr2, T flag)
	{
		//checks if input is within the same machine
		T lastval = ptr2->get_machineid();	
		T firstval = ptr->get_machineid();	

		if (firstval == lastval)
		{
			return ptr;
		}

		//Condition of p==e is checked 
		if (lastval > firstval) {
			if (input > lastval || input <= firstval)
			{
				return ptr;
			}
		}

		else if (firstval > lastval) {
			if (input > lastval && input <= firstval)
			{
				return ptr;
			}
		}

		Machine<T, U>* tmp = new Machine<T, U>;
		tmp = ptr;

		//The following algorithm uses the FingerTables and takes jumps according to the algorithm provided in instructions
		while (1) {

			T check = 0;

			FingerTable<T>* obj = tmp->get_table();
			T p = tmp->get_machineid();

			Machine<T, U>* tmp2 = new Machine<T, U>;
			tmp2 = obj->search_byindex(0);
			T ftpFirst = tmp2->get_machineid();

			// p < e and e ≤ FTp [1]
			if (p < input && input <= ftpFirst) {

				tmp = tmp2;
				if (flag == 1) {
					cout << "Forwading request to : " << tmp->get_machineid() << endl;
				}
				check = 1;
				return tmp;
			}

			//Rotation Check
			if (tmp->get_machineid() == tail->get_machineid()) {

				if (input > tail->get_machineid()) {
					tmp = head;
					if (flag == 1) {
						cout << "Forwading request to : " << tmp->get_machineid() << endl;
					}
					check = 1;
					return tmp;
				}

				if (input <= head->get_machineid()) {
					tmp = head;
					if (flag == 1) {
						cout << "Forwading request to : " << tmp->get_machineid() << endl;
					}
					check = 1;
					return tmp;
				}
			}

			Machine<T, U>* largest = new Machine<T, U>;
			T index = 0;
			T large = 0;
			
			for (int i = 0; i < bitsize - 1; i++)
			{
				T ans1, ans2;
				tmp2 = obj->search_byindex(i + 1);
				ans2 = tmp2->get_machineid();
				tmp2 = obj->search_byindex(i);
				ans1 = tmp2->get_machineid();

				if (ans1 >= large) {
					large = ans1;
					index = i;
				}
				if (ans2 >= large) {
					large = ans2;
					index = i + 1;
				}
				//FTp[j] < e ≤ FTp[j + 1]
				if (ans1 < input && input <= ans2) {
					tmp = tmp2;
					if (flag == 1) {
						cout << "Forwading request to : " << tmp->get_machineid() << endl;
					}
					check = 1;
					break;
				}

			}

			//If all conditions are failed, it jumps to the largest 
			if (check == 0) {
				largest = obj->search_byindex(index);
				tmp = largest;
				if (flag == 1) {
					cout << "Forwading request to : " << tmp->get_machineid() << endl;
				}
			
			}
		
		}

	}

	/*The following code takes the machine for query generation and then reaches at the appropriate machine to 
	insert data in the AVL Tree and the File of the machine. */
	void insert_data() {
	
		if (head == NULL)
		{
			cout << "No machines available, returning to options" << endl;
			return;
		}
		display_machines();

		T index;
		T x;

		while (1) {
			while (1)
			{
				cout << "Enter the machine id you want to insert data from : ";
				cin >> x;

				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100,'\n');
				}
				else {
					break;
				}
			}
			T check = 0;
			for (int i = 0; i < totalmachines; i++)
			{
				T ans = arr[i];
				if (ans == x) {
					index = i;
					check = 1;
				}
			}

			if (check == 1) {
				break;
			}

			cout << "Invalid ID" << endl;
		}

		//Start is the machine where the query for insertion is generated
		Machine<T, U>* start = new Machine<T, U>;
		//Previous is its predecessor machine
		Machine<T, U>* previous = new Machine<T, U>;

		//Previous is used to determine whether the query is resolved at the Start machine or will it be forwarded (helps to check p==e condition)
		if (index == 0) {
			start = head;
			previous = tail;
		}
		else {
			T y = arr[index - 1];
			//flag is set zero in searching algorithm in order to not print the path while setting the initial pointers
			previous = searching_algorithm(y, head, tail, 0);	
			start = searching_algorithm(x, head, tail, 0);
		}

		while (1) {
			//now actual insertion begins
			cin.ignore();
			U val;
			U key;
			T input2 = 0;
			
			cout << "Enter the value you wish to insert : ";
			getline(cin, val);
			
			cout << "Enter its key : ";
			getline(cin, key); 
			
			cin.clear();

			T id = hashingfunc(key, bitsize);
			cout << "Id of key generated by hashing : " << id << endl;
			Machine<T, U>* ptr2 = new Machine<T, U>;

			ptr2 = searching_algorithm(id, start, previous, 1);		//flag turned on to print the path
			//Appropriate machine found from searching algorithm
			cout << "At machine : " << ptr2->get_machineid() << endl;
			ptr2->machine_insertdata(id, val);		//machine_insertdata is in machine.h

			while (1)
			{
				cout << "Enter 1 to print Machine ID " << ptr2->get_machineid() << "'s AVL TREE , enter anything else to resume: " << endl;
				cin >> input2;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input2 == 1) {
				ptr2->display_tree();
			}
			
			cout << endl;
			while (1)
			{
				cout << "Enter 1 to enter further data, or enter anything else to exit: " << endl;
				cin >> input2;

				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input2 != 1) {
				break;
			}
		}
	}

	/*The following code asks for the machine for query generation and then reaches at the appropriate machine for that
	query. And then searches for the data from the AVL tree and gets the value from the File. */
	void search_value() {
		if (head == NULL)
		{
			cout << "No machines available, returning to options" << endl;
			return;
		}
		display_machines();

		T index;
		T x;

		while (1) {
			while (1)
			{
				cout << "Enter the machine id you want to start searching from: ";
				cin >> x;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}

			T check = 0;
			for (int i = 0; i < totalmachines; i++)
			{
				T ans = arr[i];
				if (ans == x) {
					index = i;
					check = 1;
				}
			}

			if (check == 1) {
				break;
			}

			cout << "Invalid ID" << endl;
		}
		//Start is the machine where the query for search is generated
		Machine<T,U>* start = new Machine<T, U>;
		//Previous is its predecessor machine
		Machine<T,U>* previous = new Machine<T, U>;

		//Previous is used to determine whether the query is resolved at the Start machine or will it be forwarded (helps to check p==e condition)
		if (index == 0) {
			start = head;
			previous = tail;
		}
		else {
			T y = arr[index - 1];
			//flag is set zero in searching algorithm in order to not print the path while setting the initial pointers
			previous = searching_algorithm(y, head, tail, 0);
			start = searching_algorithm(x, head, tail, 0);
		}

		while (1) {
			//Now the search begins based on the key of the value
			cin.ignore();
			U val;
			U key;
			T input2 = 0;

			cout << "Enter the key you want to search : ";
			getline(cin, key);
			cin.clear();

			T id = hashingfunc(key, bitsize);
			cout << "ID of key generated by hashing : " << id << endl;
			Machine<T,U>* ptr2 = new Machine<T, U>;

			ptr2 = searching_algorithm(id, start, previous, 1);		//flag turned on to print the path
			cout << "At Machine : " << ptr2->get_machineid() << endl;
			//The query has reached the appropriate machine i.e. ptr2 
			
			ptr2->search_Machinedata(id);	//search_Machinedata is in machine.h 
			cout << endl;
		
			while (1)
			{
				cout << "Enter 1 to search further data, or enter anything else to exit: " << endl;
				cin >> input2;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input2 != 1) {
				break;
			}
		
		}
		
	}

	/* The following code takes the machine for query generation and finds the appropriate machine to resolve the query.
	The data is deleted from the AVL Tree and the File	*/
	void delete_data() {
		if (head == NULL)
		{
			cout << "No machines available, returning to options" << endl;
			return;
		}
		display_machines();

		T index;
		T x;

		while (1) {

			while (1)
			{
				cout << "Enter the machine id you want to start deletion from: ";
				cin >> x;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			T check = 0;
			for (int i = 0; i < totalmachines; i++)
			{
				T ans = arr[i];
				if (ans == x) {
					index = i;
					check = 1;
				}
			}

			if (check == 1) {
				break;
			}

			cout << "Invalid ID" << endl;
		}
		//Start is the machine where the query for deletion is generated
		Machine<T, U>* start = new Machine<T, U>;
		//Previous is its predecessor machine
		Machine<T, U>* previous = new Machine<T, U>;

		//Previous is used to determine whether the query is resolved at the Start machine or will it be forwarded (helps to check p==e condition)
		if (index == 0) {
			start = head;
			previous = tail;
		}
		else {
			T y = arr[index - 1];
			//flag is set zero in searching algorithm in order to not print the path while setting the initial pointers
			previous = searching_algorithm(y, head, tail, 0);
			start = searching_algorithm(x, head, tail, 0);
		}

		while (1) {
			//Now the deletion begins based on the key input
			cin.ignore();
			U val;
			U key;
			T input2 = 0;

			cout << "Enter the key you want to delete : ";
			getline(cin, key);
			cin.clear();

			T id = hashingfunc<U>(key, bitsize);
			Machine<T, U>* ptr2 = new Machine<T, U>;

			ptr2 = searching_algorithm(id, start, previous, 1);		//flag turned on to print the path
			cout << "At Machine : " << ptr2->get_machineid() << endl;
			//The query has reached the appropriate machine i.e. ptr2
			
			T deletion_check = ptr2->delete_Machinedata(id);  //delete_Machinedata is in machine.h
			//deletion check receives an int value to check whether deletion took place or not
			cout << endl;
			
			if (deletion_check == 1)
			{
				cout << "Deleted :" << id << endl;
				ptr2->display_tree();
			}
			else {
				cout << "Deletion unsuccessful" << endl;
			}
			
			while (1)
			{
				cout << "Enter 1 to delete further data, or enter anything else to exit: " << endl;
				cin >> input2;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input2 != 1) {
				break; 
			}
		}
	}

	/*The following function prints the routing table of a machine by asking for the machine id and then 
	gets its FingerTable and displays the Doubly Linked List of that FingerTable */
	void print_routingTable() {
		if (head == NULL)
		{
			cout << "No machines available, returning to options" << endl;
			return;
		}
		while (1) {
			display_machines();
			T input;

			T x;
			while (1) {

				while (1)
				{
					cout << "Enter the machine id to print its routing table : ";
					cin >> x;
					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}

				T check = 0;
				for (int i = 0; i < totalmachines; i++)
				{
					T ans = arr[i];
					if (ans == x) {
						check = 1;
					}
				}

				if (check == 1) {
					break;
				}

				cout << "Invalid ID" << endl;
			}
			
			//The machine ptr is searched with the given machine id from the searching algorithm
			Machine<T, U>* ptr = searching_algorithm(x, head, tail, 0);
			
			ptr->get_table()->DisplayList();		//Displays the table
			cout << endl;
			
			while (1)
			{
				cout << "Enter 1 to check more routing tables, or enter anything else to return to main : " << endl;
				cin >> input;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input != 1) {
				break;
			}

		}

	}

	/*The following function prints the AVL Tree of a machine by asking for the machine id and then displays its AVL Tree */
	void print_avl() {
		if (head == NULL)
		{
			cout << "No machines available, returning to options" << endl;
			return;
		}
		display_machines();

		while (1) {

			T returnCheck = 0;

			T x;
			while (1) {

				while (1)
				{
					cout << "Enter the machine id to print its AVL TREE : ";
					cin >> x;
					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}
				T check = 0;
				for (int i = 0; i < totalmachines; i++)
				{
					T ans = arr[i];
					if (ans == x) {
						check = 1;
					}
				}

				if (check == 1) {
					break;
				}

				cout << "Invalid ID" << endl;
			}
			
			//The machine ptr is searched with the given machine id from the searching algorithm
			Machine<T, U>* ptr = new Machine<T, U>;
			ptr = searching_algorithm(x, head, tail, 0);
			
			ptr->display_tree();	//Displays the AVL Tree of that machine

			while (1)
			{
				cout << "Enter 1 to check another Machine's AVL TREE or enter anything else to return to main: " << endl;
				cin >> returnCheck;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (returnCheck != 1) {
				break;
			}

		}
	
	}

	/*The following code removes a machine from the Ring DHT and distributes the data accordingly. It asks for the
	 machine id to be removed. */
	void Remove_Machine()
	{
		T index;
		T x;
		//When all machines are deleted
		if (totalmachines == 0)
		{
			cout << "No machines exist, returning to options" << endl;
			return;
		}

		while (1) {
			
			display_machines();
			
			while (1) {
				
				while (1)
				{
					cout << "Enter the machine id you want to delete : ";
					cin >> x;
					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}
				T check = 0;
				for (int i = 0; i < totalmachines; i++)
				{
					T ans = arr[i];
					if (ans == x) {
						index = i;
						check = 1;
					}
				}

				if (check == 1) {
					break;
				}

				cout << "Invalid ID" << endl;
			}
			//Start is the machine which is to be deleted 
			Machine<T, U>* start = new Machine<T, U>;
			//Previous is its predecessor machine
			Machine<T, U>* previous = new Machine<T, U>;

			if (index == 0) {
				start = head;
				previous = tail;
			}
			else {
				T y = arr[index - 1];
				previous = searching_algorithm(y, head, tail, 0);
				start = searching_algorithm(x, head, tail, 0);
			}
			
			//When there exists only one machine in the DHT 
			if (start == previous)
			{
				head = NULL;
				tail = NULL;
				delete start;
				totalmachines--;
				cout << "No more machines left, now returning to options " << endl;
				return;
			}
			else {
				//Successor is the next machine of the machine to be deleted
				Machine<T, U>* successor = new Machine<T, U>;
				successor = start->get_next();
				
				//The data from the machine to be removed is distributed to the successor machine
				start->distribute_onRemoval(successor);
	
				//Following is the code to delete a node from a Singly Circular Linked List of DHT
				if (start->get_machineid() == head->get_machineid()) {
					if (head->get_next() == head) {
						head = NULL;
						tail = NULL;
						delete start;
					}
					else {
						Machine<T, U>* tmp = new Machine<T, U>;
						tmp = head;
						
						head = tmp->get_next();
						tail->set_next(head);
						delete tmp;
						
						tail->set_next(head);
						if (head == tail)
						{
							head->set_next(tail);
						}
					}
				}
				else {
					previous->set_next(start->get_next());
					if (start->get_machineid() == tail->get_machineid())
					{
						tail = previous;
						tail->set_next(head);
					}
					delete start;
				} 
				
				totalmachines--;
				cout << "Machine deleted successfully" << endl;
				make_all_tables();		//FingerTables of all machines are updated 
				
				T input = 0;
				cout << endl;
				
				while (1)
				{
					cout << "Enter 1 to delete more machines, or enter anything else to return to main : " << endl;
					cin >> input;
					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}
				if (input != 1) {
					break;
				}
			
			}
		
		}
	
	}

	/*The following code inserts a machine in the Ring DHT and distributes the data accordingly. It asks for the
	 machine id to be inserted. */
	void Insert_Machine()
	{
		while (1) {
			T id;

			//When there is no space left in the identifier space of the DHT to add a new machine
			if (totalmachines == pow(2, bitsize)) {
				cout << "Machines Maxed out,cannot enter anymore machines in your system, returning to Options" << endl;
				return;
			}

			while (1) {
				//It asks for Manual or Automatic assignation of machine ID to be inserted
				T input;
				display_machines();
				
				while (1)
				{
					cout << "Enter 1 to automatically assign machine id " << endl;
					cout << "Enter 2 to manually assign machine id " << endl;
					cin >> input;
					//Validation to check if cin is in the fail state or not 
					if (cin.fail())
					{
						cout << "Please enter an integer" << endl;
						cin.clear();
						cin.ignore(100, '\n');
					}
					else {
						break;
					}
				}
				if (input == 1)
				{
					while (1) {
						U x;
						cout << "Auto machine assigning" << endl;
						while (1)
						{
							cout << "Enter the machine id you want to insert : ";
							cin >> x;
							//Validation to check if cin is in the fail state or not 
							if (cin.fail())
							{
								cout << "Please enter an integer" << endl;
								cin.clear();
								cin.ignore(100, '\n');
							}
							else {
								break;
							}
						}
						id = hashingfunc(x, bitsize);

						T check = 0;
						for (int i = 0; i < totalmachines; i++)
						{
							T ans = arr[i];
							if (ans == id) {
								check = 1;
							}
						}

						if (check != 1) {
							break;
						}

						cout << "Machine ID Already present" << endl;
					}
					break;
				}
				else if (input == 2) {

					while (1) {
						cout << "Manual Machine Assigning" << endl;
						
						while (1)
						{
							cout << "Enter the machine id you want to insert : ";
							cin >> id;
							//Validation to check if cin is in the fail state or not 
							if (cin.fail())
							{
								cout << "Please enter an integer" << endl;
								cin.clear();
								cin.ignore(100, '\n');
							}
							else {
								break;
							}
						}
						T check = 0;
						for (int i = 0; i < totalmachines; i++)
						{
							T ans = arr[i];
							if (ans == id) {
								check = 1;
							}
						}

						if (check != 1) {
							break;
						}
						cout << "Machine ID Already present" << endl;
					}
					break;
				}
				else {
					cout << "Invalid Input,Enter Again" << endl;
				}
			}

			DHT_list_insert(id);	//machine is inserted in the ring 
			totalmachines++;
			make_all_tables();		//FingerTables are updated 

			//ptr is the machine inserted
			Machine<T, U>* ptr = new Machine<T, U>;
			ptr = searching_algorithm(id, head, tail, 0);
			
			//Successor is the next machine 
			Machine<T, U>* successor = ptr->get_next();

			/*Data from the successor is distributed to the new machine based on the IDs which are within
			the range of the new machine */
			successor->distribute_onAddition(ptr);
			cout << "Machine Added Successfully" << endl;

			T input2 = 0;
			while (1)
			{
				cout << "Enter 1 to add more machines, Enter anything else to return to Options: ";
				cin >> input2;
				//Validation to check if cin is in the fail state or not 
				if (cin.fail())
				{
					cout << "Please enter an integer" << endl;
					cin.clear();
					cin.ignore(100, '\n');
				}
				else {
					break;
				}
			}
			if (input2 != 1) {
				break;
			}
		
		}

	}

	//Deconstructor 
	~DHT()
	{
		//deletes the Singly Circular Linked List of DHT
		Machine<T,U>* tmp = head;
		Machine<T,U>* tmp2 = NULL;
		while (tmp != tail)
		{
			tmp2 = tmp->get_next();
			delete tmp;
			tmp = tmp2;
		}
		delete tail;
		head = NULL;
		tail = NULL;
	}
};