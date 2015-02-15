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
	player.Update(*this);
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
