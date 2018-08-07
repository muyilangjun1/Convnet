#include "Input_layer.h"



Input_layer::Input_layer(size_t Row, size_t Col)
	:
	Layer(Row, Col)
{

}

Input_layer::~Input_layer()
{

}

void Input_layer::SetImage(const string& file_dir_)
{
	string tmp(file_dir_);
	srand((unsigned int)time(0));
	int ran = (rand() % 10) + 1;
	tmp.append(to_string(ran));
	tmp.append(".txt");

	this->data_ = ImportImageData(tmp);
	for (size_t r = 0; r < this->Row_; r++) {
		for (size_t c = 0; c < this->Col_; c++)
		{
			(*this->output_->at(0))(r, c) = stof(this->data_.back());
			this->data_.pop_back();
		}
	}
}

void Input_layer::Simulate()
{
	if (this->output_->empty())
		throw ExceptionHandler("Input matrix is empty.");
}