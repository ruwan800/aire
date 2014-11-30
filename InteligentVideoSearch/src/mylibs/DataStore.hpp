#include <iostream>
#include <string>

using namespace std;

class DataStore{
public:
	static int getCount(string query);
	static double getDoubleValue(string query);
};
