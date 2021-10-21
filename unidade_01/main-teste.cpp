#include "Poly.hpp"

int main(int argc, char *argv[]){
	Poly p1(1);
	
	std::cout << p1;

	std::cout << p1.getGrau() << '\t' << p1.isZero() << '\t' << p1.empty() << '\n';

	return 0;
}