#include "../header/int/Player.h"
#include "../header/int/Scene.h"


void Player::Update(const Scene& mainScene){
	Vector3 newPos = camera.position;
	float deltaTimeClamp = min(0.53, mainScene.timer.deltaTime);
	if(mainScene.input.GetKey('w')){
		Vector3 moveVec = camera.Forward();
		moveVec.y = 0;
		moveVec.Normalize();
		newPos = newPos + moveVec * 5 * deltaTimeClamp;// mainScene.timer.deltaTime;
	}
	if(mainScene.input.GetKey('s')){
		Vector3 moveVec = camera.Forward() * -1;
		moveVec.y = 0;
		moveVec.Normalize();
		newPos = newPos + moveVec * deltaTimeClamp;// mainScene.timer.deltaTime;
	}
	if(mainScene.input.GetKey('a')){
		Vector3 moveVec = camera.Right() * -1;
		moveVec.y = 0;
		moveVec.Normalize();
		newPos = newPos + moveVec * deltaTimeClamp;// mainScene.timer.deltaTime;
	}
	if(mainScene.input.GetKey('d')){
		Vector3 moveVec = camera.Right();
		moveVec.y = 0;
		moveVec.Normalize();
		newPos = newPos + moveVec * deltaTimeClamp;// mainScene.timer.deltaTime;
	}
	if(mainScene.input.GetKey('q')){
		camera.position.y +=  mainScene.timer.deltaTime;
	}
	if(mainScene.input.GetKey('z')){
		camera.position.y -= mainScene.timer.deltaTime;
	}

	if(mainScene.input.GetKeyDown(' ')){
		if(state == PLAYER_STATE::GROUNDED){
			velocity = 1.5f;
			state = PLAYER_STATE::JUMPING;
		}
	}

	if(state == PLAYER_STATE::JUMPING){
		velocity -= gravity * mainScene.timer.deltaTime;
		if(velocity < 0){
			state = PLAYER_STATE::FALLING;
		}
	}
	else if(state == PLAYER_STATE::FALLING){
		velocity -= gravity * mainScene.timer.deltaTime;
	}

	float oldY = camera.position.y;
	camera.position = mainScene.level.ResolveCollisions(camera.position, newPos);

	float groundY = cameraHeight + mainScene.level.FindHeight(camera.position);
	camera.position.y += velocity * mainScene.timer.deltaTime;
	if(camera.position.y <= groundY){
		camera.position.y = groundY;
		velocity = 0;
		state = PLAYER_STATE::GROUNDED;
	}
	else{
		if(state == PLAYER_STATE::GROUNDED){
			state = PLAYER_STATE::FALLING;
		}
	}

	camera.rotation = Quaternion(Y_AXIS, mainScene.input.mouseX/80) *  Quaternion(X_AXIS, mainScene.input.mouseY/80);

}

