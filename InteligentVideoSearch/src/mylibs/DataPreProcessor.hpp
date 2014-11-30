#ifndef DATAPREPROCESSOR_HPP_
#define DATAPREPROCESSOR_HPP_
#include <iostream>

using namespace std;
class DataPreProcessor{
public:
	static void processCameraFocusedArea(int* dataArray, int size,
			int* region1, int* region2, int* region3);
	static void processEdgesPercentages(double* dataArray, int size,
				double* region1, double* region2, double* region3);
	static void processPitchAvailability(int* dataArray, int size,
			int* region1, int* region2, int* region3);
	static void isPitchZooming(double* dataArray, int size,
			int* region1, int* region2, int* region3);
};

#endif
