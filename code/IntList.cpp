/* 
 * File:   IntList.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "IntList.hpp"

IntList::IntList(){}
void IntList::create(int size){
        max_items = size;
        index = -1;
        values_list = new int[max_items];
}
IntList::IntList(int size){
        max_items = size;
        index = -1;
        values_list = new int[max_items];
}
IntList::~IntList(){ 
        delete [] values_list;
}
    
void IntList::empty(){ index = -1; }  // esvazia a lista
void IntList::add(int value){  // adiciona um valor no final da lista
        if (value > max_items){ 
//-----            cout << "\r\n---> ADDING LARGER VALUE THAN LIST SIZE <---\r\n";
        }
        if (index+1 == max_items){
            cout << "\r\n---> OVERFLOW ADDING IN LIST: "; this->print(); cout << "\r\n";
            exit(3);
        }
        else{
            index++;
            values_list[index] = value;
        }
}
// inverte a values_list de index_start a index_end, menos os nós depósito da solução
void IntList::reverse_no_depot(int index_start, int index_end, int depot_node){
        int *new_list = new int[max_items];
        int aux_ind_end = index_end;
        for (int i=0; i < max_items; i++){
            if ((i < index_start) || (i > index_end)) { // copiar valores normalmente
                new_list[i] = values_list[i];
            }
            else{ // inverter valores
              
                if (values_list[aux_ind_end] == depot_node) aux_ind_end--;

                if (values_list[i] == depot_node){
                    new_list[i] = depot_node;
                }
                else{
                    new_list[i] = values_list[aux_ind_end];
                    aux_ind_end--;    
                }
                
            }
        }
        delete [] values_list;
        values_list = new_list;
}
    // inverte a values_list de index_start a index_end:
void IntList::reverse(int index_start, int index_end){
        int *new_list = new int[max_items];
        int aux_ind_end = index_end;
        for (int i=0; i < max_items; i++){
            if ((i < index_start) || (i > index_end)) { // copiar valores normalmente
                new_list[i] = values_list[i];
            }
            else{ // inverter valores
                new_list[i] = values_list[aux_ind_end];
                aux_ind_end--;
            }
        }
        delete [] values_list;
        values_list = new_list;
}
void IntList::remove (int value){
        int *aux_values = new int[max_items];
        int index_aux = 0;
        for (int i=0; i<=index; i++){
            if (values_list[i] != value){
                aux_values[index_aux] = values_list[i];
                index_aux++;    
            }
        }
        delete [] values_list;
        values_list = aux_values;
        index--;
}
    
void IntList::change_values(int index_i, int index_j){
        int aux_value = values_list[index_i];
        values_list[index_i] = values_list[index_j];
        values_list[index_j] = aux_value;
    }
int IntList::size() { return max_items; }  // retorna a capacidade da lista
int IntList::n_items(){ return index + 1; }  // retorna o número de itens na lista
int IntList::value(int ind) {  // retorna um valor da lista pelo índice
        return values_list[ind]; 
}  
int IntList::last_value(){  // retorna o último valor da lista
        return values_list[index];
}
bool IntList::on_the_list(int value){  // o valor está na lista?
        bool ret = false;
        for (int i=0; i <= index; i++){
            if (value == values_list[i]) {
                ret = true;
                i = index + 1;
            }
        }
        return ret;
}
bool IntList::is_full(){  // a lista está cheia?
        if ((index+1) == max_items) return true;
        else return false;
}
bool IntList::is_empty(){  // a lista está vazia?
        if (index == -1) return true;
        else return false;
}
    
int IntList::next_value(int value){
    int next_value = -1;
    for (int i=0; i <= index; i++){
        if (value == values_list[i]) {
            if (i != index){
                next_value = values_list[i+1];
            }
        }
    }
    return next_value;
}

void IntList::save(ofstream& file_out){
    if (index == -1){
        file_out << "--> EMPTY LIST";
    }
    else{
        for (int i=0; i<=index; i++){
            if (values_list[i] < 10) file_out << " ";
            file_out << values_list[i] << " ";
        }
    }
}
        
void IntList::print(){  // imprime a lista
    if (index == -1){
        cout << "--> EMPTY LIST";
    }
    else{
        for (int i=0; i<=index; i++){
            if (values_list[i] < 10) cout << " ";
            cout << values_list[i] << " ";
        }
    }
}
void IntList::print_add1(){  // imprime a lista adicionando 1 a todos os valores
//        cout << "ADD1: "; 
        if (index == -1){
            cout << "--> EMPTY LIST";
        }
        else{
            for (int i=0; i<=index; i++){
                if (values_list[i] < 10) cout << " ";
                cout << values_list[i] + 1 << " ";
            }
        }
}
