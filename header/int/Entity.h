#ifndef ENTITY_H
#define ENTITY_H

#include "SCTransform.h"

struct Entity{
	SCTransform transform;
	int id;
	int flags;

	Entity();
};

#endif