#include <cmath>
#include "coord.h"

using namespace std;

/// Construtores
Coord::Coord(): lin(-1), col(-1) {}

Coord::Coord(unsigned L, unsigned C): lin(L), col(C) {}

/// Teste de validade
bool Coord::valida() const
{
  return lin>=0 && col>=0;
}

/// Comparacao
bool Coord::operator==(const Coord& C) const
{
  return lin==C.lin && col==C.col;
}

bool Coord::operator!=(const Coord& C) const
{
  return !operator==(C);
}

/// Operacoes aritmeticas
Coord Coord::operator+(const Coord& C) const
{
  return Coord(lin+C.lin, col+C.col);
}

Coord Coord::operator-(const Coord& C) const
{
  return Coord(lin-C.lin, col-C.col);
}

/// Funcoes de E/S de coordenadas
std::istream& operator>>(std::istream& I, Coord& C)
{
  I >> C.lin >> C.col;
  return I;
}

std::ostream& operator<<(std::ostream& O, const Coord& C)
{
  O << C.lin << ';' << C.col;
  return O;
}

/// Valor absoluto de uma coordenada
Coord abs(const Coord& C)
{
  return Coord(abs(C.lin), abs(C.col));
}

/// Norma (modulo) de uma coordenada
double norm(const Coord& C)
{
  return sqrt(C.lin*C.lin + C.col*C.col);
}
