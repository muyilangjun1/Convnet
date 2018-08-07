#include "Convnet.h"


Convnet::Convnet()
	:
	Learning_rate_(0.5f)
{
	this->size_ = 8;
	this->topology_ = new vector<string>;
	this->Layer_list_ = new vector<Layer*>;
	this->Error_Vec_ = new Matrix(1, 16, 0, Zeroes);
	this->Expected_Output_Vec_ = new Matrix(1, 16, 0, Zeroes);
	size_t row = 64;
	size_t col = 64;
	for (int i = 0; i < (int)this->size_; i++)
	{
		if (i == 0)
		{
			this->topology_->push_back(string("Input layer"));
			this->Layer_list_->push_back(new Input_layer(row,col));
		}
		else if (i == 1)
		{
			this->topology_->push_back(string("Conv layer"));
			this->Layer_list_->push_back(new Conv_layer(row, col, this->Layer_list_->back(),3,3,3,3));
		}
		else if (i == 2)
		{
			this->topology_->push_back(string("Pool layer"));
			row = row / 2;
			col = col / 2;
			this->Layer_list_->push_back(new Pool_layer(row, col, this->Layer_list_->back(),3));
		}
		else if (i == 3)
		{
			this->topology_->push_back(string("Conv layer"));
			this->Layer_list_->push_back(new Conv_layer(row, col, this->Layer_list_->back(),6,3,3,2));
		}
		else if (i == 4)
		{
			this->topology_->push_back(string("Pool layer"));
			row = row / 2;
			col = col / 2;
			this->Layer_list_->push_back(new Pool_layer(row, col, this->Layer_list_->back(), 6));
		}
		else if (i == 5)
		{
			this->topology_->push_back(string("FC layer"));
			this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, row,col,i,6,2));
			dynamic_cast<FC_layer*>(this->Layer_list_->back())->IsTraining(false);
		}
		else if (i == 6)
		{
			this->topology_->push_back(string("FC layer"));
			this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, row * col * 6,16,i,1,2));
			dynamic_cast<FC_layer*>(this->Layer_list_->back())->IsTraining(true);
		}
		else
		{
			this->topology_->push_back(string("FC layer"));
			this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, 16, 16,i, 1,2));
		}
	}
}

Convnet::Convnet(const vector<string>& topology, size_t Row, size_t Col)
	:
	Learning_rate_(0.1f)
{
	this->size_ = topology.size();
	this->topology_ = new vector<string>;
	this->Layer_list_ = new vector<Layer*>;
	this->Error_Vec_ = new Matrix(1,16,0,Zeroes);
	this->Expected_Output_Vec_ = new Matrix(1, 16, 0, Zeroes);
	size_t row = Row;
	size_t col = Col;
	for (int i = 0; i < (int)this->size_; i++)
	{
		this->topology_->push_back(topology.at(i));
		if (!topology.at(i).compare("Input layer"))
		{
			this->Layer_list_->push_back(new Input_layer());
		}
		else if (!topology.at(i).compare("Conv layer"))
		{
			this->Layer_list_->push_back(new Conv_layer(row, col, this->Layer_list_->back(),5));
		}
		else if (!topology.at(i).compare("ReLU layer"))
		{
			this->Layer_list_->push_back(new ReLU_Layer(row, col, this->Layer_list_->back()));
		}
		else if (!topology.at(i).compare("Pool layer"))
		{
			row = row / 2;
			col = col / 2;
			this->Layer_list_->push_back(new Pool_layer(row, col, this->Layer_list_->back()));
		}
		else if (!topology.at(i).compare("FC layer"))
		{
			if (!topology.at(i - 1).compare("FC layer"))
			{
				if (i < (int)this->size_ - 1)
				{
					this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, 16,16,1));
				}
				else
				{
					this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, 16,10,1));
				}
			}
			else
			{
				this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), i, row ,col,8));
			}
		}
		else
		{
			throw ExceptionHandler("Incorrect layer name!");
		}
	}
}

Convnet::~Convnet()
{

}

size_t Convnet::GetNumberOfLayers() const
{
	return this->size_;
}

void Convnet::AddFClayer()
{
	this->Layer_list_->pop_back();
	/*TO DO - need to free all of the memory of the last layer*/
	this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), 34, 16,16,1));
	this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), 32, 16,16,1));
	this->size_ += 1;
}

void Convnet::EraseFClayer()
{
	/*TO DO - need to check of the erased layer is FC_layer. if not need to do some changes.*/
	this->Layer_list_->pop_back();
	this->Layer_list_->pop_back();
	this->Layer_list_->push_back(new FC_layer(this->Layer_list_->back(), 31, 16,16,1));
	this->size_ -= 1;
}

void Convnet::SetImage(const string& file_dir_)
{
	dynamic_cast<Input_layer*>(this->Layer_list_->front())->SetImage(file_dir_);
}

void Convnet::SetBias(float Val)
{
	for (size_t i = size_ - 1; (i > 0) && (!this->topology_->at(i).compare("FC_layer")); i--)
		dynamic_cast<FC_layer*>(this->Layer_list_->at(i))->SetBias(Val);
}

void Convnet::SetLearningRate(float Val)
{
	this->Learning_rate_ = Val;
}

void Convnet::SetActivationFunction(int Idx)
{
	for (size_t i = size_ - 1; (i > 0) && (!this->topology_->at(i).compare("FC_layer")); i--)
		dynamic_cast<FC_layer*>(this->Layer_list_->at(i))->SetActivationFunction(Idx);
}

void Convnet::ShafelWeights()
{
	for (int i = 0; i < (int)this->topology_->size(); i++)
	{
		if (!this->topology_->at(i).compare("Conv_layer"))
		{
			dynamic_cast<Conv_layer*>(this->Layer_list_->at(i))->ShafelFilters();
		}
		else if (!this->topology_->at(i).compare("FC_layer"))
		{
			dynamic_cast<FC_layer*>(this->Layer_list_->at(i))->ShafelWeights();
		}
	}
}

void Convnet::FeedForward(const string& file_dir_)
{
	this->SetImage(file_dir_);
	for (int i = 1; i < (int)this->topology_->size(); i++)
	{
		if (!this->topology_->at(i).compare("Conv layer"))
			this->Layer_list_->at(i)->Simulate();

		else if (!this->topology_->at(i).compare("ReLU layer"))
			this->Layer_list_->at(i)->Simulate();

		else if (!this->topology_->at(i).compare("Pool layer"))
			this->Layer_list_->at(i)->Simulate();

		else
			this->Layer_list_->at(i)->Simulate();
	}
}

void Convnet::BackPropagation()
{
	(*this->Error_Vec_)		= (*this->Expected_Output_Vec_) - (*this->Layer_list_->back()->GetOutputMatrix().at(0));
	Matrix* gradient_runner = new Matrix(*this->Error_Vec_);
	gradient_runner->DotMatrices(*(dynamic_cast<FC_layer*>(this->Layer_list_->back())->GetDerivativeMatrix()));

	this->Layer_list_->at(this->Layer_list_->size() - 2)->GetOutputMatrix().at(0)->Transpose();
	(*dynamic_cast<FC_layer*>(this->Layer_list_->back())->GetDeltaWeightMatrix()) += (*this->Layer_list_->at(this->Layer_list_->size() - 2)->GetOutputMatrix().at(0)) * (*gradient_runner) * this->Learning_rate_;
	this->Layer_list_->at(this->Layer_list_->size() - 2)->GetOutputMatrix().at(0)->Transpose();

	for (size_t FC_l = this->size_ - 1; (FC_l > 0) && (!this->Layer_list_->at(FC_l - 2)->LayerClassification().compare("FC layer")); FC_l--)
	{
		FC_layer* K = dynamic_cast<FC_layer*>(this->Layer_list_->at(FC_l));
		FC_layer* J = dynamic_cast<FC_layer*>(this->Layer_list_->at(FC_l - 1));
		FC_layer* I = dynamic_cast<FC_layer*>(this->Layer_list_->at(FC_l - 2));

		gradient_runner->Transpose();
		J->GetDerivativeMatrix()->Transpose();
		(*gradient_runner) = (*J->GetDerivativeMatrix()->*((*K->GetWeightMatrix()) * (*gradient_runner)));
		J->GetDerivativeMatrix()->Transpose();
		gradient_runner->Transpose();

		I->GetOutputMatrix().at(0)->Transpose();
		(*J->GetDeltaWeightMatrix()) += ((*I->GetOutputMatrix().at(0)) * (*gradient_runner)) * this->Learning_rate_;
		I->GetOutputMatrix().at(0)->Transpose();
	}
	gradient_runner->~Matrix();
}

void Convnet::SetChangeInWeights(int average)
{
	for (int i = 0; i < (int)this->topology_->size();i++)
	{
		//if (!this->Layer_list_->at(i)->LayerClassification().compare("Conv layer"))
			//dynamic_cast<Conv_layer*>(this->Layer_list_->at(i))->CalculateNewWeights();

		if (!this->Layer_list_->at(i)->LayerClassification().compare("FC layer") && this->Layer_list_->at(i - 1)->LayerClassification().compare("Pool layer"))
			dynamic_cast<FC_layer*>(this->Layer_list_->at(i))->CalculateNewWeights(average);
	}
}

void Convnet::Training(int NumberOfEpoch, int NumberOfSubEpoch, int PrintTime, int average)
{
	this->GenerateRandomNumbers(NumberOfEpoch * NumberOfSubEpoch);
	for (int epoch = 0; epoch < NumberOfEpoch; epoch++)
	{
		for (int Subepoch = 0; Subepoch < NumberOfSubEpoch; Subepoch++)
		{
			this->StringToSemiBinaryNumber(this->RandomNumbers_->back());
			this->FeedForward(this->RandomNumbers_->back());
			this->BackPropagation();
			if (Subepoch < NumberOfSubEpoch - 1)
			{
				this->RandomNumbers_->pop_back();
			}
		}

		this->SetChangeInWeights(average);

		if ((PrintTime != 0) && (epoch % PrintTime == 0))
		{
			this->PrintResult(this->RandomNumbers_->back(), epoch + 1);
		}

		this->RandomNumbers_->pop_back();
	}
}

void Convnet::Simulation()
{
	for (int i = 0; i < (int)this->size_;i++)
	{
		if ((!this->Layer_list_->at(i)->LayerClassification().compare("FC layer")))
			dynamic_cast<FC_layer*>(this->Layer_list_->at(i))->IsTraining(false);
	}
	this->GenerateRandomNumbers(2);
	this->StringToSemiBinaryNumber(this->RandomNumbers_->at(1));
	FeedForward(this->RandomNumbers_->at(1));
	PrintResult(this->RandomNumbers_->at(1),0);

	this->StringToSemiBinaryNumber(this->RandomNumbers_->at(0));
	FeedForward(this->RandomNumbers_->at(0));
	PrintResult(this->RandomNumbers_->at(0), 0);
}

void Convnet::SaveWeightsToDataFile()
{

}

void Convnet::LoadWeightsFromDataFile()
{

}

void Convnet::PrintConvnetTopology() const
{
	cout << "Convolutional Neural Net Work Topology:" << endl;
	for (int i = 0; i < (int)this->topology_->size(); i++)
		cout << "Layer No. " << i + 1 << ", " << this->topology_->at(i).c_str() << "." << endl;
	cout << endl;
}

void Convnet::PrintResult(const string& number, int epoch) const
{
	this->StringToSemiBinaryNumber(number);
	cout << "Epoch No. " << epoch << ": " << "Expected number: " << this->StringToInt(number) << "." << endl;
	cout << "Expected output:  ";
	for (int i = 0; i < 10; i++)
	{
		cout << (*this->Expected_Output_Vec_)(0,i);
		if (i < 9)
			cout << ", ";
	}

	cout << endl << "NeuralNet result: ";
	dynamic_cast<FC_layer*>(this->Layer_list_->back())->Printlayer();
}

void Convnet::PrintWeights() const
{
	/*Not really necessary method.*/
}

void Convnet::GenerateRandomNumbers(int size)
{
	this->RandomNumbers_ = new vector<string>;

	for (int i = 0; i < size; i++)
	{
		if (0)
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXzero");
		else if (0)//i%10 == 1
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXone");
		else if (0)//i%10 == 2
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXtwo");
		else if (0)//i%10 == 3
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXthree");
		else if (0)//i%10 == 4
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfour");
		else if (0)//i%10 == 5
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfive");
		else if (0)//i%10 == 6
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXsix");
		else if (0)//i%10 == 7
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXseven");
		else if (i%2 == 0)//i%10 == 8
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXeight");
		else
			this->RandomNumbers_->push_back("C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXnine");
	}
}

int Convnet::StringToInt(const string& number) const
{
	if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXzero"))
		return 0;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXone"))
		return 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXtwo"))
		return 2;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXthree"))
		return 3;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfour"))
		return 4;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfive"))
		return 5;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXsix"))
		return 6;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXseven"))
		return 7;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXeight"))
		return 8;

	return 9;
}

void Convnet::StringToSemiBinaryNumber(const string& number) const
{
	this->Expected_Output_Vec_->SetMatrixToZero();

	if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXzero"))
		(*this->Expected_Output_Vec_)(0,0) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXone"))
		(*this->Expected_Output_Vec_)(0,1) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXtwo"))
		(*this->Expected_Output_Vec_)(0, 2) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXthree"))
		(*this->Expected_Output_Vec_)(0, 3) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfour"))
		(*this->Expected_Output_Vec_)(0, 4) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXfive"))
		(*this->Expected_Output_Vec_)(0, 5) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXsix"))
		(*this->Expected_Output_Vec_)(0, 6) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXseven"))
		(*this->Expected_Output_Vec_)(0, 7) = 1;
	else if (CheckForSubString(number, "C:/Users/gal/source/repos/Convnet/Convnet/txtnumbers/dataXeight"))
		(*this->Expected_Output_Vec_)(0, 8) = 1;
	else
		(*this->Expected_Output_Vec_)(0, 9) = 1;
}