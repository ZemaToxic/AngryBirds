#include  "../Header/game.h"
Level level;
Camera* main_camera;

ShaderLoader shader_loader;
Contact contact_listener;
GameState current_state;

b2World* world;
box2D* b_ground;

std::vector<box2D*> wall_obj;
std::vector<box2D*> player_obj;
std::vector<box2D*> obstacle_obj;
std::vector<box2D*> indestructible_obj;

glm::vec2 startPoint = { 0,0 };
glm::vec2 endPoint = { 0,0 };
bool pressed = false;

float scale = 10.0f;

void Game::slingShotStart()
{
	const auto x = Input::GetInstance().GetMousePos().x / scale;
	const auto y = (utils::window_height / scale) - (Input::GetInstance().GetMousePos().y / scale);

	if(startPoint.x <= 0)
	{
		startPoint = { x, y };
		printf("Sling start -> x:%f y:%f \n", startPoint.x, startPoint.y);
	}
	pressed = true;
}

void Game::slingShotEnd()
{
	const auto x = Input::GetInstance().GetMousePos().x / scale;
	const auto y = (utils::window_height / scale ) - (Input::GetInstance().GetMousePos().y / scale);

	if (endPoint.x <= 0)
	{
		float multi = 25.0f;
		glm::vec2 vect = glm::normalize(startPoint - endPoint);

		float distance = glm::distance(startPoint, endPoint);
		float forcef = distance * multi;

		b2Vec2 force = { forcef * vect.x, forcef * vect.y };

		endPoint = { x, y };
		printf("Sling End -> x:%f y:%f \n", endPoint.x, endPoint.y);

		player_obj[0]->get_body()->ApplyLinearImpulse(force, player_obj[0]->get_body()->GetPosition(), true);

		startPoint = { 0,0 };
		endPoint = { 0,0 };
		pressed = false;
	}
}

void Game::createLevel()
{
	// Init Box2D
	const b2Vec2 gravity(0.0f, -98.0f);
	world = new b2World(gravity);
	world->SetContactListener(&contact_listener);
	// Load the shaderLoader for the Light
	const GLuint program = shader_loader.create_program((char*)"Assets/Shaders/texture.vs", (char*)"Assets/Shaders/texture.fs");
	// A fixture is required for box 2d to make physics equations
	b2FixtureDef fixture_def;
	fixture_def.density = 1.0f;
	fixture_def.friction = 1.0f;
	// Bouncyness (Higher = Bouncier)
	fixture_def.restitution = 0.25f;

	// Create bounding boxes
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { (-20) / scale, (utils::window_height / 2) / scale }, { (20) / scale , (utils::window_height) / scale }));
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { (utils::window_width + 20 ) / scale, (utils::window_height / 2) / scale }, { (20) / scale, (utils::window_height) / scale }));
	wall_obj.push_back(new box2D(world, kQuad, wall, fixture_def, false, "Assets/stone.jpg", main_camera, program, { (utils::window_width / 2) / scale, (utils::window_height + 20) / scale }, { (utils::window_width) / scale, (20) / scale }));
	
	// Ground																																			
	b_ground = new box2D(world, kQuad, scenery, fixture_def, false, "Assets/ground.jpg", main_camera, program, { 500 / scale, 25 / scale }, { 500 / scale, 25 / scale });
	
	// Level 1
	if (level.getLevel() == 1) {

		// Obstacles (boards/planks etc)
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {80, 11}, {2, 6  }));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {90, 11}, {2, 6  }));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {85, 19}, {10, 2 }));
		obstacle_obj.push_back(new box2D(world, kQuad, obstacle, fixture_def, true, "Assets/wood.jpg", main_camera, program, {85, 23}, {2, 2  }));
		
		// Enemies (pigs etc)
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 75, 7.5}, { 2.5, 2.5 }));
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 85, 7.5 }, { 2.5,  2.5  }));
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 95, 7.5 }, {2.5, 2.5  }));

		// Bird (should fall and land on ground)	
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb1.jpg", main_camera, program, {25, 7.5 }, {2.5, 2.5}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb2.jpg", main_camera, program, {10, 7.5 }, {2.5, 2.5}));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb3.jpg", main_camera, program, {5, 7.5 }, {2.5,  2.5}));
	}
	// Level 2
	if (level.getLevel() == 2) {

		// Obstacles (boards/planks etc)
		indestructible_obj.push_back(new box2D(world, kQuad, scenery, fixture_def, false, "Assets/stone.jpg", main_camera, program, { 80, 11 }, { 2, 6 }));
		// Enemies (pigs etc)
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 75, 7.5 }, { 2.5, 2.5 }));
		//obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 85, 7.5 }, { 2.5,  2.5 }));
		obstacle_obj.push_back(new box2D(world, kSphere, enemy, fixture_def, true, "Assets/pig.jpg", main_camera, program, { 95, 7.5 }, { 2.5, 2.5 }));

		// Bird (should fall and land on ground)	
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb1.jpg", main_camera, program, { 25, 7.5 }, { 2.5, 2.5 }));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb2.jpg", main_camera, program, { 10, 7.5 }, { 2.5, 2.5 }));
		player_obj.push_back(new box2D(world, kSphere, player, fixture_def, true, "Assets/birb3.jpg", main_camera, program, { 5, 7.5 }, { 2.5,  2.5 }));
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
	switch (current_state)
	{
		// Menu
	case 0:
		{
			
		}
		break;

		// Game
	case 1:
		if (level.getLevel() == 1) {
			// Wall render
			for (auto& i : wall_obj) { i->render(); }
			// Box2D Ground Render
			b_ground->render();
			// Box2D Player Render
			for (auto& i : player_obj) { i->render(); }
			// Obstacles and Enemies
			for (auto& i : obstacle_obj) { i->render(); }
			// Instructables
			for (auto& i : indestructible_obj) { i->render(); }
		}
		if (level.getLevel() == 2) {
			// Box2D Ground Render
			b_ground->render();
			// Box2D Player Render
			for (auto& i : player_obj) { i->render(); }
			// Obstacles and Enemies
			for (auto& i : obstacle_obj) { i->render(); }
			// Instructables
			for (auto& i : indestructible_obj) { i->render(); }
		}
		break;

		// Pause
	case 2:
		{
			
		}
		break;

		// Quit
	case 3:
		{
			
		}

		break;

	default:
		break;
	}

}

void Game::update()
{
	if (Input::GetInstance().GetKeyState('1') == INPUT_HOLD) { current_state = menu; }
	if (Input::GetInstance().GetKeyState('2') == INPUT_HOLD) { current_state = game; }
	if (Input::GetInstance().GetKeyState('3') == INPUT_HOLD) { current_state = quit; }


	// At some point in process you must tell the world when to step, or the timings for physics equations
	const float32 time_step = 1.0f / 60.0f;
	const int32 velocity_iterations = 8;
	const int32 position_iterations = 3;
	world->Step(time_step, velocity_iterations, position_iterations);

	switch (current_state)
	{
		// Menu
	case 0:
	{
		// --- TO DO --- //

		// Start Button

		// Quit Button
	}
	break;
	// Game
	case 1:
	{
		if (!player_obj.empty())
		{

			if (Input::GetInstance().GetMouseState(MOUSE_LEFT) == INPUT_HOLD) { slingShotStart(); }
			if ((Input::GetInstance().GetMouseState(MOUSE_LEFT) == INPUT_RELEASE) && pressed) { slingShotEnd(); }

			for (auto& i : player_obj) { i->process(); }
			for (auto& i : indestructible_obj) { i->process(); }

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
		else
		{
			current_state = gameover;
		}
	}
	break;

	// Gameover
	case 2:
	{
		// --- TO DO --- //

		// Print Game over text

		// Make a back to menu button
	}
	break;

	// Quit
	case 3:
	{
		player_obj.clear();
		wall_obj.clear();
		obstacle_obj.clear();
		indestructible_obj.clear();
		glutLeaveMainLoop();
	}
	break;

	default:
		break;
	}
}
