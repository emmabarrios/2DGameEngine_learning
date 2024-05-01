#include "Game.h"
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>

//#include <chrono>
//#include <thread>


glm::vec2 playerPosition;
glm::vec2 playerVelocity;

Game::Game() {
	isRunning = false;
	std::cout << "Game Constructor Called\n";
}
Game::~Game() {
	std::cout << "Game Destructor Called\n";
}
void Game::Initialize() {
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cerr << "Error initializing SDL.\n";
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
		std::cerr << "Error creating SDL window.\n";
		return;
	}

	renderer = SDL_CreateRenderer(window, -1, 0);

	// renderer == NULL
	if (!renderer) {
		std::cerr << "Error creating SDL renderer.\n";
	}

	SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);

	isRunning = true;
}
void Game::Setup() {
	playerPosition = glm::vec2(10,20);
	playerVelocity = glm::vec2(1,0);
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


	int targetTime = millisecsPreviousFrame + MILLISECS_PER_FRAME;

	// Frame rate control: Delay updating the frame until the elapsed time reaches the target time.
	// This ensures each frame maintains a consistent interval as defined by MILLISECS_PER_FRAME.
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), targetTime)) { 
		/* we wait :) */ 

		// uncle GPT suggested waiting a few milliseconds to yield the processor and not charge it with bussy waiting
		// Reduce CPU usage by sleeping briefly
		//std::this_thread::sleep_for(std::chrono::milliseconds(1));
	};


	// Store the current frame time
	// we need to know how much did the last frame last
	// this function returns the current ticks(milliseconds) that we are running the program
	millisecsPreviousFrame = SDL_GetTicks();

	playerPosition.x += playerVelocity.x;
	playerPosition.y += playerVelocity.y;
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


