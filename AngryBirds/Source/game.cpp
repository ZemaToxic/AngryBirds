#include  "../Header/game.h"

Level level;

Camera* main_camera;
GLuint main_program;
ShaderLoader shader_loader;
Contact contactListener;

b2World* world;

box2D* bGround;
box2D* bBirb;

std::vector<box2D*> playerObj;
std::vector<box2D*> obstacleObj;

void Game::getWindowPos()
{
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

	const auto x = Input::GetInstance().GetMousePos().x;
	const auto y = utils::window_height - Input::GetInstance().GetMousePos().y;

	for (auto& i : playerObj)
	{
		i->set_pos({ x, y });
	}
}

void Game::createLevel()
{
	// Init Box2D
	const b2Vec2 gravity(0.0f, -1500.0f);
	world = new b2World(gravity);
	world->SetContactListener(&contactListener);
	// Load the shaderLoader for the Light
	const GLuint program = shader_loader.create_program((char*)"Assets/Shaders/texture.vs", (char*)"Assets/Shaders/texture.fs");

	// Level 1
	if(level.getLevel() == 1)
	{
		// A fixture is required for box 2d to make physics equations
		b2FixtureDef fixture_def;
		fixture_def.density = 1.0f;
		fixture_def.friction = 0.3f;
		// Bouncyness (Higher = Bouncier)
		fixture_def.restitution = 0.5f;

		// Ground																																			
		bGround = new box2D(world, BOX, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, { 500, 25 }, { 500, 25 });
		// Obstacles (boards/planks etc)
		obstacleObj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, { 800, 110 }, { 20, 60 }));
		obstacleObj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, { 900, 110 }, { 20, 60 }));
		obstacleObj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, { 850, 250 }, { 100, 20 }));
		obstacleObj.push_back(new box2D(world, BOX, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, { 850, 300 }, { 20, 20 }));
		// Enemies (pigs etc)
		obstacleObj.push_back(new box2D(world, CIRCLE, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 0, 0 }, { 25, 25 }));		
	
		// Bird (should fall and land on ground)																											
		playerObj.push_back(new box2D(world, CIRCLE, kSphere, player, fixture_def, true, "Assets/birb1.jpg", main_camera, program, { 100, 100 }, { 25, 25 }));
		fixture_def.restitution = 1.5f;
		playerObj.push_back(new box2D(world, CIRCLE, kSphere, player, fixture_def, true, "Assets/birb2.jpg", main_camera, program, { 100, 100 }, { 25, 25 }));
		fixture_def.restitution = 5.5f;
		playerObj.push_back(new box2D(world, CIRCLE, kSphere, player, fixture_def, true, "Assets/birb3.jpg", main_camera, program, { 100, 100 }, { 25, 25 }));
	}
	// Level 2
	if(level.getLevel() == 2)
	{
		// A fixture is required for box 2d to make physics equations
		b2FixtureDef fixture_def;
		fixture_def.density = 1.0f;
		fixture_def.friction = 0.3f;
		// Bouncyness (Higher = Bouncier)
		fixture_def.restitution = 0.5f;

		// Ground																																			
		bGround = new box2D(world, BOX, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, { 500, 25 }, { 500, 25 });
		// Bird (should fall and land on ground)																											
		playerObj.push_back(new box2D(world, CIRCLE, kSphere, player, fixture_def, true, "Assets/birb.jpg", main_camera, program, { 100, 100 }, { 25, 25 }));
		// Enemies (pigs etc)
		obstacleObj.push_back(new box2D(world, CIRCLE, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 0, 0 }, { 25, 25 }));
	}
}

void Game::init()
{
	// Make a new Camera and initialize it
	main_camera = new Camera();
	main_camera->initialize();

	createLevel();
}

void Game::render()
{
	if (level.getLevel() == 1)
	{
		// Box2D Ground Render
		bGround->render();
		// Box2D Box Render
		for (auto& i : playerObj)
		{
			i->render();
		}
		// Obstacles and Enemies
		for (auto& i : obstacleObj)
		{
			i->render();
		}
	}

	if (level.getLevel() == 2)
	{
		// Box2D Ground Render
		bGround->render();
		// Box2D Box Render
		for (auto& i : playerObj)
		{
			i->render();
		}
		// Obstacles and Enemies
		for (auto& i : obstacleObj)
		{
			i->render();
		}
	}
}

void Game::update()
{
	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 100.0f;
	const int32 velocity_iterations = 8;
	const int32 position_iterations = 3;
	world->Step(time_step, velocity_iterations, position_iterations);

	if (level.getLevel() == 1)
	{
		// Update box2D Ground physics
		bGround->process();
		// Update box2D Box physics for the birb(s)
		for (auto i = 0; i < static_cast<int>(playerObj.size()); i++)
		{
			playerObj[i]->process();
			if (!playerObj[i]->get_body()->IsActive())
			{
				playerObj.erase(playerObj.begin() + i);
			}
		}
		if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD)
		{
			getWindowPos();
		}

		// Obstacles and Enemies
		for (auto i = 0; i < static_cast<int>(obstacleObj.size()); i++)
		{
			obstacleObj[i]->process();
			if (!obstacleObj[i]->get_body()->IsActive())
			{
				obstacleObj.erase(obstacleObj.begin() + i);
			}
		}
		if(obstacleObj.empty())
		{
			level.setLevel(2);
			createLevel();
		}
	}
	if (level.getLevel() == 2)
	{
		// Update box2D Ground physics
		bGround->process();
		// Update box2D Box physics for the birb(s)
		for (auto i = 0; i < static_cast<int>(playerObj.size()); i++)
		{
			playerObj[i]->process();
			if (!playerObj[i]->get_body()->IsActive())
			{
				playerObj.erase(playerObj.begin() + i);
			}
		}

		if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD)
		{
			getWindowPos();
		}

		// Obstacles and Enemies
		for (auto i = 0; i < static_cast<int>(obstacleObj.size()); i++)
		{
			obstacleObj[i]->process();
			if (!obstacleObj[i]->get_body()->IsActive())
			{
				obstacleObj.erase(obstacleObj.begin() + i);
			}
		}
		if (obstacleObj.empty())
		{
			level.setLevel(3);
			createLevel();
		}
	}
}
