/* 
 * File:   Node.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// classe que representa um nó euclidiano do grafo

#if !defined(__NODE_HPP__)
    #define __NODE_HPP__

    class Node{     // vértice de uma grafo euclidiano
        double cx;  // coordenada plana x
        double cy;  // coordenada plana y
    public:
        Node();
        void create(double x, double y);
        ~Node();
        double x();
        double y();
    };

#endif
