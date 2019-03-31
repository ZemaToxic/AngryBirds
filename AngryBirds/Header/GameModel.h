#include "utils.h"
#include "ShaderLoader.h"
class Light;
class Camera;
#pragma once
class GameModel
	{
	public:
		GameModel(ModelType modelType, Camera* _camera, std::string texFileName);
		~GameModel();
		void update();
		void render();
		void setPosition(glm::vec3 _position);
		void setScale(glm::vec3 _scale);
		void setRotation(glm::vec3 angle);
		void setRotationAxis(glm::vec3 rotationAxis);
		void setColor(glm::vec3 _color);
		void setCamera(Camera* camera);
		void setSpeed(float _speed);
		void setProgram(GLuint program);
		void setTexture(std::string texFileName);
		void setSpecularStrength(float strength);
		glm::vec3 getPosition();
		glm::vec3 getScale();
		glm::vec3 getRotation();
		glm::vec3 getRotationAxis();
		glm::vec3 getColor();
		void rotate(glm::vec3 axis);
		Camera* camera;
		Light* light;
		bool bIsTextureSet = false;
		float speed;
		std::vector<VertexFormat> vertices;
		std::vector<GLuint> indices;
		int width, height;
		glm::vec3 position;
		glm::vec3 scale;
		glm::vec3 angle;
		glm::vec3 rotationAxis;
		glm::vec3 color;
		GLuint vao;
		GLuint vbo;
		GLuint ebo;
		GLuint texture;
		GLuint program;
		glm::mat4 model;
		float specularStrength;
		float ambientStrength;
	};
