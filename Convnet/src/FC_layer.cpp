#include "FC_layer.h"



FC_layer::FC_layer(Layer* Prev_layer, int RandomNumber, size_t Row_mat, size_t Col_mat, size_t Layer_Num, size_t Sheet, int AFidx)
	:
	Layer("FC layer", Sheet,1,Col_mat),
	AFidx_(AFidx),
	bias_(0.1f),
	Layer_Num_(Layer_Num)
{
	this->Prev_layer_ = Prev_layer;

	if (this->Prev_layer_->LayerClassification().compare("FC layer"))
	{
		this->output_->at(0)->ResizeMatrix(1,Col_mat * Row_mat * Sheet);
		this->Col_ = Col_mat * Row_mat * Sheet;
		this->Number_Of_Neurons_ = Col_mat * Row_mat * Sheet;
	}

	else
	{
		this->weight_matrix_ = new Matrix(Row_mat, Col_mat, RandomNumber);
		this->delta_weight_matrix_ = new Matrix(Row_mat, Col_mat, Zeroes);
		this->Derivative_output_ = new Matrix(1, Col_mat, 0, Zeroes);
	}
}

FC_layer::~FC_layer()
{

}

Matrix* FC_layer::GetDerivativeMatrix() const
{
	return this->Derivative_output_;
}

Matrix* FC_layer::GetWeightMatrix() const
{
	return this->weight_matrix_;
}

Matrix* FC_layer::GetDeltaWeightMatrix()
{
	return this->delta_weight_matrix_;
}

void FC_layer::SetActivationFunction(int Idx)
{
	this->AFidx_ = Idx;
}

void FC_layer::SetBias(float Val)
{
	this->bias_ = Val;
}

void FC_layer::ShafelWeights()
{
	this->weight_matrix_->ShafelValues();
}

void FC_layer::SetDeltaWeights(float Val, size_t RowIdx, size_t ColIdx)
{
	(*this->delta_weight_matrix_)(RowIdx, ColIdx) = Val;
}

void FC_layer::SetDeltaWeights(const Matrix& rhs)
{
	*this->delta_weight_matrix_ += rhs;
}

void FC_layer::CalculateNewWeights(int average)
{
	(*this->weight_matrix_) += ((*this->delta_weight_matrix_) / (float)average);
	this->delta_weight_matrix_->SetMatrixToZero();
}

void FC_layer::Simulate()
{
	Checker(2);

	if (!this->Prev_layer_->LayerClassification().compare("Pool layer"))
	{
		this->output_->at(0)->ReduceMatricesToOne(this->Prev_layer_->GetOutputMatrix());
	}

	else
	{
		if (this->Layer_Num_ < 7)
		{
			this->output_->at(0)->ActivateMatrix(*(this->Prev_layer_->GetOutputMatrix().at(0)) * (*this->weight_matrix_), this->AFidx_, this->bias_);
			this->Derivative_output_->DerivativeMatrix(*(this->Prev_layer_->GetOutputMatrix().at(0)) * (*this->weight_matrix_), this->AFidx_, this->bias_);
		}

		else
		{
			this->output_->at(0)->ActivateMatrix(*(this->Prev_layer_->GetOutputMatrix().at(0)) * (*this->weight_matrix_), this->AFidx_, this->bias_);
			this->Derivative_output_->DerivativeMatrix(*(this->Prev_layer_->GetOutputMatrix().at(0)) * (*this->weight_matrix_), this->AFidx_, this->bias_);
			this->output_->at(0)->Norm();
			this->Derivative_output_->Norm();
			//this->output_->at(0)->SoftMaxMatrix();
			//this->Derivative_output_->SoftMaxMatrix();
		}
		/*
		if (this->Layer_Num_ == 7)
		{
			this->output_->at(0)->Norm();
			this->Derivative_output_->Norm();
		}*/
		
		/*if (this->Layer_Num_ < 7 && this->training_ == true)
		{
			srand((unsigned int)time(0));
			for (size_t i = 0; i < this->Number_Of_Neurons_; i++)
			{
				if ((float)rand() / (RAND_MAX) < 0.2)
				{
					this->output_->at(0)->operator()(0, i) = 0;
					this->Derivative_output_->operator()(0, i) = 0;
				}
			}
		}*/
	}
}

void FC_layer::Printlayer() const
{
	for (int i = 0; i < 10; i++)
	{
		cout << (*this->output_->at(0))(0, i);
		if (i < 9)
			cout << ", ";
	}
	cout << endl;
}

void FC_layer::IsTraining(bool training)
{
	this->training_ = training;
}

void FC_layer::Checker(int Idx, int IdxToCheck) const
{
	switch (Idx)
	{
	case 1:
	{

		break;
	}
	case 2:
	{
		if (this->Prev_layer_->LayerClassification().compare("Pool layer") != 0 && this->Prev_layer_->LayerClassification().compare("FC layer") != 0)
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

