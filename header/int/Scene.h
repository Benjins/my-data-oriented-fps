#ifndef SCENE_H
#define SCENE_H

#include "Model.h"
#include "Component.h"
#include "Entity.h"
#include "Input.h"

#define MODEL_COUNT 12
#define PHYSICS_COUNT 12
#define RENDERING_COUNT 12
#define ENTITY_COUNT 12

struct Scene{

	Input input;
	Model meshes [MODEL_COUNT];
	PhysicsComp physics[PHYSICS_COUNT];
	RenderingComp rendering[RENDERING_COUNT];
	//Texture textures[TEXTURE_COUNT];
	Entity entities[ENTITY_COUNT];

	SCTransform camera;

	int playerId;

	int meshCount;
	int physCount;
	int rendCount;
	int entityCount;

	//Terrain level;

	Scene();

	void Update();

	void UpdateCamera();

	void Render();

	//Model* GetMesh();

	RenderingComp* AddRenderer(const Entity& entity);
	Entity* AddEntity();
};

#endif