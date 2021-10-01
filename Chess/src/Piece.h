#pragma once

#include "SDL.h"

enum class Type
{
	Empty, Pawn, Rook, Knight, Bishop, Queen, King
};

class Piece
{
private:
	// Side - white or black
	// Might be a good idea to make it a boolean
	int m_Side = -1;
	Type m_Type = Type::Empty;
	SDL_Texture* m_Texture = nullptr;

	// TODO: Add one more atribute called m_HasMoved
	// for pawns' to be able to move 2 squares first time
	// and for castling

public:
	Piece();
	Piece(int side, Type t);
	void SetSide(int side) { m_Side = side; }
	void SetType(Type t) { m_Type = t; }
	int GetSide() { return m_Side; }
	Type GetType() { return m_Type; }
	void SetTexture(SDL_Texture *texture) { m_Texture = texture; }
	SDL_Texture* GetTexture() { return m_Texture; }
	void SetTypeAndTexture(Type type, SDL_Texture *texture) 
	{ 
		m_Type = type; 
		m_Texture = texture; 
	}
};