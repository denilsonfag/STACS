/* 
 * File:   TacoAlgorithm.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "TacoAlgorithm.hpp"

    TacoAlgorithm::TacoAlgorithm (int m, DoubleMatrix* &cost_matrix, Ant* &ants){
        this->m = m;
        this->cost_matrix = cost_matrix;
        this->ants = ants;
    }
    
    // retorna o índice da formiga com menor rota parcial
    int TacoAlgorithm::select_ant(){
        int selected;
        double less_cost;
        for (int k=0; k < m; k++){
            if (k==0) {
                selected = k;
                less_cost = ants[k].cost_route();
            }
            else{
                if (ants[k].cost_route() < less_cost){
                    selected = k;
                    less_cost = ants[k].cost_route();
                }
            }
        }
        return selected;
    }
    
    // retorna o índice da formiga que resulta no melhor movimento
    int TacoAlgorithm::check_better_ant(int next_node){
        int better_ant;
        double better_predict_route;
        double predict_route;
        for (int k=0; k < m; k++){
            predict_route = this->predict_route(k, next_node);
            if (k == 0){
                better_ant = k;
                better_predict_route = predict_route;
            }
            else{
                if (predict_route < better_predict_route){
                    better_ant = k;
                    better_predict_route = predict_route;
                }
            }
        }
        return better_ant;
    }
    
    // calcula a previsão da rota de uma formiga caso adicione o nó escolhido
    double TacoAlgorithm::predict_route(int k, int next_node){
        // rota parcial da formiga em análise:
        double part_route = ants[k].cost_route();
        // distância entre o nó atual da formiga em análise e o próximo nó escolhido:
        double dist_choosed = cost_matrix->get_value(ants[k].current_node(),next_node);
        // distância entre o próximo nó e o nó de partida da formiga em análise:
        double dist_starting = cost_matrix->get_value(next_node, ants[k].starting_node());
        return part_route + dist_choosed + dist_starting;
    }
