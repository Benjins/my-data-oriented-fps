#ifndef PLAYER_H
#define PLAYER_H

#include "SCTransform.h"

struct Scene;

enum struct PLAYER_STATE{
	GROUNDED,
	JUMPING,
	FALLING
};

struct Player{
	SCTransform camera;
	float cameraHeight;
	float velocity;
	float gravity;
	PLAYER_STATE state;

	void Update(Scene& mainScene);
};

#endif
