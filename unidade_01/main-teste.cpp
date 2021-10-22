#include <iostream>
#include <cmath>

#include "Poly.hpp"

int main(int argc, char *argv[]){
	Poly p1(2);

	std::cout << p1(1.5) << '\n';

	return 0;
}