#include "../Header/camera.h"
float cameraSpeed = 0.5f;
Camera::Camera() = default;
Camera::~Camera() = default;

void Camera::initialize()
{
	// Define the camera
	camera_pos_ = glm::vec3(0.0f, 0.0f, 10.0f);
	camera_front_ = glm::vec3(0.0f, 0.0f, -1.0f);
	camera_up_ = glm::vec3(0.0f, 1.0f, 0.0f);

	// Define the Matrix(s)
	view_matrix_ = lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
	projection_matrix_ = glm::perspective(70.0f, float(utils::window_width) / float(utils::window_height), 1.0f, 10000.f);
	ortho_matrix_ = glm::ortho(0.0f, float(utils::window_width) / 10, 0.0f, float(utils::window_height) / 10, 0.1f, 100.0f);

	// For Debug purposes
	if (utils::DEBUG) { std::cout << "Camera Initialized\n"; }
}

void Camera::update_camera()
{
	// Forward / Backward
	if (Input::GetInstance().GetKeyState('w') == INPUT_HOLD) { moveForward(); }
	if (Input::GetInstance().GetKeyState('s') == INPUT_HOLD) { moveBackward(); }
	// Left / Right
	if (Input::GetInstance().GetKeyState('a') == INPUT_HOLD) { moveLeft(); }
	if (Input::GetInstance().GetKeyState('d') == INPUT_HOLD) { moveRight(); }
	// Up / Down
	if (Input::GetInstance().GetKeyState('q') == INPUT_HOLD) { moveUp(); }
	if (Input::GetInstance().GetKeyState('e') == INPUT_HOLD) { moveDown(); }

	if (Input::GetInstance().GetKeyState('z') == INPUT_HOLD) { printf("Camera location x:%f, y:%f z:%f \n", camera_pos_.x, camera_pos_.y, camera_pos_.y); }
	view_matrix_ = lookAt(camera_pos_, camera_pos_ + camera_front_, camera_up_);
}

glm::vec3& Camera::get_camera_position() { return camera_pos_; }

glm::vec3& Camera::get_camera_front() { return camera_front_; }

glm::vec3& Camera::get_camera_up() { return camera_up_; }

glm::mat4& Camera::get_view_matrix() { return view_matrix_; }

glm::mat4& Camera::get_projection_matrix() { return projection_matrix_; }

glm::mat4& Camera::get_ortho_matrix() { return ortho_matrix_; }

// Camera Movement 
void Camera::moveForward() { camera_pos_ += camera_front_ * cameraSpeed; }

void Camera::moveBackward() { camera_pos_ -= camera_front_ * cameraSpeed; }

void Camera::moveLeft() { camera_pos_ -= normalize(cross(camera_front_, camera_up_)) * cameraSpeed; }

void Camera::moveRight() { camera_pos_ += normalize(cross(camera_front_, camera_up_)) * cameraSpeed; }

void Camera::moveUp() { camera_pos_ = glm::vec3(camera_pos_.x, camera_pos_.y + 0.5f, camera_pos_.z); }

void Camera::moveDown() { camera_pos_ = glm::vec3(camera_pos_.x, camera_pos_.y - 0.5f, camera_pos_.z); }
