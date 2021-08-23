
#pragma once
#include "DHT.h"

int main() {

	int bitsize, machineNo,totalSize;

	//Bitsize input (Bitsize cannot be zero)
	while (1) {
		while (1)
		{
			cout << "Enter bit size for DHT : ";
			cin >> bitsize;
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
		if (bitsize != 0) {
			break;
		}
		cout << "Inavlid bitsize" << endl;
	}
	totalSize = pow(2, bitsize);

	//Number of Machines Input (Machines cannot be zero and they cannot exceed the total size of identifier space)
	while (1) {
		while (1)
		{
			cout << "Enter number of machines : ";
			cin >> machineNo;
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
		if (machineNo <= totalSize && machineNo!=0) {
			break;
		}
		cout << endl;
		cout << "Invalid number of machines" << endl;

	}
	
	//Option for Auto and Manual assignation
	//A DHT Object is made and parametrized constructors are called accordingly
	while (1) {
		int input;
	
		while (1)
		{
			cout << "Enter 1 to Auto assign ID" << endl;
			cout << "Enter 2 to assign ID to each machine " << endl;
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
			string* arr = new string[machineNo];
			DHT<int,string> obj(bitsize, machineNo, arr);
			break;
		}
		else if (input == 2) {
			DHT<int,string> obj(bitsize, machineNo);
			break;
		}
		else {
			cout << endl;
			cout << "Invalid Entry, Enter Again" << endl;
		}
		
	}
	return 0;
}