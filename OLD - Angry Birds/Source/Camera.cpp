/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	Camera.cpp
Description	:	Contains camera functions
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

//Local Includes
#include "Camera.h"
#include "GameScene.h"

/*
**Description:	Camera Constructor, Initilisation and location 
**Author:		Matthew Seymour, Matthew Seymour
**Parameters:	N/A
**Return:		N/A
*/
Camera::Camera()
{
	m_cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
	m_CameraPos = glm::vec3(0.0f, 0.0f, 1.0f);
	m_CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
	m_CameraDirection = normalize(m_CameraPos - m_CameraTarget);

	m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
	m_CameraRight = normalize(cross(m_Up, m_CameraDirection));

	m_CameraUp = cross(m_CameraDirection, m_CameraRight);

	m_ProjMatrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
};

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
Camera::~Camera()
{
};

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
void Camera::Update(bool*KeyStates)
{

	double _DeltaTime = (GameScene::GetInstance().GetDeltaTime() / 100);

//	m_CameraTarget = GameScene::GetInstance()._Player.GetPosition();

	float movementSpeed = 1.0f;

	if (KeyStates[(unsigned char)'i'] == BUTTON_DOWN)
	{
		m_CameraPos.z -= _DeltaTime * cameraSpeed;
	}else
	if (KeyStates[(unsigned char)'k'] == BUTTON_DOWN)
	{
		m_CameraPos.z += _DeltaTime * cameraSpeed;
	}
	if (KeyStates[(unsigned char)'l'] == BUTTON_DOWN)
	{
		m_CameraPos.x += _DeltaTime * cameraSpeed;
	}
	else
		if (KeyStates[(unsigned char)'j'] == BUTTON_DOWN)
		{
			m_CameraPos.x -= _DeltaTime * cameraSpeed;
		}

	view = glm::lookAt(m_CameraPos, m_CameraTarget, m_Up);
	
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
glm::mat4 Camera::GetModelMatrix()
{
	return m_ModelMatrix;
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
glm::mat4 Camera::GetViewMatrix()
{
	return view;
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
glm::mat4 Camera::GetProjectionMatrix()
{
	return m_ProjMatrix;
}