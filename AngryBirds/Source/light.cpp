#include "../Header/light.h"

Light::Light(Camera* _camera, float _ambientStrength, float _specularStrength, glm::vec3 _lightColor)
{
	camera_ = _camera;

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	localPosition = glm::vec3(0.0f, 10.0f, 95.0f);

	color = glm::vec3(1.0f, 1.0f, 1.0f);
	lightColour = _lightColor;

	angle = glm::vec3(0.1f, 0.5f, 0.2f);
	utils::setSphereData(vertices, indices);


	glGenVertexArrays(1, &lightVao);
	glBindVertexArray(lightVao);

	glGenBuffers(1, &lightVbo);
	glBindBuffer(GL_ARRAY_BUFFER, lightVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &lightEbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEbo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Light::render()
{
	glUseProgram(lightProgram);

	glm::mat4 model;
	model = translate(model, localPosition);
	// Move the Sphere slightly below the ligth source
	model = translate(model, glm::vec3(0.0f, -2.0f, 1.0f));
	model = glm::scale(model, scale);

	glm::mat4 vp = camera_->get_projection_matrix() * camera_->get_view_matrix();
	GLint vpLoc = glGetUniformLocation(lightProgram, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(lightProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));

	// lighting calculations

	GLint colorLoc = glGetUniformLocation(lightProgram, "objectColor");
	glUniform3f(colorLoc, color.x, color.y, color.z);

	GLuint cameraPosLoc = glGetUniformLocation(lightProgram, "cameraPos");
	glUniform3f(cameraPosLoc, camera_->get_camera_position().x, camera_->get_camera_position().y,
	            camera_->get_camera_position().z);

	GLuint lightPosLoc = glGetUniformLocation(lightProgram, "lightPos");
	glUniform3f(lightPosLoc, this->getPosition().x, this->getPosition().y, this->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(lightProgram, "lightColour");
	glUniform3f(lightColorLoc, this->getColour().x, this->getColour().y, this->getColour().z);

	GLuint specularStrengthLoc = glGetUniformLocation(lightProgram, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(lightProgram, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);


	glBindVertexArray(lightVao);
	//glDrawArrays(GL_TRIANGLES, 0, indices.size());
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void Light::updateLight(unsigned char keystate[])
{
	// Reset Light
	if (keystate[unsigned char('r')] == BUTTON_DOWN)
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		localPosition = glm::vec3(0.0f, 10.0f, 95.0f);
		angle = glm::vec3(0.1f, 0.5f, 0.2f);
	}
	// Move the Light forward and backward
	if (keystate[unsigned char('i')] == BUTTON_DOWN)
	{
		moveForward();
	}
	if (keystate[unsigned char('k')] == BUTTON_DOWN)
	{
		moveBackward();
	}
	// Move the ligth left and right
	if (keystate[unsigned char('j')] == BUTTON_DOWN)
	{
		moveLeft();
	}
	if (keystate[unsigned char('l')] == BUTTON_DOWN)
	{
		moveRight();
	}
	// Move the Light up and down
	if (keystate[unsigned char('u')] == BUTTON_DOWN)
	{
		moveUp();
	}
	if (keystate[unsigned char('o')] == BUTTON_DOWN)
	{
		moveDown();
	}
	// Debugging 
	if (utils::DEBUG)
	{
		printf("Light Position: x: %f, y: %f, z: %f \n", localPosition.x, localPosition.y, localPosition.z);
	}
}

void Light::setProgram(GLuint program)
{
	this->lightProgram = program;
}

void Light::setPosition(glm::vec3 _lightPosition)
{
	this->localPosition = _lightPosition;
}

void Light::setColour(glm::vec3 _lightColour)
{
	this->lightColour = _lightColour;
}

void Light::setSpecularStrength(float _specularStrength)
{
	specularStrength = _specularStrength;
}

void Light::setAmbientStrength(float _ambientStrength)
{
	ambientStrength = _ambientStrength;
}

glm::vec3 Light::getPosition()
{
	return (localPosition);
}

glm::vec3 Light::getColour()
{
	return (lightColour);
}

void Light::moveForward()
{
	localPosition -= glm::vec3(0.0f, 0.0f, 1.0f) * 0.1f;
}

void Light::moveBackward()
{
	localPosition += glm::vec3(0.0f, 0.0f, 1.0f) * 0.1f;
}

void Light::moveLeft()
{
	localPosition -= glm::vec3(1.0f, 0.0f, 0.0f) * 0.1f;
}

void Light::moveRight()
{
	localPosition += glm::vec3(1.0f, 0.0f, 0.0f) * 0.1f;
}

void Light::moveUp()
{
	localPosition += glm::vec3(0.0f, 1.0f, 0.0f) * 0.1f;
}

void Light::moveDown()
{
	localPosition -= glm::vec3(0.0f, 1.0f, 0.0f) * 0.1f;
}

Light::~Light()
= default;
