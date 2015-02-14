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

Vector3 SCTransform::Forward() const{
	if(parent != NULL){
		return Rotate(parent->Forward(), rotation);
	}
	return Rotate(Z_AXIS, rotation);
}

Vector3 SCTransform::Up() const{
	if(parent != NULL){
		return Rotate(parent->Up(), rotation);
	}
	return Rotate(Y_AXIS, rotation);
}

Vector3 SCTransform::Right() const{
	if(parent != NULL){
		return Rotate(parent->Right(), rotation);
	}
	return Rotate(X_AXIS, rotation);
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

Mat4x4 SCTransform::GetCameraMatrix() const{
	Mat4x4 transMat;

	Mat4x4 linMat;
	Mat4x4 affMat;

	linMat.SetColumn(0, Vector4(Rotate(X_AXIS, rotation.Conjugate()), 0));
	linMat.SetColumn(1, Vector4(Rotate(Y_AXIS, rotation.Conjugate()), 0));
	linMat.SetColumn(2, Vector4(Rotate(Z_AXIS, rotation.Conjugate()), 0));
	linMat.SetColumn(3, Vector4(0,0,0,1));

	affMat.SetRow(0, Vector4(X_AXIS, -position.x));
	affMat.SetRow(1, Vector4(Y_AXIS, -position.y));
	affMat.SetRow(2, Vector4(Z_AXIS, -position.z));
	affMat.SetRow(3, Vector4(0,0,0,1));

	transMat = linMat * affMat;

	if(parent != NULL){
		return transMat * parent->GetCameraMatrix();
	}

	return transMat;
}

Vector3 SCTransform::GlobalToLocal(const Vector3& global) const{
	Vector3 localVec = global;
	if(parent != NULL){
		localVec = parent->GlobalToLocal(global);
	}
    localVec = localVec - position;
    localVec = Rotate(localVec, rotation.Conjugate());
    localVec = localVec.Scaled(Vector3( 1 / scale.x,
                                        1 / scale.y,
                                        1 / scale.z));

    return localVec;
}
