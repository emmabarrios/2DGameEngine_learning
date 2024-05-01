#ifndef GAME_H
#define GAME_H

#include<SDL.h>

// how many frames are refreshed in one second
const int FPS = 500;

// how many secods takes a frame to last (or expected)
const int MILLISECS_PER_FRAME = 1000 / FPS;

class Game {
private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool isRunning;
	int millisecsPreviousFrame = 0;

public:
	Game();
	~Game();

	void Initialize();
	void Destroy();
	void Run();

	void ProcessInput();
	void Update();
	void Render();
	void Setup();

	int windowWidth;
	int windowHeight;
}; 
#endif
