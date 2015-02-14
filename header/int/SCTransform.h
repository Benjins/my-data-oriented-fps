#ifndef SCTRANSFORM_H
#define SCTRANSFORM_H

#include "../ext/Vector3.h"
#include "../ext/Quaternion.h"
#include "Mat4x4.h"

struct SCTransform{
	SCTransform* parent;

	Vector3 position;
	Quaternion rotation;
	Vector3 scale;

	SCTransform();

	SCTransform(const SCTransform& orig);

	Vector3 Forward() const;
	Vector3 Up() const;
	Vector3 Right() const;

	Vector3 GlobalToLocal(const Vector3& original) const;

	Mat4x4 LocalToGlobalMatrix() const;
	Vector3 LocalToGlobal(const Vector3& global) const;
	Mat4x4 GetCameraMatrix() const;
};

#endif