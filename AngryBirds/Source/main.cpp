// Includes
#include "../Header/utils.h"
#include "../Header/camera.h"
#include "../Header/light.h"
#include "../Header/shaderLoader.h"
#include "../Header/Box2DObject.h"

GLuint main_program;
ShaderLoader shader_loader;
ModelType model_type = kCube;

Camera* main_camera;
Light* main_light;
GameModel* model;

b2World* world;
CBox2DObject* bGround;
CBox2DObject* bBox;

CBox2DObject* bObstacle1;
CBox2DObject* bObstacle2;
CBox2DObject* bObstacle3;
CBox2DObject* bObstacle4;

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
	const GLuint light_program = shader_loader.create_program((char*)"Assets/Shaders/Light.vs", (char*)"Assets/Shaders/Light.fs");
	main_light = new Light(main_camera, utils::ambientStrength, utils::specularStrength, glm::vec3(1.0f, 1.0f, 1.0f));
	main_light->setProgram(light_program);

	// Create a new Cube and pass the light value
	model = new GameModel(model_type, main_camera, "Assets/front.jpg", main_light, utils::ambientStrength, utils::specularStrength);
	model->setProgram(light_program);

	// Init Box2D
	const b2Vec2 gravity(0.0f, -200.0f);
	world = new b2World(gravity);

	//A fixture is required for box 2d to make physics equations
	b2FixtureDef fixture_def;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.3f;
	fixture_def.restitution = 0.5f; //Bouncyness

	// Ground
	bGround = new CBox2DObject(world, BOX, fixture_def, false, "Assets/front.jpg", main_camera, main_light, {0, -35}, {75, 10});
	bGround->setProgram(light_program);
	// Box (should fall and land on ground)
	bBox = new CBox2DObject(world, BOX, fixture_def, true, "Assets/front.jpg", main_camera, main_light);
	bBox->setProgram(light_program);
}

// Update called each "frame"
void update()
{
	// For Debug purposes
	if (utils::DEBUG)
	{
		std::cout << "Update Called\n";
	}
	//At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 120.0f;
	const int32 velocity_iterations = 6;
	const int32 position_iterations = 2;
	world->Step(time_step, velocity_iterations, position_iterations);

	// Update box2D Ground physics
	bGround->process();
	bGround->update();
	// Update box2D Box physics for the box
	bBox->process();
	bBox->update();

	// Update the Model's
	model->update();

	// Update Camera (Check for keyboard input)
	main_camera->update_camera(utils::key_state);
	// Update Light (Check for keyboard input)
	main_light->updateLight(utils::key_state);
	// Options Menu (quit etc)
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
	// Box2D Ground Render
	bGround->render();
	// Box2D Box Render
	bBox->render();
	// Model Render function
	model->render();

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
	glutInitWindowPosition(int(utils::window_pos_x), int(utils::window_pos_y));
	glutInitWindowSize(int(utils::window_width), int(utils::window_height));
	glutCreateWindow("Angry Birds");

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
