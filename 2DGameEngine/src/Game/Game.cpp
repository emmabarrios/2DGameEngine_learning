#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include "../Logger/Logger.h"

//#include <chrono>
//#include <thread>


glm::vec2 playerPosition;
glm::vec2 playerVelocity;

Game::Game() {
	isRunning = false;
	Logger::Log("Game Constructor Called");
	//std::cout << "Game Constructor Called\n";
}
Game::~Game() {
	Logger::Err("Game Destructor Called");
	//std::cout << "Game Destructor Called\n";
}
void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		Logger::Err("Error initializing SDL");
		//std::cerr << "Error initializing SDL.\n";
		return;
	}

	// Declares a variable of type SDL_DisplayMode and its passed as a reference to SDL_GetCurrentDisplayMode so it can be tampered in memory
	SDL_DisplayMode displayMode;
	SDL_GetCurrentDisplayMode(0, &displayMode);

	windowWidth = 800;//displayMode.w;
	windowHeight = 600;//displayMode.h;

	// To open a window without a title, we pass NULL as the first parameter
	// Creates an SDL_CreateWindow object in memory and return it's memory address
	window = SDL_CreateWindow(
		NULL, 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		windowWidth,
		windowHeight,
		SDL_WINDOW_BORDERLESS);
	
	// window == NULL
	if (!window) {
		Logger::Err("Error creating SDL window");
		//std::cerr << "Error creating SDL window.\n";
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	// renderer == NULL
	if (!renderer) {
		Logger::Err("Error creating SDL renderer");
		//std::cerr << "Error creating SDL renderer.\n";
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}
void Game::Setup() {
	playerPosition = glm::vec2(10,20);
	playerVelocity = glm::vec2(50,0.0);
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

	// we could actually remove the frame rate cap logic now since we are using the deltaTime
	//int millisecsCurrentFrame = SDL_GetTicks();

	//int elapsedTime = millisecsCurrentFrame - millisecsPreviousFrame;

	//int timeToWait = MILLISECS_PER_FRAME - elapsedTime;

	//if (timeToWait > 0) {
	//	SDL_Delay(timeToWait);
	//}

	// differente in ticks sonce the last frame, converted to seconds
	double deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0;

	millisecsPreviousFrame = SDL_GetTicks();

	// D = V * T
	playerPosition.x += playerVelocity.x * deltaTime;
	playerPosition.y += playerVelocity.y * deltaTime;
}
void Game::Render() {
	SDL_SetRenderDrawColor(renderer, 21, 21, 21, 255);
	SDL_RenderClear(renderer);


	/*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect player = {10, 10, 20, 20};
	SDL_RenderFillRect(renderer, &player);*/

	//// Loads a png texture
	SDL_Surface* surface = IMG_Load("./assets/images/tank-tiger-right.png");
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_FreeSurface(surface);

	//// place texture in the renderer at a destination location
	SDL_Rect destRect = {
		static_cast<int>(playerPosition.x), 
		static_cast<int>(playerPosition.y),
		32, 
		32};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);
	SDL_DestroyTexture(texture);

	SDL_RenderPresent(renderer);
}


