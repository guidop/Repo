#include "StdAfx.h"
#include "LapackClass.h"
#include <stdio.h>
#include <vector>
#include "Matrix.h"

using namespace std;


extern "C" void dgesv_(const int *N, const int *nrhs, double *A, const int *lda, int
	*ipiv, double *b, const int *ldb, int *info );

extern "C" void dgels_(const char *trans, const int *M, const int *N, const int *nrhs,
    double *A, const int *lda, double *b, const int *ldb, double *work,
    const int * lwork, int *info); 

extern "C" void dgemm_(const char *transA, const char *transB, const int *M, const int *N, const int *K, 
	double *alpha, double *a, const int *lda, double *b, const int *ldb, double *beta,
	double *c,  const int *ldc);


extern "C" void dgetri_(int* n, double* a, int* lda,int* ipiv, double* work, int* lwork, int* info );

extern "C" void dgetrf_(int* m, int* n, double* a, int* lda,int* ipiv, int* info);



LapackClass::LapackClass(void)
{
}


LapackClass::~LapackClass(void)
{
}

void LapackClass::calcMatrix()
{
    double A[9] = {3, 4, 5, 1, 2, 4, 5, 1, 3};
    double b[3] = {1,2,3};

    int N = 3;
    int nrhs = 1;
    int lda = 3;
    int ipiv[3];
    int ldb = 3;
    int info;
    
    dgesv_(&N, &nrhs, A, &lda, ipiv, b, &ldb, &info);

    if(info == 0) /* succeed */
	printf("The solution is %lf %lf %lf\n", b[0], b[1], b[2]);
    else
	fprintf(stderr, "dgesv_ fails %d\n", info);
	
	
}

void LapackClass::calcMatrix2(double *matrixC)
{

	//perform C = alpha*A*B + beta*C. 
	// if beta equal 0, no C must be provided
	//so this is the product C = A*B
			
	double alpha = 1.0;
	double beta = 0.0;
	char notrans = 'N';
	int lda = 3;    

	int rowsOfMatrixA = 3;
	
	vector<double> matrixA;
	//first column of A
	matrixA.push_back(1);
	matrixA.push_back(5);
	matrixA.push_back(2);
	//second column of A
	matrixA.push_back(1);
	matrixA.push_back(2);
	matrixA.push_back(4);
	//third column of A
	matrixA.push_back(5);
	matrixA.push_back(3);
	matrixA.push_back(7);
     	  
	
	int columnsOfMatrixB = 1;	
	vector<double> matrixB;
	matrixB.push_back(1);
	matrixB.push_back(1);
	matrixB.push_back(1);
							   		
	int ldc = 3;    
	
	 dgemm_(&notrans, &notrans, &rowsOfMatrixA, &columnsOfMatrixB,
		 &rowsOfMatrixA, &alpha, &matrixA[0], &lda, 
		 &matrixB[0], &rowsOfMatrixA, &beta, matrixC, &ldc);	
}


void LapackClass::calcMatrixProduct(CMatrix& A, vector<double> & B, vector<double> & C, int columnsOfMatrixB, char transposeA, char transposeB)
{

	//perform C = alpha*A*B + beta*C. 
	// if beta equal 0, no C must be provided
	//so this is the product C = A*B
			
	double alpha = 1.0;
	double beta = 0.0;

	int rowsOfMatrixA  = A.m_Data.at(0).size();
	int columnsOfMatrixA  = A.m_Data.size();
	int lda = max(A.m_Data.at(0).size(),A.m_Data.size());
							   		
	int ldc = rowsOfMatrixA;

	C.assign(rowsOfMatrixA*columnsOfMatrixB,0);
	
	double *matrixAInternal = new double[rowsOfMatrixA*columnsOfMatrixA];
	for (int i=0; i< rowsOfMatrixA; i++)
		for (int j=0; j< columnsOfMatrixA; j++)
		{
			matrixAInternal[i*rowsOfMatrixA+j] = A.m_Data[i][j];
		}

	 dgemm_(&transposeA, &transposeB, &rowsOfMatrixA, &columnsOfMatrixB,
		 &rowsOfMatrixA, &alpha,matrixAInternal, &lda, 
		 &B[0], &rowsOfMatrixA, &beta, &C[0], &ldc);	



	 delete[] matrixAInternal;
}

void LapackClass::InvertMatrix(vector<double> & flattenMatrix)
{
	
	int error = 10;
	
	int rows = (int)sqrt((double)flattenMatrix.size());
	int * ipiv = new int[rows*rows];
	
	dgetrf_(&rows, &rows, &flattenMatrix[0], &rows, ipiv, &error);

	error = 10;
	
	int lwork = rows;
	double *work = new double[lwork];

	dgetri_(&rows, &flattenMatrix[0], &rows, ipiv, work, &lwork, &error );
	
	
	delete[] ipiv;
	delete[] work;
}

