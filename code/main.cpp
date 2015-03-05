/* 
 * File:   main.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "ControlExperiment.hpp"
#include "StandardExperiment.hpp"
#include "RealExperiment.hpp"

#include <iostream>
using namespace std;

int main() {
    cout << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);  // formato de impressão de pontos flutuantes
    if (MODE_EXECUTION == 1){  // experimento padrão
        StandardExperiment *se = new StandardExperiment();
        se->run_standard_experiment();  // sem uma instância como parâmetro. é carregada a instância modelo defida em control.cpp
        delete se;
    }
    else{  // experimento com dados reais
        RealExperiment *re = new RealExperiment();
        re->run_real_experiment();
        delete re;
    }
    return 0;
}
