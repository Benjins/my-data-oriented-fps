#include "../header/int/Level.h"
#include "../header/int/Component.h"
#include "../header/int/Texture.h"
#include "../header/int/RaycastHit.h"

#ifndef INT_MIN
#define INT_MIN (1<<31)
#endif

float Level::FindHeight(const Vector3& position) const{
	Vector3 localPos = transform.GlobalToLocal(position);

	Vector2 gridPos = Vector2(localPos.x, localPos.z);

	float currHeight = -100;
	int currDepth = INT_MIN;
	for(auto iter = floors.begin(); iter != floors.end(); iter++){
		if(iter->depth > currDepth && BoundsContains(gridPos, iter->ulCorner, iter->brCorner)){
			currDepth = iter->depth;
			currHeight = iter->floorHeight;
		}
	}

	localPos.y = currHeight;
	Mat4x4 mat = transform.LocalToGlobalMatrix();
	Vector3 globalPos = mat * localPos;
	return globalPos.y;
}

//Is pt contained in the AABB formed by ul (upper left) and br (bottom right)
bool BoundsContains(const Vector2& pt, const Vector2& ul, const Vector2& br){
	return pt.x > ul.x && pt.y > ul.y
		&& pt.x < br.x && pt.y < br.y;
}

Vector3 Level::ResolveCollisions(const Vector3& from, const Vector3& to) const{
	Vector3 localFrom = from;// transform.GlobalToLocal(from);
	Vector3 localTo   = to;// transform.GlobalToLocal(to);

	for(auto iter = walls.begin(); iter != walls.end(); iter++){
		float wallWidth = iter->width;

		Vector3 edge1 = Vector3(iter->start.x, iter->height, iter->start.y) - Vector3(iter->start.x, 0, iter->start.y);
		Vector3 edge2 = Vector3(iter->end.x, 0, iter->end.y) - Vector3(iter->start.x, 0, iter->start.y);
		Vector3 normal = CrossProduct(edge1, edge2).Normalized();

		float toDist = DotProduct(localTo - Vector3(iter->start.x, 0, iter->start.y), normal);
		float fromDist = DotProduct(localFrom - Vector3(iter->start.x, 0, iter->start.y), normal);
		if((fromDist > 0) ? (toDist < wallWidth) : (toDist > -wallWidth)){
			Vector2 floorProjectionFrom = Vector2(localFrom.x, localFrom.z);
			Vector2 wallDiffFrom = floorProjectionFrom - iter->start;
			Vector2 wallVecFrom = (iter->end - iter->start);
			float overlapFrom = DotProduct(wallDiffFrom, wallVecFrom);

			Vector2 floorProjectionTo = Vector2(localTo.x, localTo.z);
			Vector2 wallDiffTo = floorProjectionTo - iter->start;
			Vector2 wallVecTo = (iter->end - iter->start);
			float overlapTo = DotProduct(wallDiffTo, wallVecTo);

			bool isOverlapFrom = overlapFrom < (iter->end - iter->start).MagnitudeSquared() && overlapFrom > 0;
			bool isOverlapTo = overlapTo < (iter->end - iter->start).MagnitudeSquared() && overlapTo > 0;

			if((isOverlapFrom || isOverlapTo) && localFrom.y <= iter->height + /*player height*/0.1f){
				//cout << "Recalc pos.\n";
				Vector3 collisionPlane = Vector3(iter->start.x, 0, iter->start.y) + normal * (fromDist > 0 ? wallWidth : -wallWidth) * 1.001f;
				Vector3 toProject = localTo - collisionPlane;
				Vector3 projection = VectorProject(toProject, normal);
				Vector3 result = (toProject - projection) + collisionPlane;

				localTo = result;
			}
		}
	}

	return localTo;
}

void Level::SetRenderingCompMesh(RenderingComp& rend, const string& texture){
	rend.entity = -1;
	
	vector<Vector3> verts;
	vector<Vector2> uvs;

	for(auto iter = floors.begin(); iter != floors.end(); iter++){
		Floor floor = *iter;
		verts.push_back(Vector3(floor.ulCorner.x, floor.floorHeight, floor.ulCorner.y));
		verts.push_back(Vector3(floor.brCorner.x, floor.floorHeight, floor.ulCorner.y));
		verts.push_back(Vector3(floor.ulCorner.x, floor.floorHeight, floor.brCorner.y));
		verts.push_back(Vector3(floor.ulCorner.x, floor.floorHeight, floor.brCorner.y));
		verts.push_back(Vector3(floor.brCorner.x, floor.floorHeight, floor.ulCorner.y));
		verts.push_back(Vector3(floor.brCorner.x, floor.floorHeight, floor.brCorner.y));

		uvs.push_back(Vector2(0,0));
		uvs.push_back(Vector2(0,1));
		uvs.push_back(Vector2(1,0));
		uvs.push_back(Vector2(1,0));
		uvs.push_back(Vector2(0,1));
		uvs.push_back(Vector2(1,1));
	}

	for(auto iter = walls.begin(); iter != walls.end(); iter++){
		Wall wall = *iter;

		verts.push_back(Vector3(wall.start.x, 0,           wall.start.y));
		verts.push_back(Vector3(wall.start.x, wall.height, wall.start.y));
		verts.push_back(Vector3(wall.end.x,   wall.height, wall.end.y));

		verts.push_back(Vector3(wall.end.x,   wall.height, wall.end.y));
		verts.push_back(Vector3(wall.start.x, 0,           wall.start.y));
		verts.push_back(Vector3(wall.end.x,   0,           wall.end.y));

		uvs.push_back(Vector2(0,0));
		uvs.push_back(Vector2(0,1));
		uvs.push_back(Vector2(1,1));

		uvs.push_back(Vector2(1,1));
		uvs.push_back(Vector2(0,0));
		uvs.push_back(Vector2(1,0));
	}

	rend.vertCount = verts.size();

	glGenBuffers(1, &rend.verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rend.verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*verts.size(), &(verts[0]), GL_STATIC_DRAW);
	
	glGenBuffers(1, &rend.uvsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, rend.uvsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2)*uvs.size(), &(uvs[0]), GL_STATIC_DRAW);

	rend.texObject = new Texture(GL_TEXTURE_2D, texture);
	rend.texObject->Load();
	rend.texture = rend.texObject->textureObj;

	string shader = "data/shader";
	string vertShader;
	string fragShader;
	if(! (ReadFile(shader + ".vs", vertShader) && ReadFile(shader + ".fs", fragShader))){
		cout << "Error reading files: " << shader << ".vs and " << shader << ".fs" << endl;
		return;
	}

	rend.shaderProgram = glCreateProgram();
	AddShader(rend.shaderProgram, vertShader.c_str(), GL_VERTEX_SHADER);
	AddShader(rend.shaderProgram, fragShader.c_str(), GL_FRAGMENT_SHADER);
	
	glLinkProgram(rend.shaderProgram);

	GLint Success;
	glGetProgramiv(rend.shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		char ErrorLog[1024];
		glGetProgramInfoLog(rend.shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(rend.shaderProgram);
}

RaycastHit RaycastFloor(Vector3 origin, Vector3 direction, Floor floor){
	float yDiff = origin.y - floor.floorHeight;
	float hitDepth = yDiff/direction.y;

	if(hitDepth >= -0.0001f){
		RaycastHit x;
		x.hit = false;
		return x;
	}

	Vector3 projectedPositon = origin - (direction.Normalized() * hitDepth);
	if(	   projectedPositon.x >= floor.ulCorner.x && projectedPositon.x <= floor.brCorner.x 
		&& projectedPositon.z >= floor.ulCorner.y && projectedPositon.z <= floor.brCorner.y)
	{
		RaycastHit x;
		x.hit = true;
		x.depth = -hitDepth;
		x.worldPos = projectedPositon;
		return x;
	}

	RaycastHit x;
	x.hit = false;
	return x;
}

RaycastHit RaycastWall(Vector3 origin, Vector3 direction, Wall wall){
	float yMin=0, yMax=wall.height;
	Vector3 directionNormal = direction.Normalized();

	Vector3 wallToOrigin = origin - Vector3(wall.start.x, 0, wall.start.y);

	Vector2 wallTangent = wall.end - wall.start;
	float wallLength = wallTangent.Magnitude();
	wallTangent = wallTangent / wallLength;

	Vector3 wallTangent3d = Vector3(wallTangent.x, 0, wallTangent.y);
	Vector3 wallNormal3d  = CrossProduct(wallTangent3d, Y_AXIS);
	
	Vector3 transformedOrigin = Vector3(DotProduct(wallToOrigin, wallTangent3d),
										wallToOrigin.y,
										DotProduct(wallToOrigin, wallNormal3d));

	Vector3 transformedDirection = Vector3( DotProduct(directionNormal, wallTangent3d),
											directionNormal.y,
											DotProduct(directionNormal, wallNormal3d));

	float planeCollisionDepth = transformedOrigin.z/transformedDirection.z;

	if(planeCollisionDepth >= -0.0001f){
		RaycastHit x;
		x.hit = false;
		return x;
	}

	Vector3 potentialCollision = transformedOrigin - (transformedDirection * planeCollisionDepth);
	
	if(	 potentialCollision.x <= wallLength    && potentialCollision.x >= 0
	  && potentialCollision.y <= wall.height && potentialCollision.y >= 0)
	{
		RaycastHit x;
		x.hit = true;
		x.depth = -planeCollisionDepth;
		x.worldPos = origin - (direction * planeCollisionDepth);
		return x;
	}

	RaycastHit x;
	x.hit = false;
	return x;
}
