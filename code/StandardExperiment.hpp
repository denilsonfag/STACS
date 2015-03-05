/* 
 * File:   StandardExperiment.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__STANDARDEXPER_HPP__)
    #define __STANDARDEXPER_HPP__

    #include "ControlExperiment.hpp"

    #include "MtspInstance.hpp"
    #include "MtspSolution.hpp"
    #include "AcoMtspAlgorithm.hpp"
    #include <fstream>
    #include <iomanip>
    using namespace std;

    class StandardExperiment{
        int depot;
        long seed_random;           // semente dos randomicos do experimento
        ofstream f_log_stand_exper;
        ofstream f_sols_aco;
    public:
        StandardExperiment();
        ~StandardExperiment();
        void save_header_exper();        // salva o cabeçalho do experimento
        void run_standard_experiment();  // carrega a instância de ControlACO.hpp
    };

#endif
