/*
 * project1.cpp
 *
 *  Created on: Sep 5, 2012
 *      Author: sigma
 */

#include <iostream>
using namespace std;

int main(int arg_count, char* args[])
{
	cout << "Shaun Carlson" << endl;
	cout << "CS3060 - 601" << endl << endl;

	for (int i = 1; i < arg_count; i++)
		cout << args[i] << endl;

	return 0;
}
