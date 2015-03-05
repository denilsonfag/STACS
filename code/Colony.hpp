/* 
 * File:   Colony.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__COLONY_HPP__)
    #define  __COLONY_HPP__

    #include "DoubleMatrix.hpp"
    #include "Ant.hpp"
    #include "Random.hpp"

    class Colony{
        int depot;
        int n;
        int m;
        DoubleMatrix* cost_matrix;
        IntList* visited_list;    // lista de nós visitados da colônia
        IntList* candidate_list;  // lista de nós permitidos a uma formiga em um movimento
        Ant* ants;                // formigas da colônia
    public:
        Colony(int n, int m, DoubleMatrix* &cost_matrix);
        ~Colony();

        // posiciona as formiga em nós inicias aleatórios não repetidos:
        void reset_ants_random(Random* &rand);

        // posiciona todas as formigas no depósito e insere a posição das equipes na visited_list:
        void reset_ants_depot();

        void reset_ants_by_limit(Random* &rand, int created_sols_counter, int iteration_counter, int non_improved_counter);

        void reset_ants_positions_teams(IntList* &positions_teams);

        void add_visited_list_no_valid_nodes(IntList * &valid_nodes_instance);
        // esvazia a lista de nós visitados:
        void empty_visited_list();
        // a visited_list está cheia?:
        bool is_full_visited_list();
        // atualiza a candidate_list para a formiga em movimento:
        void update_candidate_list(int type_solution, int moving_ant, IntList* &sorted_lists);
        // movimenta a formiga escolhida para o nó escolhido: 
        void movement(int moving_ant, int next_node);

        IntList* candidate_list_colony();
        Ant* ants_colony();
        // retorna o custo atual da maior rota entre as formigas
        double longest_route_curr();
        // retorna a soma atual das rotas das formigas
        double sum_route_curr();

        void print_visited_list();
        void print_candidate_list();
        void print_sorted_lists(IntList* &sorted_lists);
        // imprime em detalhes a lista de nós candidatos:
        void print_detail_cands_list(int moving_ant, double** &pheromone_matrix);
    };

#endif
