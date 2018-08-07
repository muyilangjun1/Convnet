#ifndef _FULLY_CONNECTED_LAYER_H_
#define _FULLY_CONNECTED_LAYER_H_

#include "Layer.h"
#include "Pool_layer.h"
using namespace std;
using namespace Mat;
using namespace exceptionh;

class FC_layer : public Layer
{
public:
	FC_layer(Layer* Prev_layer, int RandomNumber, size_t Row_mat, size_t Col_mat,size_t Layer_Num, size_t Sheet = 1, int AFidx = 3);
	~FC_layer();

	Matrix* GetDerivativeMatrix() const;
	Matrix* GetWeightMatrix() const;
	Matrix* GetDeltaWeightMatrix();

	void SetActivationFunction(int Idx);
	void SetBias(float Val);
	void ShafelWeights();
	void SetDeltaWeights(float Val, size_t RowIdx, size_t ColIdx);
	void SetDeltaWeights(const Matrix& rhs);
	void CalculateNewWeights(int average);
	void Simulate();

	void Checker(int Idx, int IdxToCheck = 0) const;
	void Printlayer() const;
	void IsTraining(bool training);
protected:

	Matrix* Derivative_output_;
	Matrix* weight_matrix_;
	Matrix* delta_weight_matrix_;
	Layer*  Prev_layer_;
	size_t Layer_Num_;
	int AFidx_;
	float bias_;
	bool training_;
};

#endif