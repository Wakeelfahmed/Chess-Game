#include<iostream>
#include<Windows.h>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
using namespace std;
SDL_Window* window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640/*width*/, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
void Create_Board() {
	SDL_Rect Box = { 0,0,80,80}; COORD position = { 0 };
	bool toggle = 0;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (toggle)
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
			else
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
			SDL_RenderFillRect(renderer, &Box);
			Box.x += 80;
			if(j != 7)
			toggle = !toggle;
		}
		Box.x = 0;
		Box.y += 80;
	}
}
class Piece {
	string Piece_Name;
	COORD Piece_Position;
	char Peice_Color;
	//dead or alive;
};
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	//int MouseX, MouseY;
	SDL_Event event;
	while (true) {
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT || event.key.keysym.sym == SDLK_ESCAPE) {
				//SDL_DestroyTexture(buttonTextTexture);
				//SDL_FreeSurface(buttonTextSurface);
				SDL_DestroyRenderer(renderer);
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION)	//Mouse is hovering
			{
				//SDL_GetMouseState(&MouseX, &MouseY);
			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				//SDL_GetMouseState(&MouseX, &MouseY);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {	//mouse click on Button
				//SDL_GetMouseState(&MouseX, &MouseY);
			}
		}
		SDL_SetRenderDrawColor(renderer, 130, 214, 240, 50);
		SDL_RenderClear(renderer);
		Create_Board();
		SDL_RenderPresent(renderer);	//Final Output to SDL window

		//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
}