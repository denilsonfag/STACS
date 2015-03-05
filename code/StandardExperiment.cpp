/* 
 * File:   StandardExperiment.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "StandardExperiment.hpp"

StandardExperiment::StandardExperiment(){
    depot = DEPOT_INDEX;
    
    f_log_stand_exper.open("outs/log_stand_experiment.txt", ios::app);
    f_log_stand_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
    f_sols_aco.open("outs/plot_best_sols_aco.txt", ios::app);
    f_sols_aco << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION); 
}
StandardExperiment::~StandardExperiment(){
    f_sols_aco.close();
    f_log_stand_exper.close();
}

void StandardExperiment::run_standard_experiment(){

    // cada loop é um experimento independente
    for (int exper_counter=0; exper_counter < 1; exper_counter++){  

        double time_ini_experiment = (double)clock();  // instante do início do experimento

        // carregando instância configurada em ControlACO.hpp:
        MtspInstance* model_instance = new MtspInstance(N_SALESMEN, depot); 
        model_instance->load_euclidean_model_instance();

        // salvando e imprimindo:
        this->save_header_exper();      // salvando os parâmetros do experimento 
        if (PISO){ 
            cout << "\r\n-----------------------------------------------------\r\n";
            cout << "Experiment " << exper_counter + 1 << "\r\n";
            model_instance->print_instance();
        }
        
        // formato de impressão de pontos flutuantes:
        cout << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);  
        
        // melhores resultados do experimento:
        double best_longest_exper;  // minimizar rota mais longa
        double best_totsol_exper;   // minimizar soma das rotas
        // dados da melhor solução de cada execução:
        double* longest_best_sols = new double[N_EXECUTIONS];
        double* totsol_best_sols = new double[N_EXECUTIONS];
        double* cycle_best_sols = new double[N_EXECUTIONS];
        double* time_best_sols = new double[N_EXECUTIONS];

        // cada loop é uma execuçao independente:
        for (int exec_counter=0; exec_counter < N_EXECUTIONS; exec_counter++){  // execuções do algoritmo
            if(PISO){ cout << "\r\nExecution " << exec_counter + 1 << "\r\n"; }

            double time_ini_execution = (double)clock();  // instante do início da execução
            int n = model_instance->get_n_nodes();                  // definindo numero de nós da instância
            int m = model_instance->get_n_salesmen();               // definindo número de caixeiros da instância
            
            // A semente é alterada a cada execução para experimentos padrão:
            seed_random = (long)time(NULL);  // gerando semente aleatória

            AcoMtspAlgorithm* aco_mtsp_app = new AcoMtspAlgorithm(seed_random, model_instance);  // carregando aplicação
            MtspSolution* aco_solution = aco_mtsp_app->execute();  // executando a aplicação. A melhor solução é armazenada em best_sol_exe
            delete aco_mtsp_app;
            
            // salvando em best_sols_aco_to_plot.txt as melhores soluções de cada execução, de acordo com o objetivo
            Node* plan_nodes_vector = model_instance->get_plan_nodes_vector();
            aco_solution->save_to_plot(f_sols_aco, plan_nodes_vector);

            // salvando títulos dos campos da tabela:
            if (exec_counter == 0){
                f_log_stand_exper << "\r\nBest solution of each execution:";
                f_log_stand_exper << "\r\nexec\tlongest\t\ttsolut\t\tcycle\t\ttime(ms)\tseed execution\r\n";
            }

            // copiando custos da melhor solução da execução:
            longest_best_sols[exec_counter] = aco_solution->get_longest_route();
            totsol_best_sols[exec_counter] = aco_solution->get_total_cost();
            cycle_best_sols[exec_counter] = aco_solution->get_iteration_sol();
            time_best_sols[exec_counter] = aco_solution->get_time_sol();

            // salvando as melhores soluções de cada execução:
            double longest = aco_solution->get_longest_route();
            double tsolut = aco_solution->get_total_cost();
            int cycle = aco_solution->get_iteration_sol();
            int time = aco_solution->get_time_sol();
            int seed_r = aco_solution->get_seed_rand();
            // atualizando melhores resultados do experimento:
            if ((exec_counter == 0) || (longest < best_longest_exper)){
                best_longest_exper = longest;
            }        
            if ((exec_counter == 0) || (tsolut < best_totsol_exper)){
                best_totsol_exper = tsolut;
            }        
            f_log_stand_exper << exec_counter+1 << "\t" << longest << "\t\t" << tsolut << "\t\t" << cycle << "\t\t" << time << "\t\t" << seed_r << "\r\n";
            
            delete aco_solution;

            double time_execution = (((double)clock() - time_ini_execution) / CLOCKS_PER_SEC) * 1000;
            if(PISO){ cout << "Total time execution: " << time_execution << " miliseconds\r\n"; }
        }

        Utilities u;
        double avglongests = u.average(longest_best_sols, N_EXECUTIONS);
        double avgtsoluts = u.average(totsol_best_sols, N_EXECUTIONS);
        double avgcycles = u.average(cycle_best_sols, N_EXECUTIONS);
        double avgttimes = u.average(time_best_sols, N_EXECUTIONS);
        double sdlongests = u.std_dev(longest_best_sols, N_EXECUTIONS);
        double sdtsoluts = u.std_dev(totsol_best_sols, N_EXECUTIONS);
        double sdcycles = u.std_dev(cycle_best_sols, N_EXECUTIONS);
        double sdttimes = u.std_dev(time_best_sols, N_EXECUTIONS);

        f_log_stand_exper << setprecision(2);
        f_log_stand_exper << "\r\nAvgs\t" << avglongests << "\t\t" << avgtsoluts << "\t\t" << avgcycles << "\t\t" << avgttimes << "\r\n";
        f_log_stand_exper << "SDs\t" << sdlongests << "\t\t" << sdtsoluts << "\t\t" << sdcycles << "\t\t" << sdttimes << "\r\n";
        f_log_stand_exper << setprecision(FLOAT_PRECISION);
        
        f_log_stand_exper << "\r\nBest cost:\t";
        if (APP_OBJECTIVE == 1){  // minimizar a longest_route 
            f_log_stand_exper << best_longest_exper;
        }
        else{
            f_log_stand_exper << best_totsol_exper;
        }
        f_log_stand_exper << "\r\n";
        
        double time_experiment = (((double)clock() - time_ini_experiment) / CLOCKS_PER_SEC) * 1000;
        if(PISO){ cout << "\r\nTotal time experiment: " << (int)time_experiment << " miliseconds\r\n"; }
        f_log_stand_exper << "\r\nTotal time experiment: " << (int)time_experiment << " miliseconds\r\n";

        delete [] time_best_sols;
        delete [] cycle_best_sols;
        delete [] totsol_best_sols;
        delete [] longest_best_sols;

        delete model_instance;
    }
}

void StandardExperiment::save_header_exper(){
    f_log_stand_exper << "\r\n-----------------------------------------------------\r\n";
    
    f_log_stand_exper << "Experiment objective:\t\t";
    if (APP_OBJECTIVE == 1) f_log_stand_exper << "minimize longest route (workload balance)\r\n";
    if (APP_OBJECTIVE == 2) f_log_stand_exper << "minimize sum routes (basic MTSP)\r\n";
    
    f_log_stand_exper << "Type solutions:\t\t\t";
    if (TYPE_MTSP_SOLS == 1) f_log_stand_exper << "basic MTSP: single depot and closed routes\r\n";
    if (TYPE_MTSP_SOLS == 2) f_log_stand_exper << "MTSP with multiple starting depots and single end depot (open routes)\r\n";
    
    f_log_stand_exper << "Model instance:\t\t\t"; 
    switch (MODEL_INSTANCE){
        case 0: f_log_stand_exper << "Test grids\r\n"; break;
        case 1: f_log_stand_exper << "TSPLIB Eil51 (51 nodes)\r\n"; break;
        case 2: f_log_stand_exper << "TSPLIB Eil76 (76 nodes)\r\n"; break;
        case 3: f_log_stand_exper << "TSPLIB Eil101 (101 nodes)\r\n"; break;
        case 4: f_log_stand_exper << "TSPLIB Pr76 (76 nodes)\r\n"; break;
        case 5: f_log_stand_exper << "TSPLIB Pr1002 (1002 nodes)\r\n"; break;
        case 6: f_log_stand_exper << "sgb128 (128 nodes)\r\n"; break;
    }
   
    f_log_stand_exper << "Salesmen number:\t\t" << N_SALESMEN << "\r\n";
   
    f_log_stand_exper << "Depot node:\t\t\t" << depot << "\r\n";    
    
    f_log_stand_exper << "Independent executions:\t\t" << N_EXECUTIONS << "\r\n";    
    
    f_log_stand_exper << "Stopping criterion:\t\t";
    if (NO_IMP_CYCLES > 0) f_log_stand_exper << NO_IMP_CYCLES << " cycles without improved solutions\r\n";
    if (MAX_TIME_EXEC > 0) f_log_stand_exper << MAX_TIME_EXEC << " seconds per executin\r\n";
    if (MAX_CYCLES > 0) f_log_stand_exper << MAX_CYCLES << " cycles per execution\r\n";

    f_log_stand_exper << "\r\nAnt Colony System parameters:\r\n";
    f_log_stand_exper << setiosflags (ios::fixed) << setprecision(2);
    f_log_stand_exper << "   N (iterations per cycle):\t" << N << "\r\n";
    f_log_stand_exper << "   q0 (determinism level):\t" << Q0 << "\r\n";
    f_log_stand_exper << "   alfa (pheromone weight):\t" << ALFA << "\r\n";
    f_log_stand_exper << "   beta (visibility weight):\t" << BETA << "\r\n";
    f_log_stand_exper << "   ksi (pheromone persit LPU):\t" << KSI << "\r\n";
    f_log_stand_exper << "   ro (pheromone persit GPU):\t" << RO << "\r\n";
    f_log_stand_exper << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);
    
    f_log_stand_exper << "\r\nExperiment parameters:\r\n"; 
    
    f_log_stand_exper << "   Candidate list length:\t" << CL_LENGTH << "\r\n";
    
    f_log_stand_exper << "   Local Pheromone Update:\t"; if (LPU) f_log_stand_exper << "ON\r\n"; else f_log_stand_exper << "OFF\r\n";
   
    f_log_stand_exper << "   Global Pheromone Update:\t"; 
    if (GPU) {
        f_log_stand_exper << "ON"; 
        if (GPUBSF) f_log_stand_exper << "\t--> best-so-far solution updating"; 
        else  f_log_stand_exper << "\t--> best solution each cycle updating";
        if (GPUNODEP != 0) f_log_stand_exper << "\t--> not depositing or evaporanting on edges connected to depot"; 
        f_log_stand_exper << "\r\n";
    } else f_log_stand_exper << "OFF\r\n";

    f_log_stand_exper << "   Type cost matrix: \t\t";
    if (TYPE_EUCLID_MATRIX == 0) f_log_stand_exper << "double\r\n";
    if (TYPE_EUCLID_MATRIX == 1) f_log_stand_exper << "integer\r\n";
       
    f_log_stand_exper << "   Start node ants:\t\t"; 
    if (TYPE_MTSP_SOLS == 2) f_log_stand_exper << "accordding positions of instance (open routes)\r\n"; 
    if (TYPE_MTSP_SOLS == 1){
        if (ANTS_INIT_NODES == 1) f_log_stand_exper << "all starting from single depot\r\n"; 
        if (ANTS_INIT_NODES == 2) f_log_stand_exper << "random nodes in all solutions\r\n"; 
        if (ANTS_INIT_NODES == 3) f_log_stand_exper << "random nodes each " << CHG_INIT_NODES << " cycles without improvement\r\n";
    }
    
    f_log_stand_exper << "   Check best ant repetitions:\t" << CBA_REPET;
    if (CBA_REPET == 0){  // check better ant repetitions
        f_log_stand_exper << "\t--> no checking";
    }
    f_log_stand_exper << "\r\n";

    f_log_stand_exper << "\r\nLocal Search:\r\n";
    f_log_stand_exper << "   2-opt (all created solutions):\t\t"; if (LS2O) f_log_stand_exper << "ON\r\n"; else f_log_stand_exper << "OFF\r\n";
    f_log_stand_exper << "   3-opt (only best cycle solutions):\t\t"; if (LS3O) f_log_stand_exper << "ON\r\n"; else f_log_stand_exper << "OFF\r\n";
}
