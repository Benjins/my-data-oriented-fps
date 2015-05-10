#ifndef RAYCASTHIT_H
#define RAYCASTHIT_H

#include "../ext/Vector3.h"

struct RaycastHit{
	Vector3 worldPos;
	float depth;
	int entityId;
	bool hit;
};

#endif