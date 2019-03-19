/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Utils.h
Description	:	Defines structs and useful functions.
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

#pragma once

#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include "Dependencies\soil\SOIL.h"
#include "Dependencies\glm\glm.hpp"
#include "Dependencies\glm\gtc\matrix_transform.hpp"
#include "Dependencies\glm\gtx\transform.hpp"
#include "Dependencies\glm\gtc\type_ptr.hpp"
#include "Dependencies\Box2D\Box2D.h"
#include "ShaderLoader.h"



#ifndef UTILS_H
#define UTILS_H

#define WIDTH 800
#define HEIGHT 600

#define TICKRATE 1.0f / 60.0f
#define GRAVITY -9.81f

#define BUTTON_UP 0
#define BUTTON_DOWN 1

#define M_PI  3.145f

#define M2P  20.0f
#define P2M (1 / M2P)

class Models;
class Player;

#endif