#pragma once

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

public:
	Piece();
	Piece(int side, Type t);
	void setSide(int side) { m_Side = side; }
	void setType(Type t) { m_Type = t; }
	int getSide() { return m_Side; }
	Type getType() { return m_Type; }
};