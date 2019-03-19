/*
Bachelor of Software Engineering
Media Design School
Auckland
NewZealand

(c) 2005 - 2016 Media Design School

File Name	:	GameScene.cpp
Description	:	Calls to all objects within the game
Author		:	Matthew Seymour
mail		:	matthew.sey6443@mediadesign.school.nz
*/

#include "GameScene.h"

//Library includes
#include <ctime>

#define SPAWN_GROUND addRect(WIDTH / 2, HEIGHT - 550, WIDTH, 30, false);

// Static Variables
GameScene* GameScene::s_pGame = 0;

b2World* world;

int TESTBS;
int limit;

b2Body* addRect(int x, int y, int w, int h, bool dyn)
{
	b2BodyDef bodydef;

	float xPos = (x * P2M);
	float yPos = (y * P2M);
	//bodydef.position.Set(xPos, yPos);

	if (dyn == true)
	{
		bodydef.type = b2_dynamicBody;
	}
	b2Body* body = world->CreateBody(&bodydef);
	//	body->GetPosition();
	float wSize = (P2M * w);
	float ySize = (P2M * h);

	b2PolygonShape shape;
	shape.SetAsBox(wSize / 2, ySize / 2/*, b2Vec2(xPos, yPos), 0*/);

	//shape.SetAsBox(wSize / 2, ySize / 2);

	b2FixtureDef fixturedefRect;
	fixturedefRect.shape = &shape;
	fixturedefRect.density = 10.0;
	body->CreateFixture(&fixturedefRect);
	
	bodydef.position.x = xPos;
	bodydef.position.y = yPos;
	
	return body;
}

b2Body* addCircle(int x, int y, int r, bool dyn)
{
	b2BodyDef bodydef;

	float xPos = (x * P2M);
	float yPos = (y * P2M);

	if (dyn)
	{
		bodydef.type = b2_dynamicBody;
		bodydef.awake = true;
	}
	b2Body* body = world->CreateBody(&bodydef);

	b2CircleShape circleShape;

	
	circleShape.m_p.Set(xPos, yPos); 
	circleShape.m_radius = r; //radius

	b2FixtureDef fixturedefCirc;
	fixturedefCirc.shape = &circleShape;
	body->CreateFixture(&fixturedefCirc);

	return body;
}

GameScene::GameScene()
{
	// ----- Init Physics Yo
	world = new b2World(b2Vec2(0.0f, -9.81f));
	world->SetAllowSleeping(false);
	SPAWN_GROUND;
}

GameScene::~GameScene()
{

}


void GameScene::drawSquare(b2Vec2* points, b2Vec2 center, float angle)
{
	glColor3f(1, 1, 1);
	glPushMatrix();
	glTranslatef((center.x * M2P) , (center.y * M2P), 0);
	glRotatef(angle*180.0 / M_PI, 0, 0, 1);
	glBegin(GL_QUADS);
	for (int i = 0; i < 4; i++)
	{
	//	addRect(WIDTH, HEIGHT, 300, 30, false); // <- Add ground i think?
	//	std::cout << points[i].x << " POINTS X,Y "; std::cout << points[i].y << std::endl;
		glVertex2f((points[i].x * M2P), (points[i].y * M2P));
	//	std::cout << (points[i].x * M2P) << " WITH M2P "; std::cout << (points[i].y * M2P) << std::endl;
	}	
	glEnd();
	glPopMatrix();
}

void drawSquarev2()
{
	glColor3f(1, 0, 0);
	glPushMatrix();

	glBegin(GL_QUADS);

		glVertex2f(300, 300);
		glVertex2f(300, 100);
		glVertex2f(600, 100);
		glVertex2f(600, 300);

	glEnd();
	glPopMatrix;
}
//Calls to the render function for each of the models
void GameScene::RenderScene()
{
	if (clock() - m_dLastTick > 1.0f / 60.0f)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);		//clear

		b2Body* temp = world->GetBodyList();
		b2Vec2 points[4];
		while (temp)
		{
			for (int i = 0; i < 4; i++)
			{
				points[i] = ((b2PolygonShape*)temp->GetFixtureList()->GetShape())->GetVertex(i);
			}
			drawSquare(points, temp->GetLocalCenter(), temp->GetAngle());
			//drawSquarev2();
			temp = temp->GetNext();
		}
		
		glutSwapBuffers();
		
		if (TESTBS > 120)
		{
			TESTBS = 0;
			limit = 0;
		}
		TESTBS++;
	}
}

//Takes in key press and updates the corresponding shapes.
/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
void GameScene::Update(bool*KeyStates)
{
	if (clock() - m_dLastTick > 1.0f / 60.0f)
	{
		m_dDeltaTime = (clock() - m_dLastTick);
	
		_Camera->Update(KeyStates);
	
		if (KeyStates[(unsigned char)'x'] == BUTTON_DOWN)
		{
			if (limit == 0)
			{
				addRect(200, 200, 50, 50, true);
//				addCircle(100, 100, 2, true);
				std::cout << "I DID A THING" << std::endl;
				limit += 1;
			}
		}

		m_dLastTick = clock();
		world->Step((1.0f / 60.0f), 5, 5);
	}
}

//Creates the shapes objects and adds them to a vector
/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
void GameScene::Init()
{
	_Camera = new Camera();

	ShaderLoader shaderLoader;
	program = shaderLoader.CreateProgram("VAO_Triangle.vs", "VAO_Triangle.fs");

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_FILL - Fills colour, GL_LINE - Wireframe

	
	limit = 0;
	m_dLastTick = clock();
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
GameScene& GameScene::GetInstance()
{
	if (s_pGame == 0)
	{
		s_pGame = new GameScene();
	}
	return (*s_pGame);
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
Camera* GameScene::GetCam()
{
	return _Camera;
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
void GameScene::DestroyInstance()
{
	delete s_pGame;
	s_pGame = 0;
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
bool GameScene::InitFmod(){

	return true;

}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
const bool GameScene::LoadAudio(){

	return true;

}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
double GameScene::GetDeltaTime()
{
	return m_dDeltaTime;
}

/*
**Description:
**Author:		Matthew Seymour
**Parameters:
**Return:
*/
void GameScene::SetTextLabel(char* _text)
{
}
