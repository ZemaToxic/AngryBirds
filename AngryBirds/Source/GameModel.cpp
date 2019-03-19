#include "../Header/GameModel.h"
#include "../Header/Camera.h"
#include "../Header/Light.h"

GameModel::GameModel(ModelType modelType, Camera* _camera, std::string texFileName, Light* _light,
                     float _ambientStrength, float _specularStrength)
{
	camera = _camera;
	light = _light;

	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	position = glm::vec3(0.0, 0.0, 0.0);
	color = glm::vec3(1.0f, 1.0f, 1.0f);

	ambientStrength = _ambientStrength;
	specularStrength = _specularStrength;
	speed = 0.05f;
	rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);

	switch (modelType)
	{
	case kTriangle: utils::setTriData(vertices, indices);
		break;
	case kQuad: utils::setQuadData(vertices, indices);
		break;
	case kCube: utils::setCubeData(vertices, indices);
		break;
	case kSphere: utils::setSphereData(vertices, indices);
		break;
	}

	for (auto item : vertices)
	{
		printf("GAME MODEL position: %f, %f, %f \n", item.pos.x, item.pos.y, item.pos.z);
	}

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * indices.size(), &indices[0], GL_STATIC_DRAW);

	this->setTexture(texFileName);

	//Attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (GLvoid*)nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
	                      (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat),
	                      (void*)(offsetof(VertexFormat, VertexFormat::normal)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GameModel::~GameModel()
{
}

void GameModel::update(GLfloat time)
{
	//model = glm::rotate(model,glm::radians(45.0f) * time, glm::vec3(0.0, 1.0, 0.0f));		
	//model = glm::translate(model, position);
}

void GameModel::render(/*glm::vec3 pos*/)
{
	glUseProgram(program);

	//if(bIsTextureSet)
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glUniform1i(glGetUniformLocation(program, "Texture"), 0);

	glm::mat4 model;
	model = translate(model, position);
	model = glm::scale(model, scale);

	glm::mat4 vp = camera->get_projection_matrix() * camera->get_view_matrix();
	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, value_ptr(vp));

	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));


	// lighting calculations

	GLint colorLoc = glGetUniformLocation(program, "objectColor");
	glUniform3f(colorLoc, color.x, color.y, color.z);

	GLuint cameraPosLoc = glGetUniformLocation(program, "cameraPos");
	glUniform3f(cameraPosLoc, camera->get_camera_position().x, camera->get_camera_position().y,
	            camera->get_camera_position().z);

	GLuint lightPosLoc = glGetUniformLocation(program, "lightPos");
	glUniform3f(lightPosLoc, this->light->getPosition().x, this->light->getPosition().y, this->light->getPosition().z);

	GLuint lightColorLoc = glGetUniformLocation(program, "lightColor");
	glUniform3f(lightColorLoc, this->light->getColor().x, this->light->getColor().y, this->light->getColor().z);


	GLuint specularStrengthLoc = glGetUniformLocation(program, "specularStrength");
	glUniform1f(specularStrengthLoc, specularStrength);

	GLuint ambientStrengthLoc = glGetUniformLocation(program, "ambientStrength");
	glUniform1f(ambientStrengthLoc, ambientStrength);


	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}


void GameModel::rotate(glm::vec3 axis)
{
	this->angle.x += axis.x * speed * 20;
	this->angle.y += axis.y * speed * 20;
	this->angle.z += axis.z * speed * 20;
}

void GameModel::setTexture(std::string texFileName)
{
	//** load texture
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//** loadImage and create texture
	// Load image, create texture and generate mipmaps
	int width, height;
	unsigned char* image = SOIL_load_image(texFileName.c_str(), &width, &height, nullptr, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	printf("Filename %s \n", texFileName.c_str());

	//bIsTextureSet = true;
}

// setters and getters

void GameModel::setScale(glm::vec3 _scale)
{
	this->scale = _scale;
}

void GameModel::setRotation(glm::vec3 angle)
{
	this->angle = angle;
}

void GameModel::setRotationAxis(glm::vec3 rotationAxis)
{
	this->rotationAxis = rotationAxis;
}

void GameModel::setPosition(glm::vec3 _position)
{
	this->position = _position;
}

void GameModel::setColor(glm::vec3 _color)
{
	this->color = _color;
}

void GameModel::setCamera(Camera* camera)
{
}

void GameModel::setSpeed(float _speed)
{
	this->speed = _speed;
}

void GameModel::setProgram(GLuint program)
{
	this->program = program;
}

void GameModel::setSpecularStrength(float strength)
{
	specularStrength = strength;
}

glm::vec3 GameModel::getPosition()
{
	return position;
}

glm::vec3 GameModel::getColor()
{
	return this->color;
}

glm::vec3 GameModel::getScale()
{
	return this->scale;
}

glm::vec3 GameModel::getRotation()
{
	return this->angle;
}

glm::vec3 GameModel::getRotationAxis()
{
	return rotationAxis;
}
