#include "../mylibs/BayesClassifier.hpp"
#include "../mylibs/DataStore.hpp"
#include "../mylibs/Util.h"

#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using namespace std;

/* Defining constants Euler's number and PI */
const double e = 2.718281828;
const double pi = 22.0/7;

/* converts any data type into string and returns it.
 * This method is used in string concatenation in queries  */
template<typename T>
string Str(const T & t) {
	ostringstream os;
	os << t;
	return os.str();
}

/* Returns the probability of a class */
double probabilityOfClass(int clz) {
	/* Taking the count of all rows */
	int totalClass = DataStore::getCount("SELECT COUNT(*) FROM bayes_table");
	/* Taking the count of rows belong to a given class */
	int classValue = DataStore::getCount("SELECT COUNT(*) FROM bayes_table WHERE cls=" + Str(clz));

	double classProbability = (classValue + 0.0) / totalClass;
	return classProbability;
}

/* Returns the probability of non-continuous attribute*/
double getProbabilityOfRegion(int clz, int currentVal, string colName) {
	/* Taking a row count which are relevant to a given class and equal to a given value */
	int currentValueInRegion = DataStore::getCount("SELECT COUNT("+ Str(colName) +") FROM bayes_table WHERE cls =" +
			Str(clz) + " and "+ Str(colName) +"=" + Str(currentVal));
	/* Taking the count of rows belong to a given class */
	int currentClz= DataStore::getCount("SELECT COUNT("+ Str(colName) +") FROM bayes_table WHERE cls =" + Str(clz));

	double regionOneProbability = (currentValueInRegion + 0.0)/ currentClz;
	return regionOneProbability;
}

/* Returns Gaussian naive Bayes of a continuous attribute */
double getProbabilityOfEdgePercentage(int clz, int currentVal, string colName){
	/* Taking the varience */
	double varience = DataStore::getDoubleValue("SELECT VARIANCE("+ Str(colName) +") FROM bayes_table WHERE cls=" + Str(clz));
	/* Taking the mean */
	double mean = DataStore::getDoubleValue("SELECT AVG("+ Str(colName) +") FROM bayes_table WHERE cls=" + Str(clz));

	/* Calculation of Gaussian naive Bayes */
	double p1 = 1.0 / (sqrt(2 * pi * pow(varience, 2)));
	double p2 = (pow((currentVal - mean), 2)) / (2 * pow(varience, 2));
	double probability = p1 * pow(e, (-1.0 * p2));

	return probability;
}

int BayesClassifier::takeDecision(int regOne, int regTwo, int regThree,
		int regFour, int regFive, int regSix) {
	double ProbabilityArray[4];

	/* Counting probability of every class and put it into an array */
	for (int i = 1; i < 5; i++) {
		double prob1 = getProbabilityOfRegion(i, regOne, "reg1");
		double prob2 = getProbabilityOfRegion(i, regTwo, "reg2");
		double prob3 = getProbabilityOfRegion(i, regThree, "reg3");
		double prob4 = getProbabilityOfEdgePercentage(i, regFour, "preg1");
		double prob5 = getProbabilityOfEdgePercentage(i, regFive, "preg2");
		double prob6 = getProbabilityOfEdgePercentage(i, regSix, "preg3");
		double probAll = probabilityOfClass(i);

		double finalProbability = prob1 * prob2 * prob3
				* prob4 * prob5 * prob6	* probAll;
		ProbabilityArray[i - 1] = finalProbability;
	}

	/* Returns class index which has highest probability */
	return (Util::getHigestDoublePosition(ProbabilityArray, 4) + 1);
}
