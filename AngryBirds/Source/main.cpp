// Includes
#include "../Header/utils.h"
#include "../Header/camera.h"
#include "../Header/light.h"
#include "../Header/shaderLoader.h"
#include "../Header/box2DObj.h"
#include "../Header/input.h"

GLuint main_program;
ShaderLoader shader_loader;

Camera* main_camera;
Light* main_light;

GameModel* backGround;
b2World* world;

box2D* bGround;
box2D* bBirb;

box2D* bObstacle1;
box2D* bObstacle2;
box2D* bObstacle3;
box2D* bObstacle4;


//Calls to the input class from here down
/********************************************/
inline void KeyDown(unsigned char key, int x, int y)
{
	Input::GetInstance().KeyboardDown(key, x, y);
}

inline void KeyUp(unsigned char key, int x, int y)
{
	Input::GetInstance().KeyboardUp(key, x, y);
}

inline void Mouse(int button, int glutState, int x, int y)
{
	Input::GetInstance().MouseClick(button, glutState, x, y);
}

inline void PassiveMouse(int x, int y)
{
	Input::GetInstance().MouseMove(x, y);
}


void getWindowPos()
{
	float x = Input::GetInstance().GetMousePos().x;
	float y = utils::window_height - Input::GetInstance().GetMousePos().y;

	/*
		Screen	  0, 0			      1000, 0
		World	-65, 30				    65, 30
				 
				 _______________________
				|						|
				|						|
				|						|
				|		   0,0			|
				|						|
				|						|
				|_______________________|
				 
		
		Screen	-65, -30				 65, -30
 		World	  0, 500			   1000, 500

	*/

	printf("Mouse Pos x:%f y:%f\n", x, y);


	bBirb->set_pos({x, y});
}

// Initialise the "Game"
void init()
{
	// Make a new Camera and initialize it
	main_camera = new Camera();
	main_camera->initialize();

	// Load the shaderLoader for the Light
	const GLuint light_program = shader_loader.create_program((char*)"Assets/Shaders/Light.vs", (char*)"Assets/Shaders/Light.fs");
	main_light = new Light(main_camera, utils::ambientStrength, utils::specularStrength, glm::vec3(1.0f, 1.0f, 1.0f));
	main_light->setProgram(light_program);

	// Init Box2D
	const b2Vec2 gravity(0.0f, -200.0f);
	world = new b2World(gravity);

	// A fixture is required for box 2d to make physics equations
	b2FixtureDef fixture_def;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.3f;
	// Bouncyness (Higher = Bouncier)
	fixture_def.restitution = 0.5f;

	// Create a new Cube and pass the light value
	backGround = new GameModel(kQuad, main_camera, "Assets/background.jpg", main_light, utils::ambientStrength, utils::specularStrength);
	backGround->setProgram(light_program);
	backGround->setScale({75.0f, -40.0f, 1.0f});

	// Ground																																							// Pos	   // Size
	bGround = new box2D(world, BOX, kQuad, fixture_def, false, "Assets/ground.jpg", main_camera, main_light, {500,250 }, {75, 10});
	bGround->setProgram(light_program);
	// Bird (should fall and land on ground)																															// Pos	   // Size
	bBirb = new box2D(world, CIRCLE, kSphere, fixture_def, false, "Assets/birb.jpg", main_camera, main_light, { 0, 0 }, { 4, 4 });
	bBirb->setProgram(light_program);

	// Obstacles etc
	bObstacle1 = new box2D(world, BOX, kQuad, fixture_def, true, "Assets/wood.jpg", main_camera, main_light, {35, -15}, {2, 6});
	bObstacle2 = new box2D(world, BOX, kQuad, fixture_def, true, "Assets/wood.jpg", main_camera, main_light, {45, -15}, {2, 6});
	bObstacle3 = new box2D(world, BOX, kQuad, fixture_def, true, "Assets/wood.jpg", main_camera, main_light, {40, -10}, {100, 20});
	bObstacle4 = new box2D(world, BOX, kQuad, fixture_def, true, "Assets/wood.jpg", main_camera, main_light, {40, -8}, {2, 2});

	bObstacle1->setProgram(light_program);
	bObstacle2->setProgram(light_program);
	bObstacle3->setProgram(light_program);
	bObstacle4->setProgram(light_program);
}

// Update called each "frame"
void update()
{
	// Update Camera (Check for keyboard input)
	main_camera->update_camera(utils::key_state);
	// Update Light (Check for keyboard input)
	main_light->updateLight(utils::key_state);

	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 120.0f;
	const int32 velocity_iterations = 6;
	const int32 position_iterations = 2;
	world->Step(time_step, velocity_iterations, position_iterations);

	// Update box2D Ground physics
	bGround->process();
	bGround->update();
	// Update box2D Box physics for the birb
	bBirb->process();
	bBirb->update();

	if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD)
	{
		getWindowPos();
	}

	//printf("Birb pos x:%f y:%f\n", bBirb->get_pos().x, bBirb->get_pos().y);

	// Obstacles
	bObstacle1->process();
	bObstacle2->process();
	bObstacle3->process();
	bObstacle4->process();

	bObstacle1->update();
	bObstacle2->update();
	bObstacle3->update();
	bObstacle4->update();

	glutPostRedisplay(); // Do not move this.
}

// Render... does as it says
void render()
{
	// Set the background colour
	glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Light Render function
	main_light->render();
	// Render the background
	backGround->render();
	// Box2D Ground Render
	bGround->render();
	// Box2D Box Render
	bBirb->render();

	// Obstacles
	bObstacle1->render();
	bObstacle2->render();
	bObstacle3->render();
	bObstacle4->render();

	glutSwapBuffers(); // swap buffers
}

// Main function Loop
int main(int argc, char** argv)
{
	// Init glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(int(utils::window_pos_x), int(utils::window_pos_y));
	glutInitWindowSize(int(utils::window_width), int(utils::window_height));
	glutCreateWindow("Angry Birds - Crystal Seymour");

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
	glutKeyboardFunc(KeyDown); // Keyboard press callback
	glutKeyboardUpFunc(KeyUp); // Keyboard release callback
	glutMouseFunc(Mouse);
	glutPassiveMotionFunc(PassiveMouse);
	glutReshapeFunc(utils::handle_resize); // Window Resize callback
	glutIdleFunc(update); // Update callback
	glutMainLoop(); // Main loop

	return 0;
}