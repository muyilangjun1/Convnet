#include "Convnet.h"
#include "ImageEditor.h"
#include <iostream>

using namespace std;

int main()
{
	/*string folder("C:/Users/gal/source/repos/Convnet/Convnet/numbers/");
	string folder2("C:/Users/gal/Pictures/num/eight");
	string folder3("");
	//ResizeAllImages(folder2, folder);
	ConvertAllImages(folder,folder3);*/
	int total_training, sub_training, print_num, avg;
	cin >> total_training >> sub_training >> print_num >> avg;

	Convnet* cnn = new Convnet();
	cnn->Training(total_training,sub_training,print_num,avg);
	cnn->Simulation();
	
	return 0;
}