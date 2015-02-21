#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "Component.h"
#include "Entity.h"
#include "Input.h"
#include "Timer.h"
#include "Level.h"
#include "Player.h"

#define MODEL_COUNT 12
#define PHYSICS_COUNT 12
#define RENDERING_COUNT 12
#define ENTITY_COUNT 12
#define ENEMY_COUNT 12

struct Scene{

	Input input;
	Timer timer;
	//Model meshes [MODEL_COUNT];
	PhysicsComp physics[PHYSICS_COUNT];
	RenderingComp rendering[RENDERING_COUNT];
	EnemyComp enemies[ENEMY_COUNT];
	//Texture textures[TEXTURE_COUNT];
	Entity entities[ENTITY_COUNT];

	Player player;

	Level level;

	//SCTransform camera;
	float cameraHeight;

	int playerId;

	int meshCount;
	int physCount;
	int rendCount;
	int entityCount;
	int enemyCount;

	//Terrain level;

	Scene();

	void Update();

	void Render();

	//Model* GetMesh();

	RenderingComp* AddRenderer(const Entity* entity);
	PhysicsComp* AddPhysics(const Entity* entity);
	EnemyComp* AddEnemy(const Entity* entity);
	Entity* AddEntity();
};

#endif