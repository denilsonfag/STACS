/* 
 * File:   EuclideanGraph.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// classe que representa o grafo
#if !defined(__GRAPH_HPP__)
    #define __GRAPH_HPP__

    #include "ControlSTACS.hpp"
    #include "ControlExperiment.hpp"
    #include "Node.hpp"
    #include "DoubleMatrix.hpp"

    #include <iostream>
    #include <fstream>
    #include <cmath>
    using namespace std;

    class EuclideanGraph{
        Node* plan_nodes_vector;        // vetor de vértices planos (representados por coordenadas)
        DoubleMatrix* euclidean_matrix; // matriz de distâncias euclidianas

        int n_nodes;                    // numero de nós do grafo
        int index_last_nodes_vector;    // aponta o ultimo vértice inserido no grafo (vetor de vértices)
        ifstream file_load;             // para carregar a instância de arquivo
        const char* file_name;          // nome do arquivo da instância
    public:
        EuclideanGraph();
        ~EuclideanGraph();

        void load_model_graph();
        void create_empty_graph(int n_nodes);
        
        void add_vertex(double x, double y);
        void calcule_euclidean_distances();
        void calcule_euclidean_distances_div1000();
        
        int n_nodes_graph();
        Node* get_nodes_vector();
        DoubleMatrix* get_euclidean_matrix();
        double get_value_euclidean_matrix(int i, int j);

        void print_nodes();
        void print_euclidean_matrix();
        void print_col_dist_matrix(int col);
        void print_visibility_matrix();
    };

#endif
