#ifndef _COORD_H_
#define _COORD_H_
#include <iostream>

/// As coordenadas de uma celula do mapa
struct Coord
{
public:
  int lin,col;

  Coord();
  Coord(unsigned L, unsigned C);

  // Teste de validade
  bool valida() const;

  // Comparacao
  bool operator==(const Coord& C) const;
  bool operator!=(const Coord& C) const;

  // Operacoes aritmeticas
  Coord operator+(const Coord& C) const;
  Coord operator-(const Coord& C) const;
};

/// Funcoes de E/S de coordenadas
std::istream& operator>>(std::istream& I, Coord& C);
std::ostream& operator<<(std::ostream& O, const Coord& C);

/// Valor absoluto de uma coordenada
Coord abs(const Coord& C);

/// Norma (modulo) de uma coordenada
double norm(const Coord& C);

#endif // _COORD_H_
