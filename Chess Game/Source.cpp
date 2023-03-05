#include<iostream>
#include<Windows.h>
#include"C:\SDL2-devel-2.26.1-VC\include\SDL.h"
#include"C:\SDL2-devel-2.26.1-VC\include\SDL_image.h"
#include"c:\SDL2-devel-2.26.1-VC\include\SDL2_gfxPrimitives.h"
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
	//bool Set_Tile_For_Kill() //Also Checks for Check_Mate
	//{
	//	if (Chess_Piece->Piece_Name == PieceName::King)
	//		Has_A_kill_Move = 1;
	//	else
	//		return 1;
	//}
};
class Piece {
public:
	SDL_Texture* Piece_Image{ nullptr };
	PieceName Piece_Name{};
	COORD Piece_Position{};
	char Piece_Color{};
	bool dead_or_alive{ 1 };
	virtual void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {}
	virtual bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int x, int y) { return 0; }
	virtual void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves, bool King_in_check) {}
	bool Check_for_Check(Chess_Board_Grid Board_Grid[8][8], char Piece_Color) {
		COORD King_Pos;
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++) {
				if (Board_Grid[x][y].Chess_Piece)
					if (Board_Grid[x][y].Chess_Piece && (Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::King)
						&& Board_Grid[x][y].Chess_Piece->Piece_Color == Piece_Color)
						King_Pos = { short(x), short(y) };
			}
		bool Possible_Attack = 0;
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++)
				if (Board_Grid[x][y].Chess_Piece && Board_Grid[x][y].Chess_Piece->Piece_Color != this->Piece_Color) {
					//Possible_Attack = check_Valid_Move(Board_Grid, x, y, King_Pos.X, King_Pos.Y);
					Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, King_Pos.X, King_Pos.Y);
					if (Possible_Attack)
						cout << "";
					if (Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::Queen)
						cout << "";
					Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, King_Pos.X, King_Pos.Y);
					if (Possible_Attack)
						return Possible_Attack;
				}
		return Possible_Attack;
	}
};
class Rook : public Piece
{
public:
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int X_to_check, int Y_to_check) {
		for (int k = j + 1; k < 8; k++)//right
		{
			if (i == X_to_check && k == Y_to_check)
				return 1;
			else if (Board_Grid[i][k].Chess_Piece) {
				break;
			}
		}
		for (int k = j - 1; k >= 0; k--)//left
		{
			if (Board_Grid[i][k].Has_A_Piece == NULL)
			{
				if (i == X_to_check && k == Y_to_check)
					return 1;
			}
			else if (Board_Grid[k][j].Has_A_Piece) {
				break;
			}
		}
		for (int k = i - 1; k >= 0; k--) //upWards
		{
			if (Board_Grid[k][j].Has_A_Piece == NULL)
			{
				if (k == X_to_check && j == Y_to_check)
					return 1;
			}
			else if (Board_Grid[k][j].Has_A_Piece) {
				break;
			}
		}
		/*i is for vertical
		j is for horizontal*/
		for (int k = i + 1; k < 8; k++)  //Down Wards
		{
			if (k == X_to_check && j == Y_to_check)
				return 1;
			else if (Board_Grid[k][j].Has_A_Piece) {
				break;
			}
		}
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		if (!Set_or_Clear_Moves) {// Clear Previosly set Moves
			for (int k = 0; k < 8; k++)
			{
				Board_Grid[i][k].has_a_Valid_Move = 0;
				Board_Grid[i][k].Has_A_kill_Move = 0;
				Board_Grid[k][j].has_a_Valid_Move = 0;
				Board_Grid[k][j].Has_A_kill_Move = 0;
				return;
			}
		}

		for (int k = j + 1; k < 8; k++)//right
		{
			if (Board_Grid[i][k].Has_A_Piece == NULL) {
				Board_Grid[i][k].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[i][k].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[i][k].Has_A_kill_Move = 1;
				break;
			}
		}
		for (int k = j - 1; k >= 0; k--)//left
		{
			if (Board_Grid[i][k].Has_A_Piece == NULL) {
				Board_Grid[i][k].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[i][k].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[i][k].Has_A_kill_Move = 1;
				break;
			}
		}
		for (int k = i - 1; k >= 0; k--) //upWards
		{
			if (Board_Grid[k][j].Has_A_Piece == NULL) {
				Board_Grid[k][j].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[k][j].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[k][j].Has_A_kill_Move = 1;
				break;
			}
		}
		/*i is for vertical
		j is for horizontal*/
		for (int k = i + 1; k < 8; k++)  //Down Wards
		{
			if (Board_Grid[k][j].Has_A_Piece == NULL) {
				Board_Grid[k][j].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[k][j].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[k][j].Has_A_kill_Move = 1;
				break;
			}
		}
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves, bool King_in_check) {
		bool Possble_Attack_to_king;
		Rook* Temp = this;
		for (int k = j + 1; k < 8; k++)//right
		{
			if (Board_Grid[i][k].Has_A_Piece == NULL) {
				Board_Grid[i][k].Has_A_Piece = 1;
				Board_Grid[i][k].Chess_Piece = Temp;

				Board_Grid[i][j].Has_A_Piece = 0;
				Board_Grid[i][j].Chess_Piece = NULL;
				Possble_Attack_to_king = Check_for_Check(Board_Grid, Piece_Color);
				if (Possble_Attack_to_king) {
					Board_Grid[i][k].Has_A_Piece = 0;
					Board_Grid[i][k].Chess_Piece = NULL;
					Board_Grid[i][j].Chess_Piece = Temp;
					Board_Grid[i][j].Has_A_Piece = 1;
				}
				else {
					Board_Grid[i][k].has_a_Valid_Move = 1;
					Board_Grid[i][k].Has_A_Piece = 0;
					Board_Grid[i][k].Chess_Piece = NULL;
					Board_Grid[i][j].Chess_Piece = Temp;
					Board_Grid[i][j].Has_A_Piece = 1;
				}
			}
			else {
				if (Board_Grid[i][k].Chess_Piece->Piece_Color != this->Piece_Color) {
					Piece* Temp = Board_Grid[i][k].Chess_Piece;
					Board_Grid[i][k].Has_A_Piece = 0;
					//Board_Grid[i][k].Chess_Piece = NULL;
					Board_Grid[i][k].Chess_Piece = this;
					Possble_Attack_to_king = Check_for_Check(Board_Grid, Piece_Color);
					if (Possble_Attack_to_king) {
						Board_Grid[i][k].Chess_Piece = Temp;
						Board_Grid[i][k].Has_A_Piece = 1;
					}
					else {
						Board_Grid[i][k].Has_A_kill_Move = 1;
						Board_Grid[i][k].Has_A_Piece = 1;
						Board_Grid[i][k].Chess_Piece = Temp;
					}
				}
				break;
			}
		}
		for (int k = j - 1; k >= 0; k--)//left
		{
			if (Board_Grid[i][k].Has_A_Piece == NULL) {
				Board_Grid[i][k].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[i][k].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[i][k].Has_A_kill_Move = 1;
				break;
			}
		}
		for (int k = i - 1; k >= 0; k--) //upWards
		{
			if (Board_Grid[k][j].Has_A_Piece == NULL) {
				Board_Grid[k][j].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[k][j].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[k][j].Has_A_kill_Move = 1;
				break;
			}
		}
		/*i is for vertical
		j is for horizontal*/
		for (int k = i + 1; k < 8; k++)  //Down Wards
		{
			if (Board_Grid[k][j].Has_A_Piece == NULL) {
				Board_Grid[k][j].has_a_Valid_Move = 1;
			}
			else {
				if (Board_Grid[k][j].Chess_Piece->Piece_Color != this->Piece_Color)
					Board_Grid[k][j].Has_A_kill_Move = 1;
				break;
			}
		}
	}
};
class Bishop : public Piece
{
public:
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int x_to_check, int y_to_check) {
		for (int x = i + 1, y = j + 1; x < 8 && y < 8; x++, y++) {
			if (x_to_check == x && y_to_check == y) {
				return 1;
			}
			//else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
			//	//Board_Grid[x][y].Has_A_kill_Move = 1;
			//	//return 1;
			//	break;
			//}
			else if (Board_Grid[x][y].Has_A_Piece || Board_Grid[x][y].Chess_Piece) {
				break;
			}
		}
		for (int x = i - 1, y = j - 1; x >= 0 && y >= 0; x--, y--) {
			if (x_to_check == x && y_to_check == y) {
				return 1;
			}
			//else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
			//	//Board_Grid[x][y].Has_A_kill_Move = 1;
			//	break;
			//}
			else if (Board_Grid[x][y].Has_A_Piece) {
				break;
			}
		}
		for (int x = i + 1, y = j - 1; x < 8 && y >= 0; x++, y--) {
			if (x_to_check == x && y_to_check == y) {
				return 1;
			}
			//else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
			//	//Board_Grid[x][y].Has_A_kill_Move = 1;
			//	break;
			//}
			else if (Board_Grid[x][y].Has_A_Piece) {
				break;
			}
		}
		for (int x = i - 1, y = j + 1; x >= 0 && y < 8; x--, y++) {
			if (x_to_check == x && y_to_check == y) {
				return 1;
			}
			//else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
			//	//Board_Grid[x][y].Has_A_kill_Move = 1;
			//	break;
			//}
			else if (Board_Grid[x][y].Has_A_Piece) {
				break;
			}
		}
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves)
	{
		// Clear all previous moves for the bishop
		if (Set_or_Clear_Moves == false) {
			return;
		}
		for (int x = i + 1, y = j + 1; x < 8 && y < 8; x++, y++) {
			if (Board_Grid[x][y].Chess_Piece == nullptr) {
				Board_Grid[x][y].has_a_Valid_Move = 1;
			}
			else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
				Board_Grid[x][y].Has_A_kill_Move = 1;
				break;
			}
			else {
				break;
			}
		}
		for (int x = i - 1, y = j - 1; x >= 0 && y >= 0; x--, y--) {
			if (Board_Grid[x][y].Chess_Piece == nullptr) {
				Board_Grid[x][y].has_a_Valid_Move = 1;
			}
			else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
				Board_Grid[x][y].Has_A_kill_Move = 1;
				break;
			}
			else {
				break;
			}
		}
		for (int x = i + 1, y = j - 1; x < 8 && y >= 0; x++, y--) {
			if (Board_Grid[x][y].Chess_Piece == nullptr) {
				Board_Grid[x][y].has_a_Valid_Move = 1;
			}
			else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
				Board_Grid[x][y].Has_A_kill_Move = 1;
				break;
			}
			else {
				break;
			}
		}
		for (int x = i - 1, y = j + 1; x >= 0 && y < 8; x--, y++) {
			if (Board_Grid[x][y].Chess_Piece == nullptr) {
				Board_Grid[x][y].has_a_Valid_Move = 1;
			}
			else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
				Board_Grid[x][y].Has_A_kill_Move = 1;
				break;
			}
			else {
				break;
			}
		}
	}
};
class Queen : public Piece
{
public:
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int X_to_check, int Y_to_check) {
		Bishop Temp;
		Rook Temp1;
		Temp.Piece_Color = Temp1.Piece_Color = Piece_Color;
		if (Temp.check_Valid_Move(Board_Grid, i, j, X_to_check, Y_to_check))
			return 1;
		if (Temp1.check_Valid_Move(Board_Grid, i, j, X_to_check, Y_to_check))
			return 1;
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		Bishop Temp;
		Rook Temp1;
		Temp.Piece_Color = Temp1.Piece_Color = Piece_Color;
		Temp.Valid_Moves(Board_Grid, i, j, Set_or_Clear_Moves);
		Temp1.Valid_Moves(Board_Grid, i, j, Set_or_Clear_Moves);
	}
};
class King : public Piece
{
public:
	//bool Under_Check{ 0 };
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int x_to_check, int y_to_check) {
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0)
					continue; // don't move to the same spot
				int new_x = i + dx;
				int new_y = j + dy;
				if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
				{
					if (new_x == x_to_check && new_y == y_to_check) {
						return 1;
					}
				}
			}
		}
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		if (!Set_or_Clear_Moves)
			return;
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0)
					continue; // don't move to the same spot

				int new_x = i + dx;
				int new_y = j + dy;
				if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
				{
					if (!Board_Grid[new_x][new_y].Chess_Piece || Board_Grid[new_x][new_y].Chess_Piece->Piece_Color != Piece_Color) {
						bool Possible_Attack = 0;
						// empty space or enemy piece
						for (int x = 0; x < 8; x++)
						{
							if (Possible_Attack)
								break;
							for (int y = 0; y < 8; y++) {
								if (Board_Grid[x][y].Has_A_Piece) {
									if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
										if (new_x == 6 && new_y == 4 && x == 6) {
											cout << "";
										}
										Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, new_x, new_y);
										if (Possible_Attack) {
											cout << "";
											Possible_Attack = 0;
										}
										Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, new_x, new_y);
										if (Possible_Attack)
											break;
									}
								}
							}
						}
						if (!Possible_Attack && !Board_Grid[new_x][new_y].Has_A_Piece)
							Board_Grid[new_x][new_y].has_a_Valid_Move = true;
						else if (Board_Grid[new_x][new_y].Chess_Piece && !Possible_Attack) {
							Board_Grid[new_x][new_y].Has_A_kill_Move = true;
						}
						Possible_Attack = 0;
					}
				}
			}
		}
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves, bool King_in_check) {
		for (int dx = -1; dx <= 1; dx++) {
			for (int dy = -1; dy <= 1; dy++) {
				if (dx == 0 && dy == 0)
					continue; // don't move to the same spot

				int new_x = i + dx;
				int new_y = j + dy;
				if (new_x >= 0 && new_x < 8 && new_y >= 0 && new_y < 8)
				{
					if (!Board_Grid[new_x][new_y].Chess_Piece || Board_Grid[new_x][new_y].Chess_Piece->Piece_Color != Piece_Color) {
						bool Possible_Attack = 0;
						// empty space or enemy piece
						for (int x = 0; x < 8; x++)
						{
							if (Possible_Attack)
								break;
							for (int y = 0; y < 8; y++) {
								if (Board_Grid[x][y].Has_A_Piece) {
									if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color) {
										if (new_x == 6 && new_y == 4 && x == 6) {
											cout << "";
										}
										Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, new_x, new_y);
										if (Possible_Attack) {
											cout << "";
											Possible_Attack = 0;
										}
										Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, new_x, new_y);
										if (Possible_Attack)
											break;
									}
								}
							}
						}
						if (!Possible_Attack && !Board_Grid[new_x][new_y].Has_A_Piece)
							Board_Grid[new_x][new_y].has_a_Valid_Move = true;
						else if (Board_Grid[new_x][new_y].Chess_Piece && !Possible_Attack) {
							Board_Grid[new_x][new_y].Has_A_kill_Move = true;
						}
						Possible_Attack = 0;
					}
				}
			}
		}
	}

};
class Knight : public Piece
{
public:
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int X_to_check, int Y_to_check) {
		int moves[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2} };
		for (int k = 0; k < 8; k++) {
			int x = i + moves[k][0];
			int y = j + moves[k][1];
			if (x >= 0 && x < 8 && y >= 0 && y < 8) {
				if (x == X_to_check && y == Y_to_check) {
					return 1;
				}
			}
		}
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves)
	{
		if (Set_or_Clear_Moves == false)
			return;

		int moves[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2} };
		for (int k = 0; k < 8; k++) {
			int x = i + moves[k][0];
			int y = j + moves[k][1];
			if (x >= 0 && x < 8 && y >= 0 && y < 8) {
				if (Board_Grid[x][y].Chess_Piece == nullptr) {
					Board_Grid[x][y].has_a_Valid_Move = 1;
				}
				else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color)
					Board_Grid[x][y].Has_A_kill_Move = 1;
			}
		}
	}
	virtual void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves, bool King_in_check) {
		int moves[8][2] = { {-2, -1}, {-2, 1}, {2, -1}, {2, 1}, {-1, -2}, {-1, 2}, {1, -2}, {1, 2} };
		for (int k = 0; k < 8; k++) {
			int x = i + moves[k][0];
			int y = j + moves[k][1];
			if (x >= 0 && x < 8 && y >= 0 && y < 8) {
				if (Board_Grid[x][y].Chess_Piece == nullptr) {
					Board_Grid[x][y].has_a_Valid_Move = 1;
				}
				else if (Board_Grid[x][y].Chess_Piece->Piece_Color != Piece_Color)
					Board_Grid[x][y].Has_A_kill_Move = 1;
			}
		}
	}
};
class Pawn : public Piece {
public:
	bool check_Valid_Move(Chess_Board_Grid Board_Grid[8][8], int i, int j, int X_to_check, int Y_to_check) {
		if (this->Piece_Color == 'B') {
			if (j != 0 && ((i - 1 == X_to_check) && ((j - 1) == Y_to_check)))
				return 1;
			if (j != 7 && ((i - 1 == X_to_check) && ((j + 1) == Y_to_check)))
				return 1;
		}
		else {
			if (j != 0 && ((i + 1 == X_to_check) && ((j - 1) == Y_to_check)))
				return 1;
			if (j != 7 && (((i + 1) == X_to_check) && ((j + 1) == Y_to_check))) {
				return 1;
			}
		}
		return 0;
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves) {
		if (!Set_or_Clear_Moves) {// Clear Previosly set Moves
			Board_Grid[i - 1][j].has_a_Valid_Move = 0;
			Board_Grid[i - 2][j].has_a_Valid_Move = 0;
			Board_Grid[i - 1][j - 1].Has_A_kill_Move = 0;
			Board_Grid[i - 1][j + 1].Has_A_kill_Move = 0;
			return;
		}
		if (this->Piece_Color == 'B') {
			if (Board_Grid[i - 1][j].Has_A_Piece == NULL) {
				Board_Grid[i - 1][j].has_a_Valid_Move = 1;
				if (Board_Grid[i - 2][j].Has_A_Piece == NULL && i == 6)
					Board_Grid[i - 2][j].has_a_Valid_Move = 1;
			}
			if (j != 0 && Board_Grid[i - 1][j - 1].Has_A_Piece)
				if (Board_Grid[i - 1][j - 1].Chess_Piece->Piece_Color == 'W')
					Board_Grid[i - 1][j - 1].Has_A_kill_Move = 1;
			if (j != 7 && Board_Grid[i - 1][j + 1].Has_A_Piece)
				if (Board_Grid[i - 1][j + 1].Chess_Piece->Piece_Color == 'W')
					Board_Grid[i - 1][j + 1].Has_A_kill_Move = 1;
		}
		else {
			if (Board_Grid[i + 1][j].Has_A_Piece == NULL) {
				Board_Grid[i + 1][j].has_a_Valid_Move = 1;
				if (Board_Grid[i + 2][j].Has_A_Piece == NULL && i == 1)
					Board_Grid[i + 2][j].has_a_Valid_Move = 1;
			}
			if (j != 0 && Board_Grid[i + 1][j - 1].Has_A_Piece)
				if (Board_Grid[i + 1][j - 1].Chess_Piece->Piece_Color == 'B')
					Board_Grid[i + 1][j - 1].Has_A_kill_Move = 1;
			if (j != 7 && Board_Grid[i + 1][j + 1].Has_A_Piece)
				if (Board_Grid[i + 1][j + 1].Chess_Piece->Piece_Color == 'B')
					Board_Grid[i + 1][j + 1].Has_A_kill_Move = 1;
		}
	}
	void Valid_Moves(Chess_Board_Grid Board_Grid[8][8], int i, int j, bool Set_or_Clear_Moves, bool King_in_check) {
		if (!Set_or_Clear_Moves) {// Clear Previosly set Moves
			Board_Grid[i - 1][j].has_a_Valid_Move = 0;
			Board_Grid[i - 2][j].has_a_Valid_Move = 0;
			Board_Grid[i - 1][j - 1].Has_A_kill_Move = 0;
			Board_Grid[i - 1][j + 1].Has_A_kill_Move = 0;
			return;
		}
		if (this->Piece_Color == 'B') {
			Piece Temp = *this;
			bool Check_If_Still_in_Check = 1;
			if (Board_Grid[i - 1][j].Has_A_Piece == NULL) {
				Board_Grid[i - 1][j].Chess_Piece = new Piece(Temp);
				Board_Grid[i - 1][j].Has_A_Piece = 1;
				Check_If_Still_in_Check = Check_for_Check(Board_Grid, Piece_Color);
				if (Check_If_Still_in_Check) {
					Board_Grid[i - 1][j].Chess_Piece = nullptr;
					Board_Grid[i - 1][j].Has_A_Piece = 0;
					Board_Grid[i - 1][j].Chess_Piece = NULL;
				}
				else {
					Board_Grid[i - 1][j].Chess_Piece = nullptr;
					Board_Grid[i - 1][j].Has_A_Piece = 0;
					Board_Grid[i - 1][j].has_a_Valid_Move = 1;
				}

				if (Board_Grid[i - 2][j].Has_A_Piece == NULL && i == 6) {
					Board_Grid[i - 2][j].Chess_Piece = new Piece(Temp);
					Board_Grid[i - 2][j].Has_A_Piece = 1;
					Check_If_Still_in_Check = Check_for_Check(Board_Grid, Piece_Color);
					if (Check_If_Still_in_Check) {
						Board_Grid[i - 2][j].Chess_Piece = nullptr;
						Board_Grid[i - 2][j].Has_A_Piece = 0;
					}
					else {
						Board_Grid[i - 2][j].Chess_Piece = nullptr;
						Board_Grid[i - 2][j].Has_A_Piece = 0;
						Board_Grid[i - 2][j].has_a_Valid_Move = 1;
					}
				}

			}
			if (j != 0 && Board_Grid[i - 1][j - 1].Has_A_Piece)
				if (Board_Grid[i - 1][j - 1].Chess_Piece->Piece_Color == 'W')
					Board_Grid[i - 1][j - 1].Has_A_kill_Move = 1;
			if (j != 7 && Board_Grid[i - 1][j + 1].Has_A_Piece)
				if (Board_Grid[i - 1][j + 1].Chess_Piece->Piece_Color == 'W')
					Board_Grid[i - 1][j + 1].Has_A_kill_Move = 1;
		}
		else {
			if (Board_Grid[i + 1][j].Has_A_Piece == NULL) {
				Board_Grid[i + 1][j].has_a_Valid_Move = 1;
				if (Board_Grid[i + 2][j].Has_A_Piece == NULL && i == 1)
					Board_Grid[i + 2][j].has_a_Valid_Move = 1;
			}
			if (j != 0 && Board_Grid[i + 1][j - 1].Has_A_Piece)
				if (Board_Grid[i + 1][j - 1].Chess_Piece->Piece_Color == 'B')
					Board_Grid[i + 1][j - 1].Has_A_kill_Move = 1;
			if (j != 7 && Board_Grid[i + 1][j + 1].Has_A_Piece)
				if (Board_Grid[i + 1][j + 1].Chess_Piece->Piece_Color == 'B')
					Board_Grid[i + 1][j + 1].Has_A_kill_Move = 1;
		}
	}
};
class Board {
	Chess_Board_Grid Board_Grid[8][8];
	Piece* White_Pieces[16];
	Piece* Black_Pieces[16];
	bool Turn{ 0 };
	char King_Color_Under_Check{ ' ' };
	bool Check{ 0 };
	COORD Last_Moved_Tile{ -1 };
	COORD Lastest_Moved_Tile{ -1 };
public:
	COORD Get_King_Pos(char Color) {
		COORD King_Pos;
		for (int x = 0; x < 8; x++)
			for (int y = 0; y < 8; y++) {
				if (Board_Grid[x][y].Chess_Piece)
					if ((Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::King) && Board_Grid[x][y].Chess_Piece->Piece_Color == Color)
					{
						King_Pos = { short(x), short(y) };
						return King_Pos;
					}
			}
	}
	void Check_for_Input(int Mouse_X, int Mouse_Y) {
		static int Temp_i = 0, Temp_j = 0;
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++) {
				if ((Board_Grid[i][j].has_a_Valid_Move || Board_Grid[i][j].Has_A_kill_Move) && Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y)) {
					if (Board_Grid[i][j].Has_A_kill_Move)
						Board_Grid[i][j].Chess_Piece->dead_or_alive = 0;
					if ((i == 0 && Board_Grid[Temp_i][Temp_j].Chess_Piece->Piece_Color == 'B'
						|| i == 7 && Board_Grid[Temp_i][Temp_j].Chess_Piece->Piece_Color == 'W')
						&& (Board_Grid[Temp_i][Temp_j].Chess_Piece->Piece_Name == PieceName::Pawn))
					{
						Piece* Temp = Board_Grid[Temp_i][Temp_j].Chess_Piece;
						for (int k = 0; k < 16; k++)
						{
							if (Black_Pieces[k] == Temp && Temp->Piece_Color == 'B') {
								delete Black_Pieces[k];
								Black_Pieces[k] = new Queen;
								Black_Pieces[k]->Piece_Name = PieceName::Queen;
								Black_Pieces[k]->Piece_Position = Temp->Piece_Position;
								Black_Pieces[k]->Piece_Color = Temp->Piece_Color;
								Black_Pieces[k]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenB.png");
								Board_Grid[Temp_i][Temp_j].Chess_Piece = Black_Pieces[k];
								break;
							}
							else if (White_Pieces[k] == Temp)
							{
								delete White_Pieces[k];
								White_Pieces[k] = new Queen;
								White_Pieces[k]->Piece_Name = PieceName::Queen;
								White_Pieces[k]->Piece_Position = Temp->Piece_Position;
								White_Pieces[k]->Piece_Color = Temp->Piece_Color;
								White_Pieces[k]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenW.png");
								Board_Grid[Temp_i][Temp_j].Chess_Piece = White_Pieces[k];
								break;
							}
						}
					}
					Board_Grid[Temp_i][Temp_j].Chess_Piece->Piece_Position = { short(j * 80), short(i * 80) };
					Board_Grid[i][j].Chess_Piece = Board_Grid[Temp_i][Temp_j].Chess_Piece;
					Board_Grid[Temp_i][Temp_j].Chess_Piece = nullptr;
					Clear_All_Moves();
					Board_Grid[Temp_i][Temp_j].Has_A_Piece = 0;
					Board_Grid[Temp_i][Temp_j].Chess_Piece = nullptr;
					Board_Grid[Temp_i][Temp_j].Button_Pushed = 0;
					Board_Grid[i][j].Has_A_Piece = 1;
					Board_Grid[i][j].Button_Pushed = 0;
					Last_Moved_Tile = { short(Board_Grid[Temp_i][Temp_j].Chess_Board.x), short(Board_Grid[Temp_i][Temp_j].Chess_Board.y) };
					Lastest_Moved_Tile = { short(Board_Grid[i][j].Chess_Board.x), short(Board_Grid[i][j].Chess_Board.y) };
					Turn = !Turn;
					bool to_Break = 0;
					int x, y;
					bool check = 0;
					for (x = 0; x < 8; x++)
					{

						for (y = 0; y < 8; y++)
							if (Board_Grid[x][y].Chess_Piece)
								if (Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::King && Board_Grid[x][y].Chess_Piece->Piece_Color != Board_Grid[i][j].Chess_Piece->Piece_Color)
								{
									goto Skip1;
									break;
								}
					}
				Skip1:
					check = Board_Grid[i][j].Chess_Piece->check_Valid_Move(Board_Grid, i, j, x, y);
					if (check) {
						Check = 1;
						King_Color_Under_Check = Board_Grid[x][y].Chess_Piece->Piece_Color;
						//Board_Grid[x][y].Chess_Piece.Under_Check = 1;
					}
					else
						Check = 0;
					COORD W_King_Pos, B_King_Pos;
					for (int x = 0; x < 8; x++)
						for (int y = 0; y < 8; y++) {
							if (Board_Grid[x][y].Chess_Piece)
								if ((Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::King) && Board_Grid[x][y].Chess_Piece->Piece_Color == 'W')
									W_King_Pos = { short(x), short(y) };
								else if ((Board_Grid[x][y].Chess_Piece->Piece_Name == PieceName::King) && Board_Grid[x][y].Chess_Piece->Piece_Color == 'B')
									B_King_Pos = { short(x), short(y) };
						}
					bool Possible_Attack = 0;
					for (int x = 0; x < 8; x++)
						for (int y = 0; y < 8; y++)
						{
							if (Board_Grid[x][y].Chess_Piece && Board_Grid[x][y].Chess_Piece->Piece_Color != 'B') {
								Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, B_King_Pos.X, B_King_Pos.Y);
								if (Possible_Attack) {
									check = 1;  King_Color_Under_Check = 'B';
									goto Done;
								}
								else
									check = 0;
							}
							else if (Board_Grid[x][y].Chess_Piece && Board_Grid[x][y].Chess_Piece->Piece_Color != 'W') {
								Possible_Attack = Board_Grid[x][y].Chess_Piece->check_Valid_Move(Board_Grid, x, y, W_King_Pos.X, W_King_Pos.Y);
								if (Possible_Attack) {
									check = 1;  King_Color_Under_Check = 'W';
									goto Done;
								}
								else
									check = 0;
								//goto Skip;
							}
						}
				Done:
					cout << "";

				}
				else if (Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y) && Board_Grid[i][j].Has_A_Piece)
					if (Board_Grid[i][j].Button_Pushed || !Check_if_any_Piece_Selected()) {
						if ((!Turn && Board_Grid[i][j].Chess_Piece->Piece_Color == 'B'))
							return;
						else if ((Turn && Board_Grid[i][j].Chess_Piece->Piece_Color == 'W'))
							return;
						Board_Grid[i][j].Button_Pushed = !(Board_Grid[i][j].Button_Pushed);
						if (Board_Grid[i][j].Chess_Piece->Piece_Color == King_Color_Under_Check && Check)
							Board_Grid[i][j].Chess_Piece->Valid_Moves(Board_Grid, i, j, Board_Grid[i][j].Button_Pushed, 1);
						else
							Board_Grid[i][j].Chess_Piece->Valid_Moves(Board_Grid, i, j, Board_Grid[i][j].Button_Pushed);
						//else
							//Board_Grid[i][j].Chess_Piece->Valid_Moves(Board_Grid, i, j, Board_Grid[i][j].Button_Pushed, 1);

						if (Board_Grid[i][j].Button_Pushed) {
							Temp_i = i;
							Temp_j = j;
						}
						else {
							Clear_All_Moves();
						}
					}
			}
	}
	void Create_Board() {
		SDL_Rect Box = { 0,0,80,80 }; COORD position = { 0 };
		SDL_Texture* TempT = IMG_LoadTexture(renderer, "./assets/72ppi/Last Move.png");
		bool toggle = 0;
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 8; j++)
			{
				Board_Grid[i][j].Chess_Board = { Box.x, Box.y, 80, 80 };
				if (Board_Grid[i][j].Chess_Piece && (Board_Grid[i][j].Chess_Piece->Piece_Name == PieceName::King) && (Board_Grid[i][j].Chess_Piece->Piece_Color == King_Color_Under_Check) && Check)
					SDL_SetRenderDrawColor(renderer, 146, 9, 9, 255);

				else if (Board_Grid[i][j].has_a_Valid_Move) {
					SDL_SetRenderDrawColor(renderer, 239, 239, 32, 255);
				}
				else if (Board_Grid[i][j].Has_A_kill_Move && Board_Grid[i][j].Chess_Piece->Piece_Name == PieceName::King)
					SDL_SetRenderDrawColor(renderer, 146, 9, 9, 255);

				else if (Board_Grid[i][j].Has_A_kill_Move)
					SDL_SetRenderDrawColor(renderer, 226, 22, 22, 255);
				else if (Board_Grid[i][j].Button_Pushed)
					SDL_SetRenderDrawColor(renderer, 48, 222, 42, 255);
				else if (Board_Grid[i][j].Button_Hovered)
					SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
				else {
					if (toggle)
						SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
					else
						SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				}
				/*else {
					if (Board_Grid[i][j].has_a_Valid_Move) {
						SDL_SetRenderDrawColor(renderer, 239, 239, 32, 255);
					}
					else if (Board_Grid[i][j].Has_A_kill_Move)
						SDL_SetRenderDrawColor(renderer, 226, 30, 30, 255);
					else if (Board_Grid[i][j].Button_Pushed)
						SDL_SetRenderDrawColor(renderer, 48, 222, 42, 255);
					else if (Board_Grid[i][j].Button_Hovered)
						SDL_SetRenderDrawColor(renderer, 216, 216, 216, 255);
				}*/
				SDL_RenderFillRect(renderer, &Box);
				int texW, texH;

				if (Board_Grid[i][j].has_a_Valid_Move) {
					if (toggle) {
						thickLineRGBA(renderer, Box.x, Box.y + 2, Box.x + 80, Box.y + 2, 5, 0, 0, 0, 255); //top
						thickLineRGBA(renderer, Box.x + 2, Box.y, Box.x + 2, Box.y + 80, 5, 0, 0, 0, 255); //left
						thickLineRGBA(renderer, Box.x, Box.y + 77, Box.x + 80, Box.y + 77, 5, 0, 0, 0, 255);//bottom
						thickLineRGBA(renderer, Box.x + 77, Box.y, Box.x + 77, Box.y + 77, 5, 0, 0, 0, 255);//right
					}
					else {
						thickLineRGBA(renderer, Box.x, Box.y + 2, Box.x + 80, Box.y + 2, 5, 255, 255, 255, 255); //top
						thickLineRGBA(renderer, Box.x + 2, Box.y, Box.x + 2, Box.y + 80, 5, 255, 255, 255, 255); //left
						thickLineRGBA(renderer, Box.x, Box.y + 77, Box.x + 80, Box.y + 77, 5, 255, 255, 255, 255);//bottom
						thickLineRGBA(renderer, Box.x + 77, Box.y, Box.x + 77, Box.y + 77, 5, 255, 255, 255, 255);//right
					}
				}
				if (Board_Grid[i][j].Chess_Board.x == Last_Moved_Tile.X && Board_Grid[i][j].Chess_Board.y == Last_Moved_Tile.Y)
				{
					if (TempT)
						SDL_DestroyTexture(TempT);
					TempT = IMG_LoadTexture(renderer, "./assets/72ppi/Last Move.png");
					SDL_QueryTexture(TempT, NULL, NULL, &texW, &texH);
					SDL_Rect dstrect = { Board_Grid[i][j].Chess_Board.x + ((Board_Grid[i][j].Chess_Board.w - texW) / 2), Board_Grid[i][j].Chess_Board.y + ((Board_Grid[i][j].Chess_Board.h - texW) / 2), texW, texH };
					SDL_RenderCopy(renderer, TempT, NULL, &dstrect);
				}
				else if (Board_Grid[i][j].Chess_Board.x == Lastest_Moved_Tile.X && Board_Grid[i][j].Chess_Board.y == Lastest_Moved_Tile.Y)
				{
					if (TempT)
						SDL_DestroyTexture(TempT);
					TempT = IMG_LoadTexture(renderer, "./assets/72ppi/Lastest Move.png");
					SDL_QueryTexture(TempT, NULL, NULL, &texW, &texH);
					SDL_Rect dstrect = { Board_Grid[i][j].Chess_Board.x + ((Board_Grid[i][j].Chess_Board.w - texW) / 2), Board_Grid[i][j].Chess_Board.y + ((Board_Grid[i][j].Chess_Board.h - texW) / 2), texW, texH };
					SDL_RenderCopy(renderer, TempT, NULL, &dstrect);
				}
				if (Board_Grid[i][j].Chess_Piece)
				{
					SDL_Rect Temp = { Board_Grid[i][j].Chess_Piece->Piece_Position.X, Board_Grid[i][j].Chess_Piece->Piece_Position.Y - 5, 80, 80 };
					if (Board_Grid[i][j].Chess_Piece->dead_or_alive)
						SDL_RenderCopy(renderer, Board_Grid[i][j].Chess_Piece->Piece_Image, NULL, &Temp);
				}
				Box.x += 80;
				if (j != 7)
					toggle = !toggle;
			}
			Box.x = 0;
			Box.y += 80;
		}
		SDL_DestroyTexture(TempT);
	}
	void load_Pawn() {
		for (int i = 8; i < 16; i++)
		{
			Board_Grid[1][i - 8].Chess_Piece = new Pawn;
			Board_Grid[1][i - 8].Chess_Piece->Piece_Color = 'W';
			Board_Grid[1][i - 8].Chess_Piece->Piece_Position.X = Board_Grid[1][i - 8].Chess_Board.x;
			Board_Grid[1][i - 8].Chess_Piece->Piece_Position.Y = Board_Grid[1][i - 8].Chess_Board.y;
			Board_Grid[1][i - 8].Chess_Piece->Piece_Name = PieceName::Pawn;
			Board_Grid[1][i - 8].Chess_Piece->Piece_Image = IMG_LoadTexture(renderer, "./assets/PawnW.png");
			Board_Grid[1][i - 8].Has_A_Piece = 1;

			Board_Grid[6][i - 8].Chess_Piece = new Pawn;
			Board_Grid[6][i - 8].Chess_Piece->Piece_Color = 'B';
			Board_Grid[6][i - 8].Chess_Piece->Piece_Position.X = Board_Grid[6][i - 8].Chess_Board.x;
			Board_Grid[6][i - 8].Chess_Piece->Piece_Position.Y = Board_Grid[6][i - 8].Chess_Board.y;
			Board_Grid[6][i - 8].Chess_Piece->Piece_Name = PieceName::Pawn;
			Board_Grid[6][i - 8].Chess_Piece->Piece_Image = IMG_LoadTexture(renderer, "./assets/PawnB.png");
			Board_Grid[6][i - 8].Has_A_Piece = 1;
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
			Board_Grid[0][i].Has_A_Piece = 1;
			Board_Grid[7][i].Has_A_Piece = 1;
			if (i == 0 || i == 7) {
				White_Pieces[i] = new Rook; 	White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i] = new Rook;		Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::Rook;	Black_Pieces[i]->Piece_Name = PieceName::Rook;

				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/RookW.png");
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/RookB.png");
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
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
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];

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
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/QueenB.png");
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
			}
			else if (i == 4) {
				White_Pieces[i] = new King;		White_Pieces[i]->Piece_Color = 'W';
				Black_Pieces[i] = new King;		Black_Pieces[i]->Piece_Color = 'B';
				White_Pieces[i]->Piece_Name = PieceName::King;	Black_Pieces[i]->Piece_Name = PieceName::King;
				White_Pieces[i]->Piece_Position.X = Board_Grid[0][i].Chess_Board.x;
				White_Pieces[i]->Piece_Position.Y = Board_Grid[0][i].Chess_Board.y;
				White_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KingW.png");
				Board_Grid[0][i].Chess_Piece = White_Pieces[i];

				Black_Pieces[i]->Piece_Image = IMG_LoadTexture(renderer, "./assets/KingB.png");
				Black_Pieces[i]->Piece_Position.X = Board_Grid[7][i].Chess_Board.x;
				Black_Pieces[i]->Piece_Position.Y = Board_Grid[7][i].Chess_Board.y;
				Board_Grid[7][i].Chess_Piece = Black_Pieces[i];
			}
		}
	}
	void Check_for_Hovering(int Mouse_X, int Mouse_Y) {
		for (int i = 0; i < 8; i++)
			for (int j = 0; j < 8; j++)
				if (Board_Grid[i][j].Has_A_Piece) {
					if ((!Turn && Board_Grid[i][j].Chess_Piece->Piece_Color == 'B') && Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y))
						return;
					else if ((Turn && Board_Grid[i][j].Chess_Piece->Piece_Color == 'W') && Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y))
						return;
					Board_Grid[i][j].Button_Hovered = Board_Grid[i][j].Check_if_Mouse_in_Button_Area(Mouse_X, Mouse_Y);
				}
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
		int Vertical_Correction = 4;
		for (int i = 0; i < 16; i++)
		{
			//Temp = { White_Pieces[i]->Piece_Position.X, White_Pieces[i]->Piece_Position.Y - Vertical_Correction, 80, 80 };
			//if (White_Pieces[i]->dead_or_alive)
			//	SDL_RenderCopy(renderer, White_Pieces[i]->Piece_Image, NULL, &Temp);
			//Temp = { Black_Pieces[i]->Piece_Position.X, Black_Pieces[i]->Piece_Position.Y - Vertical_Correction, 80, 80 };
			//if (Black_Pieces[i]->dead_or_alive)
			//	SDL_RenderCopy(renderer, Black_Pieces[i]->Piece_Image, NULL, &Temp);
			//if (i == 7)
			//	Vertical_Correction = 10;
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
			if (event.type == SDL_QUIT) {
				//	//SDL_DestroyTexture(buttonTextTexture);
				//	//SDL_FreeSurface(buttonTextSurface);
				//	SDL_DestroyRenderer(renderer);
				//	SDL_DestroyWindow(window);
				//	SDL_Quit();
				exit(0);
				return 0;
			}
			if (event.type == SDL_MOUSEMOTION)	//Mouse is hovering
			{
				SDL_GetMouseState(&MouseX, &MouseY);
				Chess.Check_for_Hovering(MouseX, MouseY);
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