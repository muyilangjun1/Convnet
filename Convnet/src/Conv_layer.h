#ifndef _CONVOLUTION_LAYER_H_
#define _CONVOLUTION_LAYER_H_

#include "Layer.h"
#include <algorithm>
using namespace std;
using namespace Mat;
using namespace exceptionh;

class Conv_layer : public Layer
{
public:
	Conv_layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet = 1, size_t F_Row = 3, size_t F_Col = 3, int Number_Of_Filters = 2);
	~Conv_layer();

	vector<Matrix*>* GetDerivativeMatrix() const;

	void CreateFilters();
	void ChangeFilter(const Matrix& NewFilter, int FilterIdx);
	void ChangeNumberOfFilters(int NewNumber);
	void ShafelFilters();
	void SetDeltaWeights(float Val, int RowIdx, int ColIdx, int Sheet);
	void CalculateNewWeights();
	virtual void Simulate();

	void Checker(int Idx, int IdxToCheck = 0) const;

protected:

	int Number_Of_Filters_;
	size_t F_Row_;
	size_t F_Col_;
	vector<Matrix*>* Filter_Vec_;
	vector<Matrix*>* Derivative_output_;
	vector<Matrix*>* delta_Filter_Vec_;
	Layer* Prev_layer_;
};

#endif