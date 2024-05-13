#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"
#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/RigidBodyComponent.h"
#include "../Systems/MovementSystem.h"

glm::vec2 playerPosition;
glm::vec2 playerVelocity;

Game::Game() {
	isRunning = false;
	registry = std::make_unique<Registry>();
	Logger::Log("Game Constructor Called");
}
Game::~Game() {
	Logger::Err("Game Destructor Called");
}
void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL");
		return;
	}

	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;//displayMode.w;
	windowHeight = 600;//displayMode.h;

	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS);
	
	if (!window) {
		Logger::Err("Error creating SDL window");
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		Logger::Err("Error creating SDL renderer");
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}
void Game::Setup() {

	// Add the systems that need to be processed in our game
	registry->AddSystem<MovementSystem>();

	Entity tank = registry->CreateEntity();

	tank.AddComponent<TransformComponent>(glm::vec2(10.0, 30.0), glm::vec2(1.0, 1.0), 0.0);
	tank.AddComponent<RigidBodyComponent>(glm::vec2(10.0, 50.0));

	// Remove a component from the entity
	tank.RemoveComponent<TransformComponent>();
}
void Game::Destroy() {
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
void Game::Run() {
	Setup();
	while (isRunning) {
		ProcessInput();
		Update();
		Render();
	}
}
void Game::ProcessInput() {
	SDL_Event sdlEvent;
	while (SDL_PollEvent(&sdlEvent)) {
		switch (sdlEvent.type) {
			case SDL_QUIT:
				isRunning = false;
				break;
			case SDL_KEYDOWN:
				if (sdlEvent.key.keysym.sym == SDLK_ESCAPE) {
					isRunning = false;
				}
				break;
		}
	}
}
void Game::Update() {

	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;
	millisecsPreviousFrame = SDL_GetTicks();

	// Ask all the systems to update
	registry->GetSystem<MovementSystem>().Update();
}
void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);



	SDL_RenderPresent(renderer);
}


