#ifndef _INPUT_LAYER_H_
#define _INPUT_LAYER_H_

#include "Layer.h"
#include "ImageEditor.h"
#include <vector>

using namespace std;
using namespace Mat;
using namespace exceptionh;

class Input_layer :public Layer
{
public:
	Input_layer(size_t Row = 128, size_t Col = 128);
	~Input_layer();

	void SetImage(const string& file_dir_ = "");
	virtual void Simulate();

protected:

	vector<string> data_;
};


#endif // !_INPUT_LAYER_H_

