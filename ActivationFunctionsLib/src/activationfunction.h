#ifndef _ACTIVATION_FUNCTIONS_H_
#define _ACTIVATION_FUNCTIONS_H_

#include <iostream>
#include <math.h>
#include <ExceptionHandler.h>
using namespace std;
using namespace exceptionh;

namespace activeF {
	float ActivationF(float x, float alpha = 1.0, bool derivative = false, int AFindex = 1);
	float sigmoidFunc(float x, bool derivative = false);
	float tanhFunc(float x, bool derivative = false);
	float cotFunc(float x, bool derivative = false);
	float SoftsignFunc(float x, bool derivative = false);
	float ISRUFunc(float alpha, float x, bool derivative = false);
	float LeakyReLUFunc(float x, bool derivative = false);
	float PReLUFunc(float alpha, float x, bool derivative = false);
	float ELUFunc(float alpha, float x, bool derivative = false);
	float ISRLUFunc(float alpha, float x, bool derivative = false);
	float SoftPlusFunc(float x, bool derivative = false);
	float BentIdentityFunc(float x, bool derivative = false);
	float SiLUFunc(float x, float FuncResult, float DivFuncResult, bool derivative = false);
	float SoftEXPFunc(float alpha, float x, bool derivative = false);
	float SinusoidFunc(float x, bool derivative = false);
	float ReLUFunc(float x, bool derivative = false);
	float SincFunc(float x, bool derivative = false);
	float GaussianFunc(float x, bool derivative = false);
}
#endif
