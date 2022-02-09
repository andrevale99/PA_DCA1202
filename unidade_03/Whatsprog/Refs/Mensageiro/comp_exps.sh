#!/bin/bash


mkdir Exp_0 Exp_1 Exp_2

g++-11  mensageiro0_cliente.cpp mysocket.cpp -lpthread -o Cliente_0.out
g++-11  mensageiro0_servidor.cpp mysocket.cpp -lpthread -o Servidor_0.out

mv Cliente_0.out Servidor_0.out Exp_0

g++-11  mensageiro1_cliente.cpp mysocket.cpp -lpthread -o Cliente_1.out
g++-11  mensageiro1_servidor.cpp mysocket.cpp -lpthread -o Servidor_1.out

mv Cliente_1.out Servidor_1.out Exp_1
