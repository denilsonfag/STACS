/* 
 * File:   ControlRealExperiment.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__CONTROLEXPERIMENT_HPP__)
    #define  __CONTROLEXPERIMENT_HPP__

    #define MODE_EXECUTION         2  // 1: experimento com instâncias modelo; 2: experimento com dados reais
    #define TYPE_EUCLID_MATRIX     0  // tipo de dados da matriz de distâncias euclidianas: (0)double, (1)int
    #define DEPOT_INDEX            0  // índice da instância que corresponde ao depósito
    #define FLOAT_PRECISION        2  // precisão decimal de impressão de pontos flutuantes 
    

    // Parâmetros para experimentos com dados reais:
    
    #define STATIC_SIMULATION      1  // simulação com instâncias estáticas com todos os serviços dos dias de trabalho
    #define NEAREST_TEAM           0  // não usa ACO: despacha o serviço que estiver mais próximo da equipe
    #define N_SIMULATIONS_BY_DAY 10  // número de execuções para cada dia de trabalho
    #define TYPE_COST_MATRIX       1  // 1:euclidiana, 2: distâncias reais, 3: tempos reais, 4:tempo + deslocamento
    #define DESPATCH_REAL_SOL      1  // definindo o horário de despacho do serviço a partir do momento que uma equipe inicia o deslocamento para ele na solução real calculada
    #define INDEX_DAY_TEST        -1  // index do dia de trabalho para testes. Se -1, todos os dias serão simulados

    // parametros de resultados opcionais a salvar:
    #define SAVE_DAY_STATE_CHANGES      1  // salva todas as mudanças de estado dos dias de trabalho no log do experimento  
    #define SAVE_PARTIAL_DAY_SOLS_PLOT  1  // salva todas as soluções ACO geradas durante as simulações

    // Parâmetros de impressão:
    #define PRINT_COSTS_MATRIX    0  // imprimir as três matrizes logo após sua construção
    #define PRINT_DAY_SERVICES    0  // imprimir os serviços do dia de trabalho
    #define PRINT_EMERGENCY_CARE  0  // imprimir a melhora dos atendimentos de emergência


    // Parâmetros para experimentos com instâncias modelo:

    #define N_EXECUTIONS         10  // número de execuções independentes do algorimto ACO
    #define TYPE_MTSP_SOLS        1  // 1: finais fechados; 2: finais abertos (determina o tipo da solução alterando a posição inicial das formigas)
    #define MODEL_INSTANCE        1  // 1: eil51, 2: eil76, 3: eil101, 4: pr76, 5: pr1002, 6: sgb128, 0: grids para testes
    #define N_SALESMEN            4  // número de equipes do experimento
    #define TEST                 -1  // para diminuir o número de nós da instância em testes. -1 = todos os nós    

    // parâmetros de impressão:
    #define PISO    1  // imprimir todas as soluções melhoradas em cada execução    
    #define PDMT    0  // imprimir a matriz de distâncias no início da execução 
    #define PASO    0  // imprimir todas as melhores soluções de cada ciclo
    #define PDCR    0  // imprimir detalhes da construção das rotas
    #define PART    0  // imprimir todas as rotas geradas em todas as soluções
    
    #define PRLS    0  // imprimir melhoramento pelas buscas locais
    #define PDNO    0  // imprimir detalhes da busca local no cross
    #define PDLS    0  // imprimir detalhes das buscas locais 2 e 3-opt

    #define PPOE    0  // imprimir a matriz de feromônio no início e no final da execução    
    #define PPLU    0  // imprimir a matriz de feromônio a cada atualização local
    #define PPGU    0  // imprimir a matriz de feromônio a cada atualização global

#endif
