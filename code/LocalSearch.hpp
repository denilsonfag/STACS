/* 
 * File:   LocalSearch.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__LOCALSEARCH_HPP__)
    #define __LOCALSEARCH_HPP__

    #include "ControlExperiment.hpp"

    #include "DoubleMatrix.hpp"
    #include "MtspInstance.hpp"
    #include "MtspSolution.hpp"
    #include "Intersection.hpp"

    class LocalSearch{
        int depot;
        DoubleMatrix* cost_matrix;
        double time_ini_execution;
        int size_sol;
        int index_first;
        int index_last;
    public:
        LocalSearch(double time_ini_execution, DoubleMatrix* &cost_matrix);
        ~LocalSearch();

        // realiza a busca local 2-opt e atualiza solution com a melhor encontrada
        bool two_opt(MtspSolution* &solution);
        // realiza a busca local 3-opt e atualiza solution com a melhor encontrada
        bool three_opt(MtspSolution* &solution);

        double gain_change_two_nodes(int index_a, int index_b, MtspSolution* &solution);
        double partial_cost(int before, int current, int after, MtspSolution* &solution);
    };

#endif
