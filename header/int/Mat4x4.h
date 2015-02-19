#ifndef MAT4X4_H
#define MAT4X4_H

#include "Vector4.h"
#include "../ext/Vector3.h"

struct Mat4x4{
	float m[4][4];

	Mat4x4();
	Mat4x4(const Mat4x4& orig);

	inline Vector4 operator*(const Vector4& param) const;
	Vector3 operator*(const Vector3& param) const;
	Mat4x4 operator*(const Mat4x4& param) const;

	Vector4 GetColumn(int index) const;
	Vector4 GetRow(int index) const;

	Mat4x4 GetTranspose() const;

	void SetRow(int index, const Vector4& value);
	void SetColumn(int index, const Vector4& value);

};


float DotProduct(const float a[], const float b[], int length);

#endif
