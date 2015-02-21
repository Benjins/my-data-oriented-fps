#include "../header/int/Scene.h"

Scene::Scene(){
	meshCount = 0;
	physCount = 0;
	rendCount = 0;
	entityCount = 0;

	playerId = 0;

	player.camera = SCTransform();
}

void Scene::Update(){
	entities[0].transform.rotation = entities[0].transform.rotation * Quaternion(Y_AXIS, timer.deltaTime);
	player.Update(*this);
	for(int i = 0; i < enemyCount; i++){
		enemies[i].Update(*this);
	}
	cout << "Time for this frame: " << timer.deltaTime * 1000 << " ms.\n";
}

void Scene::Render(){
	for(int i = 0; i < rendCount; i++){
		rendering[i].Render(*this);
	}
}

//Pass NULL to add the rendering component for the level
RenderingComp* Scene::AddRenderer(const Entity* entity){
	if(rendCount >= RENDERING_COUNT){
		cout << "\nHit max renderer limit.\n";
		return NULL;
	}

	if(entity == NULL){
		rendering[rendCount].entity = -1;
	}
	else{
		rendering[rendCount].entity = entity->id;
	}
	RenderingComp* rend = &rendering[rendCount];
	rendCount++;
	return rend;
}

EnemyComp* Scene::AddEnemy(const Entity* entity){
	if(enemyCount >= ENEMY_COUNT){
		cout << "\nHit max enemy limit.\n";
		return NULL;
	}

	if(entity != NULL){
		enemies[enemyCount].entity = entity->id;
	}

	EnemyComp* enemy = &enemies[enemyCount];
	enemy->speed = 0.6f;
	enemyCount++;
	return enemy;
}

PhysicsComp* Scene::AddPhysics(const Entity* entity){
	if(physCount >= PHYSICS_COUNT){
		cout << "\nHit max enemy limit.\n";
		return NULL;
	}

	if(entity != NULL){
		physics[physCount].entity = entity->id;
	}

	PhysicsComp* phys = &physics[physCount];
	physCount++;
	return phys;
}

Entity* Scene::AddEntity(){
	if(entityCount >= ENTITY_COUNT){
		cout << "\nHit max entity limit.\n";
		return NULL;
	}

	entities[entityCount] = Entity();
	entities[entityCount].id = entityCount;

	Entity* ent = &entities[entityCount];
	entityCount++;
	return ent;
}
