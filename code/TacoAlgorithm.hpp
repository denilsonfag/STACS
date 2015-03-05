/* 
 * File:   TacoAlgorithm.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__TACOALGOR_HPP__)
    #define __TACOALGOR_HPP__

    #include "DoubleMatrix.hpp"
    #include "Ant.hpp"

    class TacoAlgorithm{
        int m;
        DoubleMatrix* cost_matrix;
        Ant* ants;
    public:
        TacoAlgorithm (int m, DoubleMatrix* &cost_matrix, Ant* &ants);

        // retorna o índice da formiga com menor rota parcial
        int select_ant();

        // retorna o índice da formiga que resulta no melhor movimento
        int check_better_ant(int next_node);

        // calcula a previsão da rota de uma formiga caso adicione o nó escolhido
        double predict_route(int k, int next_node);
    };

#endif
