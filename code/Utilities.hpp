/* 
 * File:   Utilities.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__UTILITIES_HPP__)
    #define __UTILITIES_HPP__

    #include <cmath>
    #include "IntList.hpp"

    class Utilities{
    public:
        // recebe um vetor de double e ordena os índices numa lista
        // o vetor e a lista devem ter o mesmo número de itens
        void sort_double(double* &double_vector, IntList &sorted_list);
        // retorna o índice do maior valor de um vetor de doubles:
        int argmax(int n_cands, double* &double_vector);
        // retorna a média de um vetor de doubles
        double average (double* &values, int n_values);
        // retorna o desvio padrão de um vetor de doubles
        double std_dev (double* &values, int n_values);
    };

#endif
