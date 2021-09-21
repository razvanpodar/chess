#include "Piece.h"

Piece::Piece() 
	: m_Side(-1), m_Type(Type::Empty)
{ }

Piece::Piece(int side, Type t)
	: m_Side(side), m_Type(t)
{ }

