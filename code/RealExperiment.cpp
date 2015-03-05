/* 
 * File:   RealExperiment.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "RealExperiment.hpp"

RealExperiment::RealExperiment(){
    // criando apenas estruturas, com tamanhos mínimos
    list_id_work_days = new IntList(1);
    current_work_day = new WorkDay(-1);
    // abrindo arquivos de resultados
    f_log_exper.open("outs/log_real_experiment.txt", ios::app);
    f_log_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
        // arquivo com os custos gerados para cada dia de trabalho em cada simulação:
    f_simul_res.open("outs/simulations_resume.txt", ios::app);
    f_simul_res << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
    
    f_time_execs.open("outs/time_execs.txt", ios::app);
    f_time_execs << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
}

RealExperiment::~RealExperiment(){
    f_time_execs.close();
    f_simul_res.close();    
    f_log_exper.close();
    delete current_work_day;    
    delete list_id_work_days;
}

void RealExperiment::run_real_experiment(){
    time_ini_experiment = (double)clock();  // instante do início do experimento            
    RealData* real_data = new RealData();
    int n_work_days = real_data->count_work_days();  // contando número de dias de trabalho na base de dados
    real_data->load_ids_work_days(list_id_work_days, n_work_days);  // criado a lista com as ids dos dias de trabalho
    delete real_data;
    
    for (int current_day = 0; current_day < n_work_days; current_day++){
        
        if (INDEX_DAY_TEST != -1){
            current_day = INDEX_DAY_TEST;
        }
        
        f_log_exper << "\r\n------------------------------\r\nDia de Trabalho: " << list_id_work_days->value(current_day) << "\r\n";
        
        for (int counter_day_simulations=1; counter_day_simulations <= N_SIMULATIONS_BY_DAY; counter_day_simulations++){
            
            double time_ini_execution_day = (double)clock();  // instante do início do experimento     
            
            f_log_exper << "\r\n----------\r\nSimulacao: " << counter_day_simulations << "\r\n";
            
            int current_id_work_day = list_id_work_days->value(current_day);
            delete current_work_day;
            current_work_day = new WorkDay(current_id_work_day);
            
            current_work_day->load_data_work_day(counter_day_simulations);

            // a semente é alterada a cada execução de um dia de trabalho
            seed_random = (long)time(NULL);  // gerando semente aleatória
            f_log_exper << "Semente rândomica utilizada: " << seed_random << "\r\n";
            this->save_aco_parameters();
            
            if (STATIC_SIMULATION){
                current_work_day->execute_static_simulation(seed_random, counter_day_simulations, f_log_exper, f_simul_res);
            }
            else{
                current_work_day->execute_simulation(seed_random, counter_day_simulations, f_log_exper, f_simul_res);
            }
            
            // gravando o tempo total da simulação:
            double time_simulation = (((double)clock() - time_ini_execution_day) / CLOCKS_PER_SEC) * 1000;
            f_log_exper << "\r\nTempo total da simulacao: " << (int)time_simulation << " milissegundos\r\n";
            f_simul_res << (int)time_simulation << "\r\n";
            
            if (counter_day_simulations == 1){
                f_time_execs << "\r\n" << current_id_work_day << "\t";
            }
            f_time_execs << (int)time_simulation << "\t";
        }
        if (INDEX_DAY_TEST != -1){
        current_day = n_work_days;
        }
    }    
    
    double time_experiment = (((double)clock() - time_ini_experiment) / CLOCKS_PER_SEC) * 1000;
    f_log_exper << "\r\n------------------------------\r\nTempo total do experimento: " << (int)time_experiment << " milissegundos\r\n";
}

void RealExperiment::save_aco_parameters(){
    f_log_exper << "Parametros ACO:\r\n";
    f_log_exper << "   Objetivo MTSP:\t";
    if (APP_OBJECTIVE == 1) f_log_exper << "minimizar maior rota (workload balance)\r\n";
    if (APP_OBJECTIVE == 2) f_log_exper << "minimizar custo total\r\n";
    f_log_exper << "   Criterio de parada:\t";
    if (NO_IMP_CYCLES > 0) f_log_exper << NO_IMP_CYCLES << " ciclos sem melhora da solucao\r\n";
    if (MAX_TIME_EXEC > 0) f_log_exper << MAX_TIME_EXEC << " segundos por execucao\r\n";
    if (MAX_CYCLES > 0) f_log_exper << MAX_CYCLES << " ciclos por execucao\r\n";
    f_log_exper << "   Tamanho da lista de candidatos:\t" << CL_LENGTH << "\r\n";
    f_log_exper << setiosflags (ios::fixed) << setprecision(2);
    f_log_exper << "   N (solucoes geradas a cada ciclo):\t" << N << "\r\n";
    f_log_exper << "   q0 (nível de determinismo):\t\t" << Q0 << "\r\n";
    f_log_exper << "   alfa (peso do feromonio):\t\t" << ALFA << "\r\n";
    f_log_exper << "   beta (peso da visibilidade):\t\t" << BETA << "\r\n";
    f_log_exper << "   ksi (persitencia do feromonio nas atualizacoes locais):\t" << KSI << "\r\n";
    f_log_exper << "   ro (persitencia do feromonio nas atualizacoes globais):\t" << RO << "\r\n";
    
    f_log_exper << "\r\nBusca Local:\r\n";
    f_log_exper << "   2-opt (todas as solucoes criadas):\t\t"; if (LS2O) f_log_exper << "ON\r\n"; else f_log_exper << "OFF\r\n";
    f_log_exper << "   3-opt (somente nas melhores soluções dos ciclos):\t\t"; if (LS3O) f_log_exper << "ON\r\n"; else f_log_exper << "OFF\r\n";
    
    f_log_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
}
