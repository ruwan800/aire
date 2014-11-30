#include "../mylibs/Util.h"
#include <stdio.h>
#include <math.h>
#include <iostream>

using namespace std;

/* Assigns zero to all elements of a given array */
void Util::makeZeroIntArray(int* array, int size){
	for(int i = 0; i < size; i++){
			array[i] = 0;
	}
}

void Util::makeZeroDoubleArray(double* array, int size){
	for(int i = 0; i < size; i++){
			array[i] = 0.0;
	}
}

/* Prints a given array */
void Util::printIntArray(int* array, int size){
	for(int i = 0; i < size; i++){
		printf("%d ", array[i]);
	}
}

void Util::printDoubleArray(double* array, int size){
	for(int i = 0; i < size; i++){
		cout << array[i] << " ";
	}
}

/* Returns the index which has highest element value of an int array*/
int Util::getHigestIntPosition(int* array, int size){
	int highestPosition = 0, temp = 0;

	for(int i = 1; i < size; i++){
		int val =  array[i];
		if(temp < val){
			temp = array[i];
			highestPosition = i;
		}
	}

	return highestPosition;
}

/* Returns the index which has highest element value of a double array*/
int Util::getHigestDoublePosition(double* array, int size){
	int highestPosition = 0;
	double temp = 0.0;

	for(int i = 0; i < size; i++){
		double val =  array[i];
		if(temp < val){
			temp = array[i];
			highestPosition = i;
		}
	}

	return highestPosition;
}

/* Add all elements in an array */
int Util::addIntArray(int* arr, int size){
	int sum = 0;
	for(int i = 0; i < size; i++){
		sum += arr[i];
	}

	return sum;
}

double Util::addDoubleArray(double* arr, int size){
	double sum = 0.0;
	for(int i = 0; i < size; i++){
		sum += arr[i];
	}

	return sum;
}

double Util::getHueFromBGR(int b, int g, int r){
	double rd = r/255.0;
	double gd = g/255.0;
	double bd = b/255.0;

	double hue;

	double cmax = max(rd, max(gd, bd));
	double cmin = min(rd, min(gd, bd));

	double dif = cmax - cmin ;

	if(cmax == rd){
		hue = 60.0 *(fmod(((gd-bd) /dif), 6));
	}
	else if(cmax == gd){
		hue = 60.0 *(((bd-rd)/dif)+2);
	}
	else if(cmax == bd){
		hue = 60.0 *(((rd-gd)/dif)+4);
	}

	return hue;
}

double Util::getSaturationFromBGR(int b, int g, int r){
	double rd = r/255.0;
	double gd = g/255.0;
	double bd = b/255.0;

	double cmax = max(rd, max(gd, bd));
	double cmin = min(rd, min(gd, bd));

	double dif = cmax - cmin ;
	double saturation;

	if(dif == 0){
		saturation = 0.0;
	}
	else{
		saturation = dif/cmax;
	}

	return (saturation * 100);
}
