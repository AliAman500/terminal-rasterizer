#include "math.h"
#include <cmath>
#include <iostream>

Vector4f::Vector4f(float x, float y, float z, float w): x(x), y(y), z(z), w(w) {}
Vector4f::Vector4f(): x(0), y(0), z(0), w(0) {}
Vector4f Vector4f::operator-() const { return Vector4f(-x, -y, -z, -w); }
void Vector4f::print() const { std::cout << "[" << x << ", " << y << ", " << z << ", " << z << "]" << std::endl; }

Vector3f::Vector3f(float x, float y, float z): x(x), y(y), z(z) {}
Vector3f::Vector3f(): x(0), y(0), z(0) {}
Vector3f Vector3f::operator-() const { return Vector3f(-x, -y, -z); }
void Vector3f::print() const {
		std::cout << "[" << x << ", " << y << ", " << z << "]" << std::endl;
}

Vector2f::Vector2f(float x, float y): x(x), y(y) {}
Vector2f::Vector2f(): x(0), y(0) {}
void Vector2f::print() const {
		std::cout << "[" << x << ", " << y << "]" << std::endl;
}

Matrix4f::Matrix4f() {
	m[0] = 1; m[1] = 0; m[2] = 0; m[3] = 0;
	m[4] = 0; m[5] = 1; m[6] = 0; m[7] = 0;
	m[8] = 0; m[9] = 0; m[10] = 1; m[11] = 0;
	m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
}

Matrix4f Matrix4f::operator*(const Matrix4f& other) const {
	Matrix4f result;

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			result.put(i, j, 0);
			for (int k = 0; k < 4; ++k) {
				result.put(i, j, result.m[i * 4 + j] + m[i * 4 + k] * other.m[k * 4 + j]);
			}
	}
}

	return result;
}

Vector4f Matrix4f::operator*(const Vector4f& vec) const {
   Vector4f result(0, 0, 0, 0);

	result.x = m[0] * vec.x + m[1] * vec.y + m[2] * vec.z + m[3] * vec.w;
	result.y = m[4] * vec.x + m[5] * vec.y + m[6] * vec.z + m[7] * vec.w;
	result.z = m[8] * vec.x + m[9] * vec.y + m[10] * vec.z + m[11] * vec.w;
	result.w = m[12] * vec.x + m[13] * vec.y + m[14] * vec.z + m[15] * vec.w;

	return result;
}

void Matrix4f::put(int row, int column, float v) {
	m[row * 4 + column] = v;
}

void Matrix4f::print() const {
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			std::cout << m[i * 4 + j] << ' ';
		}
		std::cout << std::endl;
	}
}

Matrix4f createProjectionMatrix(float aspect, float fov, float znear, float zfar) {
	float yscale = (1.0f / tanf(fov / 2.0f)) * aspect;
	float xscale = yscale / aspect;
	float frustumLength = zfar - znear;
	
	Matrix4f projection;
	
	projection.put(0, 0, xscale);
	projection.put(1, 1, yscale);
	projection.put(2, 2, -((zfar + znear) / frustumLength));
	projection.put(2, 3, -1);
	projection.put(3, 2, -((2.0f * znear * zfar) / frustumLength));
	projection.put(3, 3, 0);

	return projection;
}

Matrix4f createTransformationMatrix(const Vector3f& translate, const Vector3f& rotation, const Vector3f& scale) {
	Matrix4f transformation;

	float cosX = cosf(rotation.x);
	float sinX = sinf(rotation.x);
	float cosY = cosf(rotation.y);
	float sinY = sinf(rotation.y);
	float cosZ = cosf(rotation.z);
	float sinZ = sinf(rotation.z);

	transformation.m[0] = scale.x * (cosZ * cosY);
	transformation.m[1] = scale.y * (cosZ * sinY * sinX - sinZ * cosX);
	transformation.m[2] = scale.z * (sinZ * sinX + cosZ * sinY * cosX);
	transformation.m[3] = translate.x;

	transformation.m[4] = scale.x * (sinZ * cosY);
	transformation.m[5] = scale.y * (cosZ * cosX + sinZ * sinY * sinX);
	transformation.m[6] = scale.z * (sinZ * sinY * cosX - cosZ * sinX);
	transformation.m[7] = translate.y;

	transformation.m[8] = -scale.x * sinY;
	transformation.m[9] = scale.y * cosY * sinX;
	transformation.m[10] = scale.z * cosY * cosX;
	transformation.m[11] = translate.z;

	transformation.m[12] = 0;
	transformation.m[13] = 0;
	transformation.m[14] = 0;
	transformation.m[15] = 1;

	return transformation;
}

bool isInsideTriangle(const Vector2f& p, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3) {
	float denominator = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);
	float alpha = ((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) / denominator;
	float beta = ((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) / denominator;
	float gamma = 1.0f - alpha - beta;

	return alpha > 0 && beta > 0 && gamma > 0;
}

float calcDepth(int px, int py, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, float z1, float z2, float z3) {
	float area = 0.5f * (-v2.y*v3.x + v1.y*(-v2.x + v3.x) + v1.x*(v2.y - v3.y) + v2.x*v3.y);

	float w1 = (0.5f * (-v2.y*px + v3.y*px + v1.x*v2.y - v3.x*v2.y - v1.x*py + v3.x*py)) / area;
	float w2 = (0.5f * (v1.y*px - v3.y*px - v1.x*py + v3.x*py + v1.y*v3.x - v3.y*v1.x)) / area;
	float w3 = 1 - w1 - w2;

	float z = w1 * z1 + w2 * z2 + w3 * z3;

	return z;
}