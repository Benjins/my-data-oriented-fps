#ifndef COMPONENT_H
#define COMPONENT_H

#define GLEW_STATIC
#include <GL/glew.h>
#include "../ext/Vector3.h"
#include <string>

using std::string;

struct Entity; struct Texture;

struct Component{
	int id;
	int entity;
	int flags;
};

struct RenderingComp : public Component{
	GLuint shaderProgram;
	GLuint texture;
	Texture* texObject;
	GLuint verticesVBO;
	GLuint uvsVBO;
	int vertCount;

	void Render(const Entity* entities) const;

	void SetMeshMatTexture(const string& shader, const string& mesh, const string& texture);

	~RenderingComp();
};

void AddShader(GLuint ShaderProgram, const char* pShaderText, GLenum ShaderType);

bool ReadFile(string fileName, string& readInto);

struct PhysicsComp : public Component{
	Vector3 position;
	Vector3 size;

	//Collision stuff
};


#endif