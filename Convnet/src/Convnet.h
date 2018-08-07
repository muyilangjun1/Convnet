#ifndef _CONVOLUTIONAL_NEURAL_NET_WORK_
#define _CONVOLUTIONAL_NEURAL_NET_WORK_

#include "Input_layer.h"
#include "Conv_layer.h"
#include "FC_layer.h"
#include "Pool_layer.h"
#include "ReLU_Layer.h"
#include <math.h>
using namespace std;
using namespace Mat;
using namespace exceptionh;

class Convnet
{
public:

	Convnet();
	Convnet(const vector<string>& topology, size_t Row = 128, size_t Col = 128);
	~Convnet();

	size_t GetNumberOfLayers() const;

	void AddFClayer();
	void EraseFClayer();

	void SetImage(const string& file_dir_ = "");
	void SetBias(float Val);
	void SetLearningRate(float Val);
	void SetActivationFunction(int Idx);
	void ShafelWeights();

	void FeedForward(const string& file_dir_ = "");
	void BackPropagation();
	void SetChangeInWeights(int average);
	void Training(int NumberOfEpoch, int NumberOfSubEpoch, int PrintTime = 0, int average = 1);
	void Simulation();
	void SaveWeightsToDataFile();
	void LoadWeightsFromDataFile();

	void PrintConvnetTopology() const;
	void PrintResult(const string& number, int epoch = 0) const;
	void PrintWeights() const;

protected:

	int StringToInt(const string& number) const;
	void GenerateRandomNumbers(int size);
	void StringToSemiBinaryNumber(const string& number) const;
	size_t size_;
	vector<Layer*>* Layer_list_;
	vector<string>* topology_;
	vector<string>* RandomNumbers_;
	float Learning_rate_;
	Matrix* Error_Vec_;
	Matrix* Expected_Output_Vec_;
};

#endif