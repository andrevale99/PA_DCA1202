GCC = g++

FLAGS = -Wall -std=c++17

TESTE = Poly-main.cpp Poly.cpp
#TESTE = main-teste.cpp Poly.cpp

OUT = poly.out

all: $(OUT)

$(OUT): $(TESTE)
	$(GCC) $(FLAGS) $(TESTE) -o $(OUT)
	@echo "";

clear: 
	@rm $(OUT)