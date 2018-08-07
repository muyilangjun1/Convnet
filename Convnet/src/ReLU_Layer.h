#ifndef _RELU_LAYER_H_
#define _RELU_LAYER_H_

#include "Layer.h"
#include <algorithm>
using namespace std;
using namespace Mat;

class ReLU_Layer :public Layer
{
public:

	ReLU_Layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet = 1);
	~ReLU_Layer();

	void Simulate();

	void Checker(int Idx, int IdxToCheck = 0) const;

protected:

	Layer * Prev_layer_;
};

#endif