#include  "../Header/game.h"
Level level;
Camera* main_camera;
GLuint main_program;
ShaderLoader shader_loader;
Contact contact_listener;
b2World* world;
box2D* b_ground;
std::vector<box2D*> player_obj;
std::vector<box2D*> obstacle_obj;

void Game::getWindowPos()
{
	const auto x = Input::GetInstance().GetMousePos().x;
	const auto y = utils::window_height - Input::GetInstance().GetMousePos().y;
	player_obj[0]->set_pos({x, y});
	printf("Mouse pos x:%f y:%f \n", x, y);
}

void Game::createLevel()
{
	// Init Box2D
	const b2Vec2 gravity(0.0f, -980);
	world = new b2World(gravity);
	world->SetContactListener(&contact_listener);
	// Load the shaderLoader for the Light
	const GLuint program = shader_loader.create_program((char*)"Assets/Shaders/texture.vs", (char*)"Assets/Shaders/texture.fs");

	// Level 1
	if (level.getLevel() == 1) {
		// A fixture is required for box 2d to make physics equations
		b2FixtureDef fixture_def;
		fixture_def.density = 1.0f;
		fixture_def.friction = 0.3f;
		// Bouncyness (Higher = Bouncier)
		fixture_def.restitution = 0.5f;

		// Ground																																			
		b_ground = new box2D(world, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, {500, 25}, {500, 25});
		
		// Obstacles (boards/planks etc)
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {800, 110}, {20, 60}));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {900, 110}, {20, 60}));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {850, 190}, {100, 20}));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {850, 230}, {20, 20}));
		
		// Enemies (pigs etc)
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, {750, 75}, {25, 25}));
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, {850, 75}, {25, 25}));
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, {950, 75}, {25, 25}));

		// Bird (should fall and land on ground)	
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb1.jpg", main_camera, program, {150, 75}, {25, 25}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb2.jpg", main_camera, program, {100, 75}, {25, 25}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb3.jpg", main_camera, program, {50, 75}, {25, 25}));
	}
	// Level 2
	if (level.getLevel() == 2) {
		// A fixture is required for box 2d to make physics equations
		b2FixtureDef fixture_def;
		fixture_def.density = 1.0f;
		fixture_def.friction = 0.3f;
		// Bouncyness (Higher = Bouncier)
		fixture_def.restitution = 0.5f;

		// Ground																																			
		b_ground = new box2D(world, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, {500, 25}, {500, 25});
		// Obstacles (boards/planks etc)
		// Enemies (pigs etc)
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, {750, 75}, {25, 25}));
		
		// Bird (should fall and land on ground)																											
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb1.jpg", main_camera, program, {150, 75}, {25, 25}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb2.jpg", main_camera, program, {100, 75}, {25, 25}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb3.jpg", main_camera, program, {50, 75}, {25, 25}));
	}
}

void Game::init()
{
	// Make a new Camera and initialize it
	main_camera = new Camera();
	main_camera->initialize();
	// Call the level setup function
	createLevel();
}

void Game::render()
{
	if (level.getLevel() == 1) {
		// Box2D Ground Render
		b_ground->render();
		// Box2D Player Render
		for (auto& i : player_obj) { i->render(); }
		// Obstacles and Enemies
		for (auto& i : obstacle_obj) { i->render(); }
	}
	if (level.getLevel() == 2) {
		// Box2D Ground Render
		b_ground->render();
		// Box2D Player Render
		for (auto& i : player_obj) { i->render(); }
		// Obstacles and Enemies
		for (auto& i : obstacle_obj) { i->render(); }
	}
}

void Game::update()
{
	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 120.0f;
	const int32 velocity_iterations = 8;
	const int32 position_iterations = 3;
	world->Step(time_step, velocity_iterations, position_iterations);
	if (level.getLevel() == 1) {
		// Update box2D Ground physics
		b_ground->process();
		// Update box2D Box physics for the birb(s)
		for (auto i = 0; i < static_cast<int>(player_obj.size()); i++) {
			player_obj[i]->process();
			if (!player_obj[i]->get_body()->IsActive()) { player_obj.erase(player_obj.begin() + i); }
		}
		if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD) { getWindowPos(); }

		// Obstacles and Enemies
		for (auto i = 0; i < static_cast<int>(obstacle_obj.size()); i++) {
			obstacle_obj[i]->process();
			if (!obstacle_obj[i]->get_body()->IsActive()) { obstacle_obj.erase(obstacle_obj.begin() + i); }
		}
		if (obstacle_obj.empty()) {
			player_obj.clear();
			level.setLevel(2);
			createLevel();
		}
	}
	if (level.getLevel() == 2) {
		// Update box2D Ground physics
		b_ground->process();
		// Update box2D Box physics for the birb(s)
		for (auto i = 0; i < static_cast<int>(player_obj.size()); i++) {
			player_obj[i]->process();
			if (!player_obj[i]->get_body()->IsActive()) { player_obj.erase(player_obj.begin() + i); }
		}
		if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD) { getWindowPos(); }

		// Obstacles and Enemies
		for (auto i = 0; i < static_cast<int>(obstacle_obj.size()); i++) {
			obstacle_obj[i]->process();
			if (!obstacle_obj[i]->get_body()->IsActive()) { obstacle_obj.erase(obstacle_obj.begin() + i); }
		}
		if (obstacle_obj.empty()) {
			level.setLevel(3);
			createLevel();
		}
	}
}
