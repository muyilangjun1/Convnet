#include <iostream>
#include <string>
#include <opencv2\opencv.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <stdint.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <ExceptionHandler.h>
#include <vector>

using namespace std;
using namespace experimental;
using namespace filesystem;
using namespace exceptionh;
using namespace cv;

/*ResizeImage.
* resize the image to 128x128 pixels and save it in different folder.
*/
void ResizeImage(const string& target_file, const string& save_in_this_folder);

/*ResizeAllImages.
* resize all images in the folder to 128x128 pixels and save it in different folder.
*/
void ResizeAllImages(const string& target_dir, string& save_in_this_folder);

/*ConvertImage.
* this method takes a jpg image and convert each pixel to 1 or 0 and save it in a text file.
* 1 represent white color and 0 represent black color.
* the size of the image needs to be 128x128 (16,384 input neurons).
*
*	params@: file_name		   - the name of the text file (e.g: "data.txt" or "dir/data.txt")
*							     if the file dose not exist the method create one with the given name.
*			 save_in_this_file - if you want to save the convertion in a specific data file.
*								 also create file if the file dose not exist.
*/
void ConvertImage(const string& file_name, const string& save_in_this_file);

void ConvertAllImages(const string& file_dir, const string& save_in_this_file);

/*ImportImageData.
* open the text file and load the data into string object.
*/
vector<string> ImportImageData(const string& file_dir_);

/*CheckForSubString.
* check if the file ends with .png or .jpg . (i.e: check if the file is an image)
* return true if the file ends with .png or .jpg and false otherwise.
*/
bool CheckForSubString(const string& source, const string& substring);

/*EditFileName.
* return just the name of the file, without the ending and path.
*/
string EditFileName(const string& source, bool withjpg);