/* 
 * File:   MtspSolution.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__MTSPSOLUTION_HPP__)
    #define __MTSPSOLUTION_HPP__

    #include "ControlSTACS.hpp"
    #include "Node.hpp"
    #include "IntList.hpp"
    #include "DoubleMatrix.hpp"
    using namespace std;

    class MtspSolution{
        int depot;
        int size;                  // número de nós da solução
        IntList *nodes_list;       // lista com os nós da solução ordenados
        double total_cost;         // custo da soma das rotas
        double longest_route_cost; // custo da maior rota
        int iteration;             // ciclo em que a solução foi gerada 
        double time_exec;          // tempo de execução, em milissegundos
        long random_seed;          // semente dos randômicos utilizada para criar a solução
    public:
        MtspSolution(int n, int m);
        MtspSolution(int size);
        ~MtspSolution();

        void reset();
        void add(int node, double cost);

        // inverte a solução a partir de index_start (inclusive) a index_end (inclusive) e a reacalcula
        void reverse(int index_start, int index_end,  DoubleMatrix* &cost_matrix);

        // troca os nós das posições enviadas e recalcula a solução
        void change_nodes(int index_i, int index_j, DoubleMatrix* &cost_matrix);
        // atualiza total_cost e longest_route_cost
        void recalculate_solution(DoubleMatrix* &cost_matrix);
        // retorna verdadeiro se a solução for válida:
        bool test_validate();

        int n_nodes();
        int node(int ind);
        int last_node();
        bool is_full();
        bool is_empty();

        void set_longest_route(double cost);
        void set_iteration(int c);
        void set_time(double t);
        void set_random_seed(long s);
        void set_total_cost(double t_cost);

        double get_total_cost();
        double get_longest_route();
        int get_iteration_sol();
        int get_time_sol();
        long get_seed_rand();

        int next_node(int current_node);
        
        void print();
        void save_how_list(ofstream& file_out);
        void save_to_plot(ofstream& file_out, Node*& nodes);
        void save_longest_cost(ofstream& file_out);
        void save_total_cost(ofstream& file_out);
    };

#endif
