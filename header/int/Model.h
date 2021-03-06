#ifndef MODEL_H
#define MODEL_H

#include <GL/glew.h>
#include <vector>
#include <string>
#include "../ext/Vector2.h"
#include "../ext/Vector3.h"

using std::vector; using std::string;

struct Face{
	int v0;
	int v1;
	int v2;
	
	Vector2 uv0;
	Vector2 uv1;
	Vector2 uv2;
	
	Face();
	Face(int _v0, int _v1, int _v2);
};

struct Vertex{
	Vector3 position;
	Vector3 color;
	//Vector3 uv;
	
	Vertex(void);
	Vertex(Vector3 _position);
};

struct Model{
	vector<Vertex> vertices;
	vector<Face> faces;

	//string name;
	string fileName;
	
	Model(void);
	Model(const Model& model);
	Model(string fileName);
	
	int GLVertexCount() const;
	
	void ImportFromOBJ(string fileName);
};

Vertex ParseVertexLine(string line);

Vector2 ParseUVLine(string line);

Face ParseFaceLine(string line, const vector<Vector2>& uvs);

vector<string> SplitStringByDelimiter(string searchIn, string delimiter);

#endif