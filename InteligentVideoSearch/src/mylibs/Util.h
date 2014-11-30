#ifndef UTIL_H_
#define UTIL_H_

class Util {
public:
	static void makeZeroIntArray(int* array, int size);
	static void makeZeroDoubleArray(double* array, int size);
	static void printIntArray(int* array, int size);
	static void printDoubleArray(double* array, int size);
	static int getHigestIntPosition(int* array, int size);
	static int getHigestDoublePosition(double* array, int size);
	static int addIntArray(int* array, int size);
	static double addDoubleArray(double* array, int size);

	static double getHueFromBGR(int b, int g, int r);
	static double getSaturationFromBGR(int b, int g, int r);

};

#endif
