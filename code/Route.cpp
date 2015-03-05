/* 
 * File:   Route.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "Route.hpp"

    Route::Route(){}
    Route::Route(int size){
        max_size = size;
        route_list = new IntList(max_size); 
        total_cost = 0.0;
        temporal_cost = 0.0;
    }
    void Route::create(int size){
        max_size = size;
        route_list = new IntList(max_size); 
        total_cost = 0.0;
        temporal_cost = 0.0;
    }
    Route::~Route(){
        delete route_list;
    }
    
    void Route::reset(){
        route_list->empty();
        total_cost = 0.0;
        temporal_cost = 0.0;
    }
    void Route::add_node(int node, double cost){
        route_list->add(node);
        total_cost = total_cost + cost;
    }
    
    void Route::sort(int init_node, int type_solution){ // ordena a rota para começar e terminar em init_node
        int n_nodes = route_list->n_items();
        int *old_route = new int[n_nodes]; 
        for (int i=0; i<n_nodes; i++){ // copiando a rota para old_route
            old_route[i] = route_list->value(i);
        }
        route_list->empty();  // resetando a rota
        int i_init_route;  // indica a posição do depósito na rota original
        bool flag = false;
        for (int i=0; i < n_nodes; i++){ 
            if (flag){
                route_list->add(old_route[i]);
            }
            else{
                if (old_route[i] == init_node){  // encontrou o depósito
                    route_list->add(old_route[i]); // o depósito é o primeiro da rota 
                    i_init_route = i; // grava a posição do depósito
                    flag = true;  // inicia gravação dos outros nós
                }
            }
        }
        // copiando os nós do início da rota:
        int ind;
        if (type_solution == 1) ind = 1;  // o primeiro nó é igual ao último, por isso i=1
        if (type_solution == 2) ind = 0;
        for (int i = ind; i < i_init_route; i++){  
            route_list->add(old_route[i]);
        }
        // para rotas que já estiverem ordenadas:
        if (type_solution == 1){
            if (old_route[0] != init_node) route_list->add(init_node);
        }  
          
        delete [] old_route;
    }
    
    double Route::cost(){ return total_cost; }
    int Route::last_node(){ return route_list->last_value(); }
    int Route::n_nodes(){ return route_list->n_items(); }
    int Route::node(int ind) {return route_list->value(ind); }

    bool Route::is_empty(){
        if (route_list->is_empty()) return true;
        else return false;
    }
    
    
    // polimorfismo específico para experimento com dados reais:
    void Route::add_node(int node, double cost, double temp_cost){
        route_list->add(node);
        total_cost = total_cost + cost;
        temporal_cost = temporal_cost + temp_cost;
    }
    
    void Route::add_to_temporal_cost(double time_cost){
        temporal_cost = temporal_cost + time_cost;
    }
    double Route::get_temporal_cost(){
        return temporal_cost;
    }
    
    
    void Route::print(){
        for (int i=0; i<route_list->n_items(); i++){
            cout << i+1 << "\t";
            cout << route_list->value(i) << "\r\n";
        }
        cout << "--> cost route: " << total_cost << "\r\n";
    }
    void Route::print_short(){
        int size_route = route_list->n_items();
        cout << "\tpontos visitados: ";
        cout << size_route;
        cout << "\tcusto total: ";
        cout << total_cost;
        cout << "\tcusto temporal: ";
        cout << temporal_cost;
        cout << "\trota: ";
        route_list->print();
        cout << "\r\n";
    }
    void Route::save_short(ofstream& file_out){
        int size_route = route_list->n_items();
        file_out << "\tpontos visitados: ";
        file_out << size_route;
        file_out << "\tcusto total: ";
        file_out << total_cost;
        file_out << "\tcusto temporal: ";
        file_out << temporal_cost;
        file_out << "\t\trota: ";
        route_list->save(file_out);
        file_out << "\r\n";
    }
    