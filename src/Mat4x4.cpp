#include "../header/int/Mat4x4.h"

Mat4x4::Mat4x4(){
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			m[i][j] = (i == j ? 1 : 0);
		}
	}
}

Mat4x4::Mat4x4(const Mat4x4& orig){
	memcpy(m, orig.m, 16*sizeof(float));
}

inline Vector4 Mat4x4::operator*(const Vector4& param) const{
	return Vector4( DotProduct(param, GetRow(0)),
					DotProduct(param, GetRow(1)),
					DotProduct(param, GetRow(2)),
					DotProduct(param, GetRow(3)));
}

Vector3 Mat4x4::operator*(const Vector3& param) const{
	Vector4 result = *this * (Vector4(param, 1));
	return Vector3(result.x, result.y, result.z);
}

Mat4x4 Mat4x4::operator*(const Mat4x4& param) const{
	Mat4x4 result;
	for(int j = 0; j < 4; j++){
		for(int i = 0; i < 4; i++){
			result.m[i][j] = DotProduct(GetRow(i), param.GetColumn(j));
		}
	}

	return result;
}

Vector4 Mat4x4::GetColumn(int index) const{
	float col[4];
	
	for(int i = 0; i < 4; i++){
		col[i] = m[i][index];
	}
	
	return Vector4(col);
}

Vector4 Mat4x4::GetRow(int index) const{
	return Vector4( (float*)m[index]);
}

Mat4x4 Mat4x4::GetTranspose() const{
	Mat4x4 transpose;
	for(int i = 0; i < 4; i++){
		transpose.SetRow(i, GetColumn(i));
	}

	return transpose;
}

void Mat4x4::SetRow(int index, const Vector4& value){
	m[index][0] = value.w;
	m[index][1] = value.x;
	m[index][2] = value.y;
	m[index][3] = value.z;
}

void Mat4x4::SetColumn(int index, const Vector4& value){
	m[0][index] = value.w;
	m[1][index] = value.x;
	m[2][index] = value.y;
	m[3][index] = value.z;
}

float DotProduct(const float a[], const float b[], int length){
	float total = 0;
	for(int i = 0; i < length; i++){
		total += (a[i] * b[i]);
	}
	
	return total;
}