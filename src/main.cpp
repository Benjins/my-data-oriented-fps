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
void OnKeyUp(unsigned char key, int x, int y);

void OnMouse(int button, int state, int x, int y);
void OnPassiveMouse(int x, int y);

int main(int argc, char** argv){
	
	glutInit(&argc, argv);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(1280, 720);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("my-engine");

	glutMouseFunc(OnMouse);
	glutPassiveMotionFunc(OnPassiveMouse);

	glutKeyboardFunc(OnKey);
	glutKeyboardUpFunc(OnKeyUp);

	glutSetKeyRepeat(GLUT_KEY_REPEAT_OFF);

#ifndef __APPLE__
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		printf("Error: '%s'\n", glewGetErrorString(res));
	}
#endif

	glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);

	Entity* cube = mainScene.AddEntity();
	cube->transform.position = Vector3(0,3,-5);
	RenderingComp* comp = mainScene.AddRenderer(*cube);

	comp->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture.bmp");

	Entity* floor = mainScene.AddEntity();
	floor->transform.position = Vector3(0,0,0);
	floor->transform.scale = Vector3(3,3,3);
	RenderingComp* floorRend = mainScene.AddRenderer(*floor);

	floorRend->SetMeshMatTexture("data/shader", "data/floor.obj", "data/Texture2.bmp");

	mainScene.camera.position = Vector3(0,2,0);

	while(running){
		glutMainLoopEvent();
		mainScene.Update();
		Render();
		glutPostRedisplay();

		mainScene.input.EndFrame();
	}


	return 0;
}

void Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float time = 0;
	time += 0.001f;

	mainScene.entities[0].transform.rotation = Quaternion(X_AXIS, time);
	mainScene.entities[0].transform.position.z = sinf(time/25) + 0.2f;

	mainScene.Render();

	glutSwapBuffers();
}

void OnKey(unsigned char key, int x, int y){
	if(key == 'x' || key == 'X'){
		running = false;
	}
	mainScene.input.PushKey(key);
}

void OnKeyUp(unsigned char key, int x, int y){
	mainScene.input.ReleaseKey(key);
}

void OnMouse(int button, int state, int x, int y){
	if(state == GLUT_DOWN){
		mainScene.input.PushMouse(button);
	}
	else if(state == GLUT_UP){
		mainScene.input.ReleaseMouse(button);
	}
}

void OnPassiveMouse(int x, int y){
	mainScene.input.mouseX = x;
	mainScene.input.mouseY = y;
}