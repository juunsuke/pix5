#pragma once


class Camera
{
public:

	Matrix mat;
	// Camera's projection matrix


	Camera();
	// Identity camera

	Camera(const Matrix& mat);
	// Copy the given matrix

	~Camera();


	void set() const;
	// Set the camera's matrix as the current OpenGL projection matrix
};


