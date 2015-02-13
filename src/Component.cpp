#include "../header/int/Component.h"
#include "../header/int/Entity.h"
#include "../header/int/Model.h"
#include "../header/int/Texture.h"

#include <fstream>

using std::ifstream;

void RenderingComp::Render(const Entity* entities) const{
	glUseProgram(shaderProgram);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	Mat4x4 matrix = entities[entity].transform.LocalToGlobalMatrix();
	GLuint pos = glGetUniformLocation(shaderProgram, "_objectMatrix");
	glUniformMatrix4fv(pos, 1, GL_TRUE,  &matrix.m[0][0]);

	Mat4x4 perspMatrix = GetPerspectiveMatrix(8.0f/6,80,0.2f,200.0f);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "_perspMatrix"), 1, GL_TRUE,  &perspMatrix.m[0][0]); 

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

RenderingComp::~RenderingComp(){
	if(texObject != NULL){
		delete texObject;
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
		printf("Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
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