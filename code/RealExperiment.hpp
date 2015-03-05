/* 
 * File:   RealExperiment.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// classe para realização do experimento com dados reais
#if !defined(__REALEXPERIMENT_HPP__)
    #define __REALEXPERIMENT_HPP__

    #include "WorkDay.hpp"    
    #include "RealData.hpp"
    #include "IntList.hpp"

    class RealExperiment{
        int time_ini_experiment;     // instante de referẽncia do início do experimento
        IntList* list_id_work_days;  // lista com os códigos dos dias de trabalho da base de dados
        WorkDay* current_work_day;   // dia de trabalho atual
        long seed_random;            // semente dos randomicos do experimento
        ofstream f_log_exper;        // salvamento em arquivo
        ofstream f_simul_res;        // arquivo com o resumo das simulações
        ofstream f_time_execs;
    public:
        RealExperiment();
        ~RealExperiment();
        void run_real_experiment();
        void save_aco_parameters();
    };

#endif
