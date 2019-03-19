/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	main.cpp
Description	:	Point of program entry
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

#include "GameScene.h"
#include <iostream>



bool* KeyStates = new bool[256];



void keyboard(unsigned char key, int x, int y){
	KeyStates[key] = BUTTON_DOWN;
}

void keyboard_up(unsigned char key, int x, int y){
	KeyStates[key] = BUTTON_UP;
}

void renderscene()
{
	GameScene::GetInstance().RenderScene();
}

void update()
{
	GameScene::GetInstance().Update(KeyStates);

	glutPostRedisplay();
}


int main(int argc, char **argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(250, 250);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Angry Birds");

	glewInit();

	if (glewIsSupported("GL_VERSION_4_5"))
	{
		std::cout << "GLEW version is 4.5\n";
	}
	else{
		std::cout << "GLEW 4.5 is not supported\n";
	}
	
	GameScene& rGame = GameScene::GetInstance();

	GameScene::GetInstance().Init();

	glEnable(GL_DEPTH_TEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //clear

	// register callbacks
	glutKeyboardFunc(keyboard);
	glutKeyboardUpFunc(keyboard_up);

	glutDisplayFunc(renderscene);
	glutIdleFunc(update);
	glutMainLoop();

	GameScene::DestroyInstance();

	return 0;
}