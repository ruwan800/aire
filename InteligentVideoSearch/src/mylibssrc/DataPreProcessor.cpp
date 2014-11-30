#include "../mylibs/DataPreProcessor.hpp"
#include "../mylibs/Util.h"
#include <math.h>

using namespace std;

void DataPreProcessor::processCameraFocusedArea(int* dataArray, int size,
		int* region1, int* region2, int* region3){
	int totReg = 0;
	double average = 0.0;
	double regiionSize = size / 3.0;

	for(int i = 0; i < size / 3; i++){
		totReg += dataArray[i];
	}
	average = totReg / (regiionSize);
	*region1 = (int)round(average);

	totReg = 0;
	for(int i = (size / 3); i < (size * 2) / 3; i++){
		totReg += dataArray[i];
	}
	average = totReg / (regiionSize);
	*region2 = (int)round(average);

	totReg = 0;
	for(int i = (2 * size / 3); i < size; i++){
		totReg += dataArray[i];
	}
	average = totReg / (regiionSize);
	*region3 = (int)round(average);
}

void DataPreProcessor::processEdgesPercentages(double* dataArray, int size,
				double* region1, double* region2, double* region3){
		double totReg = 0.0;
		double average = 0.0;
		double regiionSize = size / 3.0;

		for(int i = 0; i < size / 3; i++){
			totReg += dataArray[i];
		}
		*region1 = totReg / (regiionSize);

		totReg = 0.0;
		for(int i = (size / 3); i < (size * 2) / 3; i++){
			totReg += dataArray[i];
		}
		*region2 = totReg / (regiionSize);

		totReg = 0.0;
		for(int i = (2 * size / 3); i < size; i++){
			totReg += dataArray[i];
		}
		*region3 = totReg / (regiionSize);
}

void DataPreProcessor::processPitchAvailability(int* dataArray, int size,
			int* region1, int* region2, int* region3){
		int totReg = 0;
		double average = 0.0;
		double regiionSize = size / 3.0;

		for(int i = 0; i < size / 3; i++){
			totReg += dataArray[i];
		}
		average = totReg / (regiionSize);
		*region1 = (int)round(average);

		totReg = 0;
		for(int i = (size / 3); i < (size * 2) / 3; i++){
			totReg += dataArray[i];
		}
		average = totReg / (regiionSize);
		*region2 = (int)round(average);

		totReg = 0;
		for(int i = (2 * size / 3); i < size; i++){
			totReg += dataArray[i];
		}
		average = totReg / (regiionSize);
		*region3 = (int)round(average);
}

void DataPreProcessor::isPitchZooming(double* dataArray, int size,
		int* region1, int* region2, int* region3){

	double regSize = size / 3.0;

	double isLarger[((int)regSize)];
	Util::makeZeroDoubleArray(isLarger, ((int)regSize));

	double totalIsLarge;
	double average = 0.0;

	for(int i = 0; i < ((int)regSize - 1); i++){
		if(dataArray[i + 1] > 0){
			totalIsLarge += dataArray[i + 1] - dataArray[i];
			cout << dataArray[i + 1] << " - " << dataArray[i] << " = "<<(dataArray[i + 1] - dataArray[i]) << "\n";
		}

	}
	//totalIsLarge = Util::addDoubleArray(isLarger, regSize);
	//cout << totalIsLarge << "\n";
	if(totalIsLarge > 0){
		*region1 = 1;
	} else {
		*region1 = 0;
	}

	Util::makeZeroDoubleArray(isLarger, ((int)regSize));

	int start = (size / 3);
	int end = ((size * 2) / 3) - 1;
	totalIsLarge = 0.0;
	for(int i = start; i < end; i++){
		if(dataArray[i] > 0){
			totalIsLarge += dataArray[i + 1] - dataArray[i];
		}
		//cout << totalIsLarge << "\n";
	}

	//cout << totalIsLarge << "\n";

	//totalIsLarge = Util::addDoubleArray(isLarger, regSize);
	if(totalIsLarge > 0){
			*region2 = 1;
		} else {
			*region2 = 0;
		}

	Util::makeZeroDoubleArray(isLarger, ((int)regSize));

	start = (2 * size / 3);
	end = size;

	totalIsLarge = 0.0;
	for(int i = (2 * size / 3); i < end; i++){
		if(dataArray[i] > 0){
			totalIsLarge += dataArray[i + 1] - dataArray[i];
		}
		//cout << totalIsLarge << "\n";
	}

	//totalIsLarge = Util::addDoubleArray(isLarger, regSize);
	if(totalIsLarge > 0){
				*region3 = 1;
			} else {
				*region3 = 0;
			}
	//cout << totalIsLarge << "\n";
}



