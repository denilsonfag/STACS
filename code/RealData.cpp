/* 
 * File:   RealData.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "RealData.hpp"

RealData::RealData(){
    depot = DEPOT_INDEX;
    data_file_name = "data/data.txt";
    data_file.open(data_file_name, ios::in);
    id_register = 0;  // o final do arquivo é -1
}
RealData::~RealData(){
    data_file.close();
}

void RealData::go_init_file(){
    data_file.close();
    data_file.open(data_file_name, ios::in);
}

// os tempos de execução são convertidos para segundos:
void RealData::read_next_register(){
    data_file >> id_register;
    data_file >> id_work_day;
    data_file >> id_point;
    data_file >> utm_x;
    data_file >> utm_y;
    if (id_point != depot){  // os registros dos depósitos não possuem os valores abaixo no arquivo
        data_file >> id_team;
        data_file >> type_service;
        data_file >> time_dispatch;
        data_file >> time_execution;
        time_execution = time_execution*60;  // convertendo tempo de execução para segundos
        data_file >> deadline_execution;
        data_file >> km_ini;
        data_file >> km_end;
    }
}     

int RealData::count_work_days(){
        
    bool first_register = true;
    int counter_days = 0;
    int current_day;

    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (first_register){
                first_register = false;
                current_day = id_work_day;
                counter_days++;
            }
            if (current_day != id_work_day){
                current_day = id_work_day;
                counter_days++;
            }
        }
        else{
            flag = false;
        }
    }
    return counter_days;
}
void RealData::load_ids_work_days(IntList* &list_id_work_days, int n_work_days){
    
    delete list_id_work_days;
    list_id_work_days = new IntList(n_work_days);
        
    bool first_register = true;
    int counter_days = 0;
    int current_day;
    
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();
        
        if (id_register != -1){
            if (first_register){
                first_register = false;
                current_day = id_work_day;
                list_id_work_days->add(current_day);
                counter_days++;
            }
            if (current_day != id_work_day){
                current_day = id_work_day;
                list_id_work_days->add(current_day);
                counter_days++;
            }
        }
        else{
            flag = false;
        }
    }
}

int RealData::count_services(int selected_id_work_day){
    
    int counter_services = 0;

    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_work_day == selected_id_work_day){
                if (id_point != depot){
                    counter_services++;
                }
            }
        }
        else{
            flag = false;
        }
    }
    return counter_services;
}
int RealData::count_teams(int selected_id_work_day){

    int counter_teams = 0;
    
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    int current_team;
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_work_day == selected_id_work_day){
                if (id_point != depot){
                    if (counter_teams == 0){
                        counter_teams++;
                        current_team = id_team;
                    }
                    else{
                        if (current_team != id_team){
                            counter_teams++;
                            current_team = id_team;
                        }
                    }
                }        
            }
        }
        else{
            flag = false;
        }
    }
    return counter_teams;
}

void RealData::load_service_orders(int selected_id_work_day, ServiceOrder* &day_services){

    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_work_day == selected_id_work_day){
                if (id_point != depot){  // o índice do depósito não é inicializado

                    day_services[id_point].new_service(utm_x, utm_y, type_service, time_dispatch, time_execution, deadline_execution, km_ini, km_end);

                }
            }
        }
        else{
            flag = false;
        }
    }
}
void RealData::load_euclidean_graph(int selected_id_work_day, MtspInstance* &instance){
    
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_work_day == selected_id_work_day){
                if (id_point != depot){

                    instance->add_plan_node(utm_x, utm_y);

                }
            }
        }
        else{
            flag = false;
        }
    }
}
int RealData::x_depot(){

    int coord_x = -1;
    
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_point == depot){
                coord_x = utm_x;
                flag = false;
            }
        }
        else{
            flag = false;
        }
    }
    return coord_x;
}
int RealData::y_depot(){
    
    int coord_y = -1;
    
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes

        this->read_next_register();

        if (id_register != -1){
            if (id_point == depot){
                coord_y = utm_y;
                flag = false;
            }
        }
        else{
            flag = false;
        }
    }
    return coord_y;
}

// é definido um novo tempo de despacho para os serviços quando calculados os custos das rotas reais utilizando os dados considerados.
void RealData::load_real_solution(int selected_id_work_day, MtspSolution* &real_solution, MtspInstance* &instance, ServiceOrder* &day_services){
    
    DoubleMatrix* cost_matrix = instance->get_cost_matrix();
    real_solution->reset(); // a solução real é do tipo de finais fechados
    data_file.close();
    data_file.open(data_file_name, ios::in);
    
    double current_route_cost = 0.0;
    bool first_point_solution = true;

    bool flag = true;
    while (flag) {  // lendo todo o arquivo das matrizes
        this->read_next_register();
        if (id_register != -1){ // fim do arquivo
            if (id_work_day == selected_id_work_day){  // este serviço faz parte da solução real
                
                double adding_cost;
                if (first_point_solution){ // primeiro ponto = depósito
                    first_point_solution = false;
                    adding_cost = 0.0;
                }
                else{
                    int last_point_inserted = real_solution->last_node();
                    adding_cost = cost_matrix->get_value(last_point_inserted, id_point);
                }
                
                // definindo o horário de despacho do serviço a a partir do momento que uma equipe inicia o deslocamento para ele na solução real
                if (DESPATCH_REAL_SOL){
                    int old_time = day_services[id_point].get_time_dispatch();
                    if (old_time > 0){ // o serviço não faz parte da instância inicial
                        day_services[id_point].set_time_despatch(current_route_cost); // atualizando o horário de despacho
                    }
                }
                
                // atualizando custo da rota parcial real:
                current_route_cost = current_route_cost + adding_cost;
                
                if (id_point == depot){
                    current_route_cost = 0.0;
                }
                else{
                    day_services[id_point].set_total_time_execution_real_solution(current_route_cost);
                }
                
                real_solution->add(id_point, adding_cost);  // será usado recalculate_solution para determinar os custos
            }
        }
        else{
            flag = false;
        }
    }
    real_solution->recalculate_solution(cost_matrix);  // recalculando os custos (redundante)
}
