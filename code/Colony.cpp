/* 
 * File:   Colony.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "Colony.hpp"

    Colony::Colony(int n, int m, DoubleMatrix* &cost_matrix){
        depot = DEPOT_INDEX;
        this->n = n;
        this->m = m;
        this->cost_matrix = cost_matrix;
        // criando visited e candidate lists:
        visited_list = new IntList(n-1);  // o depósito não é incluído
        candidate_list = new IntList(n-1);  // o nó atual nunca é incluído
        // criando formigas:
        ants = new Ant[m];  // as formigas correspondem aos caixeiros
        for (int k=0; k < m; k++){
            ants[k].create(n);
        }
    }
    Colony::~Colony(){
        delete [] ants;
        delete candidate_list;
        delete visited_list;
    }
    
    // posiciona as formiga em nós inicias aleatórios não repetidos:
    void Colony::reset_ants_random(Random* &rand){
        int raffled;
        int start_node;
        for (int k=0; k<m; k++){
            bool flag = true;
            while(flag){
                raffled = rand->raffle_int(n); // retorna um inteiro aleatório entre 1 e n
                start_node = raffled - 1;  // os nós são indexados de 0 a n-1
                if (!visited_list->on_the_list(start_node)){  // se o nó não estiver na visited_list
                    if (start_node != depot) visited_list->add(start_node); // adicionando nós iniciais à lista de visitados
                    ants[k].reset(start_node);  // reiniciando formiga
                    flag = false;
                }
            }
        }
    }
    
    // posiciona todas as formigas no depósito e insere a posição das equipes na visited_list:
    void Colony::reset_ants_depot(){
        int start_node = depot;
        for (int k=0; k<m; k++){
            ants[k].reset(start_node);  // reiniciando formiga
        }
    }

    void Colony::reset_ants_by_limit(Random* &rand, int created_sols_counter, int iteration_counter, int non_improved_counter){
        if (((created_sols_counter == 0) && (iteration_counter == 0)) || ((non_improved_counter != 0) && (non_improved_counter % NO_IMP_CYCLES == 0))) {
            this->reset_ants_random(rand);
        }
        for (int k=0; k<m; k++){
            int start_node = ants[k].starting_node();
            if (!visited_list->on_the_list(start_node)){  // se o nó não estiver na visited_list
                if (start_node != depot) visited_list->add(start_node); // adicionando nós iniciais à lista de visitados
                ants[k].reset(start_node);  // reiniciando formiga
            }
        }
    }
    
    void Colony::reset_ants_positions_teams(IntList* &positions_teams){
        for (int k=0; k<m; k++){
            int start_node = positions_teams->value(k);
            ants[k].reset(start_node);  // reiniciando formiga
            if (start_node != depot){
                if (!visited_list->on_the_list(start_node)){
                    visited_list->add(start_node); // adicionando nós iniciais à lista de visitados
                }
            }
        }
    }
    
    void Colony::add_visited_list_no_valid_nodes(IntList * &valid_nodes_instance){
        for (int i=0; i<n; i++){
            if (!valid_nodes_instance->on_the_list(i)){  // se o nó não for válido, inserir na tabu_list
                if (i != depot){
                    if (!visited_list->on_the_list(i)){
                        visited_list->add(i);
                    }
                }
            }
        }
//-----         cout << "-> visited_list: "; visited_list->print(); cout << "\r\n";
    }
    
    // esvazia a lista de nós visitados:
    void Colony::empty_visited_list(){ visited_list->empty(); }
    // a visited_list está cheia?:
    bool Colony::is_full_visited_list(){
        if (visited_list->is_full()) return true;
        else return false;
    }
    // atualiza a candidate_list para a formiga em movimento:
    void Colony::update_candidate_list(int type_solution, int moving_ant, IntList* &sorted_lists){
        candidate_list->empty();
        int current_node = ants[moving_ant].current_node();
        int aux_node;
        for (int i=0; i<n; i++){
          
            aux_node = sorted_lists[current_node].value(i);
            
            if (aux_node == depot){
                if (!ants[moving_ant].depot_visited()){  // a formiga ainda não visitou o depósito
                    if (type_solution ==  1){  // o depósito pode ser adicionado "no meio" apenas para soluções com rotas fechadas
                        candidate_list->add(depot);
                    }
                }
            }
            else{
                if (!visited_list->on_the_list(aux_node)){  // aux_node ainda não foi visitado
                    
                    if (aux_node < n ){ // erro do dia de trabalho 6 na criação de sorted_lists
                        candidate_list->add(aux_node);
                    }
                    
                }
            }
            // limitando a candidate_list pelo parâmetro CL:
            if (candidate_list->n_items() == CL_LENGTH) i = n;
        }
    }
    // movimenta a formiga escolhida para o nó escolhido: 
    void Colony::movement(int moving_ant, int next_node){
        int current_node = ants[moving_ant].current_node();
        double cost = cost_matrix->get_value(current_node, next_node);
        ants[moving_ant].move(next_node, cost);
        if (next_node != depot) visited_list->add(next_node);
    }
    
    IntList* Colony::candidate_list_colony(){ return candidate_list; }
    Ant* Colony::ants_colony(){ return ants; }
    // retorna o custo atual da maior rota entre as formigas
    double Colony::longest_route_curr(){
        double l_route;
        for (int k=0; k<m; k++){
            if (k == 0){
                l_route = ants[k].cost_route();
            }
            else{
                if (ants[k].cost_route() > l_route){
                    l_route = ants[k].cost_route();
                }
            }
        }
        return l_route;
    }
    // retorna a soma atual das rotas das formigas
    double Colony::sum_route_curr(){
        double t_route = 0.0;
        for (int k=0; k<m; k++){
            t_route = t_route + ants[k].cost_route();
        }
        return t_route;
    }
    
    void Colony::print_visited_list(){ visited_list->print(); }
    void Colony::print_candidate_list(){ candidate_list->print(); }
    // imprime em detalhes a lista de nós candidatos:
    void Colony::print_detail_cands_list(int moving_ant, double** &pheromone_matrix){
        if (!candidate_list->is_empty()){
            int current_node = ants[moving_ant].current_node();
            cout << "\r\nnodes\t";
            for (int i=0; i<candidate_list->n_items(); i++){
                cout << candidate_list->value(i) << "\t";
            }
            cout << "\r\ndist\t";
            for (int i=0; i<candidate_list->n_items(); i++){
                double cost = cost_matrix->get_value(current_node, candidate_list->value(i));
                cout << cost << "\t";
            }
            cout << "\r\npher\t";
            for (int i=0; i<candidate_list->n_items(); i++){
                cout << pheromone_matrix[current_node][candidate_list->value(i)] << "\t";
            }
            cout << "\r\n";
        }
        else{
            cout << "--> empty candidate list\r\n";
        }
    }

void Colony::print_sorted_lists(IntList* &sorted_lists){
    for (int i=0; i<n; i++){ 
        sorted_lists[i].print();
        cout << "\r\n";
    }
}