// Includes
#include "../Header/utils.h"
#include "../Header/camera.h"
#include "../Header/shaderLoader.h"
#include "../Header/box2DObj.h"
#include "../Header/input.h"
#include "../Header/contact.h"

GLuint main_program;
ShaderLoader shader_loader;
Camera* main_camera;
Contact contactListener;

GameModel* backGround;
b2World* world;

box2D* bGround;
box2D* bBirb;

std::vector<box2D*> box2DOobj;

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
		World	  0, 0			      1000, 0				 
				 _______________________
				|						|
				|						|
				|						|
				|						|
				|						|
				|						|
				|_______________________|
				 
 		Screen	  0, 500			   1000, 500
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
	const GLuint program = shader_loader.create_program((char*)"Assets/Shaders/texture.vs", (char*)"Assets/Shaders/texture.fs");

	// Init Box2D
	const b2Vec2 gravity(0.0f, -1500.0f);
	world = new b2World(gravity);
	world->SetContactListener(&contactListener);

	// A fixture is required for box 2d to make physics equations
	b2FixtureDef fixture_def;
	fixture_def.density = 1.0f;
	fixture_def.friction = 0.3f;
	// Bouncyness (Higher = Bouncier)
	fixture_def.restitution = 0.5f;

	// Create a new Cube and pass the light value
	backGround = new GameModel(kQuad, main_camera, "Assets/background.jpg");
	backGround->setProgram(program);
	backGround->setPosition({500,250,0});
	backGround->setScale({500, -250, 1.0f});

	// Ground																																			
	bGround = new box2D(world, BOX, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, {500, 25}, {500, 25});
	// Bird (should fall and land on ground)																											
	bBirb = new box2D(world, CIRCLE, kSphere, player, fixture_def, true, "Assets/birb.jpg", main_camera, program, { 0, 0 }, { 25, 25});

	// Obstacles etc
	box2DOobj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {800, 110}, {20, 60}));
	box2DOobj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {900, 110}, {20, 60}));
	box2DOobj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {850, 250}, {100, 20}));
	box2DOobj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {850, 300}, {20, 20}));
}

// Update called each "frame"
void update()
{
	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 100.0f;
	const int32 velocity_iterations = 8;
	const int32 position_iterations = 3;
	world->Step(time_step, velocity_iterations, position_iterations);

	// Update box2D Ground physics
	bGround->process();
	// Update box2D Box physics for the birb
	bBirb->process();
	//bBirb->check_collision();

	if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD)
	{
		getWindowPos();
		printf("Birb pos x:%f y:%f\n", bBirb->get_pos().x, bBirb->get_pos().y);
	}

	// Obstacles
	for (auto& i : box2DOobj)
	{
		if(!i->get_body()->IsActive())
		{
			delete i;
		}
		i->process();
	}

	glutPostRedisplay(); // Do not move this.
}

// Render... does as it says
void render()
{
	// Set the background colour
	glClearColor(0.0f, 0.0, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Render the background
	backGround->render();
	// Box2D Ground Render
	bGround->render();
	// Box2D Box Render
	bBirb->render();

	// Obstacles
	for (auto& i : box2DOobj)
	{
		i->render();
	}

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