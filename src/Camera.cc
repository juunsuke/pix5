#include "pix5.h"

namespace PIX {


Camera::Camera()
{
	mat = Matrix::identity();
}

Camera::~Camera()
{
}
	
Camera::Camera(const Matrix& mat)
{
	this->mat = mat;
}

void Camera::set() const
{
	mat.set_projection();
}








}
