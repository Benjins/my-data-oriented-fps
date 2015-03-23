#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

#include "../header/int/Scene.h"

#if defined(_WIN32) || defined(_WIN64)
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "freeglut.lib")
#endif

#define MEM_CHECK 1

#if defined(MEM_CHECK) || defined(TESTING)
#if defined(_WIN32) || defined(_WIN64)
#  define _CRTDBG_MAP_ALLOC
#  define _CRTDBG_MAP_ALLOC_NEW
#  include <crtdbg.h>
#endif
#endif
#if defined(MEM_CHECK) || defined(TESTING)
#  include <assert.h>
#endif

void Render();

Scene mainScene;

bool running = true;

void OnKey(unsigned char key, int x, int y);
void OnKeyUp(unsigned char key, int x, int y);

void OnMouse(int button, int state, int x, int y);
void OnPassiveMouse(int x, int y);

int main(int argc, char** argv){

#if (defined(_WIN32) || defined(_WIN64)) && (defined(TESTING) || defined(MEM_CHECK))
	//Windows memory leak checking
	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDOUT );
	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDOUT );
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	
	glutInit(&argc, argv);
	glutInitWindowPosition(0,0);
	glutInitWindowSize(1920, 1080);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutCreateWindow("my-fps");

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

	Floor floor = {Vector2(-6, -6), Vector2(6, 6), 0, 4.0f, 0};
	Floor floor2 = {Vector2(-2, 1), Vector2(-1, 2), 0.2f, 5.0f, 1};
	Floor floor3 = {Vector2(0, 2), Vector2(1, 3), 0.6f, 5.0f, 2};

	Wall wall = {Vector2(2, 3), Vector2(5, 1), 1.0f, 0.15f};
	Wall wall2 = {Vector2(4, -3), Vector2(5, 1), 1.0f, 0.15f};
	Wall wall3 = {Vector2(4, -3), Vector2(1, -1), 1.0f, 0.15f};

	Wall fWall1 = {Vector2(0, 2), Vector2(0, 3), 0.6f, 0.1f};
	Wall fWall2 = {Vector2(1, 2), Vector2(1, 3), 0.6f, 0.1f};
	Wall fWall3 = {Vector2(1, 2), Vector2(0, 2), 0.6f, 0.1f};
	Wall fWall4 = {Vector2(1, 3), Vector2(0, 3), 0.6f, 0.1f};

	mainScene.level.floors.push_back(floor);
	mainScene.level.floors.push_back(floor2);
	mainScene.level.floors.push_back(floor3);
	mainScene.level.walls.push_back(wall);
	mainScene.level.walls.push_back(wall2);
	mainScene.level.walls.push_back(wall3);

	mainScene.level.walls.push_back(fWall1);
	mainScene.level.walls.push_back(fWall2);
	mainScene.level.walls.push_back(fWall3);
	mainScene.level.walls.push_back(fWall4);

	mainScene.level.SetRenderingCompMesh(*mainScene.AddRenderer(NULL), "data/bad_wall_01D.bmp");

	Entity* cube = mainScene.AddEntity();
	cube->transform.position = Vector3(0,3,0);
	RenderingComp* comp = mainScene.AddRenderer(cube);
	comp->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture.bmp");

	Entity* reticle = mainScene.AddEntity();
	reticle->transform.parent = &mainScene.player.camera;
	reticle->transform.position = Vector3(0,0,0.1);
	reticle->transform.scale = Vector3(0.001f,0.001f,0.001f);
	RenderingComp* reticleRend = mainScene.AddRenderer(reticle);
	reticleRend->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture2.bmp");

	Entity* cube2 = mainScene.AddEntity();
	cube2->transform.parent = &cube->transform;
	cube2->transform.position = Vector3(0.4f,0,0);
	cube2->transform.scale = Vector3(0.3f, 0.3f, 0.5f);
	RenderingComp* cube2Rend = mainScene.AddRenderer(cube2);
	cube2Rend->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture2.bmp");

	Entity* enemy = mainScene.AddEntity();
	enemy->transform.scale = Vector3(0.1f, 0.1f, 0.1f);
	enemy->transform.position = Vector3(0, 0.3f, -5);
	RenderingComp* enemyRend = mainScene.AddRenderer(enemy);
	enemyRend->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture2.bmp");
	EnemyComp* enemyComp = mainScene.AddEnemy(enemy);
	enemyComp->targetPos = enemy->transform.position;
	enemyComp->speed = 0.5f;


	mainScene.player.camera.position = Vector3(0,0,0);
	mainScene.player.gravity = 2;
	mainScene.player.cameraHeight = 0.3f;

	while(running){
		mainScene.timer.NextFrame();
		glutMainLoopEvent();
		mainScene.Update();
		Render();
		//glutPostRedisplay();
		mainScene.input.EndFrame();

		//cout << "Frame took: " << mainScene.timer.deltaTime * 1000 << " ms.\n";
	}

#if (defined(_WIN32) || defined(_WIN64)) && (defined(MEM_CHECK) || defined(TESTING))
	assert(_CrtCheckMemory());
#endif

	return 0;
}

void Render(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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