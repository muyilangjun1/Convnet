#include "Matrix.h"


namespace Mat {
	Matrix::Matrix(size_t Row, size_t Col, int RandomNumber, ElementValues Chosen)
		:
		Row_(Row),
		Col_(Col)
	{
		this->Array_ = new float[Row * Col];
		srand((unsigned int)time(0) + RandomNumber);
		for (size_t r = 0; r < this->Row_; r++)
		{
			for (size_t c = 0; c < this->Col_; c++)
			{
				if (Chosen == Zeroes)
					this->Array_[r * Col_ + c] = 0.0;
				else if (Chosen == Ones)
					this->Array_[r * Col_ + c] = 1;
				else
					this->Array_[r * Col_ + c] = (float)rand() / (RAND_MAX) - 0.2f;
			}
		}
	}

	Matrix::Matrix(const Matrix& rhs)
	{
		Checker(rhs, 3);
		this->Row_ = rhs.GetRowSize();
		this->Col_ = rhs.GetColSize();
		this->Array_ = new float[Row_ * Col_];

		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_; c++)
			{
				this->Array_[r * Col_ + c] = rhs(r, c);
			}
	}

	Matrix::~Matrix()
	{
		if (this->Array_ != NULL)
			delete this->Array_;
	}

	size_t Matrix::GetRowSize() const
	{
		return this->Row_;
	}

	size_t Matrix::GetColSize() const
	{
		return this->Col_;
	}

	float* Matrix::GetMatrix() const
	{
		return this->Array_;
	}

	bool Matrix::IsEmpty() const
	{
		if (this->Row_ == 0 || this->Col_ == 0)
			return true;
		return false;
	}

	void Matrix::operator++()
	{
		Checker(1);
		this->operator+=(1);
	}

	void Matrix::operator--()
	{
		Checker(1);
		this->operator-=(1);
	}

	bool Matrix::operator==(const Matrix& rhs) const
	{
		Checker(1);

		if (this->Row_ != rhs.GetRowSize() || this->Col_ != rhs.GetColSize())
			return false;
		else if (*this == rhs)
			return true;

		float* tmp = rhs.GetMatrix();
		if (tmp == NULL)
			return false;

		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_; c++)
				if (this->Array_[r * this->Col_ + c] != tmp[r * this->Col_ + c])
					return false;

		return true;
	}

	bool Matrix::operator!=(const Matrix& rhs) const
	{
		return !this->operator==(rhs);
	}

	Matrix Matrix::operator+(const Matrix& rhs)
	{
		Checker(rhs, 1);

		Matrix tmp(*this);
		tmp += rhs;
		return tmp;
	}

	Matrix Matrix::operator-(const Matrix& rhs)
	{
		Checker(1);
		Matrix tmp(*this);
		tmp -= rhs;
		return tmp;
	}

	Matrix Matrix::operator*(const Matrix& rhs)
	{	
		Matrix tmp(*this);
		tmp *= rhs;
		return tmp;
	}

	Matrix Matrix::operator*(float Val)
	{
		Checker(1);

		Matrix tmp(*this);
		tmp *= Val;
		return tmp;
	}

	Matrix Matrix::operator^(float ToThePowerOf)
	{
		Checker(1);

		Matrix tmp(*this);
		tmp ^= ToThePowerOf;
		return tmp;
	}

	Matrix Matrix::operator/(float Val)
	{
		Checker(1);
		if (Val == 0)
			throw ExceptionHandler("You cannot divide by zero.");

		Matrix tmp(*this);
		tmp /= Val;
		return tmp;
	}

	Matrix Matrix::operator->*(const Matrix& rhs)
	{
		Matrix tmp(rhs);
		for (size_t r = 0; r < this->Row_; r++)
		{
			for (size_t c = 0; c < this->Col_; c++)
			{
				tmp(r, c) = this->Array_[r * this->Col_ + c] * rhs(r,c);
			}
		}
		return tmp;
	}

	Matrix& Matrix::operator=(const Matrix& rhs)
	{
		if (this != &rhs)
		{
			if (this->Array_ != NULL)
			{
				delete this->Array_;
			}
			this->Row_ = rhs.GetRowSize();
			this->Col_ = rhs.GetColSize();
			this->Array_ = new float[Row_ * Col_];
			
			concurrency::extent<2> e((int)Row_, (int)Col_);
			concurrency::array_view<const float, 2> other(e, rhs.GetMatrix());
			concurrency::array_view<float, 2> target(e, Array_);

			target.discard_data();
			if (this->Row_ == 1)
			{
				parallel_for_each(e.tile<1, 16>(), [=](concurrency::tiled_index<1, 16> t_idx) restrict(amp)
				{
					target[t_idx.global] = other[t_idx.global];
				});
				target.synchronize();
			}
			else
			{
				parallel_for_each(e.tile<16, 1>(), [=](concurrency::tiled_index<16, 1> t_idx) restrict(amp)
				{
					target[t_idx.global] = other[t_idx.global];
				});
				target.synchronize();
			}
		}

		return *this;
	}

	Matrix& Matrix::operator+=(const Matrix& rhs)
	{
		Checker(rhs, 1);

		concurrency::extent<2> e((int)this->Row_, (int)this->Col_);
		concurrency::array_view<const float, 2> other(e, rhs.GetMatrix());
		concurrency::array_view<float, 2> target(e, this->Array_);

		parallel_for_each(e.tile<16, 16>(), [=](concurrency::tiled_index<16, 16> t_idx) restrict(amp)
		{
			target[t_idx.global] += other[t_idx.global];
		});

		target.synchronize();
		return *this;
	}

	Matrix& Matrix::operator-=(const Matrix& rhs)
	{
		Checker(rhs, 1);

		concurrency::extent<2> e((int)this->Row_, (int)this->Col_);
		concurrency::array_view<const float, 2> other(e, rhs.GetMatrix());
		concurrency::array_view<float, 2> target(e, this->Array_);

		parallel_for_each(e.tile<1, 16>(), [=](concurrency::tiled_index<1, 16> t_idx) restrict(amp)
		{
			target[t_idx.global] -= other[t_idx.global];
		});

		target.synchronize();
		return *this;
	}

	Matrix& Matrix::operator*=(const Matrix& rhs)
	{
		
		int size = (int)this->Col_;
		float* target = new float[this->Row_ * rhs.GetColSize()];
		concurrency::array_view<const float, 2> a((int)this->Row_,(int)this->Col_, this->Array_);
		concurrency::array_view<const float, 2> b((int)this->Col_,(int)rhs.GetColSize(), rhs.GetMatrix());
		concurrency::array_view<float, 2> c((int)this->Row_, (int)rhs.GetColSize(), target);
		c.discard_data();

		concurrency::parallel_for_each(c.extent, [=](concurrency::index<2> idx) restrict(amp)
		{
			int row = idx[0];
			int col = idx[1];
			float sum = 0.0f;

			for (int i = 0; i < size; i++)
			{
				sum += a(row, i) * b(i, col);
			}

			c[idx] = sum;
		}
		);
		c.synchronize();

		this->Col_ = rhs.GetColSize();
		delete this->Array_;
		this->Array_ = target;

		return *this;
	}

	Matrix& Matrix::operator*=(float Val)
	{
		Checker(1);

		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			target[idx] *= Val;
		});
		target.synchronize();
		return *this;
	}

	Matrix& Matrix::operator+=(float Val)
	{
		Checker(1);

		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			target[idx] += Val;
		});
		target.synchronize();
		return *this;
	}

	Matrix& Matrix::operator-=(float Val)
	{
		Checker(1);

		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			target[idx] -= Val;
		});
		target.synchronize();
		return *this;
	}

	Matrix& Matrix::operator^=(float ToThePowerOf)
	{
		concurrency::extent<2> e((int)this->Row_, (int)this->Col_);
		concurrency::array_view<float, 2> target(e, this->Array_);

		parallel_for_each(e.tile<1, 2>(), [=](concurrency::tiled_index<1, 2> t_idx) restrict(amp)
		{
			concurrency::precise_math::powf(target[t_idx.global],ToThePowerOf);
		});
		target.synchronize();

		return *this;
	}

	Matrix& Matrix::operator/=(float Val)
	{
		Checker(1);
		if (Val == 0)
			throw ExceptionHandler("You cannot divide by zero.");
		
		this->operator*=(1 / Val);
		return *this;
	}

	ostream& operator<<(ostream& os, const Matrix& rhs) 
	{
		for (int r = 0; r < rhs.Row_; r++)
		{
			os << rhs.Array_[r * rhs.Col_];
			for (int c = 1; c < rhs.Col_; c++)
				os << ", " << rhs.Array_[r * rhs.Col_ + c];
			os << endl;
		}
		os << endl;
		return os;
	}

	istream& operator>>(istream& is, Matrix& rhs)
	{
		for (int r = 0; r < rhs.Row_; r++)
			for (int c = 1; c < rhs.Col_; c++)
				is >> rhs.Array_[r * rhs.Col_ + c];
		return is;
	}

	float& Matrix::operator()(size_t RowIdx, size_t ColIdx)
	{
		Checker(1);
		return this->Array_[RowIdx * this->Col_ + ColIdx];
	}

	float Matrix::operator()(size_t RowIdx, size_t ColIdx) const
	{
		Checker(1);
		return this->Array_[RowIdx * this->Col_ + ColIdx];
	}

	void Matrix::SetMatrixToZero()
	{
		Checker(1);

		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			target[idx] = 0;
		});
		target.synchronize();

	}

	void Matrix::Transpose()
	{
		Checker(1);

		concurrency::extent<2> e((int)this->Row_, (int)this->Col_);
		concurrency::extent<2> e2((int)this->Col_, (int)this->Row_);
		float* Arr = new float[this->Row_ * this->Col_];
		concurrency::array_view<float, 2> other(e2, Arr);
		concurrency::array_view<float, 2> target(e, this->Array_);

		if (this->Col_ >= 16)
		{
			parallel_for_each(e.tile<1, 16>(), [=](concurrency::tiled_index<1, 16> t_idx) restrict(amp)
			{
				other(t_idx.global[1], t_idx.global[0]) = target(t_idx.global[0], t_idx.global[1]);
			});
			target.synchronize();
		}
		else
		{
			parallel_for_each(e.tile<16, 1>(), [=](concurrency::tiled_index<16, 1> t_idx) restrict(amp)
			{
				other(t_idx.global[1], t_idx.global[0]) = target(t_idx.global[0], t_idx.global[1]);
			});
			target.synchronize();
		}
		
		size_t tmp = this->Row_;
		this->Row_ = this->Col_;
		this->Col_ = tmp;
		delete this->Array_;
		this->Array_ = Arr;
	}

	void Matrix::ResizeMatrix(size_t Row, size_t Col)
	{
		if (this->Array_ != NULL)
		{
			delete this->Array_;
		}

		this->Row_ = Row;
		this->Col_ = Col;
		this->Array_ = new float[Row * Col];
		ShafelValues();
	}

	void Matrix::ShafelValues()
	{
		Checker(1);

		srand((unsigned int)time(0));
		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_; c++)
				this->Array_[r * this->Col_ + c] = (float)rand() / (RAND_MAX);
	}

	void Matrix::PrintMatrix() const
	{
		cout << "Array Elements:" << endl;
		for (size_t r = 0; r < this->Row_; r++)
		{
			for (size_t c = 0; c < this->Col_; c++)
				cout << this->Array_[r * this->Col_ + c] << ", ";
			cout << endl;
		}

		cout << endl;
	}

	void Matrix::Convolution(Matrix& target, const Matrix& source)
	{	
		float* tmp = new float[source.GetRowSize() * source.GetColSize()];
		float* tmp_filter = new float[this->Row_];
		float* tmp_array_target = target.GetMatrix();
		concurrency::extent<2> e((int)source.GetRowSize(),(int)source.GetColSize());
		concurrency::array_view<float, 2> img_source(e, source.GetMatrix()), tmp_buffer(e, tmp);

		int radius = (int)(this->Row_ / 2);
		for (int i = 0; i < this->Row_; i++)
		{
			for (int j = 0; j < this->Row_;j++)
			{
				tmp_filter[j] = this->Array_[i * this->Col_ + j];
			}

			concurrency::array_view<float, 1> filter((int)this->Row_, tmp_filter);
			tmp_buffer.discard_data();

			concurrency::parallel_for_each(img_source.extent, [=](concurrency::index<2> idx) restrict(amp)
			{
				float sum = 0.0f;
				for (int k = -radius; k <= radius; k++)
				{
					if ((int)(idx[1]) + k >= 0 && (int)(idx[1]) + k < (int)(img_source.extent[1]))
					{
						int dim = concurrency::direct3d::clamp((int)(idx[1]) + k, 0, (int)(img_source.extent[1] - 1));

						concurrency::index<2> aidx(idx);
						aidx[1] = dim;

						concurrency::index<1> kidx(k + radius);
						sum += img_source[aidx] * filter[kidx];
					}
				}
				tmp_buffer[idx] = sum;
			}
			);
			tmp_buffer.synchronize();

			for (int j = 0; j < e[0] * e[1]; j++)
			{
				if (i == 0)
				{
					if (j < e[0] * (e[1] - 1))
						tmp_array_target[j + e[1]] += tmp[j];
				}

				else if (i == 2)
				{
					if (j < e[0] * (e[1] - 1))
						tmp_array_target[j] += tmp[j + e[1]];
				}

				else
				{
					tmp_array_target[j] += tmp[j];
				}
			}
		}

		delete tmp, tmp_filter;
		
		/*
		int row = (int)source.GetRowSize(), col = (int)source.GetColSize();
		float* A = source.GetMatrix();
		float* filter = this->Array_;
		float* B = target.GetMatrix();
		float firstsum = 0, secondsum = 0, thirdsum = 0;

		for (int r = 0; r < row; r++)
		{
			for (int c = 0; c < col; c++)
			{
				if (r == 0)
				{
					if (c != col - 1)
					{
						firstsum += A[c] * filter[3];
						firstsum += A[col + c] * filter[6];
					}
					secondsum += A[c] * filter[4];
					secondsum += A[col + c] * filter[7];
					if (c != 0)
					{
						thirdsum += A[c] * filter[5];
						thirdsum += A[col + c] * filter[8];
					}
				}

				else if (r == row - 1)
				{
					if (c != col - 1)
					{
						firstsum += A[col*(row - 2) + c] * filter[0];
						firstsum += A[col*(row - 1) + c] * filter[3];
					}
					secondsum += A[col*(row - 2) + c] * filter[1];
					secondsum += A[col*(row - 1) + c] * filter[4];
					if (c != 0)
					{
						thirdsum += A[col*(row - 2) + c] * filter[2];
						thirdsum += A[col*(row - 1) + c] * filter[5];
					}
				}

				else if (0 < r && r < row - 1)
				{
					if (c != col - 1)
					{
						firstsum += A[(r - 1) * col + c] * filter[0];
						firstsum += A[r * col + c] * filter[3];
						firstsum += A[(r + 1) * col + c] * filter[6];
					}
					secondsum += A[(r - 1) * col + c] * filter[1];
					secondsum += A[r * col + c] * filter[4];
					secondsum += A[(r + 1) * col + c] * filter[7];
					if (c != 0)
					{
						thirdsum += A[(r - 1) * col + c] * filter[2];
						thirdsum += A[r * col + c] * filter[5];
						thirdsum += A[(r + 1) * col + c] * filter[8];
					}
				}

				B[r * col + c] += secondsum;
				if (c != col - 1)
					B[r * col + c + 1] += firstsum;
				if (c != 0)
					B[r * col + c - 1] += thirdsum;

				firstsum = 0;
				secondsum = 0;
				thirdsum = 0;
			}
		}*/
	}

	void Matrix::ReLUMatrix(const Matrix& source, float bias)
	{
		concurrency::extent<2> e((int)this->Row_, (int)this->Col_);
		concurrency::array_view<const float, 2> other((int)source.GetRowSize(), (int)source.GetColSize(), source.GetMatrix());
		concurrency::array_view<float, 2> target(e, this->Array_);
		target.discard_data();
		parallel_for_each(e, [other, target, bias](concurrency::index<2> idx) restrict(amp)
		{
			target[idx] = (other[idx] + bias >= 0.0) ? (other[idx] + bias) : 0.0f;
		});
		target.synchronize();
	}

	void Matrix::SoftMaxMatrix()
	{
		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		float sum = 0.0f;
		parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
		{
			target[idx] = concurrency::precise_math::expf(target[idx]);
		});
		target.synchronize();

		for (int i = 0; i < this->Row_ * this->Col_; i++)
		{
			sum += this->Array_[i];
		}

		for (int i = 0; i < this->Row_ * this->Col_; i++)
		{
			this->Array_[i] /= sum;
		}
	}

	void Matrix::SVMPoolMatrix(const Matrix& source)
	{
		static const int TS = 2;
		int col = (int)this->Col_;
		concurrency::extent<2> e((int)source.GetRowSize(), (int)source.GetColSize());
		concurrency::array_view<const float, 2> other(e, source.GetMatrix());
		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		target.discard_data();

		parallel_for_each(e.tile<TS, TS>(), [target, other, col](concurrency::tiled_index<TS, TS> t_idx) restrict(amp)
		{

			tile_static float nums[TS][TS];
			nums[t_idx.local[0]][t_idx.local[1]] = other[t_idx.global];
			t_idx.barrier.wait_with_tile_static_memory_fence();
			float num = -FLT_MAX;
			for (int i = 0; i < 2; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					if (num < nums[i][j])
					{
						num = nums[i][j];
					}
				}
			}
			target(t_idx.tile[0] * col + t_idx.tile[1]) = num;
		}
		);
		target.synchronize();
	}

	void Matrix::ActivateMatrix(const Matrix& source, int AFidx, float bias)
	{
		concurrency::array_view<const float, 1> other((int)(this->Row_ * this->Col_), source.GetMatrix());
		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		switch (AFidx)
		{
		case 1://sigmoid
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{
				target[idx] = 1.0f / (1.0f + concurrency::precise_math::expf(-other[idx] - bias));
			}
			);
			target.synchronize();
			break;
		}

		case 2://tanh
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{
				target[idx] = concurrency::precise_math::tanhf(other[idx] + bias);
			}
			);
			target.synchronize();
			break;
		}

		default://ReLU
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{
				target[idx] = (other[idx] + bias >= 0.0f) ? (other[idx] + bias) : 0.0f;
			}
			);
			target.synchronize();
			break;
		}
		}
	}

	void Matrix::DerivativeMatrix(const Matrix& source, int AFidx, float bias)
	{
		concurrency::array_view<const float, 1> other((int)(this->Row_ * this->Col_), source.GetMatrix());
		concurrency::array_view<float, 1> target((int)(this->Row_ * this->Col_), this->Array_);
		switch (AFidx)
		{
		case 1:
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{	
				target[idx] = concurrency::precise_math::expf(-other[idx]-bias) / concurrency::precise_math::pow(1.0f + concurrency::precise_math::expf(-other[idx]-bias),2);
			}
			);
			target.synchronize();
			break;
		}

		case 2:
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{
				target[idx] = 1.0f - concurrency::precise_math::powf(concurrency::precise_math::tanhf(other[idx]+bias), 2);
			}
			);
			target.synchronize();
			break;
		}

		default:
		{
			parallel_for_each(target.extent, [=](concurrency::index<1> idx) restrict(amp)
			{
				target[idx] = (other[idx]+bias >= 0.0f) ? 1.0f : 0.0f;
			}
			);
			target.synchronize();
			break;
		}
		}
	}

	void Matrix::DotMatrices(const Matrix& rhs)
	{
		Checker(rhs, 1);
		float* tmp = rhs.GetMatrix();
		for (int r = 0; r < this->Row_; r++)
			for (int c = 0; c < this->Col_; c++)
				this->Array_[r * this->Col_ + c] *= tmp[r * this->Col_ + c];

	}

	void Matrix::ReduceMatricesToOne(const vector<Matrix*>& rhs)
	{
		int R_size = (int)rhs.at(0)->GetRowSize();
		int C_size = (int)rhs.at(0)->GetColSize();
		for (int s = 0; s < (int)rhs.size();s++)
		{
			float* tmp = rhs.at(s)->GetMatrix();
			for (int r = 0; r < R_size; r++)
				for (int c = 0; c < C_size; c++)
					this->Array_[s * R_size * C_size + r * C_size + c] = tmp[r * C_size + c];
		}
	}

	void Matrix::Norm()
	{
		float sum = 0.0f;

		for (int i = 0; i < this->Row_ * this->Col_; i++)
		{
			sum += this->Array_[i] * this->Array_[i];
		}

		if (sum != 0.0f)
		{
			for (int i = 0; i < this->Row_ * this->Col_; i++)
			{
				this->Array_[i] /= sqrt(sum);
			}
		}
	}

	Matrix Matrix::MatrixMul(const Matrix& rhs)
	{
		Checker(rhs, 2);
		Matrix Tmp(1,10,0,Zeroes);
		concurrency::extent<2> e1((int)this->Row_, (int)this->Col_);
		concurrency::extent<2> e2((int)rhs.GetRowSize(), (int)rhs.GetColSize());
		concurrency::extent<2> e3(e1[0], e2[1]);
		concurrency::array_view<const float, 2> vec(e1, this->Array_);
		concurrency::array_view<const float, 2> mat(e2, rhs.GetMatrix());
		concurrency::array_view<float, 2> product(e3, Tmp.GetMatrix());

		parallel_for_each(e3.tile<1, 2>(), [=](concurrency::tiled_index<1, 2> t_idx) restrict(amp)
		{
			int row = t_idx.local[0];
			int col = t_idx.local[1];
			float sum = 0;
			tile_static float locA[2][1], locB[2][1];

			for (int i = 0; i < e2[1]; i += 2)
			{
				locA[row][col] = vec(t_idx.global[0], col + i);
				locB[row][col] = mat(row + i, t_idx.global[1]);
				t_idx.barrier.wait_with_tile_static_memory_fence();

				for (int k = 0; k < e1[1]; k++)
					sum += locA[row][k] * locB[k][col];
				t_idx.barrier.wait_with_tile_static_memory_fence();
			}
			product[t_idx.global] = sum;
		});
		product.synchronize();

		return Tmp;
	}

	Matrix Matrix::ColumnSum() const
	{
		Checker(1);

		Matrix tmp(this->Row_,1,0,Zeroes);
		float sum = 0.0;
		for (size_t r=0;r<this->Row_;r++)
		{
			for (size_t c = 0; c < this->Col_; c++)
			{
				sum += this->Array_[r * this->Col_ + c];
			}
			tmp.operator()(r,1) = sum;
			sum = 0;
		}

		return tmp;
	}

	/*Matrix Matrix::Inverse()
	{
		Checker(4);
		Matrix tmp(this->Row_,this->Col_,0,Zeroes);
		double** tmp2 = this->GaussianElimination();
		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_; c++)
				tmp(r,c) = (float)tmp2[r][c];

		return tmp;
	}*/

	/*float Matrix::Determinant() const
	{
		Checker(3);
		double** tmp = this->GaussianElimination(true);
		double sum = 0.0;
		for (size_t r = 0; r < this->Row_; r++)
			sum *= tmp[r][r];

		delete tmp;
		return (float)sum;
	}*/

	/*Matrix Matrix::Eigenvalues() const
	{
		Checker(4);
		double** tmp2 = this->GaussianElimination(true);
		Matrix tmp(1, this->Col_, 0, Zeroes);

		for (size_t c = 0; c < this->Col_; c++)
			tmp(1,c) = tmp2[c][c];

		delete tmp2;
		return tmp;
	}*/

	/*float* Matrix::GaussianElimination(bool isInverse) const
	{
		Checker(1);
		float EPS = 0.000001;
		float* tmp = new float[this->Row_ * this->Col_];
		float* IndentityMat = new float[this->Row_ * this->Col_];
		for (size_t r = 0; r < this->Row_; r++)
		{
			for (size_t c = 0; c < this->Col_; c++)
			{
				tmp[r * this->Col_ + c] = this->Array_[r * this->Col_ + c];
				IndentityMat[r * this->Col_ + c] = 0.0f;
			}
			IndentityMat[r * this->Col_ + r] = 1;
		}

		for (size_t i = 0; i < this->Row_; i++)
		{
			if (tmp[i * this->Col_ + i] == 0)
			{
				bool pivot_found = false;
				for (size_t j = i + 1; j < this->Row_; j++)
				{
					if (tmp[j * this->Col_ + i] != 0)
					{
						this->SwapRows(tmp,IndentityMat,i,j);
						pivot_found = true;
						break;
					}
				}

				if (!pivot_found)
				{
					delete tmp;
					delete IndentityMat;
					throw ExceptionHandler("The matrix isn't inversable.");
				}
			}
			
			float Scale = tmp[i * this->Col_ + i];
			concurrency::array_view<float,2> A(this->Row_,this->Col_,tmp);
			parallel_for_each(A.extent, [=](concurrency::index<2> idx) restrict(amp)
			{
				A[idx] /= Scale;
			}
			);
			A.synchronize();

			for (size_t j = 0; j < this->Row_; j++)
			{
				if (j == i)
					continue;

				double num = A[j][i];
				array_view<const double,1> A(this->Col_, tmp[i]);
				array_view<double, 1> B(this->Col_, tmp[j]);
				parallel_for_each(A.extent, [=](index<1> idx) restrict(amp)
				{
					B[idx] -= num * A[idx];
				}
				);
				A.synchronize();
			}
		}

		if (isInverse)
		{
			delete IndentityMat;
			return tmp;
		}

		delete tmp;
		return IndentityMat;
	}
	*/
	/*void Matrix::SwapRows(double** A, double** B, size_t row1, size_t row2) const
	{
		double* tmp1 = A[row1];
		A[row1] = A[row2];
		A[row2] = tmp1;

		double* tmp2 = B[row1];
		B[row1] = B[row2];
		B[row2] = tmp2;

		array_view<double,2> target1(this->Row_,this->Col_,A);
		array_view<double, 2> target2(this->Row_, this->Col_, B);
		parallel_for_each(target1.extent, [=](index<2> idx) restrict(amp)
		{
			target1[idx] *= -1;
			target2[idx] *= -1;
		});
		target1.synchronize();
		target2.synchronize();
	}*/

	void Matrix::SaveMatrix(const string& _file) const
	{
		ofstream myfile;
		myfile.open(_file.c_str(), ios::app);//"MatrixData.txt" = _file
		time_t end_time = chrono::system_clock::to_time_t(chrono::system_clock::now());
		myfile << "Created date:" << time(&end_time) << endl;
		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_;c++)
					myfile << this->Array_[r * this->Col_ + c] << endl;

		myfile.close();
	}

	void Matrix::LoadMatrix(const string& _file)
	{
		ifstream myfile(_file);//"MatrixData.txt" = _file
		string Matrixdata;
		getline(myfile, Matrixdata);
		getline(myfile, Matrixdata);
		string().swap(Matrixdata);

		for (size_t r = 0; r < this->Row_; r++)
			for (size_t c = 0; c < this->Col_; c++)
			{
				getline(myfile, Matrixdata);
				this->Array_[r * this->Col_ + c] = (float)atof(Matrixdata.c_str());
				string().swap(Matrixdata);
			}
	}

	void Matrix::PushBack(float Val)
	{
		Checker(2);
		size_t size = this->Row_ * this->Col_ + 1;
		float* Tmp = new float[size];
		
		for (int i = 0; i < size; i++)
		{
			if (i < size - 1)
				Tmp[i] = this->Array_[i];
			else
				Tmp[i] = Val;
		}

		delete this->Array_;
		this->Array_ = new float[size];

		for (int i = 0; i < size; i++)
		{
			this->Array_[i] = Tmp[i];
		}

		delete Tmp;
	}

	void Matrix::PopBack()
	{
		Checker(2);

		size_t size = this->Row_ * this->Col_ - 1;
		float* Tmp = new float[size];

		for (int i = 0; i < size; i++)
		{
			Tmp[i] = this->Array_[i];
		}

		delete this->Array_;
		this->Array_ = new float[size];

		for (int i = 0; i < size; i++)
		{
			this->Array_[i] = Tmp[i];
		}

		delete Tmp;
	}

	void Matrix::Checker(int IndexToCheck) const
	{
		switch (IndexToCheck)
		{
		case 1:
		{
			if (this->Array_ == NULL)
				throw ExceptionHandler("Matrix is empty!");
			break;
		}

		case 2:
		{
			if (this->Col_ > 1)
				throw ExceptionHandler("That method can apply only on vectors.");
			break;
		}

		case 3:
		{
			throw ExceptionHandler("That method is not aviable yet.");
			break;
		}

		case 4:
		{
			if (this->Row_ == 0 || this->Col_ == 0 || this->Row_ != this->Col_)
				throw ExceptionHandler("The matrix isn't invertible.");
			break;
		}

		default:
		{
			throw ExceptionHandler("Index \"IndexToCheck\" is out of range!");
			break;
		}
		}
	}

	void Matrix::Checker(const Matrix& rhs, int IndexToCheck) const
	{
		switch (IndexToCheck)
		{
		case 1:
		{
			if (this->Array_ == NULL)
				throw ExceptionHandler("Left Matrix is empty!");
			else if (rhs.GetMatrix() == NULL)
				throw ExceptionHandler("rhs Matrix is empty!");
			else if (this->Row_ != rhs.GetRowSize() || this->Col_ != rhs.GetColSize())
				throw ExceptionHandler("Matrices dimensions most agree!!");
			break;
		}

		case 2:
		{
			if (this->Array_ == NULL)
				throw ExceptionHandler("Left Matrix is empty!");
			else if (rhs.GetMatrix() == NULL)
				throw ExceptionHandler("rhs Matrix is empty!");
			else if (this->Col_ != rhs.GetRowSize())
				throw ExceptionHandler("Matrices dimensions most agree!!");
			break;
		}

		case 3:
		{
			if (rhs.GetMatrix() == NULL)
				throw ExceptionHandler("rhs Matrix is empty!");
			break;
		}

		default:
		{
			throw ExceptionHandler("Index \"IndexToCheck\" is out of range!");
			break;
		}
		}
	}
}
