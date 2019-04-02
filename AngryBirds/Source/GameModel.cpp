#include "../Header/GameModel.h"
#include "../Header/camera.h"

GameModel::GameModel(ModelType modelType, Camera* _camera, std::string texFileName, CameraType _cam)
{
	camera = _camera;
	cam = _cam;
	if (cam == ortho)
	{
		position = glm::vec3(0.0, 0.0, 0.0);
	}
	else
	{
		position = glm::vec3(500.0, 250.0, 250.0);
	}
	scale = glm::vec3(1.0f, 1.0f, 1.0f);
	color = glm::vec3(1.0f, 1.0f, 1.0f);
	speed = 0.05f;
	rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
	switch (modelType) {
		case kTriangle: utils::setTriData(vertices, indices);
			break;
		case kQuad: utils::setQuadData(vertices, indices);
			break;
		case kCube: utils::setCubeData(vertices, indices);
			break;
		case kSphere: utils::setSphereData(vertices, indices);
			break;
	}
	for (auto item : vertices) {
		//printf("GAME MODEL position: %f, %f, %f \n", item.pos.x, item.pos.y, item.pos.z);
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::texCoord)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(offsetof(VertexFormat, VertexFormat::normal)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GameModel::~GameModel() {}

void GameModel::update()
{

}

void GameModel::render()
{
	glUseProgram(program);
	if (bIsTextureSet) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glUniform1i(glGetUniformLocation(program, "Texture"), 0);
	}
	glm::mat4 model;
	model = translate(model, position);
	model = glm::scale(model, scale);
	model = glm::rotate(model, angle.z, normalize(rotationAxis));

	glm::mat4 vp;
	switch (cam)
	{
	case ortho:
		// Orthographic
		vp = camera->get_ortho_matrix() * camera->get_view_matrix();
		break;
	case persepctive:
		// Projection
		vp = camera->get_projection_matrix() * camera->get_view_matrix();
		break;;
	}

	GLint vpLoc = glGetUniformLocation(program, "vp");
	glUniformMatrix4fv(vpLoc, 1, GL_FALSE, value_ptr(vp));
	GLint modelLoc = glGetUniformLocation(program, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}

void GameModel::rotate(glm::vec3 axis)
{
	this->angle.x = axis.x;
	this->angle.y = axis.y;
	this->angle.z = axis.z;
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
	if (!image) {
		bIsTextureSet = false;
		printf("Filename \"%s\" Failed to load \n", texFileName.c_str());
		return;
	}
	bIsTextureSet = true;
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);
	printf("Filename \"%s\" Loaded successfully \n", texFileName.c_str());
}

// setters and getters
void GameModel::setScale(glm::vec3 _scale) { this->scale = _scale; }

void GameModel::setRotation(glm::vec3 angle) { this->angle = angle; }

void GameModel::setRotationAxis(glm::vec3 rotationAxis) { this->rotationAxis = rotationAxis; }

void GameModel::setPosition(glm::vec3 _position) { this->position = _position; }

void GameModel::setColor(glm::vec3 _color) { this->color = _color; }

void GameModel::setCamera(Camera* camera) {}

void GameModel::setSpeed(float _speed) { this->speed = _speed; }

void GameModel::setProgram(GLuint program) { this->program = program; }

void GameModel::setSpecularStrength(float strength) { specularStrength = strength; }

glm::vec3 GameModel::getPosition() { return position; }

glm::vec3 GameModel::getColor() { return this->color; }

glm::vec3 GameModel::getScale() { return this->scale; }

glm::vec3 GameModel::getRotation() { return this->angle; }

glm::vec3 GameModel::getRotationAxis() { return rotationAxis; }
