#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "../header/int/Scene.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "freeglut.lib")
#endif

void Render();

Scene mainScene;

bool running = true;

void OnKey(unsigned char key, int x, int y);

int main(int argc, char** argv){
	
	glutInit(&argc, argv);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(800, 600);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("my-engine");

	glutKeyboardFunc(OnKey);

#ifndef __APPLE__
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error: '%s'\n", glewGetErrorString(res));
	}
#endif

	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	Entity* player = mainScene.AddEntity();
	player->transform.position = Vector3(0,0,-5);
	RenderingComp* comp = mainScene.AddRenderer(*player);

	comp->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture.bmp");

	while(running){
		glutMainLoopEvent();
		Render();
		glutPostRedisplay();
	}


	return 0;
}

void Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float time = 0;
	time += 0.001f;

	mainScene.entities[0].transform.rotation = Quaternion(X_AXIS, time);
	mainScene.entities[0].transform.position = Vector3(0, 0, sinf(time/25) + 0.2f);

	mainScene.Render();

	glutSwapBuffers();
}


void OnKey(unsigned char key, int x, int y){
	if(key == 'x' || key == 'X'){
		running = false;
	}
	else if(key == 'w'){
		mainScene.camera.position.z += 0.01f;
	}
	else if(key == 's'){
		mainScene.camera.position.z -= 0.01f;
	}
	else if(key == 'a'){
		mainScene.camera.position.x -= 0.01f;
	}
	else if(key == 'd'){
		mainScene.camera.position.x += 0.01f;
	}
	else if(key == 'q'){
		mainScene.camera.position.y += 0.01f;
	}
	else if(key == 'z'){
		mainScene.camera.position.y -= 0.01f;
	}
}