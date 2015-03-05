/* 
 * File:   Utilities.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

# include "Utilities.hpp"

void Utilities::sort_double(double* &double_vector, IntList& sorted_list){
        
    int n_items = sorted_list.size();
    
    bool first_insert = true;  // será a primeira inserção em sorted_list?
    double d_last_inserted;    // último double do qual foi inserido o índice em sorted_list
    
    int i_next_insert;         // índice do double que vai ser inserido
    double d_next_insert;      // double que vai ser inserido

    while (!sorted_list.is_full()){

        if (first_insert){ // primeira inserção: localizar menor double e inserir seu índice
            first_insert = false;
            for(int i=0; i<n_items; i++){
                if (i==0){
                    i_next_insert = i;
                    d_next_insert = double_vector[i];
                }
                else{
                    if(double_vector[i] < d_next_insert){
                        i_next_insert = i;
                        d_next_insert = double_vector[i];
                    }
                }
            }
        }

        else{ // não é a primeira inserção
            bool first_comp = true;  // primeira comparação 
            for(int i=0; i<n_items; i++){
                
                if (double_vector[i] == d_last_inserted){
                    if (!sorted_list.on_the_list(i)){ // o ínidice i ainda não foi inserido
                        i_next_insert = i;
                        d_next_insert = double_vector[i];
                        i = n_items;
                    }
                }
                
                if (double_vector[i] > d_last_inserted){  // valores menores já estão ordenados
                    if (first_comp){
                        first_comp = false;
                        i_next_insert = i;
                        d_next_insert = double_vector[i];
                    }
                    else{  // não é a primeira comparação
                        if(double_vector[i] < d_next_insert){
                            i_next_insert = i;
                            d_next_insert = double_vector[i];
                        }
                    }
                }
            }    
        }

        d_last_inserted = d_next_insert;
        sorted_list.add(i_next_insert);
        
        cout << "";
/*        for(int i=0; i<n_items; i++){
//            cout << double_vector[i] << "\r\n";
        }
//        cout << "\r\n";
        for(int i=0; i<n_items; i++){
//            cout << sorted_list.value(i) << "\t";
        }
//        cout << "\r\n";

        if (i_next_insert == n_items){
            cout << "\r\n ERRO";
        }
*/    
    } // end while
}
    // retorna o índice do maior valor de um vetor de doubles:
    int Utilities::argmax(int n_cands, double* &double_vector){
        int i_max;
        double max;
        for(int i=0; i<n_cands; i++){
            if (i==0){
                i_max = i;
                max = double_vector[i];
            }
            else{
                if(double_vector[i] > max){
                    i_max = i;
                    max = double_vector[i];
                }
            }
        }
        return i_max;
    }
    // retorna a média de um vetor de doubles
    double Utilities::average (double* &values, int n_values){
        double sum_values = 0;
        double av = 0;
        for (int i=0; i<n_values; i++){
            sum_values = sum_values + values[i];
        }
        av = sum_values / n_values;
        return av;
    }
    // retorna o desvio padrão de um vetor de doubles
    double Utilities::std_dev (double* &values, int n_values){
        double st_dv;
        if (n_values == 1){ st_dv = 0; }
        else{
            double av = this->average(values, n_values);
            double sum_parcs = 0;
            for (int i=0; i<n_values; i++){
                sum_parcs = sum_parcs + pow((values[i]-av),2);
            }
            st_dv = sqrt(sum_parcs/(n_values-1));
        }
        return st_dv;
    }
