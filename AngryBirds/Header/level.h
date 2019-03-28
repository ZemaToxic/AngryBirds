#pragma once
#include "utils.h"
#include "../Header/camera.h"
#include "../Header/shaderLoader.h"
#include "../Header/box2DObj.h"
#include "../Header/input.h"
#include "../Header/contact.h"

class Level
	{
	public:
		void setLevel(int _level);

		int getLevel();

		int level = 1;
	};

inline void Level::setLevel(int _level) { level = _level; }

inline int Level::getLevel() { return level; }
