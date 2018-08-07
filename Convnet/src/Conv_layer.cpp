#include "Conv_layer.h"


Conv_layer::Conv_layer(size_t Row, size_t Col, Layer* Prev_layer, size_t Sheet, size_t F_Row, size_t F_Col, int Number_Of_Filters)
	:
	Layer(Row, Col, Sheet, "Conv layer"),
	Number_Of_Filters_(Number_Of_Filters),
	F_Row_(F_Row),
	F_Col_(F_Col)
{
	this->CreateFilters();
	this->Prev_layer_ = Prev_layer;
	this->Derivative_output_ = new vector<Matrix*>;
	for (int i = 0; i < Sheet; i++)
		this->Derivative_output_->push_back(new Matrix(Row,Col,0,Zeroes));
}

Conv_layer::~Conv_layer()
{

}

vector<Matrix*>* Conv_layer::GetDerivativeMatrix() const
{
	return this->Derivative_output_;
}

void Conv_layer::CreateFilters()
{
	this->delta_Filter_Vec_ = new vector<Matrix*>;
	this->Filter_Vec_		= new vector<Matrix*>;
	for (int i = 0; i < this->Number_Of_Filters_; i++)
	{
		this->delta_Filter_Vec_->push_back(new Matrix(this->F_Row_, this->F_Col_, i));
		this->Filter_Vec_->push_back(new Matrix(this->F_Row_, this->F_Col_, i));
	}
}

void Conv_layer::ChangeFilter(const Matrix& NewFilter, int FilterIdx)
{
	Checker(1, FilterIdx);

	(*this->Filter_Vec_->at(FilterIdx)) = NewFilter;
}

void Conv_layer::ChangeNumberOfFilters(int NewNumber)
{
	Checker(3, NewNumber);
	this->Number_Of_Filters_ = NewNumber;
	/*TO DO - create or remove filters*/
}

void Conv_layer::ShafelFilters()
{
	for (int i = 0; i < (int)this->Filter_Vec_->size(); i++)
	{
		this->Filter_Vec_->at(i)->ShafelValues();
	}
}

void Conv_layer::SetDeltaWeights(float Val, int RowIdx, int ColIdx, int Sheet)
{
	(*this->delta_Filter_Vec_->at(Sheet))(RowIdx, ColIdx) = Val;
}

void Conv_layer::CalculateNewWeights()
{
	for (int i = 0; i < (int)this->Filter_Vec_->size();i++)
	{
		*this->Filter_Vec_->at(i) -= *this->delta_Filter_Vec_->at(i);
	}
}

void Conv_layer::Simulate()
{
	for (int i = 0; i < (int)this->Number_Of_Filters_; i++)
	{
		if (!this->Prev_layer_->LayerClassification().compare("Input layer"))
		{
			this->output_->at(i)->SetMatrixToZero();
			this->Filter_Vec_->at(i)->Convolution(*this->output_->at(i), *this->Prev_layer_->GetOutputMatrix().at(0));
			this->output_->at(i)->Norm();
		}
		else
		{
			for (int j = 0;j<(int)this->Prev_layer_->GetSheetNumber();j++)
			{
				this->output_->at(i * (int)this->Number_Of_Filters_ + j)->SetMatrixToZero();
				this->Filter_Vec_->at(i)->Convolution(*this->output_->at(i * (int)this->Number_Of_Filters_ + j), *this->Prev_layer_->GetOutputMatrix().at(j));
				this->output_->at(i * (int)this->Number_Of_Filters_ + j)->Norm();
			}
		}
	}
}

void Conv_layer::Checker(int Idx, int IdxToCheck) const
{
	switch (Idx)
	{
	case 1:
	{
		if (IdxToCheck < 0 || IdxToCheck >= this->Number_Of_Filters_)
			throw ExceptionHandler("Filter index is out of range!");
		break;
	}
	case 2:
	{
		if (this->Prev_layer_->LayerClassification().compare("Input layer") != 0 && this->Prev_layer_->LayerClassification().compare("Pool layer") != 0)
			throw ExceptionHandler("The Prev_layer_  points to undifined data!");
		break;
	}
	case 3:
	{
		if (IdxToCheck < 0)
			throw ExceptionHandler("Number of filter must be positive integer!");
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

