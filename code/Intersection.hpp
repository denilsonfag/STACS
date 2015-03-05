/* 
 * File:   Intersection.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__INTERSECTION_HPP__)
    #define __INTERSECTION_HPP__

    #include "Node.hpp"

    class Intersection{
        int n;
        Node* nodes_vector;
    public:
        Intersection(int n, Node* &nodes_vector);
        ~Intersection();

        // retorna verdadeiro se as arestas cruzarem:
        bool verify(int ai, int aj, int bi, int bj);
    };
    
#endif
    