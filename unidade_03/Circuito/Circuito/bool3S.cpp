#include <iostream>
#include "bool3S.h"

using namespace std;

//Os operadores logicos para a classe bool3S

// NOT 3S
bool3S operator~(bool3S x)
{
  if (x==bool3S::UNDEF) return bool3S::UNDEF;
  if (x==bool3S::TRUE) return bool3S::FALSE;
  return bool3S::TRUE;
}

// AND 3S
bool3S operator&(bool3S x1, bool3S x2)
{
  if (x1==bool3S::FALSE || x2==bool3S::FALSE) return bool3S::FALSE;
  if (x1==bool3S::UNDEF || x2==bool3S::UNDEF) return bool3S::UNDEF;
  return bool3S::TRUE;
}

void operator&=(bool3S& x1, bool3S x2)
{
  if (x1==bool3S::FALSE) return;
  if (x2==bool3S::FALSE) {x1=bool3S::FALSE; return;}
  if (x1==bool3S::UNDEF || x2==bool3S::UNDEF) {x1=bool3S::UNDEF; return;}
  x1=bool3S::TRUE;
}

// OR 3S
bool3S operator|(bool3S x1, bool3S x2)
{
  if (x1==bool3S::TRUE || x2==bool3S::TRUE) return bool3S::TRUE;
  if (x1==bool3S::UNDEF || x2==bool3S::UNDEF) return bool3S::UNDEF;
  return bool3S::FALSE;
}

void operator|=(bool3S& x1, bool3S x2)
{
  if (x1==bool3S::TRUE) return;
  if (x2==bool3S::TRUE) {x1=bool3S::TRUE; return;}
  if (x1==bool3S::UNDEF || x2==bool3S::UNDEF) {x1=bool3S::UNDEF; return;}
  x1=bool3S::FALSE;
}

// XOR 3S
bool3S operator^(bool3S x1, bool3S x2)
{
  if (x1==bool3S::UNDEF || x2==bool3S::UNDEF) return bool3S::UNDEF;
  if (x1==x2) return bool3S::FALSE;
  return bool3S::TRUE;
}

void operator^=(bool3S& x1, bool3S x2)
{
  if (x1==bool3S::UNDEF) return;
  if (x2==bool3S::UNDEF) {x1=bool3S::UNDEF; return;}
  if (x1==x2) {x1=bool3S::FALSE; return;}
  x1=bool3S::TRUE;
}

// Os operadores de incremento/decremento para a classe bool3S

// Prefixados (++B, --B: incrementa, depois retorna)

bool3S& operator++(bool3S& B)
{
  if (B==bool3S::UNDEF)
  {
    B=bool3S::FALSE;
    return B;
  }
  if (B==bool3S::FALSE)
  {
    B=bool3S::TRUE;
    return B;
  }
  // Vamos fazer um incremento circular: TRUE->UNDEF
  B=bool3S::UNDEF;
  return B;
}

bool3S& operator--(bool3S& B)
{
  if (B==bool3S::TRUE)
  {
    B=bool3S::FALSE;
    return B;
  }
  if (B==bool3S::FALSE)
  {
    B=bool3S::UNDEF;
    return B;
  }
  // Vamos fazer um decremento circular: UNDEF->TRUE
  B=bool3S::TRUE;
  return B;
}

// Posfixados (B++, B--: retorna copia do valor antes de incrementar)
// O parametro int nao serve para nada, apenas diferencia dos prefixados

bool3S operator++(bool3S& B, int)
{
  bool3S prov(B);
  ++B;
  return prov;
}

bool3S operator--(bool3S& B, int)
{
  bool3S prov(B);
  --B;
  return prov;
}

// As conversoes entre bool3S e char

// Converte um bool3S para um char que o representa (F T ?)
char toChar(bool3S B)
{
  if (B==bool3S::TRUE) return 'T';
  if (B==bool3S::FALSE) return 'F';
  return '?';
}

// Converte um char (F T ?) para o bool3S correspondente
bool3S toBool_3S(char C)
{
  C = toupper(C);
  if (C=='T') return bool3S::TRUE;
  if (C=='F') return bool3S::FALSE;
  return bool3S::UNDEF;
}

// Os operadores de entrada/saida para a classe bool3S

// Impressao de bool3S
ostream& operator<<(ostream& O, bool3S B)
{
  O << toChar(B);
  return O;
}

// Leitura de bool3S (? T F)
istream& operator>>(istream& I, bool3S& B)
{
  char prov;
  I >> prov;
  B = toBool_3S(prov);
  return I;
}
