#pragma once
#include "Matrix.h"
#include <vector>


#define DLL_EXPORT __declspec(dllexport)

class DLL_EXPORT LapackClass
{
public:
	LapackClass(void);
	~LapackClass(void);

	void calcMatrix();
	void calcMatrix2(double *matrixC);	
	void calcMatrixProduct(CMatrix& A, std::vector<double> & B, std::vector<double> & C, int columnsOfMatrixB, char transposeA, char transposeB);
	void InvertMatrix(std::vector<double> & flattenMatrix);
};

