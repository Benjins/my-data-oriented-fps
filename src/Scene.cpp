#include "../header/int/Scene.h"
#include "../header/int/RaycastHit.h"
#include <GL/freeglut.h>

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

	if(input.GetMouseUp(GLUT_RIGHT_BUTTON)){
		Entity* enemy = AddEntity();
		enemy->transform.scale = Vector3(0.2f, 0.4f, 0.2f);
		enemy->transform.position = Vector3(0, 0.3f, -5);

		RenderingComp* enemyRend = AddRenderer(enemy);
		enemyRend->SetMeshMatTexture("data/shader", "data/test.obj", "data/Texture2.bmp");

		EnemyComp* enemyComp = AddEnemy(enemy);
		enemyComp->targetPos = enemy->transform.position;
		enemyComp->speed = 0.5f;
		enemyComp->health = 5;

		PhysicsComp* physComp = AddPhysics(enemy);
		physComp->position = Vector3(0,0,0);
		physComp->size = Vector3(0.5f, 0.5f, 0.5f);
	}

	//cout << "Time for this frame: " << timer.deltaTime * 1000 << " ms.\n";
}

void Scene::Render(){
	for(int i = 0; i < rendCount; i++){
		rendering[i].Render(*this);
	}
}

RaycastHit Scene::Raycast(Vector3 origin, Vector3 direction) const{
	
	RaycastHit x;
	x.hit = false;
	x.depth = FLT_MAX;
	x.entityId = -1;

	//Iterate over level walls
	for(int i = 0; i < level.walls.size(); i++){
		RaycastHit wallHit = RaycastWall(origin, direction, level.walls[i]);
		if(wallHit.hit && wallHit.depth < x.depth){
			x = wallHit;
			x.entityId = -1;
		}
	}

	//And now floors
	for(int i = 0; i < level.floors.size(); i++){
		RaycastHit floorHit = RaycastFloor(origin, direction, level.floors[i]);
		if(floorHit.hit && floorHit.depth < x.depth){
			x = floorHit;
			x.entityId = -1;
		}
	}

	for(int i = 0; i < physCount; i++){
		Entity ent = entities[physics[i].entity];
		RaycastHit boxHit = RaycastBox(ent, physics[i], origin, direction);
		if(boxHit.hit && boxHit.depth < x.depth){
			x = boxHit;
			x.entityId = ent.id;
		}
	}

	return x;
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

void Scene::RemoveEntity(Entity* ent){
	memmove(entities + ent->id, entities + ent->id + 1, (entityCount - ent->id - 1) * sizeof(Entity));
	entityCount--;

	for(int i = 0; i < physCount; i++){
		if(physics[i].entity == ent->id){
			memmove(physics + i, physics + i + 1, (physCount - i - 1) * sizeof(PhysicsComp));
			physCount--;
			break;
		}
	}

	for(int i = 0; i < rendCount; i++){
		if(rendering[i].entity == ent->id){
			rendering[i].Release();
			memmove(rendering + i, rendering + i + 1, (rendCount - i - 1) * sizeof(RenderingComp));
			rendCount--;
			break;
		}
	}

	for(int i = 0; i < enemyCount; i++){
		if(enemies[i].entity == ent->id){
			memmove(enemies + i, enemies + i + 1, (enemyCount - i - 1) * sizeof(EnemyComp));
			enemyCount--;
			break;
		}
	}
}
