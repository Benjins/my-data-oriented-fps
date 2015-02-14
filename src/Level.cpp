#include "..\header\int\Level.h"
#include "..\header\int\Component.h"
#include "..\header\int\Texture.h"

float Level::FindHeight(const Vector3& position){
	Vector3 localPos = transform.GlobalToLocal(position);

	Vector2 gridPos = Vector2(localPos.x, localPos.z);

	float currHeight = localPos.y;
	int currDepth = INT_MIN;
	for(auto iter = floors.begin(); iter != floors.end(); iter++){
		if(iter->depth > currDepth && BoundsContains(gridPos, iter->ulCorner, iter->brCorner)){
			currDepth = iter->depth;
			currHeight = iter->floorHeight;
		}
	}

	localPos.y = currHeight;
	return transform.LocalToGlobal(localPos).y;
}

//Is pt contained in the AABB formed by ul (upper left) and br (bottom right)
bool BoundsContains(const Vector2& pt, const Vector2& ul, const Vector2& br){
	return pt.x > ul.x && pt.y > ul.y
		&& pt.x < br.x && pt.y < br.y;
}

Vector3 Level::ResolveCollisions(const Vector3& from, const Vector3& to){
	Vector3 localFrom = transform.GlobalToLocal(from);
	Vector3 localTo   = transform.GlobalToLocal(to);

	for(auto iter = walls.begin(); iter != walls.end(); iter++){
		Vector3 edge1 = Vector3(iter->start.x, iter->height, iter->start.y) - Vector3(iter->start.x, 0, iter->start.y);
		Vector3 edge2 = Vector3(iter->end.x, 0, iter->end.y) - Vector3(iter->start.x, 0, iter->start.y);
		Vector3 normal = CrossProduct(edge1, edge2);

		//cout << "LocalFrom: " << localFrom.x << ", " << localFrom.y << ", " << localFrom.z << endl;
		//Vector3(iter->start.x, 0, iter->start.y).Print();

		float fromDist = DotProduct(localFrom - Vector3(iter->start.x, 0, iter->start.y), normal);
		float   toDist = DotProduct(localTo   - Vector3(iter->start.x, 0, iter->start.y), normal);
		//cout << "ToDist: " << toDist << "  FromDist: " << fromDist << endl;
		if(toDist * fromDist < 0){
			return from;
		}
	}

	return to;
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

		uvs.push_back(Vector2(0,1));
		uvs.push_back(Vector2(1,1));
		uvs.push_back(Vector2(1,0));
		uvs.push_back(Vector2(1,0));
		uvs.push_back(Vector2(0,1));
		uvs.push_back(Vector2(1,1));
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

