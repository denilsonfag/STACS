/* 
 * File:   ProposedACOAlgorithm.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__ACOMTSPALGORITHM_HPP__)
    #define  __ACOMTSPALGORITHM_HPP__

    #include "DoubleMatrix.hpp"
    #include "MtspInstance.hpp"
    #include "Colony.hpp"
    #include "AcsAlgorithm.hpp"
    #include "TacoAlgorithm.hpp"
    #include "LocalSearch.hpp"

    class AcoMtspAlgorithm{  
        // variáveis do ambiente:
        int depot;
        int n;                      // número de nós da instância
        int m;                      // número de caixeiros
        double time_ini_execution;  // instante de início da execução
        DoubleMatrix* cost_matrix;  // matriz de custos
        Node* plan_nodes_vector;    // vetor com as coordenadas planas dos nós
        IntList* positions_teams;   // posiçoes atuais das equipes
        IntList* valid_nodes_instance; // índices das matrizes que fazem parte da isntância corrente

        // variávies da aplicação:
        int type_solution;             // tipo de solução que está senddo gerada, defina a partir de positions_teams
        DoubleMatrix* pheromone_matrix;// matriz de feromônio
        Colony* colony;                // colônia
        Ant* ants;                     // formigas
        IntList* candidate_list;       // lista de nós candidatos
        AcsAlgorithm* AntColonySystem; // rotinas do algoritmo Ant Colony System
        TacoAlgorithm* TACOAlgorithm;  // rotinas do algoritmo de Vallivaara 2008
        Random* rand;                     // gerador de números pseudo randômicos
        Utilities *util;                  // funções auxiliares
        IntList* sorted_lists;            // listas para todos os nós com as distâncias ordenadas
        LocalSearch* local_search;   // busca local aplicada às soluções geradas
        Intersection* intersec;      // busca local proposta que tenta retirar intersecções de soluções simétricas planas
        MtspNearestNeighbor* nn_app; // aplicação para criação da solução do vizinho mais próximo

        int moving_ant;    // formiga escolhida para se mover
        int current_node;  // nó atual da formiga escolhida para se mover
        int next_node;     // nó pra o qual a formiga escolhida se moverá

        int cycle_counter;     // contador de ciclos  
        int non_improved_counter_cycles;  // contador de ciclos sem melhora da best_so_far_solution da execução
        int created_sols_counter;  // contador de soluções criadas

    public:
        AcoMtspAlgorithm(long seed_random, MtspInstance* &instance);
        ~AcoMtspAlgorithm();

        int update_type_solution();

        // execução do algoritmo ACO para MTSP proposto
        // retorna a melhor solução encontrada
        MtspSolution* execute();

        // construção de uma solução:
        MtspSolution* solution_construction(IntList* &positions_teams);

        // executado no início da construção de cada solução:
        void reinitialize_ants(IntList* &positions_teams);

        // finaliando as rotas de todas as formigas:
        void finalize_routes_ants();

        // adiciona o depósito em rotas em que ele não foi visitado:
        void check_depot();

        // busca local para melhoramento das soluções:
        void local_optimization_2_opt(MtspSolution* &solution);
        void local_optimization_3_opt(MtspSolution* &solution);

        // atualização global de feromônio:
        void global_pheromone_update(MtspSolution* &solution);

        // funcões auxiliares:
        void make_sorted_lists();
        void print_sorted_lists();
        void print_pheromone_matrix();
        void save_parameters_ACS(const char* &name_file_out);

        // TESTES:
        // retorna falso se a aresta parâmetro não cruzar com as arestas das rotas parciais:
        bool check_intersection(int node_a, int node_b);
        // só quando a formiga atual atinge aproximadamente metade da sua rota, outra formiga é escolhida para se mover:
        int proposed_select_ant_random();
        // recebe o nó atual da formiga e retorna o nó escolhido:
        int proposed_state_transation_rule(int current_node, IntList* &candidate_list, Random* &rand);

/*
        // atualiza a melhor solução da iteração montando as rotas das formigas (closed_ends):
        void update_best_sol_iteration_closed_ends(MtspSolution* &best_sol_cycle);

        // atualiza a melhor solução da iteração montando as rotas das formigas (open_ends):
        void update_best_sol_iteration_open_ends(MtspSolution* &best_sol_cycle);

        // atualiza a melhor solução da execução:
        bool update_best_sol_exec(MtspSolution* &best_sol_exec);
*/
    };

#endif
    