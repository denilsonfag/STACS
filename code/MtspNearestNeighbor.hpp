/* 
 * File:   MtspNearestNeighbor.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__MTSPNEAREST_HPP__)
    #define __MTSPNEAREST_HPP__

    #include "ControlSTACS.hpp"
    #include "DoubleMatrix.hpp"
    #include "Route.hpp"
    #include "MtspSolution.hpp"

    class MtspNearestNeighbor{
        int n;
        int m;
        int depot;
        Route* routes;
        IntList* visited_list;
        MtspSolution* nn_solution;
        DoubleMatrix* cost_matrix;
    public:
        MtspNearestNeighbor(int n_nodes, int m_salesmen);
        ~MtspNearestNeighbor();
        void create_solution_workload_balance(DoubleMatrix* &cost_matrix, IntList* &valid_nodes_instance);
        void create_solution_one_by_one(DoubleMatrix* &cost_matrix, IntList* &valid_nodes_instance);

        // retorna a maior rota dentre todos os caixeiros da solução
        double larger_route_sol();
        // retorna o total da solução (soma das rotas)
        double total_sol();

        MtspSolution* get_nn_solution();
    };

#endif
