/* 
 * File:   Random.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// geracao de pseudo randomicos adaptado do livro "Numerical Recipes in C"
#if !defined(__RANDOM_HPP__)
    #define __RANDOM_HPP__

    #include <iostream>

    #define IA 16807
    #define IM 2147483647
    #define AM (1.0/IM)
    #define IQ 127773
    #define IR 2836

    class Random{
        long seed;
    public:
        Random(long s);
        ~Random();

        // retorna a semente utilizada:
        long seed_used();
        // retorna valores uniformemente distribuidos em [0,1]
        double random_number();
        // recebe um inteiro e retorna outro (aleatório) entre 1 e o inteiro recebido
        int raffle_int (int n_itens);
        // recebe um vetor de doubles e retorna um índice sorteado
        // os doubles representam a probabilidade de seu índice ser sorteado
        int raffle_double_vector (int n_itens, double*& double_vector);
    };

#endif
