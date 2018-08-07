#include "Pool_layer.h"



Pool_layer::Pool_layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet)
	:
	Layer(Row, Col, Sheet, "Pool layer")
{
	this->Prev_layer_ = Prev_layer;
}


Pool_layer::~Pool_layer()
{

}

void Pool_layer::Simulate()
{
	for (int i = 0; i < (int)this->GetSheetNumber(); i++)
	{
		this->output_->at(i)->SVMPoolMatrix(*this->Prev_layer_->GetOutputMatrix().at(i));
	}
}

void Pool_layer::Checker(int Idx, int IdxToCheck) const
{
	switch (Idx)
	{
	case 1:
	{

		break;
	}
	case 2:
	{
		if (this->Prev_layer_->LayerClassification().compare("ReLU layer") != 0)
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
