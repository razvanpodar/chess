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
	bool m_HasMoved = false;
	Type m_Type = Type::Empty;
	SDL_Texture* m_Texture = nullptr;

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
	void Moved() { m_HasMoved = true; }
	void SetMoved(bool moved) { m_HasMoved = moved; }
	bool HasMoved() { return m_HasMoved; }

};