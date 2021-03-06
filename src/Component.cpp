#include "../header/int/Component.h"
#include "../header/int/Entity.h"
#include "../header/int/Model.h"
#include "../header/int/Texture.h"
#include "../header/int/Scene.h"
#include "../header/int/RaycastHit.h"

#include <fstream>
#include <cstring>

using std::ifstream;

void RenderingComp::Render(const Scene& mainScene) const{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Mat4x4 matrix;
	if(entity != -1){
		matrix = mainScene.entities[entity].transform.LocalToGlobalMatrix();
	}
	else{
		matrix = Mat4x4();// mainScene.level.transform.LocalToGlobalMatrix();
		matrix.m[0][0] = matrix.m[1][1] = matrix.m[2][2] = 1;
	}
	GLuint pos = glGetUniformLocation(shaderProgram, "_objectMatrix");
	glUniformMatrix4fv(pos, 1, GL_TRUE,  &matrix.m[0][0]);

	Mat4x4 perspMatrix = GetPerspectiveMatrix(8.0f/6,80,0.04f,200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "_perspMatrix"), 1, GL_TRUE,  &perspMatrix.m[0][0]); 

	Mat4x4 camMatrix = mainScene.player.camera.GetCameraMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "_cameraMatrix"), 1, GL_TRUE,  &camMatrix.m[0][0]);

	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, uvsVBO);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glDrawArrays(GL_TRIANGLES, 0, vertCount);
	
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
}

void RenderingComp::SetMeshMatTexture(const string& shader, const string& mesh, const string& texture){
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
}

void RenderingComp::Release(){
	if(texObject != NULL){
		delete texObject;
		texObject = NULL;
	}	
}

RenderingComp::~RenderingComp(){
	if(texObject != NULL){
		delete texObject;
	}	
}

void EnemyComp::Update(Scene& mainScene){
	Vector3 differenceVec = mainScene.player.camera.position - mainScene.entities[entity].transform.position;
	if(differenceVec.MagnitudeSquared() <= 9.0f && differenceVec.MagnitudeSquared() >= 0.1f){
		targetPos = mainScene.player.camera.position;
	}
	else{
		targetPos = mainScene.entities[entity].transform.position;
	}

	Vector3 goalVec = targetPos - mainScene.entities[entity].transform.position;
	Vector3 posChange = goalVec.Normalized() * mainScene.timer.deltaTime * speed;
	if(goalVec.MagnitudeSquared() <= 0.00001f){
		posChange = Vector3(0,0,0);
	}

	Vector3 currentPos = mainScene.entities[entity].transform.position;
	Vector3 targetPos = currentPos + posChange;
	if((targetPos - currentPos).MagnitudeSquared() >= 0.0000001f){
		mainScene.entities[entity].transform.position = mainScene.level.ResolveCollisions(currentPos, targetPos);

		mainScene.entities[entity].transform.position.y = mainScene.level.FindHeight(mainScene.entities[entity].transform.position)
														+ mainScene.entities[entity].transform.scale.y/2;
	}
}

void EnemyComp::OnRaycastHit(Scene& mainScene, RaycastHit hit){
	health--;
	mainScene.entities[entity].transform.scale.y = ((float)health)/10 + 0.3f;
	if(health <= 0){
		mainScene.RemoveEntity(&mainScene.entities[entity]);
	}
}

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType){
    GLuint ShaderObj = glCreateShader(ShaderType);

    const GLchar* p[1];
    p[0] = pShaderText;
    GLint Lengths[1];
    Lengths[0]= strlen(pShaderText);
    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);
	
    glAttachShader(ShaderProgram, ShaderObj);

	GLint success;
	glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		GLchar InfoLog[1024];
		glGetShaderInfoLog(ShaderObj, sizeof(InfoLog), NULL, InfoLog);
		printf("Error compiling shader type %f: '%s'\n", ShaderType, InfoLog);
	}

}

bool ReadFile(string fileName, string& readInto){
	ifstream fileIn(fileName.c_str());
	
	if (fileIn.is_open()) {
        string line;
        while(!fileIn.eof()) {
			getline(fileIn, line);
            readInto.append(line);
            readInto.append("\n");
        }
        
        fileIn.close();
        
        return true;
    }
    else {
        cerr << fileName << ": unable to open file.\n";
		return false;
    }
}

RaycastHit RaycastBox(Entity ent, PhysicsComp physBox, Vector3 origin, Vector3 direction){
	float intervalMin = -FLT_MAX;
	float intervalMax =  FLT_MAX;

	float xRangeCenter = (origin.x - ent.transform.position.x)/direction.x;
	float yRangeCenter = (origin.y - ent.transform.position.y)/direction.y;
	float zRangeCenter = (origin.z - ent.transform.position.z)/direction.z;

	float xRangeWidth = abs(physBox.size.x/direction.x);
	float yRangeWidth = abs(physBox.size.y/direction.y);
	float zRangeWidth = abs(physBox.size.z/direction.z);

	intervalMin = max(intervalMin, xRangeCenter - xRangeWidth);
	intervalMin = max(intervalMin, yRangeCenter - yRangeWidth);
	intervalMin = max(intervalMin, zRangeCenter - zRangeWidth);

	intervalMax = min(intervalMax, xRangeCenter + xRangeWidth);
	intervalMax = min(intervalMax, yRangeCenter + yRangeWidth);
	intervalMax = min(intervalMax, zRangeCenter + zRangeWidth);

	if(intervalMin < intervalMax && intervalMin <= -0.0001f){
		RaycastHit x;
		x.hit = true;
		x.depth = -intervalMin;
		x.worldPos = origin + direction * x.depth;

		return x;
	}

	RaycastHit x;
	x.hit = false;
	return x;
}

Mat4x4 GetPerspectiveMatrix(float aspectRatio, float fieldOfView, float nearZ, float farZ){
	Mat4x4 persp;

	float fieldOfView_Rad = fieldOfView/180*3.14159265f;
	float tanHalfFOV = tanf(fieldOfView_Rad/2);
	float zRange = nearZ - farZ;

	float x1 = 1/aspectRatio/tanHalfFOV;
	float y2 = 1/tanHalfFOV;
	float z3 = (-nearZ - farZ)/zRange;
	float z4 = 2*farZ*nearZ/zRange;
	
	persp.SetRow(0, Vector4(x1,0,0,0));
	persp.SetRow(1, Vector4(0,y2,0,0));
	persp.SetRow(2, Vector4(0,0,z3,z4));
	persp.SetRow(3, Vector4(0,0,1,0));
	
	return persp;
}
