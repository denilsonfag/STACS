/* 
 * File:   AcsAlgorithm.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "AcsAlgorithm.hpp"

    AcsAlgorithm::AcsAlgorithm(int n, int m, DoubleMatrix* &cost_matrix, DoubleMatrix* &pheromone_matrix){
        depot = DEPOT_INDEX;
        this->n = n;
        this->m = m;
        this->cost_matrix = cost_matrix;
        this->pheromone_matrix = pheromone_matrix;
        util = new Utilities();
        probability = new double[n-1];  // vetor com os numeradores: tau^alfa * eta^beta
    }
    AcsAlgorithm::~AcsAlgorithm(){
        delete [] probability;
        delete util;
    }
    
    // inicializa a matriz de feromônio
    void AcsAlgorithm::initialize_pheromone_matrix(IntList* &valid_nodes_instance){
        // criando a solução do vizinho mais próximo:
        MtspNearestNeighbor *nn_app = new MtspNearestNeighbor(n, m); // o depósito é o nó zero do vetor de vértices
        nn_app->create_solution_workload_balance(cost_matrix, valid_nodes_instance);
        if (APP_OBJECTIVE == 1){ Cnn = nn_app->larger_route_sol(); }  // minimizar a longest_route
        else{ Cnn = nn_app->total_sol(); }  // minimizar a soluçao total
        delete nn_app;
        
        tau0 = 1/(n*Cnn);  // dorigo04, pg 71
        
        for (int i=0; i < n; i++){
            for (int j=0; j < n; j++){
                pheromone_matrix->set_value(i,j,tau0);
            }
        }
    }
    
    // retorna o nó escolhido
    // recebe o nó atual da formiga 
    int AcsAlgorithm::state_transation_rule(int current_node, IntList* &candidate_list, Random* &rand){
        int choosed_node = -1;  // nó escolhido
        int n_candidates = candidate_list->n_items(); // qtde de nós candidatos
        
        // definindo a probabilidade para todos os nós candidatos:
        for (int i=0; i < n_candidates; i++){  // índice da candidate_list e probability
            int candidate_node = candidate_list->value(i); // nó candidato
            double trail = pheromone_matrix->get_value(current_node,candidate_node);
            double cost = cost_matrix->get_value(current_node,candidate_node);
            
            // para valores 0.0 na matriz de custos:
            if (cost == 0.0) cost = 0.00000000000000000001;
            
            double visibility = 1 / cost;
            probability[i] = pow(trail, ALFA) * pow(visibility, BETA);
            
//            if (probability[i] > 9999999999){
//                cout << "\r\n\r\n---->>>> PROBABILIDADE INFINITA\r\n";
//                cout << "vertice atual: " << current_node << "\r\n";
//                cout << "vertice candidato: " << candidate_node << "\r\n";
//                cout << "feromonio: " << trail << "\r\n";
//                cout << "custo: " << visibility << "\r\n";
//                cout << "visibilidade: " << visibility << "\r\n";
//            }
            
        }
        
        // psedorandom proportional rule:
        double q = rand->random_number();
        if (q <= Q0){
            int larger_prob = util->argmax(n_candidates, probability);   // nó com maior probabilidade
            choosed_node = candidate_list->value(larger_prob);
        }
        else{
            int raffled_prob = rand->raffle_double_vector(n_candidates, probability); // nó sorteado, de acordo com as probabilidades
            if ((raffled_prob < 0) || (raffled_prob >= n_candidates)){  // ERRO NO SORTEIO 
                cout << "\r\n\r\n---->>>> ERRO NO SORTEIO\r\n";
                cout << "vertice atual: " << current_node << "\r\n";
                cout << "lista de candidatos: "; candidate_list->print(); cout << "\r\n";
                cout << "probabilidades: \r\n";
                for (int aux = 0; aux < n_candidates; aux++){
                    cout << aux << "\t" << probability[aux] << "\r\n";
                }
                cout << "\r\nmatriz de custos: "; cost_matrix->print_matrix(); cout << "\r\n";
                cout << "\r\nmatriz de feromonio: "; pheromone_matrix->print_matrix(); cout << "\r\n";
                exit(4);
            }
            else {  // sorteio OK, procedimento normal
                choosed_node = candidate_list->value(raffled_prob);
            }
        }
        return choosed_node;
    }
    
    // atualização de feromônio realizada logo após a formiga se mover para um novo nó:
    void AcsAlgorithm::local_pheromone_update(int i, int j){
        double current_tau = pheromone_matrix->get_value(i,j);
        double new_tau = ((1-KSI)*current_tau) + (KSI*tau0);  // fórmula de atualização local do ACS
        if (!((GPUNODEP) && ((i == depot) || (j == depot)))){ // verificando se está sendo depositado feromônio nas arestas que conectam ao depósito
            pheromone_matrix->set_value(i,j,new_tau);
        }
        
        if(PPLU) {
            cout <<  "\r\nlocal update:   "; 
            this->print_pheromone_matrix();
        }
    }
    
    // atualização de feromônio realizada ao final de cada ciclo (N soluções no ACS)
    void AcsAlgorithm::global_pheromone_update(MtspSolution* &best_so_far_sol){
        int n_nodes = best_so_far_sol->n_nodes();
        double longest_route = best_so_far_sol->get_longest_route();
        double total_sol = best_so_far_sol->get_total_cost();
        double best_so_far;
        
        if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
            best_so_far = longest_route;
        }
        else{  // atualizar pelo total da solução
            best_so_far = total_sol;
        }
        
        double delta_tau = 1 / (m * best_so_far);  // fórmula da atualização global do ACS (1)
        
        for (int i=1; i<n_nodes; i++){
            int a = best_so_far_sol->node(i-1);
            int b = best_so_far_sol->node(i);
            double current_tau = pheromone_matrix->get_value(a,b);
            
            double new_tau = ((1-RO)*current_tau) + (RO*delta_tau); // fórmula da atualização global do ACS (2)
            
            if (!((GPUNODEP) && ((a == depot) || (b == depot)))){
//-----                cout << "a = " << a << "   b = " << b << "\r\n";
                pheromone_matrix->set_value(a,b,new_tau);
            }
        }
        
        if(PPGU) {
            cout << "\r\n---> best_so_far_sol:   ";
            best_so_far_sol->print();
            cout <<  "\r\n---> global update:   "; 
            this->print_pheromone_matrix();
        }
    }
    
    double AcsAlgorithm::cnn() { return Cnn; }
    
    void AcsAlgorithm::print_pheromone_matrix(){
        streamsize prec = cout.precision();
        cout << setprecision(10);
        cout << "pheromone_matrix (porcentagem de tau0)     tau0: " << tau0 << "\r\n"; 
        cout << setprecision(0);
        for (int i=0; i < n; i++){
            cout << "\t" << i; 
        }
        for (int i=0; i < n; i++){
            cout << "\r\n" << i;
            for (int j=0; j < n; j++){
                double current_pher = pheromone_matrix->get_value(i,j); 
                if(current_pher == tau0){
                    cout << "\t =";
                }    
                else{
                    cout << "\t" << (current_pher / tau0) * 100 << "%";    
                }
            }
        }
        cout << "\r\n";
        cout << setprecision(prec);
    }
    