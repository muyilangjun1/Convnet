#include "ImageEditor.h"


void ResizeImage(const string& target_file, const string& save_in_this_folder) {

	Mat src = imread(target_file.c_str(), CV_LOAD_IMAGE_UNCHANGED);
	Mat dst;
	resize(src, dst, Size(64, 64), 0, 0, INTER_LINEAR);
	imwrite(save_in_this_folder.c_str(), dst);
}

void ResizeAllImages(const string& target_dir, string& save_in_this_folder) {

	string tmp;
	for (auto & p : directory_iterator(target_dir)) {
		if (CheckForSubString(p.path().string(), ".PNG") || CheckForSubString(p.path().string(), ".JPG") ||
			CheckForSubString(p.path().string(), ".png") || CheckForSubString(p.path().string(), ".jpg")) {
			tmp = EditFileName(p.path().string(),true);
			ResizeImage(p.path().string(), save_in_this_folder.append(tmp));
			save_in_this_folder.erase(save_in_this_folder.end() - (int)tmp.size(), save_in_this_folder.end());
		}
	}
}

void ConvertImage(const string& file_name, const string& save_in_this_file) {

	Mat test = imread(file_name.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
	ofstream myfile;
	string substring;
	substring.append("dataX");
	substring.append(EditFileName(file_name, false));
	substring.append(".txt");
	if (save_in_this_file != "") {
		myfile.open(save_in_this_file.c_str(), ios::app);
	}
	else {
		myfile.open(substring.c_str(), ios::app);
	}

	for (int r = 0; r < test.rows; r++) {
		for (int c = 0; c < test.cols; c++) {
			myfile << (255 - (int)test.at<uint8_t>(r, c)) << endl;
		}
	}
	myfile.close();
}

vector<string> ImportImageData(const string& file_dir_) {
	string data;
	vector<string> vec;
	ifstream myfile(file_dir_);
	if (myfile.is_open())
	{
		while (getline(myfile,data))
		{
			string tmp(data);
			vec.push_back(tmp);
		}
	}
	myfile.close();
	return vec;
}

bool CheckForSubString(const string& source, const string& substring) {

	if (source.size() < substring.size()) {
		return false;
	}

	for (int i = 0; i < (int)substring.size(); i++) {
		if (source.at((int)source.size() - 1 - i) != substring.at((int)substring.size() - 1 - i)) {
			return false;
		}
	}

	return true;
}

string EditFileName(const string& source, bool withjpg) {
	string substring;
	int count = (int)source.size() - 1;
	int stop = 0;
	for (int i = (int)source.size() - 1; i >= 0; i--) {
		if (source.at(i) == '\\') {
			substring.append(source.begin() + i + 1, source.begin() + count);
			if (withjpg) {
				substring.append(".jpg");
			}
			return substring;
		}
		if (stop == 0) {
			if (source.at(i) == '.') {
				stop = 1;
			}
			else {
				count--;
			}
		}
	}
	return "";
}

void ConvertAllImages(const string& file_dir, const string& save_in_this_file) {

	for (auto & p : directory_iterator(file_dir)) {
		if (CheckForSubString(p.path().string(), ".PNG") || CheckForSubString(p.path().string(), ".JPG") ||
			CheckForSubString(p.path().string(), ".png") || CheckForSubString(p.path().string(), ".jpg")) 
		{
			ConvertImage(p.path().string(), save_in_this_file);
		}
	}
}