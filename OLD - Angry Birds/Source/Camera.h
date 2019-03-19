/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Camera.h
Description	:	Declares camera variables
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

#pragma once

#include "Utils.h"

#ifndef _CAMERA_H
#define _CAMERA_H

class Camera
{
public:
	Camera();
	~Camera();


	//glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::vec3 m_cameraFront;
	//glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	GLfloat cameraSpeed = 0.5f;

	glm::vec3 m_CameraPos;
	glm::vec3 m_CameraTarget;
	glm::vec3 m_CameraDirection;

	glm::vec3 m_Up;
	glm::vec3 m_CameraRight;

	glm::vec3 m_CameraUp;

	glm::mat4 m_ModelMatrix;
	glm::mat4 view;
	glm::mat4 m_ProjMatrix;

	void Update(bool*KeyStates);
	glm::mat4 GetModelMatrix();
	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix();

private:
	int previousTime = 0;


};


#endif