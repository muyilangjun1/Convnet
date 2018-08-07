#include "Layer.h"



Layer::Layer(size_t Row, size_t Col)
	:
	Number_Of_Neurons_(Row * Col),
	Row_(Row),
	Col_(Col),
	Sheet_(1)
{
	this->layer_classification_ = new string("Input layer");
	this->output_ = new vector<Matrix*>;
	this->output_->push_back(new Matrix(Row, Col, 0, Zeroes));
}

Layer::Layer(size_t Row, size_t Col, size_t Sheet, const string& layer_classification)
	:
	Number_Of_Neurons_(Row * Col * Sheet),
	Sheet_(Sheet),
	Row_(Row),
	Col_(Col)
{
	this->layer_classification_ = new string(layer_classification);
	this->output_ = new vector<Matrix*>;
	for (size_t i = 0; i < Sheet; i++)
		this->output_->push_back(new Matrix(Row, Col, 0, Zeroes));
}

Layer::Layer(const string& layer_classification, size_t Sheet, size_t Row, size_t Col)
	:
	Number_Of_Neurons_(Row * Col * Sheet),
	Sheet_(Sheet),
	Row_(Row),
	Col_(Col * Sheet)
{
	this->layer_classification_ = new string(layer_classification);
	this->output_ = new vector<Matrix*>;
	this->output_->push_back(new Matrix(Row, Col, 0, Zeroes));
}

Layer::~Layer()
{

}

size_t Layer::GetSheetNumber() const
{
	return this->Sheet_;
}

size_t Layer::GetNumberOfNeurons() const
{
	return this->Number_Of_Neurons_;
}

vector<Matrix*>& Layer::GetOutputMatrix()
{
	return *this->output_;
}

string& Layer::LayerClassification()
{
	return *this->layer_classification_;
}
