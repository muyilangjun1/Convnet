#ifndef _POOL_LAYER_H_
#define _POOL_LAYER_H_

#include "Layer.h"
#include <algorithm>
using namespace std;
using namespace Mat;
using namespace exceptionh;

class Pool_layer : public Layer
{
public:

	Pool_layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet = 1);
	~Pool_layer();

	void Simulate();

	void Checker(int Idx, int IdxToCheck = 0) const;

protected:

	Layer* Prev_layer_;
};


#endif // !_POOL_LAYER_H_

