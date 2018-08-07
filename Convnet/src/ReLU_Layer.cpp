#include "ReLU_Layer.h"


ReLU_Layer::ReLU_Layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet)
	:
	Layer(Row, Col, Sheet, "ReLU layer")
{
	this->Prev_layer_ = Prev_layer;
}

ReLU_Layer::~ReLU_Layer()
{

}

void ReLU_Layer::Simulate()
{
	Checker(2);

	for (int i = 0; i < (int)this->GetSheetNumber(); i++)
		this->output_->at(i)->ReLUMatrix(*this->Prev_layer_->GetOutputMatrix().at(i), 0);
}

void ReLU_Layer::Checker(int Idx, int IdxToCheck) const
{
	switch (Idx)
	{
	case 1:
	{

		break;
	}
	case 2:
	{
		if (this->Prev_layer_->LayerClassification().compare("Conv layer") != 0)
			throw ExceptionHandler("The Prev_layer_  points to undifined data!");
		break;
	}
	case 3:
	{
		break;
	}
	case 4:
	{

		break;
	}
	default:
	{
		throw ExceptionHandler("Index Checker is out of range!");
		break;
	}
	}
}