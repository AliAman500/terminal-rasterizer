#pragma once

#include <vector>

const char COLORS[] = {' ', '.', ',', '-', ':', ';', '!', '#', '$', '@' };

struct Vector4f {
	float x, y, z, w;

	Vector4f(float x, float y, float z, float w);
	Vector4f();
	
	Vector4f operator-() const;
	void print() const;
};

struct Vector3f {
	float x, y, z;

	Vector3f(float x, float y, float z);
	Vector3f();
	
	Vector3f operator-() const;
	void print() const;
};

struct Vector2f {
	float x, y;

	Vector2f(float x, float y);
	Vector2f();
	
	void print() const;
};

struct Matrix4f {
	float m[16];

	Matrix4f();

	Matrix4f operator*(const Matrix4f& other) const;
	Vector4f operator*(const Vector4f& vec) const;

	void put(int row, int column, float v);
	void print() const;
};

struct Camera {
	Vector3f position;
	Vector3f rotation;
	Vector3f scale;

	float fov = 100.0f;
	float zfar = 1000.0f;
	float znear = 0.1f;
};

struct Rect {
	int x, y, w, h;
};

Matrix4f createTransformationMatrix(const Vector3f& translate, const Vector3f& rotation, const Vector3f& scale);
Matrix4f createProjectionMatrix(float aspect, float fov, float znear, float zfar);
bool isInsideTriangle(const Vector2f& p, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3);
float calcDepth(int px, int py, const Vector2f& v1, const Vector2f& v2, const Vector2f& v3, float z1, float z2, float z3);