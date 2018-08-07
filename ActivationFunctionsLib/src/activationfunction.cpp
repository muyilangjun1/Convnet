#include "activationFunction.h"


namespace activeF {
	float ActivationF(float x, float alpha, bool derivative, int AFindex) {

		if (AFindex < 1 || AFindex > 17) {
			throw ExceptionHandler("The activation function index is out of range!");
		}

		switch (AFindex)
		{
		case 1: {
			return sigmoidFunc(x, derivative);
		}
		case 2: {
			return tanhFunc(x, derivative);
		}
		case 3: {
			return cotFunc(x, derivative);
		}
		case 4: {
			return SoftsignFunc(x, derivative);
		}
		case 5: {
			return ISRUFunc(alpha, x, derivative);
		}
		case 6: {
			return LeakyReLUFunc(x, derivative);
		}
		case 7: {
			return PReLUFunc(alpha, x, derivative);
		}
		case 8: {
			return ELUFunc(alpha, x, derivative);
		}
		case 9: {
			return ISRLUFunc(alpha, x, derivative);
		}
		case 10: {
			return SoftPlusFunc(x, derivative);
		}
		case 11: {
			return BentIdentityFunc(x, derivative);
		}
		case 12: {
			return SiLUFunc(x, sigmoidFunc(x, false), sigmoidFunc(x, true), derivative);
		}
		case 13: {
			return SoftEXPFunc(alpha, x, derivative);
		}
		case 14: {
			return SinusoidFunc(x, derivative);
		}
		case 15: {
			return ReLUFunc(x, derivative);
		}
		case 16: {
			return SincFunc(x, derivative);
		}
		default:
			break;
		}
		return GaussianFunc(x, derivative);
	}

	float sigmoidFunc(float x, bool derivative) {
		if (derivative == true) {
			return exp(-x) / pow(1.0 + exp(-x), 2);
		}
		return 1.0 / (1.0 + exp(-x));
	}

	float tanhFunc(float x, bool derivative) {
		if (derivative == true) {
			return 1 - pow(tanh(x), 2);
		}
		return tanh(x);
	}

	float cotFunc(float x, bool derivative) {
		if (derivative == true) {
			return 1.0 / (1.0 + pow(x, 2));
		}
		return 1.0 / tan(x);
	}

	float SoftsignFunc(float x, bool derivative) {
		if (derivative == true) {
			return 1.0 / pow(1 + abs(x), 2);
		}
		return x / (1 + abs(x));
	}

	float ISRUFunc(float alpha, float x, bool derivative) {
		if (derivative == true) {
			return pow(1.0 / sqrt(1.0 + alpha * pow(x, 2)), 2);
		}
		return x / sqrt(1.0 + alpha * pow(x, 2));
	}

	float ReLUFunc(float x, bool derivative) {
		if (derivative == true) {
			float val = (x > 0.0) ? 1.0 : 0.0;
			return val;
		}
		float val = (x >= 0.0) ? x : 0.0;
		return val;
	}

	float LeakyReLUFunc(float x, bool derivative) {
		if (derivative == true) {
			float val = (x > 0.0) ? 1.0 : 0.01;
			return val;
		}
		float val = (x >= 0.0) ? x : (0.01 * x);
		return val;
	}

	float PReLUFunc(float alpha, float x, bool derivative) {
		if (derivative == true) {
			float val = (x >= 0.0) ? 1.0 : alpha;
			return val;
		}
		float val = (x >= 0.0) ? x : (alpha * x);
		return val;
	}

	float ELUFunc(float alpha, float x, bool derivative) {
		if (derivative == true) {
			float val = (x >= 0.0) ? 1 : (alpha*exp(x));
			return val;
		}
		float val = (x >= 0.0) ? x : (alpha*(exp(x) - 1));
		return val;
	}

	float ISRLUFunc(float alpha, float x, bool derivative) {
		if (derivative == true) {
			float val = (x >= 0.0) ? 1 : pow((1.0 / sqrt(1.0 + alpha * pow(x, 2))), 3);
			return val;
		}
		float val = (x >= 0.0) ? x : (x / sqrt(1.0 + alpha * pow(x, 2)));
		return val;
	}

	float SoftPlusFunc(float x, bool derivative) {
		if (derivative == true) {
			return 1.0 / (1.0 + exp(-x));
		}
		return log(1.0 + exp(x));
	}

	float BentIdentityFunc(float x, bool derivative) {
		if (derivative == true) {
			return 1 + (x / (2 * sqrt(pow(x, 2) + 1.0)));
		}
		return ((sqrt(pow(x, 2) + 1.0) - 1.0) / 2.0) + x;
	}

	float SiLUFunc(float x, float FuncResult, float DivFuncResult, bool derivative) {
		if (derivative == true) {
			return FuncResult + x * DivFuncResult;
		}
		return x * FuncResult;
	}

	float SoftEXPFunc(float alpha, float x, bool derivative) {
		if (derivative == true) {
			if (alpha < 0.0) {
				return 1.0 / (1.0 - alpha * (x + alpha));
			}
			else {
				return exp(alpha * x);
			}
		}
		else {
			if (alpha < 0.0) {
				return -(log(1 - alpha * (x + alpha))) / alpha;
			}
			else if (alpha == 0.0) {
				return x;
			}
			else {
				return alpha + ((exp(alpha * x) - 1.0) / alpha);
			}
		}
	}

	float SinusoidFunc(float x, bool derivative) {
		if (derivative == true) {
			return cos(x);
		}
		return sin(x);
	}

	float SincFunc(float x, bool derivative) {
		if (derivative == true) {
			float val = (x != 0.0) ? ((cos(x) / x) - (sin(x) / x)) : 0.0;
			return val;
		}
		float val = (x != 0.0) ? (sin(x) / x) : 1.0;
		return val;
	}

	float GaussianFunc(float x, bool derivative) {
		if (derivative == true) {
			return -2 * x*exp(-pow(x, 2));
		}
		return exp(-pow(x, 2));
	}
}