#include <cmath>
#include <deque>
#include "coord.h"
using namespace std;

class Noh
{
private:
	Coord pos;
	Coord ant;
	double g;
	double h;

public:
	friend class Labirinto;

	double custoPassado() const
	{
		return g;
	}

	double custoFuturo() const
	{
		return h;
	}

	double custoTotal() const
	{
		return g + h;
	}

	bool operator==(const Noh &N) const
	{
		return pos == N.pos;
	}

	bool operator!=(const Noh &N) const
	{
		return !operator==(N);
	}

	bool operator==(const Coord &C) const
	{
		return pos == C;
	}

	bool operator!=(const Coord &C) const
	{
		return !operator==(C);
	}

	bool operator<(const Noh &N) const
	{
		return custoTotal() < N.custoTotal();
	}

	friend ostream &operator<<(ostream &O, const Noh &N)
	{
		O << '[' << N.pos << '|' << N.ant << '|' << N.custoTotal() << ']';
		return O;
	}

	/// HEURISTICA
	void heuristica(const Coord &Dest)
	{
		Coord delta = abs(Dest - pos);

		// diagonal (8 direcoes)
		h = sqrt(2.0) * min(delta.lin, delta.col) + abs(delta.lin - delta.col);
	}
};
