/* 
 * File:   IntList.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// lista ordenada com valores inteiros

#if !defined(__INTLIST_HPP__)
    #define __INTLIST_HPP__

    #include <iostream>
    #include <fstream>
    #include <stdlib.h>
    using namespace std;

    class IntList{  
    private:
        int max_items;       // número máximo de valores da lista
        int index;           // aponta o último valor da lista. Se -1, a lista está vazia
        int *values_list;    // valores da lista
    public:    
        IntList();
        void create(int size);
        IntList(int size);
        ~IntList();

        void empty();         // esvazia a lista
        void add(int value);  // adiciona um valor no final da lista
        void reverse_no_depot(int index_start, int index_end, int depot_node);  // inverte a values_list de index_start a index_end, menos os nós depósito da solução

        void reverse(int index_start, int index_end);      // inverte a values_list de index_start a index_end:
        void remove (int value);

        void change_values(int index_i, int index_j);
        int size();          // retorna a capacidade da lista
        int n_items();       // retorna o número de itens na lista
        int value(int ind);  // retorna um valor da lista pelo índice
        int last_value();    // retorna o último valor da lista
        bool on_the_list(int value);  // o valor está na lista?
        bool is_full();      // a lista está cheia?
        bool is_empty();     // a lista está vazia?

        int next_value(int value);
        
        void save(ofstream& file_out);
        void print();        // imprime a lista
        void print_add1();   // imprime a lista adicionando 1 a todos os valores
    };

#endif
