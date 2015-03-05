/* 
 * File:   AcsAlgorithm.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__ACSALGORITHM_HPP__)
    #define  __ACSALGORITHM_HPP__

    #include "ControlSTACS.hpp"

    #include "DoubleMatrix.hpp"
    #include "Utilities.hpp"
    #include "Random.hpp"
    #include "MtspSolution.hpp"
    #include "MtspNearestNeighbor.hpp"

    #include <iostream>
    #include <fstream>
    #include <cmath>
    #include <iomanip>
    using namespace std;

    class AcsAlgorithm{
        // variáveis do ambiente:
        int depot;
        int n;                      // número de nós da instância
        int m;                      // número de caixeiros
        DoubleMatrix* cost_matrix;       // ponteiro para a matriz de distâncias
        DoubleMatrix* pheromone_matrix;  // ponteiro para a matriz de feromônio

        // variáveis locais: 
        double tau0;                // qtde inicial de feromônio
        double Cnn;                 // custo da solução nearest neighbor (total ou longest, de acordo com o objetivo)
        Utilities* util;            // funções auxiliares
        double* probability;        // vetor com os numeradores: tau^alfa * eta^beta

    public:
        AcsAlgorithm(int n, int m, DoubleMatrix* &cost_matrix, DoubleMatrix* &pheromone_matrix);
        ~AcsAlgorithm();

        // inicializa a matriz de feromônio
        void initialize_pheromone_matrix(IntList* &valid_nodes_instance);

        // retorna o nó escolhido
        // recebe o nó atual da formiga 
        int state_transation_rule(int current_node, IntList* &candidate_list, Random* &rand);

        // atualização de feromônio realizada logo após a formiga se mover para um novo nó:
        void local_pheromone_update(int i, int j);

        // atualização de feromônio realizada ao final de cada ciclo (N soluções no ACS)
        void global_pheromone_update(MtspSolution* &best_so_far_sol);
        double cnn();

        void print_pheromone_matrix();

        void save_parameters(const char* &file_out);
    };

#endif
