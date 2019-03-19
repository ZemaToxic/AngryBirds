// Includes
#include "../Header Files/utils.h"
#include "../Header Files/camera.h"
#include "../Header Files/light.h"
#include "../Header Files/shaderLoader.h"

GLuint main_program;
ShaderLoader shader_loader;

Camera* main_camera;
Light* main_light;

// Initialise the "Game"
void init()
{
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Init Called\n";
	}

	// Make a new Camera and initialize it
	main_camera = new Camera();
	main_camera->initialize();

	// Load the shaderLoader for the Light
	GLuint light_program = shader_loader.create_program((char*)"Assets/Shaders/Light.vs",
	                                                    (char*)"Assets/Shaders/Light.fs");
	main_light = new Light(main_camera, utils::ambientStrength, utils::specularStrength, glm::vec3(1.0f, 0.0f, 0.0f));
	main_light->setProgram(light_program);
}

// Update called each "frame"
void update()
{
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Update Called\n";
	}
	main_camera->update_camera(utils::key_state);
	main_light->updateLight(utils::key_state);
	utils::optionsMenu(utils::key_state);

	glutPostRedisplay(); // Do not move this.
}

// Render... does as it says
void render()
{
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Render Called\n";
	}

	// Set the background colour
	glClearColor(0.9f, 0.5f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Light Render function
	main_light->render();

	glutSwapBuffers(); // swap buffers
}

// Main function Loop
int main(int argc, char** argv)
{
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Main Called\n";
	}
	// Init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(utils::window_pos_x, utils::window_pos_y);
	glutInitWindowSize(utils::window_width, utils::window_height);
	glutCreateWindow("OpenGL");

	glewInit();
	glutSetOption(GLUT_MULTISAMPLE, 8);

	glEnable(GL_MULTISAMPLE);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glShadeModel(GL_SMOOTH);
	glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);

	init(); // Initialise entities

	// Register callbacks
	glutDisplayFunc(render); // Render callback
	glutKeyboardFunc(utils::keyboard); // Keyboard press callback
	glutKeyboardUpFunc(utils::keyboard_up); // Keyboard release callback
	glutReshapeFunc(utils::handle_resize); // Window Resize callback
	glutIdleFunc(update); // Update callback
	glutMainLoop(); // Main loop

	return 0;
}
