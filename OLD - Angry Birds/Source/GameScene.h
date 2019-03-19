/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	GameScene.cpp
Description	:	Declares game scene functions and variables
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

#pragma once

#include "Camera.h"

//Library Includes
#include <vector>

#ifndef _GAMESCENE_H
#define _GAMESCENE_H

class GameScene
{
public:
	~GameScene();

	// Singleton Methods
	static GameScene& GetInstance();
	static void DestroyInstance();

	void RenderScene();
	void Update(bool*KeyStates);
	void Init();

	bool InitFmod();
	const bool LoadAudio();
	void SetTextLabel(char* _text);

	double GetDeltaTime();

	Camera* GetCam();

	GLuint program;
private:
	double m_dDeltaTime;
	double m_dLastTick;
	Camera* _Camera;


	GameScene();
	GameScene(const GameScene& _kr);
	GameScene& operator= (const GameScene& _kr);
	void drawSquare(b2Vec2* points, b2Vec2 Center, float Angle);

protected:
	// Singleton Instance
	static GameScene* s_pGame;
};
#endif