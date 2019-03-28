#pragma once
#include "utils.h"

class Camera
	{
	public:
		Camera();

		~Camera();

		void initialize();

		void update_camera();

		glm::vec3& get_camera_position();

		glm::vec3& get_camera_front();

		glm::vec3& get_camera_up();

		glm::mat4& get_view_matrix();

		glm::mat4& get_projection_matrix();

		glm::mat4& get_ortho_matrix();


		// Camera Movement functions
		void moveForward();

		void moveBackward();

		void moveLeft();

		void moveRight();

		void moveUp();

		void moveDown();

	private:
		glm::mat4 view_matrix_;
		glm::mat4 projection_matrix_;
		glm::mat4 ortho_matrix_;
		glm::vec3 camera_pos_;
		glm::vec3 camera_front_;
		glm::vec3 camera_up_;
	};
