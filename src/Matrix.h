#pragma once


class Matrix
{
public:
	
	union
	{
		float val[16];
		float arr[4][4];
		struct
		{
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
	};
	// Raw 16 values of the 4x4 matrix
	// The memory layout is as follow (number represents indices):
	// 0   1   2   3
	// 4   5   6   7
	// 8   9   10  11
	// 12  13  14  15

	inline Matrix()
	{
		// Undefined matrix by default
	}

	inline Matrix(const Matrix& o)
	{
		// Copy a matrix
		memcpy(val, o.val, sizeof(val));
	}

	inline Matrix(float *v)
	{
		// Build a matrix from raw data, should contain 16 float values
		memcpy(val, v, sizeof(val));
	}

	~Matrix() { }


	inline Matrix& operator=(const Matrix& o)
	{
		// Copy a matrix
		memcpy(val, o.val, sizeof(val));
		return *this;
	}

	Matrix operator*(const Matrix& o) const
	{
		// Multiply this matrix(left) by another one(right)
		Matrix res;

		res._11 = _11*o._11 + _12*o._21 + _13*o._31 + _14*o._41;
		res._12 = _11*o._12 + _12*o._22 + _13*o._32 + _14*o._42;
		res._13 = _11*o._13 + _12*o._23 + _13*o._33 + _14*o._43;
		res._14 = _11*o._14 + _12*o._24 + _13*o._34 + _14*o._44;

		res._21 = _21*o._11 + _22*o._21 + _23*o._31 + _24*o._41;
		res._22 = _21*o._12 + _22*o._22 + _23*o._32 + _24*o._42;
		res._23 = _21*o._13 + _22*o._23 + _23*o._33 + _24*o._43;
		res._24 = _21*o._14 + _22*o._24 + _23*o._34 + _24*o._44;

		res._31 = _31*o._11 + _32*o._21 + _33*o._31 + _34*o._41;
		res._32 = _31*o._12 + _32*o._22 + _33*o._32 + _34*o._42;
		res._33 = _31*o._13 + _32*o._23 + _33*o._33 + _34*o._43;
		res._34 = _31*o._14 + _32*o._24 + _33*o._34 + _34*o._44;

		res._41 = _41*o._11 + _42*o._21 + _43*o._31 + _44*o._41;
		res._42 = _41*o._12 + _42*o._22 + _43*o._32 + _44*o._42;
		res._43 = _41*o._13 + _42*o._23 + _43*o._33 + _44*o._43;
		res._44 = _41*o._14 + _42*o._24 + _43*o._34 + _44*o._44;

		return res;
	}

	Matrix transpose() const
	{
		// Return a transposed version of the matrix
		Matrix res;

		for(int y = 0; y<4; y++)
			for(int x = 0; x<4; x++)
				res.arr[x][y] = arr[y][x];

		return res;
	}


	inline static Matrix identity()
	{
		// Create an identity matrix
		Matrix res;

		memset(res.val, 0, sizeof(val));
		res._11 = 1.0f;
		res._22 = 1.0f;
		res._33 = 1.0f;
		res._44 = 1.0f;

		return res;
	}

	inline static Matrix ortho(float left, float right, float top, float bottom, float near, float far)
	{
		// Create an orthographic matrix
		ASSERT(left!=right && top!=bottom && near!=far, "Matrix::ortho(): Given values would cause a division by zero")

		Matrix res;
		memset(res.val, 0, sizeof(val));

		res._11 = 2.0f/(right-left);
		res._22 = 2.0f/(top-bottom);
		res._33 = -2.0f/(far-near);
		res._44 = 1.0f;

		res._14 = -(right+left)/(right-left);
		res._24 = -(top+bottom)/(top-bottom);
		res._34 = -(far+near)/(far-near);

		return res;
	}

	inline static Matrix ortho(int width, int height, float near, float far)
	{
		return ortho(0, (float)width, 0, (float)height, near, far);
	}

	inline static Matrix ortho2d(float left, float right, float top, float bottom)
	{
		return ortho(left, right, top, bottom, -1.0f, 1.0f);
	}

	inline static Matrix ortho2d(int width, int height)
	{
		return ortho2d(0, (float)width, 0, (float)height);
	}

	static Matrix get_projection();
	static Matrix get_modelview();
	// Get the current OpenGL projection/modelview matrix as a PIX matrix
	// These functions do not change the current matrix mode

	void set_projection() const;
	void set_modelview() const;
	// Set this matrix as the OpenGL projection/modelview matrix
	// These functions will change the current matrix mode accordingly,
	// and won't restore the previous setting

	void print() const;
	// Output the matrix to standard output
	// Mostly used for debugging
};


