/* 
 * File:   Route.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__ROUTE_HPP__)
    #define __ROUTE_HPP__

    #include "IntList.hpp"

    class Route{
        int max_size;
        IntList *route_list;
        double total_cost;
        double temporal_cost; // apenas em experimentos com dados reais
    public:
        Route();
        Route(int size);
        void create(int size);
        ~Route();

        void reset();
        void add_node(int node, double cost);
        void sort(int init_node, int type_solution);  // ordena a rota para começar e terminar em init_node

        double cost();
        int last_node();
        int n_nodes();
        int node(int ind);
        bool is_empty();

        // polimorfismo específico para experimento com dados reais:
        void add_node(int node, double cost, double temp_cost);
        void add_to_temporal_cost(double time_cost);
        double get_temporal_cost();
        
        void print();
        void print_short();
        void save_short(ofstream& file_out);
    };

#endif
