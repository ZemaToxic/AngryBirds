#include  "../Header/game.h"
Level level;
Camera* main_camera;

ShaderLoader shader_loader;
Contact contact_listener;

b2World* world;
box2D* b_ground;

std::vector<box2D*> wall_obj;
std::vector<box2D*> player_obj;
std::vector<box2D*> obstacle_obj;

b2Vec2 startPoint = { 0,0 };
b2Vec2 endPoint = { 0,0 };
bool pressed = false;

void Game::slingShotStart()
{
	const auto x = Input::GetInstance().GetMousePos().x;
	const auto y = utils::window_height - Input::GetInstance().GetMousePos().y;

	//player_obj[0]->set_pos({ x, y });

	if(startPoint.x <= 0)
	{
		startPoint = { x, y };
		printf("Sling start -> x:%f y:%f \n", startPoint.x, startPoint.y);
		//player_obj[0]->get_body()->ApplyLinearImpulseToCenter({ 50000000,50000000 }, true);
	}
	pressed = true;
}

void Game::slingShotEnd()
{
	const auto x = Input::GetInstance().GetMousePos().x;
	const auto y = utils::window_height - Input::GetInstance().GetMousePos().y;

	//player_obj[0]->set_pos({ x, y });

	if (endPoint.x <= 0)
	{
		endPoint = { x, y };
		printf("Sling End -> x:%f y:%f \n", endPoint.x, endPoint.y);
		player_obj[0]->get_body()->ApplyLinearImpulseToCenter({ 50000000,50000000 }, true);
	}
}

void Game::createLevel()
{
	// Init Box2D
	const b2Vec2 gravity(0.0f, -980);
	world = new b2World(gravity);
	world->SetContactListener(&contact_listener);
	// Load the shaderLoader for the Light
	const GLuint program = shader_loader.create_program((char*)"Assets/Shaders/texture.vs", (char*)"Assets/Shaders/texture.fs");
	// A fixture is required for box 2d to make physics equations
	b2FixtureDef fixture_def;
	fixture_def.density = 0.02f;
	fixture_def.friction = 0.3f;
	// Bouncyness (Higher = Bouncier)
	fixture_def.restitution = 0.5f;

	// Create bounding boxes
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { -20, utils::window_height / 2 }, { 20, utils::window_height }));
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { utils::window_width + 20, utils::window_height / 2 }, { 20, utils::window_height }));
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { utils::window_width / 2, utils::window_height + 20 }, { utils::window_width, 20 }));
	
	// Ground																																			
	b_ground = new box2D(world, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, { 500, 25 }, { 500, 25 });
	
	// Level 1
	if (level.getLevel() == 1) {

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

		// Obstacles (boards/planks etc)
		obstacle_obj.push_back(new box2D(world, kQuad, scenery, fixture_def, true, "Assets/stone.jpg", main_camera, program, { 800, 110 }, { 20, 60 }));		
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
		// Wall render
		for (auto& i : wall_obj) { i->render(); }
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
	if (Input::GetInstance().GetMouseState(MOUSE_LEFT) == INPUT_HOLD) { slingShotStart(); }
	if ((Input::GetInstance().GetMouseState(MOUSE_LEFT) == INPUT_RELEASE) && pressed == true) { slingShotEnd(); }
	
	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 120.0f;
	const int32 velocity_iterations = 8;
	const int32 position_iterations = 3;
	world->Step(time_step, velocity_iterations, position_iterations);

	for (auto& i : player_obj) { i->process(); }

	if (level.getLevel() == 1) {
		// Update box2D Ground physics
		b_ground->process();
		// Update box2D Box physics for the birb(s)
		for (auto i = 0; i < static_cast<int>(player_obj.size()); i++) {
			player_obj[i]->process();
			if (!player_obj[i]->get_body()->IsActive()) { player_obj.erase(player_obj.begin() + i); }
		}

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
		if (Input::GetInstance().GetMouseState(0) == INPUT_HOLD) { slingShotStart(); }

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
	main_camera->update_camera();
}
