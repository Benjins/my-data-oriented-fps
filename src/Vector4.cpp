#include "../header/int/Vector4.h"
#include "../header/ext/Vector3.h"



Vector4::Vector4(){
	w = x = y = z = 0;
}

Vector4::Vector4(float _w, float _x, float _y, float _z){
	w = _w;
	x = _x;
	y = _y; 
	z = _z;
}

Vector4::Vector4(const Vector3& vec, float _w){
	w = vec.x;
	x = vec.y;
	y = vec.z;
	z = _w;
}

Vector4::Vector4(float* arrayInit){
	w = arrayInit[0];
	x = arrayInit[1];
	y = arrayInit[2];
	z = arrayInit[3];
}

float DotProduct(const Vector4& param1, const Vector4& param2){
	return param1.w * param2.w
		 + param1.x * param2.x
		 + param1.y * param2.y
		 + param1.z * param2.z;
}

