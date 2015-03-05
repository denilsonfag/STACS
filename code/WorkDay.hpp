/* 
 * File:   WorkDay.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__WORKDAY_HPP__)
    #define __WORKDAY_HPP__

    // classe que representa um dia de trabalho das equipes
    #include "ControlExperiment.hpp"
    #include "DoubleMatrix.hpp"
    #include "Node.hpp"
    #include "Route.hpp"
    #include "MtspSolution.hpp"
    #include "ServiceOrder.hpp"
    #include "EuclideanGraph.hpp"
    #include "MtspInstance.hpp"
    #include "RealData.hpp"
    #include "RealMatrix.hpp"
    #include "AcoMtspAlgorithm.hpp"
    #include <iomanip>
    using namespace std;

    class WorkDay{
              
        ofstream f_real_sols;     // soluções reais dos dias para plotagem
        ofstream f_longests;
        ofstream f_total_costs;
        
        long seed_simulation;     // semente randomica da simulção

        double time_ini_simulation;    // momento de início do experimento deste dia de trabalho
        int id_work_day;               // código do dia de trabalho
        int depot;                     // garagem da agência de atendimento 
        int n_points;                  // total de pontos do grafo (serviços + depósito)
        int n_teams;                   // numero de equipes disponíveis no dia
        RealData* real_data;           // classe para acesso aos arquivos de dados reais
        RealMatrix* real_matrix;       // acesso às matrizes de custos reais
        ServiceOrder* day_services;    // todos os serviços do dia (não inclui o depósito)
        MtspInstance* current_instance;     // Instância que será montada e passada como parâmetro para o algoritmo proposto
        DoubleMatrix* real_distance_matrix; // distâncias reais entre os pontos, em kms, a partir de MapQuest
        DoubleMatrix* real_time_matrix;     // tempos reais de deslocamento, em segundos, a partir de MapQuest

        MtspSolution* real_solution;           // soluçao com as rotas fechadas reais executadas pelas equipes
        MtspSolution* complete_final_solution; // a solução para o dia todo obtida pela simulação  
        MtspSolution* current_aco_solution;    // as soluções geradas pelo algoritmo proposto
        
        double current_time;             // tempo atual do dia de trabalho
        int next_emergency;              // índice da proóxima emergência
        double time_dispatch_next_emerg; // horário de surgimento da próxima emrgência
        int next_team_free;              // próxima equipe a ficar livre
        double time_free_next_team;      // instante em que a equipe vai conluir o serviço atual
        Route* cur_executed_routes;      // as rotas das equipes em construção
        
    public:

        WorkDay(int current_id_work_day);
        ~WorkDay();

        void load_data_work_day(int counter_day_simulations);  // carrega todos os dados do dia de trabalho
        void mount_cost_matrix();             // monta a matriz de custos da instância, a que será usada na construção das soluções
        void apply_time_factor_cost_matrix(); // calcula e aplica o fator temporal à matriz de custos
        
        void initialize_simulation();
        void finalize_simulation(int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res);
        
        void finalize_static_simulation(int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res);
        
        void execute_simulation(long seed_random, int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res);
        void execute_static_simulation(long seed_random, int counter_day_simulations, ofstream& f_log_exper, ofstream& f_simul_res);
        
        void update_next_emergency();   // atualiza next_emergency e time_dispatch_next_emergency
        void update_next_team_free();   // atualiza next_team_free e time_to_free_next_team
        
        void emergency_dispatch(int id_service, ofstream& f_log_exper);  // realiza o despacho de um serviço emergencial
        void dispatch_service_order(int team, int service_order);  // atribui um serviço à rota de uma equipe

        void initialize_cur_executed_routes(ofstream& f_log_exper);
        void update_valid_nodes_instance();
        void update_valid_all_nodes();
        
        void commercial_dispatch(int commercial_point);
        void update_positions_teams();
        void position_teams_depot();
        
        double time_next_service();
        
        void update_current_time();
        void update_state_work_day();

        void mount_final_solution();

        void print_executed_routes();
        void save_executed_routes(ofstream& file_out);
        void print_day_services();
    };

#endif    
