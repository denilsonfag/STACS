/* 
 * File:   MtspInstance.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "MtspInstance.hpp"

MtspInstance::MtspInstance(int n_salesmen, int depot){
    this->n_salesmen = n_salesmen;
    this->depot = depot;
    graph =  new EuclideanGraph();
    valid_nodes = new IntList(1);
    initial_nodes_routes = new IntList(1);
    cost_matrix = new DoubleMatrix(1);
}
MtspInstance::~MtspInstance(){
    delete cost_matrix;
    delete initial_nodes_routes;
    delete valid_nodes;
    delete graph;
}
int MtspInstance::get_n_nodes() {
    return graph->n_nodes_graph();
}
int MtspInstance::get_n_salesmen(){
    return initial_nodes_routes->n_items();
}
IntList* MtspInstance::get_positions_teams(){
    return initial_nodes_routes;
}
IntList* MtspInstance::get_valid_nodes_instance(){
    return valid_nodes;
}
DoubleMatrix* MtspInstance::get_cost_matrix(){
    return cost_matrix;
}
Node* MtspInstance::get_plan_nodes_vector(){
    return graph->get_nodes_vector();
}

int MtspInstance::get_n_valid_nodes(){
    return valid_nodes->n_items();
}
int MtspInstance::get_valid_node(int index){
    return valid_nodes->value(index);
}

void MtspInstance::load_euclidean_model_instance(){
    
    // carregando uma instância modelo em um objeto que representa um grafo euclidiano
    graph->load_model_graph();
    int n = graph->n_nodes_graph();
    
    // todos os nós estão disponíveis e são sequenciais nas instâncias modelo
    delete valid_nodes;
    valid_nodes = new IntList(n);
    for (int i=0; i < n; i++ ){
        valid_nodes->add(i);
    }
    
    // definindo o tipo da solução que será gerada definindo positions_teams
    delete initial_nodes_routes;
    initial_nodes_routes = new IntList(n_salesmen);
    for (int k=0; k < n_salesmen; k++){
        if (TYPE_MTSP_SOLS == 1){
            initial_nodes_routes->add(depot); // rotas fechadas, todas partem do depósito
        }
        else{
            initial_nodes_routes->add(k+1);   // rotas abertas com finais fixos
        }
    }
    
    // preenchendo a matriz de custos com as distância euclidianas
    delete cost_matrix;
    cost_matrix = new DoubleMatrix(n);
    for (int i=0; i < n; i++ ){
        for (int j=0; j < n; j++ ){
            double cost = graph->get_value_euclidean_matrix(i,j);
            cost_matrix->set_value(i,j,cost);
        }
    }   
}

void MtspInstance::create_empty_instance(int n_nodes, int n_teams){
    graph->create_empty_graph(n_nodes);
    
    delete valid_nodes;
    valid_nodes = new IntList(n_nodes);
    
    delete initial_nodes_routes;
    initial_nodes_routes = new IntList(n_teams);
    
    delete cost_matrix;
    cost_matrix = new DoubleMatrix(n_nodes);
}
void MtspInstance::add_plan_node(int coord_x, int coord_y){
    graph->add_vertex(coord_x, coord_y);
}
void MtspInstance::calcule_euclidean_matrix(){
    graph->calcule_euclidean_distances_div1000();
}
double MtspInstance::get_value_euclidean_matrix(int i, int j){
    return graph->get_value_euclidean_matrix(i,j);
}
double MtspInstance::get_value_cost_matrix(int i, int j){
    return cost_matrix->get_value(i,j);
}
void MtspInstance::set_value_cost_matrix(int i, int j, double cost){
    cost_matrix->set_value(i, j, cost);
}
void MtspInstance::reset_valid_nodes_instance(){
    valid_nodes->empty();
}
void MtspInstance::add_valid_node(int node){
    valid_nodes->add(node);
}
void MtspInstance::reset_positions_teams(){
    initial_nodes_routes->empty();
}
void MtspInstance::add_position_team(int position){
    initial_nodes_routes->add(position);
}

void MtspInstance::print_instance(){
    cout << "\r\nCurrent instance:\r\n";
    cout << "  valid nodes: "; valid_nodes->print(); cout << "\r\n";
    cout << "  positions_teams: "; initial_nodes_routes->print(); cout << "\r\n";
}
void MtspInstance::print_valid_nodes(){
    cout << "Valid nodes: "; valid_nodes->print(); cout << "\r\n";
}
void MtspInstance::print_positions_teams(){
    cout << "Posições das equipes: "; initial_nodes_routes->print(); cout << "\r\n";
}
void MtspInstance::print_cost_matrix(){
    cost_matrix->print_matrix();
}
void MtspInstance::print_euclidean_matrix(){
    graph->print_euclidean_matrix();
}

void MtspInstance::save_positions_teams(ofstream& file_out){
    file_out << "Posições das equipes: : "; 
    initial_nodes_routes->save(file_out); 
    file_out << "\r\n";
}
