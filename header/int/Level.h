#ifndef LEVEL_H
#define LEVEL_H

#include "SCTransform.h"
#include "../ext/Vector3.h"
#include "../ext/Vector2.h"
#include <vector>

using std::vector;

struct RenderingComp; struct RaycastHit;

struct Wall{
	Vector2 start;
	Vector2 end;
	float height;
	float width;
};

struct Floor{
	Vector2 ulCorner;
	Vector2 brCorner;
	float floorHeight;
	float ceilingHeight;
	int depth;
};

struct Level{
	SCTransform transform;

	vector<Floor> floors;
	vector<Wall> walls;

	void SetRenderingCompMesh(RenderingComp& rend, const string& texture);

	//Takes position in global coords
	float FindHeight(const Vector3& position) const;
	Vector3 ResolveCollisions(const Vector3& from, const Vector3& to) const;

};

bool BoundsContains(const Vector2& pt, const Vector2& ul, const Vector2& br);

RaycastHit RaycastWall(Vector3 origin, Vector3 direction, Wall wall);

RaycastHit RaycastFloor(Vector3 origin, Vector3 direction, Floor floor);

/*-----Planning-----
-Different height floors, ramps/stairs, walls, 
*/

#endif

