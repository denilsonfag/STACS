/* 
 * File:   Ant.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "Ant.hpp"

    Ant::Ant(){}
    void Ant::create(int n){
        depot = DEPOT_INDEX;
        route = new Route(n+1);  // tamanho máximo para m=1 com retorno ao depósito        
    }
    Ant::~Ant(){
        delete route;
    }
    
    int Ant::starting_node(){ return start_node; }
    int Ant::current_node(){ return curr_node; }
    bool Ant::depot_visited(){ return dep_visit; }
    double Ant::cost_route(){ return route->cost(); }
    int Ant::size_route(){ return route->n_nodes(); }
    int Ant::node_route(int index) { return route->node(index); }
    
    void Ant::reset (int starting_node){
        start_node = starting_node;
        curr_node = start_node;
        dep_visit = false;
        if (start_node == depot) dep_visit = true;  // o depósito pode ser o nó de partida
        route->reset();  // reiniciando a rota da formiga 
        route->add_node(start_node, 0.0);  // adicionando nó inicial
    }
    void Ant::move(int next_node, double cost){
        curr_node = next_node;
        if (next_node == depot) dep_visit = true; // verificando se a formiga está se movendo para o depósito
        route->add_node(next_node, cost);
    }
    void Ant::sort_route(int type_solution){
        route->sort(depot, type_solution);
    }
    
    void Ant::print_route(){
        route->print_short();
    }
    void Ant::print_ant(int id_ant){
        cout << "ant:" << id_ant;
        cout << "  n_star:" << start_node;
        cout << "  n_cur:" << curr_node;
        cout << "  d_vis:" << dep_visit;
    }
