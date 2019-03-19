#pragma once

#include "utils.h"
#include "camera.h"

class Light
{
public:
	// Constructor
	Light(Camera* _camera, float ambientStrength, float specularStrength, glm::vec3 _lightColour);
	~Light();

	// Light Functions
	void setPosition(glm::vec3 _lightPosition);
	void setColour(glm::vec3 _lightColour);
	void setSpecularStrength(float _specularStrength);
	void setAmbientStrength(float _ambientStrength);
	void setProgram(GLuint program);

	void updateLight(unsigned char keystate[]);
	void render();

	glm::vec3 getPosition();
	glm::vec3 getColor();

	// Light Movement functions
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
private:

	Camera* camera_;

	std::vector<VertexFormat> vertices;
	std::vector<GLuint> indices;

	glm::vec3 localPosition;
	glm::vec3 scale;
	glm::vec3 angle;
	glm::vec3 color;
	glm::vec3 lightColor;

	GLuint lightVao;
	GLuint lightVbo;
	GLuint lightEbo;
	GLuint lightProgram;

	float specularStrength;
	float ambientStrength;
};
