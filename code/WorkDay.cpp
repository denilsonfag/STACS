/* 
 * File:   WorkDay.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "WorkDay.hpp"

WorkDay::WorkDay(int current_id_work_day){
    id_work_day = current_id_work_day;
    real_data = new RealData();
    real_matrix = new RealMatrix();
    day_services = new ServiceOrder[1];
    real_distance_matrix = new DoubleMatrix(1);
    real_time_matrix = new DoubleMatrix(1);
    current_instance = new MtspInstance(1,0);    
    
    real_solution = new MtspSolution(1,1);
    complete_final_solution = new MtspSolution(1,1);
    current_aco_solution = new MtspSolution(1,1);
    
    // arquivo para gravaçao das soluções reais para plotagem
    f_real_sols.open("outs/plot_real_sols.txt", ios::app);
    f_real_sols << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
    
    f_longests.open("outs/longests.txt", ios::app);
    f_longests << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
    
    f_total_costs.open("outs/total_costs.txt", ios::app);
    f_total_costs << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
}
WorkDay::~WorkDay(){
    f_total_costs.close();
    f_longests.close();
    f_real_sols.close();
    
    delete current_aco_solution;
    delete complete_final_solution;
    delete real_solution;
    
    delete current_instance;
    delete real_time_matrix;
    delete real_distance_matrix;
    delete [] day_services;
    delete real_matrix;
    delete real_data;
}

void WorkDay::load_data_work_day(int counter_day_simulations){

    int n_services = real_data->count_services(id_work_day);
    depot = DEPOT_INDEX;
    n_points = n_services + 1; // para incluir o depósito
    n_teams = real_data->count_teams(id_work_day); // definindo número de equipes do dia de trabalho
    
    // criando instância para preencher grafo euclidiano com coordenadas UTM
    delete current_instance;
    current_instance = new MtspInstance(n_teams, depot);
    current_instance->create_empty_instance(n_points, n_teams);
    
    // inserindo o depósito como primeiro nó do grafo
    int depot_x = real_data->x_depot();
    int depot_y = real_data->y_depot();
    current_instance->add_plan_node(depot_x, depot_y);
    
    // criando o vetor de nós e calculando a matriz de distâncias euclidianas
    real_data->load_euclidean_graph(id_work_day, current_instance);
    current_instance->calcule_euclidean_matrix(); // divididas por 1000 (UTM)
    if (PRINT_COSTS_MATRIX) { 
        current_instance->print_euclidean_matrix(); 
    }
    
    // carregando matrizes com custos reais
    delete real_distance_matrix;
    real_distance_matrix = new DoubleMatrix(n_points);
    real_matrix->load_real_distance_matrix(id_work_day, real_distance_matrix);
    if (PRINT_COSTS_MATRIX) { 
        cout << "\r\nReal distance "; 
        real_distance_matrix->print_matrix(); 
    }
    delete real_time_matrix;
    real_time_matrix = new DoubleMatrix(n_points);
    real_matrix->load_real_time_matrix(id_work_day, real_time_matrix);
    if (PRINT_COSTS_MATRIX) { 
        cout << setiosflags (ios::fixed) << setprecision(0);
        cout << "\r\nReal time "; 
        real_time_matrix->print_matrix();
        cout << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
    }

    // criando estruturas com os dados de todos os serviços do dia
    delete [] day_services;
    day_services = new ServiceOrder[n_points]; // para que o índice de day_services corresponda ao índice das matrizes. O índice do deposito é desconsiderado.
    real_data->load_service_orders(id_work_day, day_services);
    
    // preenchendo a matriz de custos que será usada na criação das soluções
    // utiliza os tempos de execução em day_services, se a matriz for de tempos de desloc + tempos de execução
    this->mount_cost_matrix(); // preenche a matriz de custos da instância, a que será usada na construção das soluções
    if (PRINT_COSTS_MATRIX) { 
        cout << "\r\nApplied cost "; current_instance->print_cost_matrix();
    }
    
    // carregando solução real realizada de acordo com a matriz de custos definida em current_instance
    // já aplica DESPATCH_REAL_SOL: atualiza em day_services o tempo de execução do serviço de acordo como a solução real calculada (time_dispatch - partial_route_cost)
    delete real_solution;
    real_solution = new MtspSolution(n_points, n_teams);
    real_data->load_real_solution(id_work_day, real_solution, current_instance, day_services);
    
    // salvando a solução real para plotagem (apenas na primeira simulação do dia de trabalho)
    if (counter_day_simulations == 1){
        Node *plan_coords = current_instance->get_plan_nodes_vector();
        f_real_sols << "\r\nDia de trabalho: " << id_work_day << "\r\n";
        real_solution->save_to_plot(f_real_sols, plan_coords);
    }
    
    if (PRINT_DAY_SERVICES) { cout << "\r\n"; this->print_day_services(); }
}
// preenche a matriz de custos de current_instance, que será utilizada na aplicação
void WorkDay::mount_cost_matrix(){
    switch (TYPE_COST_MATRIX){  // matriz de custos a ser utiliada na aplicação
        case 1: {   // distâncias euclidianas
            for (int i=0; i < n_points; i++){
                for (int j=0; j < n_points; j++){
                    double cost;
                    cost = current_instance->get_value_euclidean_matrix(i,j);
                    current_instance->set_value_cost_matrix(i, j, cost);
                }
            }
            break;
        }
        case 2: {   // distâncias reais
            for (int i=0; i < n_points; i++){
                for (int j=0; j < n_points; j++){
                    double cost;
                    cost = real_distance_matrix->get_value(i, j);
                    current_instance->set_value_cost_matrix(i, j, cost);
                }
            }
            break;
        }
        case 3: {   // tempos reais
            for (int i=0; i < n_points; i++){
                for (int j=0; j < n_points; j++){
                    double cost;
                    cost = real_time_matrix->get_value(i,j);
                    current_instance->set_value_cost_matrix(i, j, cost);
                }
            }
            break;
        }
        case 4: {   // tempos reais de deslocamento + tempo de execução dos serviços
            for (int i=0; i < n_points; i++){
                for (int j=0; j < n_points; j++){
                    double cost;
                    if (i == j){
                        cost = 0;
                    }
                    else{
                        int time_exec_serv;
                        if (j != depot) {
                            time_exec_serv = day_services[j].get_time_execution();
                        }
                        else {
                            time_exec_serv = 0;
                        }
                        double time_displacement = real_time_matrix->get_value(i, j);
                        cost = time_exec_serv + time_displacement;
                    }
                    current_instance->set_value_cost_matrix(i, j, cost);
                }
            }
            break;
        }
    }
}

void WorkDay::initialize_simulation(){
    // armazena as rotas finais em construção
    cur_executed_routes = new Route[n_teams];
    for (int k = 0; k < n_teams; k++){
        cur_executed_routes[k].create(n_points+1);  // tamanho máximo para m=1 com retorno ao depósito   
    }
}
void WorkDay::initialize_cur_executed_routes(ofstream& f_log_exper){
    for (int k=0; k < n_teams; k++){
        cur_executed_routes[k].add_node(depot, 0.0, 0.0);
    }
    if (SAVE_DAY_STATE_CHANGES) { 
        f_log_exper << "\r\nInicializacao: relógio da simulacao = " << (int)current_time << " segundos\t (08:00hs)\r\n"; 
        this->save_executed_routes(f_log_exper); 
    }
}
void WorkDay::execute_simulation(long seed_random, int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res){
  
    seed_simulation = seed_random;
    
    current_time = 0.0; // relógio da simulação em segundos, definindo 0.0 como 08:00 hs do dia de trabalho atual 
    
    // criando estruturas de dados do experimento
    this->initialize_simulation();
    
    // adiciona o depósito para todas as equipes em executed_routes
    this->initialize_cur_executed_routes(f_log_exper);
    
    bool end_work_day = false;
    while (!end_work_day){  // em cada loop é despachado um serviço
    
        // atualizando next_emergency e time_dispatch_next_emergency (horário de despacho da próxima emergência)
        // se next_emergency == -1, não existem mais emergências a despachar
        this->update_next_emergency();
        
        // verifica qual a próxima equipe a ficar livre. Atualiza next_team_free e time_to_free_next_team
        this->update_next_team_free();
        
        // verificando se surgiu uma emergência até a próxima equipe ficar livre:
        if ((time_dispatch_next_emerg <= time_free_next_team) && (next_emergency != -1)){
            // despachando a emergência:
            this->emergency_dispatch(next_emergency, f_log_exper);
        }
        else{ // não surgiram emergências
            
            // atualizando os serviços válidos a partir de service_days e current_time
            // (comparando current_time com o tempo de despacho e excluindo os já executados)
            this->update_valid_nodes_instance();
            int n_valid_nodes = current_instance->get_n_valid_nodes();
            
            if (n_valid_nodes == 0){   // não existem serviços a executar em valid_nodes
                
                double time_despatch_next_service = this->time_next_service();
                if (time_despatch_next_service == -1.0){
                    end_work_day = true;   // finalizando os despachos: não existem serviço a executar
                }
                else{
                    current_time = time_despatch_next_service + 1.0;
                }
            }
    
            else {  // existem serviços a despachar
            
                int service_to_despatch; // serviço que será despachado
                int current_position = cur_executed_routes[next_team_free].last_node();  // posição atual da equipe (serviço em execução)
               
                if (n_valid_nodes < n_teams){  // se houver mais equipes que nós válidos, não gera solução ACO
                    
                    // despachar para a próxima equipe livre o serviço com melhor previsão de custo (atendimento + retorno ao depósito)
                    double best_estimated_cost;
                    for (int s=0; s < n_valid_nodes; s++){
                        
                        int service_point = current_instance->get_valid_node(s);
                        double cost_current_to_service = current_instance->get_value_cost_matrix(current_position, service_point);
                        double cost_service_to_depot = current_instance->get_value_cost_matrix(service_point, depot);
                        double estimated_cost = cost_current_to_service + cost_service_to_depot;
                        
                        if (s == 0){
                            service_to_despatch = service_point;
                            best_estimated_cost = estimated_cost;
                        }
                        else{
                            if (estimated_cost < best_estimated_cost){
                                service_to_despatch = service_point;
                                best_estimated_cost = estimated_cost;
                            }
                        }
                    }  
                }
                
                else{  // o número de serviços é igual ou maior ao número de equipes: gerar solução ACO

                    if (NEAREST_TEAM){  // despachar o serviço que estiver mais próximo da equipe
                        
                        // despachar para a próxima equipe livre o serviço com melhor previsão de custo (atendimento + retorno ao depósito)
                        double best_cost;
                        for (int s=0; s < n_valid_nodes; s++){

                            int service_point = current_instance->get_valid_node(s);
                            double cost_current_to_service = current_instance->get_value_cost_matrix(current_position, service_point);

                            if (s == 0){
                                service_to_despatch = service_point;
                                best_cost = cost_current_to_service;
                            }
                            else{
                                if (cost_current_to_service < best_cost){
                                    service_to_despatch = service_point;
                                    best_cost = cost_current_to_service;
                                }
                            }
                        }  
                    }
                    else{  // gerar solução ACO
                        
                        // atualizando a posição das equipes na instância a partir de executed_routes 
                        this->update_positions_teams();       

                        // gerando uma solução com o algoritmo ACO para a instância atual e salvando em current_best_solution
                        AcoMtspAlgorithm* aco_mtsp_app = new AcoMtspAlgorithm(seed_random, current_instance);
                        delete current_aco_solution;
                        current_aco_solution = aco_mtsp_app->execute();
                        delete aco_mtsp_app;
                        current_aco_solution->set_random_seed(seed_simulation);

                        // salvando a solução ACO gerada
                        if (SAVE_PARTIAL_DAY_SOLS_PLOT){
                            // salvando em aco_partial_sols_day_to_plot.txt a solução final para o dia de trabalho
                            ofstream f_day_aco_sols;
                            f_day_aco_sols.open("outs/plot_aco_partial_sols_day.txt", ios::app);
                            Node* plan_nodes_vector = current_instance->get_plan_nodes_vector();
                            f_day_aco_sols << "\r\nDia de trabalho: " << id_work_day << "\tSimulação: " << counter_day_simulations <<"\r\n";
                            current_aco_solution->save_to_plot(f_day_aco_sols, plan_nodes_vector);
                            f_day_aco_sols.close();
                        }
                        
                        // localizando o serviço a ser despachado à proxima equipe livre, de acordo com a solução ACO gerada
                        service_to_despatch = current_aco_solution->next_node(current_position);

                        // a solução ACO afirma que o melhor é a equipe livre retornar ao depósito
                        // solução: despachar a solução com melhor custo previsto para a equipe
                        if (service_to_despatch == depot){

                            // despachar para a próxima equipe livre o serviço com melhor previsão de custo (atendimento + retorno ao depósito)
                            double best_estimated_cost;
                            for (int s=0; s < n_valid_nodes; s++){

                                int service_point = current_instance->get_valid_node(s);
                                double cost_current_to_service = current_instance->get_value_cost_matrix(current_position, service_point);
                                double cost_service_to_depot = current_instance->get_value_cost_matrix(service_point, depot);
                                double estimated_cost = cost_current_to_service + cost_service_to_depot;

                                if (s == 0){
                                    service_to_despatch = service_point;
                                    best_estimated_cost = estimated_cost;
                                }
                                else{
                                    if (estimated_cost < best_estimated_cost){
                                        service_to_despatch = service_point;
                                        best_estimated_cost = estimated_cost;
                                    }
                                }
                            }  
                        }
                    }
                }

                // atualizando current_time (usado na atualização dos serviços válidos)
                // passa a ser o horário que a equipe ficou livre + 1 segundo 
                current_time = time_free_next_team + 1.0;
                
                // realizando o despacho do serviço escolhido à equipe escolhida:
                this->dispatch_service_order(next_team_free, service_to_despatch);
                
                if (SAVE_DAY_STATE_CHANGES) {
                    f_log_exper << "\r\nDespacho comercial: relogio da simulacao = " << (int)current_time << " segundos\r\n"; 
                    f_log_exper << "Ordem de serviço despachada: " << service_to_despatch << "\t"; 
                    
                    day_services[service_to_despatch].save_service_order(f_log_exper);
                    current_instance->save_positions_teams(f_log_exper);
                    
                    f_log_exper << "Solução ACO criada: "; 
                    current_aco_solution->save_how_list(f_log_exper);
                    
                    this->save_executed_routes(f_log_exper); 
                }
            }
        }
    }
    
    // montando a solução de executed_routes em proposed_final_solution
    this->mount_final_solution();
    
    // finalizando o experimento
    this->finalize_simulation(counter_day_simulations, f_log_exper, f_simul_res);
}
void WorkDay::mount_final_solution(){
    delete complete_final_solution;
    complete_final_solution = new MtspSolution(n_points, n_teams);
    complete_final_solution->add(depot, 0.0); // adicionando o depósito no início da solução final
    for (int k = 0; k < n_teams; k++){
        int size_route_team = cur_executed_routes[k].n_nodes();
        for (int i=1; i < size_route_team; i++){  // desconsiderando o depósito em exected_routes
            int point = cur_executed_routes[k].node(i);
            complete_final_solution->add(point, 0.0);  // os custos são calculados por recalculate_solution
        }
        complete_final_solution->add(depot, 0.0); // adicionando o depósito ao final de cada rota
    }
    
    // calculando a solução final
    DoubleMatrix* cost_matrix = current_instance->get_cost_matrix();
    complete_final_solution->recalculate_solution(cost_matrix);
    complete_final_solution->set_random_seed(seed_simulation);
}
void WorkDay::finalize_simulation(int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res){
    
    if (SAVE_DAY_STATE_CHANGES) { 
        f_log_exper << "\r\nResumo da simulacao:"; 
    }
    
    f_log_exper << "\r\nDia de trabalho: " << id_work_day << "\tserviços: " << n_points-1 << "\tequipes: " << n_teams << "\r\n";
    f_log_exper << "Solução real:\t\t"; real_solution->save_how_list(f_log_exper);
    f_log_exper << "Solução proposta:\t"; complete_final_solution->save_how_list(f_log_exper);
    
    // salvando o resumo da simulação em um arquivo próprio:
    f_simul_res << id_work_day << "\t";
    complete_final_solution->save_longest_cost(f_simul_res);    
    complete_final_solution->save_total_cost(f_simul_res);
    
    if (counter_day_simulations == 1){
        f_longests << "\r\n" << id_work_day << "\t";
        f_total_costs << "\r\n" << id_work_day << "\t";
    }
    complete_final_solution->save_longest_cost(f_longests);    
    complete_final_solution->save_total_cost(f_total_costs);
    
    // salvando em plot_final_created_sols_day.txt a solução final para o dia de trabalho
    ofstream f_day_aco_final_sols;
    f_day_aco_final_sols.open("outs/plot_final_created_sols_day.txt", ios::app);
    Node* plan_nodes_vector = current_instance->get_plan_nodes_vector();
    f_day_aco_final_sols << "\r\nDia de trabalho: " << id_work_day << "\tSimulação: " << counter_day_simulations <<"\r\n";
    complete_final_solution->save_to_plot(f_day_aco_final_sols, plan_nodes_vector);
    f_day_aco_final_sols.close();
    
    double longest_real = real_solution->get_longest_route();
    double longest_proposed = complete_final_solution->get_longest_route();
    double diff_longest = longest_real - longest_proposed;
    double improvement_longest = (diff_longest / longest_real) * 100;

    double total_real = real_solution->get_total_cost();
    double total_proposed = complete_final_solution->get_total_cost();
    double diff_total = total_real - total_proposed;
    double improvement_total = (diff_total / total_real) * 100;
    
    f_log_exper << setiosflags (ios::fixed) << setprecision(2);
    f_log_exper << "Melhoramento:\t\tmaior rota: " << improvement_longest << "%\tcusto total: " << improvement_total << "%\r\n";
    f_log_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
    
    if (PRINT_EMERGENCY_CARE){
        cout << "  Emergency care:\r\n";
        for (int s=0; s < n_points; s++){
            if (s != depot){
                if (day_services[s].is_emergency()){
                    cout << "  ";
                    day_services[s].print_total_time_execution(s);
                }
            }
        }
    }
    if (PRINT_DAY_SERVICES) { cout << "\r\n"; this->print_day_services(); }
    
    delete [] cur_executed_routes;
}

void WorkDay::execute_static_simulation(long seed_random, int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res){
  
    seed_simulation = seed_random;
    
    // todas as ordens de serviço são válidas
    this->update_valid_all_nodes();
    
    // posicionando todas as equipes no depósito:
    this->position_teams_depot();
    
    // gerando uma solução com o algoritmo ACO para a instância completa e salvando em complete_final_solution
    AcoMtspAlgorithm* aco_mtsp_app = new AcoMtspAlgorithm(seed_random, current_instance);
    delete complete_final_solution;
    complete_final_solution = aco_mtsp_app->execute();
    delete aco_mtsp_app;
    complete_final_solution->set_random_seed(seed_simulation);

    // finalizando o experimento
    this->finalize_static_simulation(counter_day_simulations, f_log_exper, f_simul_res);
}
void WorkDay::finalize_static_simulation(int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res){
    
    if (SAVE_DAY_STATE_CHANGES) { 
        f_log_exper << "\r\nResumo da simulacao:"; 
    }
    
    f_log_exper << "\r\nDia de trabalho: " << id_work_day << "\tserviços: " << n_points-1 << "\tequipes: " << n_teams << "\r\n";
    f_log_exper << "Solução real:\t\t"; real_solution->save_how_list(f_log_exper);
    f_log_exper << "Solução proposta:\t"; complete_final_solution->save_how_list(f_log_exper);
    
    // salvando o resumo da simulação em um arquivo próprio:
    f_simul_res << id_work_day << "\t";
    complete_final_solution->save_longest_cost(f_simul_res);    
    complete_final_solution->save_total_cost(f_simul_res);
    
    if (counter_day_simulations == 1){
        f_longests << "\r\n" << id_work_day << "\t";
        f_total_costs << "\r\n" << id_work_day << "\t";
    }
    complete_final_solution->save_longest_cost(f_longests);    
    complete_final_solution->save_total_cost(f_total_costs);
    
    // salvando em plot_final_created_sols_day.txt a solução final para o dia de trabalho
    ofstream f_day_aco_final_sols;
    f_day_aco_final_sols.open("outs/plot_final_created_sols_day.txt", ios::app);
    Node* plan_nodes_vector = current_instance->get_plan_nodes_vector();
    f_day_aco_final_sols << "\r\nDia de trabalho: " << id_work_day << "\tSimulação: " << counter_day_simulations <<"\r\n";
    complete_final_solution->save_to_plot(f_day_aco_final_sols, plan_nodes_vector);
    f_day_aco_final_sols.close();
    
    double longest_real = real_solution->get_longest_route();
    double longest_proposed = complete_final_solution->get_longest_route();
    double diff_longest = longest_real - longest_proposed;
    double improvement_longest = (diff_longest / longest_real) * 100;

    double total_real = real_solution->get_total_cost();
    double total_proposed = complete_final_solution->get_total_cost();
    double diff_total = total_real - total_proposed;
    double improvement_total = (diff_total / total_real) * 100;
    
    f_log_exper << setiosflags (ios::fixed) << setprecision(2);
    f_log_exper << "Melhoramento:\t\tmaior rota: " << improvement_longest << "%\tcusto total: " << improvement_total << "%\r\n";
    f_log_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
    
    if (PRINT_EMERGENCY_CARE){
        cout << "  Emergency care:\r\n";
        for (int s=0; s < n_points; s++){
            if (s != depot){
                if (day_services[s].is_emergency()){
                    cout << "  ";
                    day_services[s].print_total_time_execution(s);
                }
            }
        }
    }
    if (PRINT_DAY_SERVICES) { cout << "\r\n"; this->print_day_services(); }
}

// verifica a próxima emergência que será despachada, a partir de day_services (não utiliza current_time)
void WorkDay::update_next_emergency(){
    
    next_emergency = -1;
    time_dispatch_next_emerg = 0.0;
    
    // os indices de day_services correspondem ao id dos nodes. o índice do depósito é desconsiderado:
    for (int s=0; s < n_points; s++){ 
        if (s != depot){
            if (!day_services[s].get_executed_service()){ // o serviço ainda não foi executado
                double time_dispatch = day_services[s].get_time_dispatch();
                if (day_services[s].is_emergency()){ // o serviço a ser despachado é uma emergência
                    if (next_emergency == -1){
                        next_emergency = s;
                        time_dispatch_next_emerg = time_dispatch;
                    }
                    else{
                        if (time_dispatch < time_dispatch_next_emerg){
                            next_emergency = s;
                            time_dispatch_next_emerg = time_dispatch;
                        }
                    }
                }
            }
        }
    }
}

// verifica qual a primeira equipe que vai concluir o seu serviço atual, utilizando os custos temporais parciais das rotas (não utiliza current_time)
void WorkDay::update_next_team_free(){

    double temporal_cost_route;  // custo atual da rota da equipe

    for (int k = 0; k < n_teams; k++){
        
        temporal_cost_route = cur_executed_routes[k].get_temporal_cost();
                
        // verificando o menor custo temporal (a primeira equipe a ficar livre):
        if (k == 0){
            next_team_free = k;
            time_free_next_team = temporal_cost_route;
        }
        else{
            if (temporal_cost_route < time_free_next_team){
                next_team_free = k;
                time_free_next_team = temporal_cost_route;
            }
        }
    }
}

// verifica qual equipe está mais próxima da emergência, de acordo com a matriz de custos aplicada:
void WorkDay::emergency_dispatch(int emergency_order, ofstream& f_log_exper){
    
    int best_team;  // a melhor equipe para atender a emergência é que estiver mais próxima, de acordo com a matriz de custos aplicada
    double best_cost_to_care;  //custo entre a posição atual da equipe e a emergência
    
    // definindo a equipe mais próxima do local da emergência:
    for (int k = 0; k < n_teams; k++){
        int current_position = cur_executed_routes[k].last_node();  // posição atual da equipe (serviço em execução)
        double cost_to_care = current_instance->get_value_cost_matrix(current_position, emergency_order); // custo da matriz aplicada
        
        // verificando a melhor equipe para atendimento:
        if (k == 0){
            best_team = k;
            best_cost_to_care = cost_to_care;
        }
        else{
            if (cost_to_care < best_cost_to_care){
                best_team = k;
                best_cost_to_care = cost_to_care;
            }
        }
    }
    
    // atualizando current_time (usado na atualização dos serviços válidos)
    // passa a ser o horário do serviço adicionado de 1 segundo:
    current_time = day_services[emergency_order].get_time_dispatch() + 1.0;
    
    // realizando o despacho do serviço à equipe escolhida:
    this->dispatch_service_order(best_team, emergency_order);
    if (SAVE_DAY_STATE_CHANGES) { 
        f_log_exper << "\r\nDespacho emergencial: relogio da simulacao = " << (int)current_time << " segundos\r\n"; 
        f_log_exper << "Ordem de serviço despachada: " << emergency_order << "\t"; 
        day_services[emergency_order].save_service_order(f_log_exper);
        this->save_executed_routes(f_log_exper); 
    }
}

// realiza o despacho de um serviço à uma equipe
void WorkDay::dispatch_service_order(int team, int service_order){
    
    int current_position = cur_executed_routes[team].last_node();  // posição atual da equipe
    double cost = current_instance->get_value_cost_matrix(current_position, service_order); // custo da matriz aplicada

    // calculando custo temporal:
    double travel_time = real_time_matrix->get_value(current_position, service_order);
    double exec_time = day_services[service_order].get_time_execution();
    double temporal_cost = travel_time + exec_time;

    // adicionando o serviço à rota da equipe:
    cur_executed_routes[team].add_node(service_order, cost, temporal_cost);
    double partial_route_temporal_cost = cur_executed_routes[team].get_temporal_cost();
    day_services[service_order].set_executed_service(partial_route_temporal_cost);
}

// atualizando os serviços válidos a partir de service_days (não consulta executed_routes)
// (comparando current_time com o tempo de despacho e excluindo os já executados)
void WorkDay::update_valid_nodes_instance(){
    
    current_instance->reset_valid_nodes_instance();
    
    // os indices de day_services correspondem ao id dos nodes. o índice do depósito é desconsiderado:
    for (int s=0; s < n_points; s++){ 
        if (s!=depot){
            if (!day_services[s].get_executed_service()){ // o serviço ainda não foi executado
                if (day_services[s].get_time_dispatch() <= current_time){ // o tempo de despacho do serviço é menor ou igual ao tempo atual
                    
                    current_instance->add_valid_node(s); // adicionando o serviço como um nó válido
                
                }
            }
        }
    }
}

void WorkDay::update_valid_all_nodes(){
    
    current_instance->reset_valid_nodes_instance();
    
    // os indices de day_services correspondem ao id dos nodes. o índice do depósito é desconsiderado:
    for (int s=0; s < n_points; s++){ 
        if (s!=depot){
            current_instance->add_valid_node(s); // adicionando o serviço como um nó válido
        }
    }
}

// atualizando os nós iniciais da instância a partir de positions teams
void WorkDay::update_positions_teams(){
    current_instance->reset_positions_teams();
    for (int k = 0; k < n_teams; k++){
        current_instance->add_position_team(cur_executed_routes[k].last_node());
    }
}

// posicionando todas as equipes no depósito:
void WorkDay::position_teams_depot(){
    current_instance->reset_positions_teams();
    for (int k = 0; k < n_teams; k++){
        current_instance->add_position_team(depot);
    }
}

double WorkDay::time_next_service(){
    
    double time_dispatch_next_service = -1.0;  // indica que não há mais serviços a executar em day_services
    int next_service = -1;
    
    // os indices de day_services correspondem ao id dos nodes. o índice do depósito é desconsiderado:
    for (int s=0; s < n_points; s++){ 
        if (s != depot){
            if (!day_services[s].get_executed_service()){ // o serviço ainda não foi executado
                double time_dispatch = day_services[s].get_time_dispatch();
                if (next_service == -1){
                        next_service = s;
                        time_dispatch_next_service = time_dispatch;
                }
                else{
                    if (time_dispatch < time_dispatch_next_service){
                        next_service = s;
                        time_dispatch_next_service = time_dispatch;
                    }
                }
            }
        }
    }
    
    return time_dispatch_next_service;
}

void WorkDay::print_executed_routes(){
    cout << "Rotas parciais executadas:\r\n";
    for (int k = 0; k < n_teams; k++){
        cout << "  Equipe " << k << ": "; 
        cur_executed_routes[k].print_short();
    }
}
void WorkDay::save_executed_routes(ofstream& file_out){
    file_out << "Rotas parciais executadas:\r\n";
    for (int k = 0; k < n_teams; k++){
        file_out << "  Equipe " << k+1 << ": "; 
        cur_executed_routes[k].save_short(file_out);
    }
}
void WorkDay::print_day_services(){
    cout << "Day service orders:\r\n";
    for (int i=0; i < n_points; i++){
        if (i != depot){
            cout << "  service " << i << " : ";
            day_services[i].print_service_order();
        }
    }
}
