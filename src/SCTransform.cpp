#include "../header/int/SCTransform.h"


SCTransform::SCTransform(){
	position = Vector3(0,0,0);
	rotation = Quaternion(1,0,0,0);
	scale = Vector3(1,1,1);
	parent = NULL;
}

SCTransform::SCTransform(const SCTransform& orig){
	position = orig.position;
	rotation = orig.rotation;
	scale = orig.scale;
	parent = orig.parent;
}

Mat4x4 SCTransform::LocalToGlobalMatrix() const{
	Mat4x4 matrix;
	matrix.SetColumn(0, Vector4(Rotate(X_AXIS * scale.x, rotation), 0));
	matrix.SetColumn(1, Vector4(Rotate(Y_AXIS * scale.y, rotation), 0));
	matrix.SetColumn(2, Vector4(Rotate(Z_AXIS * scale.z, rotation), 0));
	matrix.SetColumn(3, Vector4(position));

	if(parent != NULL){
		return parent->LocalToGlobalMatrix() * matrix;
	}

	return matrix;
}

Vector3 SCTransform::LocalToGlobal(const Vector3& global) const{
	return LocalToGlobalMatrix() * global;
}