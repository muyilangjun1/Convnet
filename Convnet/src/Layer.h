#ifndef _LAYER_H_
#define _LAYER_H_

#include <Matrix.h>
#include <activationfunction.h>
#include <ExceptionHandler.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace Mat;
using namespace exceptionh;


class Layer
{
public:

	/*Input constructor layer.
	* @params: Row - by default 128.
	*		   Col - by default 128.
	*
	* @note: the input layer contain the image data. image resoluton 128 x 128.
	*/
	Layer(size_t Row = 128, size_t Col = 128);

	/*Conv,ReLu and Pool Constructor layers.
	* @params: Row					- row size.
	*		   Col					- col size.
	*		   layer_classification - \"Conv layer\" , \"ReLU layer\" or \"Pool layer\".
	*
	* @Convolution layer: calculating values using the values of the matrices from previous layer, with
	*					  the filters.
	*
	* @ReLU layer: takes the values of the matrices from the convolution layer, and pass them through
	*			   ReLU activation function.
	*
	* @Pool layer: decrease the number of the previous layer by half, using max pooling.
	*/
	Layer(size_t Row, size_t Col, size_t Sheet, const string& layer_classification);

	/*Fully Connected constructor layer.
	* @params: Number_Of_Neurons			- number of neurons in the current layer. default 16 for hidden layers.
	*		   layer_classification			- \"Hidden layer\" or \"Output layer\".
	*		   RandomNumber					- helps prevent repetitions in the value weights.
	*
	* @note: takes the output values from the previous layer, and pass them through activation function.
	*		 use this constructor for fully connected layers such as hidden layers and output layer.
	*/
	Layer(const string& layer_classification, size_t Sheet, size_t Row , size_t Col);

	~Layer();

	size_t GetSheetNumber() const;
	size_t GetNumberOfNeurons() const;
	vector<Matrix*>& GetOutputMatrix();

	virtual void Simulate() = 0;
	string& LayerClassification();

protected:
	vector<Matrix*>* output_;
	string* layer_classification_;
	size_t Number_Of_Neurons_;
	size_t Sheet_;
	size_t Row_;
	size_t Col_;
};

#endif // !_LAYER_H_

