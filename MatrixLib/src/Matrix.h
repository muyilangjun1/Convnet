#ifndef _MATRIX_H_
#define _MATRIX_H_

#include "amp.h"
#include <amp_math.h>
#include <activationfunction.h>
#include <ExceptionHandler.h>
#include <iostream>
#include <time.h>
#include <random>
#include <fstream>
#include <chrono>

using namespace std;
using namespace exceptionh;
using namespace activeF;
typedef enum { Zeroes, Random_Val , Ones} ElementValues;

namespace Mat {
	class Matrix
	{
	public:

		Matrix(size_t Row, size_t Col, int RandomNumber, ElementValues Chosen = Random_Val);
		Matrix(const Matrix& rhs);
		~Matrix();

		size_t GetRowSize() const;
		size_t GetColSize() const;
		float* GetMatrix() const;

		bool IsEmpty() const;

		void operator++();
		void operator--();
		bool operator==(const Matrix& rhs) const;
		bool operator!=(const Matrix& rhs) const;
		Matrix operator+(const Matrix& rhs);
		Matrix operator-(const Matrix& rhs);
		Matrix operator*(const Matrix& rhs);
		Matrix operator*(float Val);
		Matrix operator^(float ToThePowerOf);
		Matrix operator/(float Val);
		Matrix operator->*(const Matrix& rhs); // the same as ".*" operator in matlab.
		Matrix& operator=(const Matrix& rhs);
		Matrix& operator+=(const Matrix& rhs);
		Matrix& operator-=(const Matrix& rhs);
		Matrix& operator*=(const Matrix& rhs);
		Matrix& operator*=(float Val);
		Matrix& operator+=(float Val);
		Matrix& operator-=(float Val);
		Matrix& operator^=(float ToThePowerOf);
		Matrix& operator/=(float Val);

		friend ostream& operator<<(ostream& os, const Matrix& rhs);
		friend istream& operator>>(istream& is, Matrix& rhs);

		float& operator()(size_t RowIdx, size_t ColIdx);
		float operator()(size_t RowIdx, size_t ColIdx) const;

		void SetMatrixToZero();
		void Transpose();
		void ResizeMatrix(size_t Row, size_t Col);
		void ShafelValues();
		void PrintMatrix() const;
		void Convolution(Matrix& target, const Matrix& source);
		void ReLUMatrix(const Matrix& source, float bias = 1);
		void SoftMaxMatrix();
		void SVMPoolMatrix(const Matrix& source);
		void ActivateMatrix(const Matrix& source, int AFidx, float bias);
		void DerivativeMatrix(const Matrix& source, int AFidx, float bias);
		void DotMatrices(const Matrix& rhs);
		void ReduceMatricesToOne(const vector<Matrix*>& rhs);
		void Norm();
		Matrix MatrixMul(const Matrix& rhs);
		Matrix ColumnSum() const;
		//Matrix Inverse();
		//float Determinant() const;
		//Matrix Eigenvalues() const;
		//float* GaussianElimination(bool isInverse = false) const;

		void SaveMatrix(const string& _file) const;
		void LoadMatrix(const string& _file);

		//only for vector matrices.
		void PushBack(float Val);
		void PopBack();


	protected:
		void Checker(int IndexToCheck) const;
		void Checker(const Matrix& rhs, int IndexToCheck) const;
		//void SwapRows(double** A, double** B, size_t row1, size_t row2) const;

		size_t Row_;
		size_t Col_;
		float* Array_;
	};
}
#endif // !_MATRIX_H_