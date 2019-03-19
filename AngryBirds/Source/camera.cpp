#include "../Header Files/camera.h"

float cameraSpeed = 0.5f;

Camera::Camera()
= default;

Camera::~Camera()
= default;

void Camera::initialize()
{
	// Define the camera
	camera_pos_ = glm::vec3(0.0f, 3.0f, 20.0f);
	camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

	// Define the Matrix(s)
	view_matrix_ = lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
	projection_matrix_ = glm::perspective(45.0f, float(utils::window_width) / float(utils::window_height), 1.0f,
	                                      10000.f);
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Camera Initialized\n";
	}
}

void Camera::update_camera(unsigned char value[])
{
	// Reset Camera
	if (value[unsigned char('r')] == BUTTON_DOWN)
	{
		camera_pos_ = glm::vec3(0.0f, 3.0f, 20.0f);
		camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
		camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);
		cameraSpeed = 0.5f;
	}

	// Forward / Backward
	if (value[unsigned char('w')] == BUTTON_DOWN)
	{
		moveForward();
	}
	if (value[unsigned char('s')] == BUTTON_DOWN)
	{
		moveBackward();
	}
	// Left / Right
	if (value[unsigned char('a')] == BUTTON_DOWN)
	{
		moveLeft();
	}
	if (value[unsigned char('d')] == BUTTON_DOWN)
	{
		moveRight();
	}
	// Up / Down
	if (value[unsigned char('x')] == BUTTON_DOWN)
	{
		moveUp();
	}
	if (value[unsigned char('z')] == BUTTON_DOWN)
	{
		moveDown();
	}


	// Increase / Decrease Camera speed
	if (value[unsigned char('q')] == BUTTON_DOWN)
	{
		cameraSpeed = cameraSpeed + 1.0f;
	}
	if (value[unsigned char('e')] == BUTTON_DOWN)
	{
		cameraSpeed = cameraSpeed - 1.0f;
	}
	// Rotate Up, Down, Left, Right
	if (value[unsigned char('t')] == BUTTON_DOWN)
	{
		// rotate the camera upwards
		camera_front_ += camera_up_ * cameraSpeed;
	}
	if (value[unsigned char('g')] == BUTTON_DOWN)
	{
		// rotate the camera downwards
		camera_front_ -= camera_up_ * cameraSpeed;
	}
	if (value[unsigned char('f')] == BUTTON_DOWN)
	{
		// rotate the camera to the left
		camera_front_ -= normalize(cross(camera_front_, camera_up_)) * cameraSpeed;
	}
	if (value[unsigned char('h')] == BUTTON_DOWN)
	{
		// rotate the camera to the right
		camera_front_ += normalize(cross(camera_front_, camera_up_)) * cameraSpeed;
	}
	
	if (value[unsigned char('[')] == BUTTON_DOWN)
	{
		system("CLS");
		utils::DEBUG = !utils::DEBUG;
	}
	view_matrix_ = lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);

	// For Debug purposes
	if (utils::DEBUG)
	{
		printf("Camera Speed: %f \n", cameraSpeed);
		printf("Camera Position: x: %f, y: %f, z: %f \n", camera_pos_.x, camera_pos_.y, camera_pos_.z);
		printf("Camera Looking at: x: %f, y: %f, z: %f \n", camera_front_.x, camera_front_.y, camera_front_.z);
	}
}

glm::vec3& Camera::get_camera_position()
{
	return camera_pos_;
}

glm::vec3& Camera::get_camera_front()
{
	return camera_front_;
}

glm::vec3& Camera::get_camera_up()
{
	return camera_up_;
}

glm::mat4& Camera::get_view_matrix()
{
	return view_matrix_;
}

glm::mat4& Camera::get_projection_matrix()
{
	return projection_matrix_;
}

// Camera Movement 
void Camera::moveForward()
{
	camera_pos_ += camera_front_ * cameraSpeed;
}

void Camera::moveBackward()
{
	camera_pos_ -= camera_front_ * cameraSpeed;
}

void Camera::moveLeft()
{
	camera_pos_ -= normalize(cross(camera_front_, camera_up_)) * cameraSpeed;
}

void Camera::moveRight()
{
	camera_pos_ += normalize(cross(camera_front_, camera_up_)) * cameraSpeed;
}

void Camera::moveUp()
{
	camera_pos_ = glm::vec3(camera_pos_.x, camera_pos_.y + 0.5f, camera_pos_.z);
}

void Camera::moveDown()
{
	camera_pos_ = glm::vec3(camera_pos_.x, camera_pos_.y - 0.5f, camera_pos_.z);
}
