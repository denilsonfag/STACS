/* 
 * File:   ProposedACOAlgorithm.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "AcoMtspAlgorithm.hpp"

AcoMtspAlgorithm::AcoMtspAlgorithm(long seed_random, MtspInstance* &instance){
    depot = DEPOT_INDEX;
    this->n = instance->get_n_nodes();
    this->m = instance->get_n_salesmen();
    this->time_ini_execution = (double)clock();  // instante do início da execução
    cost_matrix = instance->get_cost_matrix();
    plan_nodes_vector = instance->get_plan_nodes_vector();
    positions_teams = instance->get_positions_teams();
    valid_nodes_instance = instance->get_valid_nodes_instance();
    // criando matriz de feromônio:
    pheromone_matrix = new DoubleMatrix(n);
    // criando a colônia:
    colony = new Colony(n, m, cost_matrix);
    ants = colony->ants_colony();
    candidate_list = colony->candidate_list_colony();
    // para acessar as rotinas ACS:
    AntColonySystem = new AcsAlgorithm(n, m, cost_matrix, pheromone_matrix);
    // para acessar as rotinas TACO:
    TACOAlgorithm = new TacoAlgorithm(m, cost_matrix, ants);
    rand = new Random (seed_random);
    util = new Utilities();
    // criando as listas com os nós ordenados pelas distâncias:
    sorted_lists = new IntList[n];
    for (int i=0; i < n; i++){ sorted_lists[i].create(n); }
    this->make_sorted_lists();
    local_search = new LocalSearch(time_ini_execution, cost_matrix);
    intersec = new Intersection(n, plan_nodes_vector);
}
AcoMtspAlgorithm::~AcoMtspAlgorithm(){
    delete intersec;
    delete local_search;
    delete [] sorted_lists;
    delete util;
    delete rand;
    delete TACOAlgorithm;
    delete AntColonySystem;
    delete colony;
    delete pheromone_matrix;
}

int AcoMtspAlgorithm::update_type_solution(){
    type_solution = 1; // finais fechados
    for (int k=0; k < m; k++){
        if (positions_teams->value(k) != depot){
            type_solution = 2; // finais abertos
        }
    }
}

// execução do algoritmo proposto:
// retorna a melhor solução da execução
MtspSolution* AcoMtspAlgorithm::execute(){
    
    // 1: finais fechados; 2: finais abertos:
    this->update_type_solution();    // atualizando tipo da solução a ser gerada a partir das posições das formigas

    // inicializando a matriz de feromônio como no ACS:
    AntColonySystem->initialize_pheromone_matrix(valid_nodes_instance);
    if(PPOE) { cout <<  "\r\nInicialization:   "; AntColonySystem->print_pheromone_matrix(); } // imprimir a matriz após inicializada
    if(PDMT) cost_matrix->print_matrix();   // imprimindo a matriz de distâncias

    double current_time = 0.0;      // contador de tempo 
    bool updated_best_exec;         // sinaliza se foi encontrada uma soluçao melhor no último ciclo

    cycle_counter = 0;   // contador de ciclos 
    non_improved_counter_cycles = 0;   // contador de ciclos sem melhora da best_so_far_solution da execução
    MtspSolution* best_solution_execution; // melhor solução da execução
    
    // em cada loop são criadas N soluções:
    while ((cycle_counter != MAX_CYCLES) && (non_improved_counter_cycles != NO_IMP_CYCLES) && ((current_time <= MAX_TIME_EXEC) || (MAX_TIME_EXEC == -1))){ // os três critérios de parada

        if (PDCR) cout << "\r\n---------> iteration " << cycle_counter + 1 << "\r\n";
        
        MtspSolution* best_solution_cycle; // melhor solução do ciclo
        
        for (created_sols_counter = 0; created_sols_counter < N; created_sols_counter++){ // em cada loop é criada uma solução

            if (PDCR) cout << "\r\n------> creating solution " << created_sols_counter+1 << "\r\n";

            // constrói uma solução
            MtspSolution* created_solution = this->solution_construction(positions_teams);
            
            if (LS2O){ // 2-opt em todas as soluções criadas
                bool updated = local_search->two_opt(created_solution);
//                if (updated) cout << "\r\n  updated 2-opt";
            }
            
            // atualizando melhor solução do ciclo:
            if (created_sols_counter == 0){
                best_solution_cycle = created_solution;
            }
            else{
                double created_cost;
                double best_cost;
                if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
                    created_cost = created_solution->get_longest_route();
                    best_cost = best_solution_cycle->get_longest_route();
                }
                else{  // atualizar pelo total da solução
                    created_cost = created_solution->get_total_cost();
                    best_cost = best_solution_cycle->get_total_cost();
                }
                if (created_cost < best_cost){ // atualizar a melhor do ciclo:
                    delete best_solution_cycle;
                    best_solution_cycle = created_solution;
                }
                else{
                    delete created_solution;
                }
            }
        }

        if (LS3O){ // 3-opt nas melhores soluções dos ciclos
            bool updated = local_search->three_opt(best_solution_cycle);
//            if (updated) cout << "\r\n  updated 3-opt";
        }
        
        if(PASO){  // imprimir todas melhores soluções de cada ciclo
            if (cycle_counter == 0){ 
                cout << "\r\n---> Improved solutions:\r\niter\tlongest\t\ttotal\t\tsolution\r\n"; 
            }
            cout << "\r\n" << cycle_counter+1 << "\t"; 
            best_solution_cycle->print();
        }

        if (GPU) {  // atualização global de feromônio ativada
            if (!GPUBSF) this->global_pheromone_update(best_solution_cycle); // atualização pela melhor solução do ciclo
        }
        
        // atualizando melhor solução da execução:
        if (cycle_counter == 0){
            best_solution_execution = best_solution_cycle;
            updated_best_exec = true;
        }
        else{
            double cycle_cost;
            double best_cost;
            if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
                cycle_cost = best_solution_cycle->get_longest_route();
                best_cost = best_solution_execution->get_longest_route();
            }
            else{  // atualizar pelo total da solução
                cycle_cost = best_solution_cycle->get_total_cost();
                best_cost = best_solution_execution->get_total_cost();
            }
            if (cycle_cost < best_cost){ // atualizar a melhor da execução
                delete best_solution_execution;
                best_solution_execution = best_solution_cycle;
                updated_best_exec = true;
            }
            else{
                delete best_solution_cycle;
                updated_best_exec = false;
            }
        }
        
        if (updated_best_exec){   // a solução foi atualizada
            non_improved_counter_cycles = 0;
            if(PISO){  // imprimir todas as soluções melhoradas
                if (cycle_counter == 0){ cout << "Improvement by ACO Algorithm:\r\n"; }
                cout << "  cycle: " << cycle_counter+1 << "  \t"; best_solution_execution->print();
            }
        }
        else{  // a solução não foi atualizada
            non_improved_counter_cycles++;
        }

        if (GPU) {  // atualização global de feromônio ativada
            if (GPUBSF) this->global_pheromone_update(best_solution_execution); // atualização pela best_so_far
        }

        // atualizando contador de iterações e de tempo:
        cycle_counter++;
        current_time = ((double)clock() - time_ini_execution)/CLOCKS_PER_SEC;

        if (PPOE){  // imprimir a matriz de feromônio ao final da execução
            cout <<  "\r\n---> Final state pheromone matrix:   "; 
            this->print_pheromone_matrix();
        }
    }
    return best_solution_execution;
}

// construção de uma solução:
MtspSolution* AcoMtspAlgorithm::solution_construction(IntList* &positions_teams){

    colony->empty_visited_list();   // esvaziando a lista de nós visitados

    colony->add_visited_list_no_valid_nodes(valid_nodes_instance);  // adicionando os nós que não são válidos à visited_list

    this->reinitialize_ants(positions_teams);  // resetando as formigas (positions_teams são adicionados à visited_list)

    while (!colony->is_full_visited_list()){   // enquanto a visited_list não estiver cheia

        moving_ant = TACOAlgorithm->select_ant();  // selecionando a formiga para se mover como val08 (menor rota parcial)

        current_node = ants[moving_ant].current_node();  // atualizando current_node

        if (PDCR){  // imprimir detalhes de construção das rotas
            cout << "\r\ncurr_ant: " << moving_ant << "   curr_node: " << current_node;
            cout << "     partial route:   "; ants[moving_ant].print_route();
            cout << "vist_list: "; colony->print_visited_list(); cout << "\r\n";
        }

        bool end_repeat = false;  // flag da repetição pela busca de uma formiga melhor
        int repeating = -1;       // contagem das repetições por uma formiga melhor

        // enquanto não for o melhor movimento
        while(!end_repeat){  
            repeating ++;

            // atualizando a lista de nós candidatos para o nó no qual a formiga escolhida se encontra:
            colony->update_candidate_list(type_solution, moving_ant, sorted_lists);

            #if PDCR
                cout << "cand_list: "; colony->print_candidate_list(); cout << "\r\n"; 
            #endif

            next_node = AntColonySystem->state_transation_rule(current_node, candidate_list, rand);  // escolha do próximo nó como no ACS

            if (PDCR){cout << "choosed_node: " << next_node << "\r\n";}

            // se REPCH == 0, não checa nenhuma vez; se REPCH == 1, igual a val08
            if (repeating < CBA_REPET) {
                // retorna a formiga que resulta no melhor movimento:
                int better_ant = TACOAlgorithm->check_better_ant(next_node);
                if (moving_ant == better_ant) {  // é o melhor movimento
                    end_repeat = true;
                }
                else{  // atualizando para a melhor formiga
                    moving_ant = better_ant;
                    current_node = ants[moving_ant].current_node();  // atualizando current_node
                }
            }
            else{
                end_repeat = true;  // alcançou limite de repetições
            }
        }

        colony->movement(moving_ant, next_node);  // movimenta a formiga escolhida para o nó escolhido
        if (LPU) AntColonySystem->local_pheromone_update(current_node, next_node);  // atualização local de feromônio como no ACS

    }

    // finalizando as rotas da formigas:
    this->finalize_routes_ants();

    // imprimir todas as rotas criadas:
    if (PART){ 
        cout << "\r\n---> criated routes:\r\n"; 
        for (int k=0; k<m; k++){ 
            cout << "ant: " << k << "   "; 
            ants[k].print_route(); 
        }
    }
    
    // criar e montar a solução de retorno:
    MtspSolution* created_solution = new MtspSolution(n, m);
    if (type_solution == 1) { // closed ends
        // organizando todas as rotas para iniciarem e terminarem no depósito:
        for (int k=0; k < m; k++){
            ants[k].sort_route(type_solution);
        }
    }
    // montando a solução com as rotas das formigas:
    double dist;
    for(int k=0; k < m; k++){
        int n_nodes = ants[k].size_route();
        for (int i=0; i < n_nodes; i++){
            int current_node_ant = ants[k].node_route(i);
            
            if (type_solution == 1){
                if ((k==0)&&(i==0)) {  // adicionando o depósito como primeiro nó
                    created_solution->add(depot , 0.0);
                }
                if ((current_node_ant != depot) || (created_solution->last_node() != depot)){
                    int node_i = created_solution->last_node();
                    dist = cost_matrix->get_value(node_i, current_node_ant);
                    created_solution->add(current_node_ant, dist);
                }
            }
            
            if (type_solution == 2){
                if (i==0) {
                    created_solution->add(current_node_ant, 0.0);
                }
                else{
                    int node_i = created_solution->last_node();
                    dist = cost_matrix->get_value(node_i,current_node_ant);
                    created_solution->add(current_node_ant, dist);
                }
            }

        }
    }
    // incluindo dados finais:
    double longest_route_current = colony->longest_route_curr();
    created_solution->set_longest_route(longest_route_current);
    created_solution->set_iteration(cycle_counter+1);
    created_solution->set_time((((double)clock() - time_ini_execution)/CLOCKS_PER_SEC)*1000); // em milisegundos
    created_solution->set_random_seed(rand->seed_used());
    
    return created_solution;
}

// executado no início da construção de cada solução:
void AcoMtspAlgorithm::reinitialize_ants(IntList* &positions_teams){
    if (type_solution == 1){ // opções apenas para soluções com rotas fechadas
        if (ANTS_INIT_NODES == 1) colony->reset_ants_depot();         // posiciona todas as formigas no depósito
        if (ANTS_INIT_NODES == 2) colony->reset_ants_random(rand);    // posiciona as formigas em nós inicias aleatórios
        if (ANTS_INIT_NODES == 3) colony->reset_ants_by_limit(rand, created_sols_counter, cycle_counter, non_improved_counter_cycles);  // nós aleatórios, alterados a cada CHANGE_INI_NODES iterações sem melhora
    }
    if (type_solution == 2) colony->reset_ants_positions_teams(positions_teams); // posiciona todas as formigas nos nós iniciais das equipes
}

// finaliando as rotas de todas as formigas:
void AcoMtspAlgorithm::finalize_routes_ants(){
    this->check_depot();  // caso alguma formiga não tenha visitado o depósito, ele é incuído no final da rota
    // se a solução for com rotas abertas e as formigas partiram das posições das equipes, a solução está pronta
    if (type_solution == 1){ // rotas fechadas: retornar aos nós iniciais
        for (int k=0; k<m; k++){ 
            current_node = ants[k].current_node();
            int starting_node = ants[k].starting_node();
            double cost = cost_matrix->get_value(current_node,starting_node);
            ants[k].move(starting_node, cost);
            if (LPU) AntColonySystem->local_pheromone_update(current_node, starting_node);
        }
    }
}

// adiciona o depósito em rotas em que ele não foi visitado:
void AcoMtspAlgorithm::check_depot(){
    for (int k=0; k < m; k++){
        if (!ants[k].depot_visited()){  // o depósito não foi visitado
            current_node = ants[k].current_node();
            double cost = cost_matrix->get_value(current_node,depot);
            ants[k].move(depot, cost);
            if (LPU) AntColonySystem->local_pheromone_update(current_node, depot);
        }
        else{
            // a solução é com finais abertos mas há formigas cujo nó inicial é o depósito. Então, deve retornar para o depósito.
            if ((type_solution == 2) && (ants[k].node_route(0) == depot)){
                current_node = ants[k].current_node();
                double cost = cost_matrix->get_value(current_node,depot);
                ants[k].move(depot, cost);
                if (LPU) AntColonySystem->local_pheromone_update(current_node, depot);
            }
        }
    }
}

// atualização global de feromônio:
void AcoMtspAlgorithm::global_pheromone_update(MtspSolution* &solution){
    AntColonySystem->global_pheromone_update(solution);
}

// funcões auxiliares:
void AcoMtspAlgorithm::make_sorted_lists(){  // cria listas com os índices ordenados pelos valores da matriz de custos
    for (int i=0; i<n; i++){  // nó de partida
        sorted_lists[i].empty();
        double* col_cost_matrix = cost_matrix->get_col_matrix(i);
        
        util->sort_double(col_cost_matrix, sorted_lists[i]);
        
    }
}
void AcoMtspAlgorithm::print_sorted_lists(){
    for (int i=0; i<n; i++){ 
        cout << i << ": ";
        sorted_lists[i].print();
        cout << "\r\n";
    }
}
void AcoMtspAlgorithm::print_pheromone_matrix(){
    AntColonySystem->print_pheromone_matrix();
}

/*
// atualiza a melhor solução da iteração montando as rotas das formigas (closed_ends):
void AcoMtspAlgorithm::update_best_sol_iteration_closed_ends(MtspSolution* &best_sol_cycle){
    double best_cost;
    double current_cost;

    double longest_route_current = colony->longest_route_curr();
    double total_sol_current = colony->sum_route_curr();

    if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
        current_cost = longest_route_current;
        best_cost = best_sol_cycle->get_longest_route();
    }
    else{  // atualizar pelo total da solução
        current_cost = total_sol_current;
        best_cost = best_sol_cycle->get_total_cost();
    }

    if (best_sol_cycle->is_empty() || current_cost < best_cost){  // atualizar best_sol_cycle
        // organizando todas as rotas para iniciarem e terminarem no depósito:
        for (int k=0; k < m; k++){
            ants[k].sort_route(type_solution);
        }
        // montando a solução com as rotas das formigas:
        int type_sol = best_sol_cycle->type_sol();
        best_sol_cycle->reset(type_sol);
        double dist;
        for(int k=0; k < m; k++){
            int n_nodes = ants[k].size_route();
            for (int i=0; i < n_nodes; i++){
                int current_node_ant = ants[k].node_route(i);
                if ((k==0)&&(i==0)) {  // adicionando o depósito como primeiro nó
                    best_sol_cycle->add(depot , 0.0);
                }
                if ((current_node_ant != depot) || (best_sol_cycle->last_node() != depot)){
                    int node_i = best_sol_cycle->last_node();
                    dist = cost_matrix->get_value(node_i, current_node_ant);
                    best_sol_cycle->add(current_node_ant, dist);
                }
            }
        }
        // incluindo dados finais:
        best_sol_cycle->set_longest_route(longest_route_current);
        best_sol_cycle->set_iteration(cycle_counter+1);
        best_sol_cycle->set_time((((double)clock() - time_ini_execution)/CLOCKS_PER_SEC)*1000); // em milisegundos
        best_sol_cycle->set_random_seed(rand->seed_used());
    }
}
// atualiza a melhor solução da iteração montando as rotas das formigas (open_ends):
void AcoMtspAlgorithm::update_best_sol_iteration_open_ends(MtspSolution* &best_sol_cycle){
    double best_cost;
    double current_cost;

    double longest_route_current = colony->longest_route_curr();
    double total_sol_current = colony->sum_route_curr();

    if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
        current_cost = longest_route_current;
        best_cost = best_sol_cycle->get_longest_route();
    }
    else{  // atualizar pelo total da solução
        current_cost = total_sol_current;
        best_cost = best_sol_cycle->get_total_cost();
    }

    if (best_sol_cycle->is_empty() || current_cost < best_cost){  // atualizar best_sol_cycle

        // montando a solução com as rotas das formigas:
        int type_sol = best_sol_cycle->type_sol();
        best_sol_cycle->reset(type_sol);
        double dist;
        for(int k=0; k < m; k++){
            int n_nodes = ants[k].size_route();
            for (int i=0; i < n_nodes; i++){
                int current_node_ant = ants[k].node_route(i);
                if (i==0) {
                    best_sol_cycle->add(current_node_ant, 0.0);
                }
                else{
                    int node_i = best_sol_cycle->last_node();
                    dist = cost_matrix->get_value(node_i,current_node_ant);
                    best_sol_cycle->add(current_node_ant, dist);
                }
            }
        }

        // incluindo dados finais:
        best_sol_cycle->set_longest_route(longest_route_current);
        best_sol_cycle->set_iteration(cycle_counter+1);
        best_sol_cycle->set_time((((double)clock() - time_ini_execution)/CLOCKS_PER_SEC)*1000); // em milisegundos
        best_sol_cycle->set_random_seed(rand->seed_used());
    }
}
// atualiza a melhor solução da execução:
bool AcoMtspAlgorithm::update_best_sol_exec(MtspSolution* &best_sol_exec){
    bool updated;
    double best_cost_exec;
    double best_cost_cycle;
    if (APP_OBJECTIVE == 1){  // atualizar pela longest_route 
        best_cost_exec = best_sol_exec->get_longest_route();
        best_cost_cycle = best_cur_sol_cycle->get_longest_route();
    }
    else{  // atualizar pelo total da solução
        best_cost_exec = best_sol_exec->get_total_cost();
        best_cost_cycle = best_cur_sol_cycle->get_total_cost();
    }
    if (best_sol_exec->is_empty() || best_cost_cycle < best_cost_exec){  // atualizar best_sol_exec
        // copiando best_sol_iteration para best_sol_exe:
        int type_sol = best_sol_exec->type_sol();
        best_sol_exec->reset(type_sol);
        // copiando nós:
        for (int i=0; i < best_cur_sol_cycle->n_nodes(); i++){
            best_sol_exec->add(best_cur_sol_cycle->node(i), 0.0);
        }
        // copiando outros dados:
        best_sol_exec->set_total_cost(best_cur_sol_cycle->get_total_cost());
        best_sol_exec->set_longest_route(best_cur_sol_cycle->get_longest_route());
        best_sol_exec->set_iteration(best_cur_sol_cycle->get_iteration_sol());
        best_sol_exec->set_time(best_cur_sol_cycle->get_time_sol());            
        best_sol_exec->set_random_seed(best_cur_sol_cycle->get_seed_rand());
        updated = true;
    }
    else{
        updated = false;
    }
    return updated;
}
*/
