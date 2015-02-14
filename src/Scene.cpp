#include "../header/int/Scene.h"

Scene::Scene(){
	meshCount = 0;
	physCount = 0;
	rendCount = 0;
	entityCount = 0;

	playerId = 0;

	camera = SCTransform();
}

void Scene::Update(){
	UpdateCamera();
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

void Scene::UpdateCamera(){
	if(input.GetKey('w')){
		Vector3 moveVec = camera.Forward();
		moveVec.y = 0;
		moveVec.Normalize();
		camera.position = camera.position + moveVec * timer.deltaTime;
	}
	if(input.GetKey('s')){
		Vector3 moveVec = camera.Forward() * -1;
		moveVec.y = 0;
		moveVec.Normalize();
		camera.position = camera.position + moveVec * timer.deltaTime;
	}
	if(input.GetKey('a')){
		Vector3 moveVec = camera.Right() * -1;
		moveVec.y = 0;
		moveVec.Normalize();
		camera.position = camera.position + moveVec * timer.deltaTime;
	}
	if(input.GetKey('d')){
		Vector3 moveVec = camera.Right();
		moveVec.y = 0;
		moveVec.Normalize();
		camera.position = camera.position + moveVec * timer.deltaTime;
	}
	if(input.GetKey('q')){
		camera.position.y +=  timer.deltaTime;
	}
	if(input.GetKey('z')){
		camera.position.y -= timer.deltaTime;
	}

	camera.rotation = Quaternion(Y_AXIS, input.mouseX/80) *  Quaternion(X_AXIS, input.mouseY/80);
}