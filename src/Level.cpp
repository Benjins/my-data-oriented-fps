#include "..\header\int\Level.h"
#include "..\header\int\Component.h"
#include "..\header\int\Texture.h"



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


/*
	Model model;
	model.ImportFromOBJ(mesh);

	int size = model.faces.size() * 3;
	vertCount = size;
	vector<Vector3> Vertices;
	vector<Vector2> uvCoords;

	
    for(int i = 0; i < model.faces.size(); i++){
		Face face = model.faces[i];

		Vertices.push_back(model.vertices[face.v2].position);
		Vertices.push_back(model.vertices[face.v1].position);
		Vertices.push_back(model.vertices[face.v0].position);
		
		uvCoords.push_back(face.uv2);
		uvCoords.push_back(face.uv1);
		uvCoords.push_back(face.uv0);
	}

	
	glGenBuffers(1, &verticesVBO);
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3)*Vertices.size(), &(Vertices[0]), GL_STATIC_DRAW);
	
	glGenBuffers(1, &uvsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vector2)*uvCoords.size(), &(uvCoords[0]), GL_STATIC_DRAW);

	texObject = new Texture(GL_TEXTURE_2D, texture);
	texObject->Load();
	this->texture = texObject->textureObj;

	string vertShader;
	string fragShader;
	if(! (ReadFile(shader + ".vs", vertShader) && ReadFile(shader + ".fs", fragShader))){
		cout << "Error reading files: " << shader << ".vs and " << shader << ".fs" << endl;
		return;
	}

	shaderProgram = glCreateProgram();
	AddShader(shaderProgram, vertShader.c_str(), GL_VERTEX_SHADER);
	AddShader(shaderProgram, fragShader.c_str(), GL_FRAGMENT_SHADER);
		
	glLinkProgram(shaderProgram);

	GLint Success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &Success);
	if (Success == 0) {
		char ErrorLog[1024];
		glGetProgramInfoLog(shaderProgram, sizeof(ErrorLog), NULL, ErrorLog);
		fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
	}

	glValidateProgram(shaderProgram);
*/