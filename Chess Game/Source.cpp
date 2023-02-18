#include<iostream>
#include<Windows.h>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_image.h"
using namespace std;
SDL_Window* window = SDL_CreateWindow("Chess Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640/*width*/, 640, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
enum class PieceName { Empty, Pawn, Knight, Bishop, Rook, Queen, King };
class Piece;
class Queen;
class Chess_Board_Grid {
public:
	bool Button_Pushed{ 0 };		bool Button_Hovered{ 0 };
	SDL_Rect Chess_Board;           bool Has_A_Piece{ 0 };
	bool has_a_Valid_Move{ 0 };     bool Has_A_kill_Move{ 0 };
	Piece* Chess_Piece{ nullptr };
	bool Check_if_Mouse_in_Button_Area(int x, int y) const { return (x >= Chess_Board.x && x < Chess_Board.x + Chess_Board.h && y >= Chess_Board.y && y < Chess_Board.y + Chess_Board.w); }
};
class Piece {
public:
	SDL_Texture* Piece_Image;
	//SDL_Texture Piece1;
	PieceName Piece_Name;
	COORD Piece_Position;
	char Piece_Color;
	bool dead_or_alive{ 1 };
	virtual void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		cout << "Valid_Moves\n";
	}
	//virtual Piece Promotion() {
	//	Piece* Promoted = new Queen;
	//	Promoted->Piece_Image = this->Piece_Image;
	//	Promoted->Piece_Name = this->Piece_Name;
	//	Promoted->Piece_Position = this->Piece_Position;
	//	Promoted->Piece_Color = this->Piece_Color;		return *Promoted;
	//}
	//Piece(string Piece_Name, char Piece_Color) : Piece_Name(Piece_Name), Piece_Color(Piece_Color) {}
};
class Queen : public Piece
{
public:
	Queen() {
	}
	Queen(const Piece& piece) {
		Piece_Image = piece.Piece_Image;
		Piece_Name = piece.Piece_Name;
		Piece_Position = piece.Piece_Position;
		Piece_Color = piece.Piece_Color;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		cout << "Queen !\n";
	}
};
class King : public Piece
{
};
class Rook : public Piece
{
};
class Bishop : public Piece
{
};
class Knight : public Piece
{
};
class Pawn : public Piece {
public:
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		if (Set_or_Clear_Moves) //Set Moves
			if (this->Piece_Color == 'B') {
				if (Board_Grid[i - 1][j].Has_A_Piece == NULL) {
					Board_Grid[i - 1][j].has_a_Valid_Move = 1;
					if (Board_Grid[i - 2][j].Has_A_Piece == NULL)
						Board_Grid[i - 2][j].has_a_Valid_Move = 1;
				}
				if (Board_Grid[i - 1][j - 1].Has_A_Piece && j != 0)
					if (Board_Grid[i - 1][j - 1].Chess_Piece->Piece_Color == 'W')
						Board_Grid[i - 1][j - 1].Has_A_kill_Move = 1;
				if (Board_Grid[i - 1][j + 1].Has_A_Piece && j != 7)
					if (Board_Grid[i - 1][j + 1].Chess_Piece->Piece_Color == 'W')
						Board_Grid[i - 1][j + 1].Has_A_kill_Move = 1;
			}
		if (!Set_or_Clear_Moves) {// Clear Previosly set Moves
			Board_Grid[i - 1][j].has_a_Valid_Move = 0;
			Board_Grid[i - 2][j].has_a_Valid_Move = 0;
			Board_Grid[i - 1][j - 1].Has_A_kill_Move = 0;
			Board_Grid[i - 1][j + 1].Has_A_kill_Move = 0;
		}
		//this = new
	}
};
class Board {
	Chess_Board_Grid Board_Grid[8][8];
	Piece* White_Pieces[16];
	Piece* Black_Pieces[16];
public:
	void Check_for_Input(int Mouse_X, int Mouse_Y) {
		static int Temp_i = 0, Temp_j = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				if ((Board_Grid[i][j].has_a_Valid_Move || Board_Grid[i][j].Has_A_kill_Move) && Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y)) {
					if (Board_Grid[i][j].Has_A_kill_Move)
						Board_Grid[i][j].Chess_Piece->dead_or_alive = 0;
					if (i == 0)
					{
						Piece* Temp = Board_Grid[Temp_i][Temp_j].Chess_Piece;
						for (int k = 0; k < 16; k++)
						{
							if (Black_Pieces[k] == Temp) {
								delete Black_Pieces[k];
								Black_Pieces[k] = new Queen;
								Black_Pieces[k]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenB.png");
								Black_Pieces[k]->Piece_Name = PieceName::Queen;
								Black_Pieces[k]->Piece_Position = Temp->Piece_Position;
								Black_Pieces[k]->Piece_Color = Temp->Piece_Color;
								//break;
								Board_Grid[Temp_i][Temp_j].Chess_Piece = Black_Pieces[k];
							}
						}
						//Board_Grid[i][j].Chess_Piece = Temp;
						//Board_Grid[i][j].Chess_Piece = ;
					}
					Board_Grid[Temp_i][Temp_j].Chess_Piece->Piece_Position = { short(j * 80), short(i * 80) };
					Board_Grid[i][j].Chess_Piece = Board_Grid[Temp_i][Temp_j].Chess_Piece;
					Board_Grid[Temp_i][Temp_j].Chess_Piece = nullptr;
					Clear_All_Moves();
					//Board_Grid[i][j].Chess_Piece->Valid_Moves(Board_Grid, i, j, Board_Grid[i][j].Button_Pushed);
					//Temp = nullptr;
					Board_Grid[Temp_i][Temp_j].Has_A_Piece = 0;
					Board_Grid[Temp_i][Temp_j].Button_Pushed = 0;
					Board_Grid[i][j].Has_A_Piece = 1;
					Board_Grid[i][j].Button_Pushed = 0;

				}
				else if (Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y) && Board_Grid[i][j].Has_A_Piece)

					if (Board_Grid[i][j].Button_Pushed || !Check_if_any_Piece_Selected()) {
						Board_Grid[i][j].Button_Pushed = !(Board_Grid[i][j].Button_Pushed);
						//if (Board_Grid[i][j].Button_Pushed)
						Board_Grid[i][j].Chess_Piece->Valid_Moves(Board_Grid, i, j, Board_Grid[i][j].Button_Pushed);
						if (Board_Grid[i][j].Button_Pushed) {
							//Temp = Board_Grid[i][j].Chess_Piece;
							//Board_Grid[i][j].Chess_Piece = nullptr;
							//Board_Grid[i][j].Has_A_Piece = 0;
							Temp_i = i;
							Temp_j = j;
						}
						else {
							//Board_Grid[i][j].Chess_Piece = Temp;
							//Temp = nullptr;
						}
					}
			}
	}
	void Create_Board() {
		SDL_Rect Box = { 0,0,80,80 }; COORD position = { 0 };
		bool toggle = 0;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Board_Grid[i][j].Chess_Board = { Box.x, Box.y, 80, 80 };
				if (toggle) {
					if (Board_Grid[i][j].has_a_Valid_Move) {
						SDL_SetRenderDrawColor(renderer, 239, 239, 32, 255);
					}
					else if (Board_Grid[i][j].Has_A_kill_Move)
						SDL_SetRenderDrawColor(renderer, 226, 30, 30, 255);
					else if (Board_Grid[i][j].Button_Pushed)
						SDL_SetRenderDrawColor(renderer, 48, 222, 42, 255);
					else if (Board_Grid[i][j].Button_Hovered)
						SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
					else
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				}
				else {
					if (Board_Grid[i][j].has_a_Valid_Move) {
						SDL_SetRenderDrawColor(renderer, 239, 239, 32, 255);
					}
					else if (Board_Grid[i][j].Has_A_kill_Move)
						SDL_SetRenderDrawColor(renderer, 226, 30, 30, 255);
					else if (Board_Grid[i][j].Button_Pushed)
						SDL_SetRenderDrawColor(renderer, 48, 222, 42, 255);
					else if (Board_Grid[i][j].Button_Hovered)
						SDL_SetRenderDrawColor(renderer, 216, 216, 216, 255);
					else
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				SDL_RenderFillRect(renderer, &Box);
				Box.x += 80;
				if (j != 7)
					toggle = !toggle;
			}
			Box.x = 0;
			Box.y += 80;
		}
	}
	void load_Pawn() {
		for (int i = 8; i < 16; i++)
		{
			White_Pieces[i] = new Pawn;
			Black_Pieces[i] = new Pawn;
			White_Pieces[i]->Piece_Color = 'W';
			Black_Pieces[i]->Piece_Color = 'B';
			White_Pieces[i]->dead_or_alive = 1;
			Black_Pieces[i]->dead_or_alive = 1;
			White_Pieces[i]->Piece_Name = PieceName::Pawn;
			Black_Pieces[i]->Piece_Name = PieceName::Pawn;

			White_Pieces[i]->Piece_Position.X = Board_Grid[1][i - 8].Chess_Board.x;
			White_Pieces[i]->Piece_Position.Y = Board_Grid[1][i - 8].Chess_Board.y;
			White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/PawnW.png");
			Board_Grid[1][i - 8].Has_A_Piece = 1;
			Board_Grid[1][i - 8].Chess_Piece = White_Pieces[i];

			//SDL_RenderCopy(renderer, White_Pieces[i].Piece, NULL, &Board_Grid[1][i - 8].Chess_Board);
			Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/PawnB.png");
			//SDL_RenderCopy(renderer, Black_Pieces[i].Piece, NULL, &Board_Grid[6][i - 8].Chess_Board);
			Black_Pieces[i]->Piece_Position.X = Board_Grid[6][i - 8].Chess_Board.x;
			Black_Pieces[i]->Piece_Position.Y = Board_Grid[6][i - 8].Chess_Board.y;
			Board_Grid[6][i - 8].Has_A_Piece = 1;
			Board_Grid[6][i - 8].Chess_Piece = Black_Pieces[i];

			//SDL_DestroyTexture(White_Pieces[i].Piece);
			//SDL_DestroyTexture(Blac/k_Pieces[i].Piece);
		}
	}
	bool Check_if_any_Piece_Selected() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (Board_Grid[i][j].Button_Pushed)
					return 1;
		return 0;
	}
	void load_Board() {
		load_Pawn();
		for (int i = 0; i < 8; i++)
		{
			if (i == 0 || i == 7) {
				White_Pieces[i] = new Rook;
				Black_Pieces[i] = new Rook;
				White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::Rook;
				Black_Pieces[i]->Piece_Name = PieceName::Rook;

				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/RookW.png");
				Board_Grid[0][i].Has_A_Piece = 1;
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/RookB.png");
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Has_A_Piece = 1;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
				//SDL_DestroyTexture(Black_Pieces[i].Piece);
				//SDL_DestroyTexture(White_Pieces[i].Piece);
			}
			else if (i == 1 || i == 6) {
				White_Pieces[i] = new Knight;
				Black_Pieces[i] = new Knight;
				White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::Knight;
				Black_Pieces[i]->Piece_Name = PieceName::Knight;
				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KnightW.png");
				Board_Grid[0][i].Has_A_Piece = 1;
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];


				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KnightB.png");
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Has_A_Piece = 1;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
			}
			else if (i == 2 || i == 5) {
				White_Pieces[i] = new Bishop;
				Black_Pieces[i] = new Bishop;
				White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::Bishop;
				Black_Pieces[i]->Piece_Name = PieceName::Bishop;
				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/BishopW.png");
				Board_Grid[0][i].Has_A_Piece = 1;
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/BishopB.png");
				Board_Grid[7][i].Has_A_Piece = 1;
				Board_Grid[7][i].Chess_Piece = White_Pieces[i];

				//SDL_DestroyTexture(Black_Pieces[i].Piece);
				//SDL_DestroyTexture(White_Pieces[i].Piece);
			}
			else if (i == 3) {
				White_Pieces[i] = new Queen;	White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i] = new Queen;	Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::Queen; Black_Pieces[i]->Piece_Name = PieceName::Queen;

				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenW.png");
				Board_Grid[0][i].Has_A_Piece = 1;
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenB.png");
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Has_A_Piece = 1;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];

				//SDL_DestroyTexture(Black_Pieces[i].Piece);
				//SDL_DestroyTexture(White_Pieces[i].Piece);
			}
			else if (i == 4) {
				White_Pieces[i] = new King;				White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i] = new King;				Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::King;
				Black_Pieces[i]->Piece_Name = PieceName::King;
				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KingW.png");
				Board_Grid[0][i].Has_A_Piece = 1;
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KingB.png");
				//SDL_RenderCopy(renderer, Black_Pieces[i].Piece, NULL, &Board_Grid[7][i].Chess_Board);
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Has_A_Piece = 1;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
				//SDL_DestroyTexture(Black_Pieces[i].Piece);
				//SDL_DestroyTexture(White_Pieces[i].Piece);
			}
		}
	}
	void Check_for_Hovering(int Mouse_X, int Mouse_Y) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (Board_Grid[i][j].Has_A_Piece)
					Board_Grid[i][j].Button_Hovered = Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y);
	}
	void Clear_All_Moves() {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				Board_Grid[i][j].has_a_Valid_Move = 0;
				Board_Grid[i][j].Has_A_kill_Move = 0;
			}
	}
	void display_Chess_Pieces() {
		SDL_Rect Temp = { 0 };
		for (int i = 0; i < 16; i++)
		{
			Temp = { White_Pieces[i]->Piece_Position.X, White_Pieces[i]->Piece_Position.Y, 80, 80 };
			if (White_Pieces[i]->dead_or_alive)
				SDL_RenderCopy(renderer, White_Pieces[i]->Piece_Image, NULL, &Temp);
			Temp = { Black_Pieces[i]->Piece_Position.X, Black_Pieces[i]->Piece_Position.Y, 80, 80 };
			if (Black_Pieces[i]->dead_or_alive)
				SDL_RenderCopy(renderer, Black_Pieces[i]->Piece_Image, NULL, &Temp);
		}
	}
};
int main(int argc, char* argv[]) {
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	int MouseX, MouseY;
	SDL_Event event;
	Board Chess;
	Chess.Create_Board();
	Chess.load_Board();
	Chess.load_Pawn();
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
				SDL_GetMouseState(&MouseX, &MouseY);
				Chess.Check_for_Hovering(MouseX, MouseY);
				//Chess;
			}
			if (event.type == SDL_MOUSEBUTTONUP)	//mouse click on Button
			{
				SDL_GetMouseState(&MouseX, &MouseY);
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {	//mouse click on Button
				SDL_GetMouseState(&MouseX, &MouseY);
				Chess.Check_for_Input(MouseX, MouseY);


			}
		}
		SDL_SetRenderDrawColor(renderer, 130, 214, 240, 50);
		SDL_RenderClear(renderer);
		Chess.Create_Board();
		Chess.display_Chess_Pieces();
		SDL_RenderPresent(renderer);	//Final Output to SDL window

		//SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	}
}