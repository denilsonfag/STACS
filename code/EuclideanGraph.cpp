/* 
 * File:   EuclideanGraph.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "EuclideanGraph.hpp"

EuclideanGraph::EuclideanGraph(){  // criando apenas estrutura, com tamanho mínimo
    index_last_nodes_vector = -1;  // vetor de nos vazio
    n_nodes = 1;
    plan_nodes_vector = new Node[n_nodes];
    euclidean_matrix = new DoubleMatrix(n_nodes);
}
EuclideanGraph::~EuclideanGraph(){       
    delete euclidean_matrix;
    delete [] plan_nodes_vector;
}
                
void EuclideanGraph::load_model_graph(){
        switch (MODEL_INSTANCE){ 
            
            case 0: {  // grids para testes
                int grid_dim = 3;  // dimensão do grid
                n_nodes = grid_dim*grid_dim;
                index_last_nodes_vector = -1;
                delete [] plan_nodes_vector;
                plan_nodes_vector = new Node[n_nodes];
                delete euclidean_matrix;  // criando matriz de distâncias
                euclidean_matrix = new DoubleMatrix(n_nodes);
                for(int i=0; i < grid_dim; i++){ // criando os nós do grafo
                    for(int j=0; j < grid_dim; j++){
                        if (index_last_nodes_vector < n_nodes){
                            this->add_vertex(i, j);
                        }
                    }
                }
                this->calcule_euclidean_distances();  // calculando a matriz de distâncias
                break;
            }
            
            case 1: {  // instância Eil51 da TSPLIB
                if (TEST == -1) n_nodes = 51;
                else n_nodes = TEST;
                file_name = "instances/eil51.txt";
                break;
            }
            
            case 2: {  // instância Eil76 da TSPLIB
                if (TEST == -1) n_nodes = 76;
                else n_nodes = TEST;
                file_name = "instances/eil76.txt";
                break;
            }
            
            case 3: {  // instância Eil101 da TSPLIB
                if (TEST == -1) n_nodes = 101;
                else n_nodes = TEST;
                file_name = "instances/eil101.txt";
                break;
            }
            
            case 4: {  // instância pr76 da TSPLIB
                if (TEST == -1) n_nodes = 76;
                else n_nodes = TEST;
                file_name = "instances/pr76.txt";
                break;
            }
            
            case 5: {  // instância pr1002 da TSPLIB
                if (TEST == -1) n_nodes = 1002;
                else n_nodes = TEST;
                file_name = "instances/pr1002.txt";
                break;
            }
            
            case 6: {  // instância sgb128
                if (TEST == -1) n_nodes = 128;
                else n_nodes = TEST;
                file_name = "instances/sgb128.txt";
                break;
            }
        }
                
        if (MODEL_INSTANCE > 0){ // excluindo grids de testes
            index_last_nodes_vector = -1;
            delete [] plan_nodes_vector;
            plan_nodes_vector = new Node[n_nodes];
            delete euclidean_matrix;  // criando matriz de distâncias
            euclidean_matrix = new DoubleMatrix(n_nodes);
            // carregando instância do arquivo:
            file_load.open(file_name, ios::in);
            int vertex;
            double cx;
            double cy;
            for (int i=0; i < n_nodes; i++){
                file_load >> vertex;
                file_load >> cx;
                file_load >> cy;
                this->add_vertex(cx, cy);
            }
            file_load.close();
            this->calcule_euclidean_distances();  // calculando a matriz de distâncias euclidianas
        }
}

void EuclideanGraph::create_empty_graph(int n_nodes){
    this->n_nodes = n_nodes;
    index_last_nodes_vector = -1;
    delete [] plan_nodes_vector;
    plan_nodes_vector = new Node[n_nodes];
    delete euclidean_matrix;  // criando matriz de distâncias
    euclidean_matrix = new DoubleMatrix(n_nodes);
}

void EuclideanGraph::add_vertex(double x, double y){
    index_last_nodes_vector++;
    plan_nodes_vector[index_last_nodes_vector].create(x, y);
}
void EuclideanGraph::calcule_euclidean_distances(){  // preenche a matriz de distâncias
    double aux1;
    double aux2;
    double dist;
    for (int i=0; i < n_nodes; i++){
        for (int j=0; j < n_nodes; j++){
            aux1 = plan_nodes_vector[i].x() - plan_nodes_vector[j].x();
            aux2 = plan_nodes_vector[i].y() - plan_nodes_vector[j].y();
            dist = sqrt(pow(aux1,2) + pow(aux2,2));
            if (TYPE_EUCLID_MATRIX == 1){    // matriz de distâncias inteiras (arredondamento)
                dist = floor(dist + 0.5);       // floor trunca para baixo 
            }
            euclidean_matrix->set_value(i,j,dist);
        }
    }       
}
void EuclideanGraph::calcule_euclidean_distances_div1000(){  // preenche a matriz de distâncias
    double aux1;
    double aux2;
    double dist;
    for (int i=0; i < n_nodes; i++){
        for (int j=0; j < n_nodes; j++){
            aux1 = plan_nodes_vector[i].x() - plan_nodes_vector[j].x();
            aux2 = plan_nodes_vector[i].y() - plan_nodes_vector[j].y();
            dist = sqrt(pow(aux1,2) + pow(aux2,2));
            dist = dist/1000;
            if (TYPE_EUCLID_MATRIX){  // matriz de distâncias inteiras (arredondamento)
                dist = floor(dist + 0.5);  // floor trunca para baixo 
            }
            euclidean_matrix->set_value(i,j,dist);
        }
    }       
}


int EuclideanGraph::n_nodes_graph() { return n_nodes; }
Node* EuclideanGraph::get_nodes_vector(){ return plan_nodes_vector; } 
DoubleMatrix* EuclideanGraph::get_euclidean_matrix(){ return euclidean_matrix; }
double EuclideanGraph::get_value_euclidean_matrix(int i, int j){
    return euclidean_matrix->get_value(i,j);
}
    
void EuclideanGraph::print_nodes(){
        for (int i=0; i <= index_last_nodes_vector; i++){
            cout << i+1 << "\t" << plan_nodes_vector[i].x() << "\t";
            cout << plan_nodes_vector[i].y() << "\r\n";
        }
}
void EuclideanGraph::print_euclidean_matrix(){
        cout << "\r\nEuclidean matrix:\r\n"; 
        for (int i=0; i < n_nodes; i++){
            cout << "\t" << i; 
        }
        for (int i=0; i < n_nodes; i++){
            cout << "\r\n" << i;
            for (int j=0; j < n_nodes; j++){
                cout << "\t" << euclidean_matrix->get_value(i,j);
            }
        }
        cout << "\r\n";
}
void EuclideanGraph::print_col_dist_matrix(int col){
        for (int i=0; i < n_nodes; i++){
            if (i==col) cout << "\t" << i; 
        }
        for (int i=0; i < n_nodes; i++){
            cout << "\r\n";
            cout << i;
            for (int j=0; j < n_nodes; j++){
                if (j==col) cout << "\t" << euclidean_matrix->get_value(i,j);
            }
        }
        cout << "\r\n";
}
void EuclideanGraph::print_visibility_matrix(){
        for (int i=0; i < n_nodes; i++){
            cout << "\t" << i; 
        }
        for (int i=0; i < n_nodes; i++){
            cout << "\r\n" << i;
            for (int j=0; j < n_nodes; j++){
                cout << "\t" << 1 / euclidean_matrix->get_value(i,j);
            }
        }
        cout << "\r\n";
}
