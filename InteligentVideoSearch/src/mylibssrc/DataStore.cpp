#include "../mylibs/DataStore.hpp"

#include <iostream>
#include <string>
#include <sstream>

#include <mysql_connection.h>
#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>

using namespace std;

const string ADDRESS = "tcp://127.0.0.1:3306";
const string PW = "123";
const string DB = "bayesdb";
const string UN = "root";

int DataStore::getCount(string query) {
	int count = 0;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		driver = get_driver_instance();
		con = driver->connect(ADDRESS, UN, PW);
		con->setSchema(DB);

		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next()) {
			count = res->getInt(1);
		}

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		return -1;
	}

	return count;
}

double DataStore::getDoubleValue(string query) {
	double count = 0.0;

	try {
		sql::Driver *driver;
		sql::Connection *con;
		sql::Statement *stmt;
		sql::ResultSet *res;

		driver = get_driver_instance();
		con = driver->connect(ADDRESS, UN, PW);
		con->setSchema(DB);

		stmt = con->createStatement();
		res = stmt->executeQuery(query);

		while (res->next()) {
			count = res->getDouble(1);
		}

		delete res;
		delete stmt;
		delete con;

	} catch (sql::SQLException &e) {
		return -1;
	}

	return count;
}

