/* 
 * File:   Ant.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__ANT_HPP__)
#define __ANT_HPP__

    #include "ControlExperiment.hpp"
    #include "ControlSTACS.hpp"
    #include "Route.hpp"

    #include <iostream>
    using namespace std;

    

    class Ant{
        int depot;
        int start_node;  // nó de início da rota da formiga
        int curr_node;   // nó em que a formiga se encontra
        bool dep_visit;  // a formiga já incluiu o depósito na sua rota?
        Route* route;    // rota executada pela formiga
    public:
        Ant();
        void create(int n);
        ~Ant();

        int starting_node();
        int current_node();
        bool depot_visited();
        double cost_route();
        int size_route();
        int node_route(int index);

        void reset (int starting_node);
        void move(int next_node, double cost);
        void sort_route(int type_solution);

        void print_route();
        void print_ant(int id_ant);
    };

#endif
