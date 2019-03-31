// Includes
#include "../Header/game.h"

//Calls to the input class from here down
/********************************************/
inline void KeyDown(unsigned char key, int x, int y) { Input::GetInstance().KeyboardDown(key, x, y); }

inline void KeyUp(unsigned char key, int x, int y) { Input::GetInstance().KeyboardUp(key, x, y); }

inline void Mouse(int button, int glutState, int x, int y) { Input::GetInstance().MouseClick(button, glutState, x, y); }

inline void PassiveMouse(int x, int y) { Input::GetInstance().MouseMove(x, y); }

// Initialise the "Game"
void init() { Game::init(); }

// Update called each "frame"
void update()
{
	Game::update();
	glutPostRedisplay(); // Do not move this.
}

// Render... does as it says
void render()
{
	// Set the background colour
	glClearColor(0.54f, 0.81f, 0.94f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Game::render();
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
	glutMouseFunc(Mouse); // Mouse click callback
	glutPassiveMotionFunc(PassiveMouse); // Mouse location callback
	glutMotionFunc(PassiveMouse); // Mouse movement callback 
	glutReshapeFunc(utils::handle_resize); // Window Resize callback
	glutIdleFunc(update); // Update callback
	glutMainLoop(); // Main loop
	return 0;
}/* TO DO *//*
 *	Create multiple birbs with different 
 *		features
 *	Add industructable objects
 *	Make a second level
 *	Increase Pigs to 3
 *
 */
