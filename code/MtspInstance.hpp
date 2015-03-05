/* 
 * File:   MtspInstance.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#if !defined(__MTSPINSTANCE_HPP__)
    #define __MTSPINSTANCE_HPP__

    #include "ControlExperiment.hpp"
    #include "IntList.hpp"    
    #include "DoubleMatrix.hpp"
    #include "EuclideanGraph.hpp"

    class MtspInstance{
        int depot;                        // indice da matriz de custos que representa o depósito (sempre zero, mas poderia ser alterado)
        int n_salesmen;                   // número de caixeiros da instância
        DoubleMatrix* cost_matrix;        // matriz assimétrica de custos (os índices representam os nós)
        IntList* valid_nodes;             // lista com os índices de nós validos correntes (apenas os existentes nesta lista farão parte das soluções)
        IntList* initial_nodes_routes;    // lista com os índices de nós correntes nos quais as equipes se encontram, nos dados reais
        EuclideanGraph* graph;            // grafo euclidiano da instância (a partir das coordenadas UTM, nos dados reais)
    
    public:
        MtspInstance(int n_salesmen, int depot);
        ~MtspInstance();
        
        int get_n_nodes();
        int get_n_salesmen();
        IntList* get_positions_teams();
        IntList* get_valid_nodes_instance();
        DoubleMatrix* get_cost_matrix();
        Node* get_plan_nodes_vector();
        int get_n_valid_nodes();
        int get_valid_node(int index);

        void load_euclidean_model_instance();
        
        void create_empty_instance(int n_nodes, int n_teams);
        void add_plan_node(int coord_x, int coord_y);
        void calcule_euclidean_matrix();
        
        double get_value_euclidean_matrix(int i, int j);
        
        double get_value_cost_matrix(int i, int j);
        void set_value_cost_matrix(int i, int j, double cost);
        void reset_valid_nodes_instance();
        void add_valid_node(int node);
        void reset_positions_teams();
        void add_position_team(int position);
        
        void print_instance();
        void print_valid_nodes();
        void print_positions_teams();
        void print_cost_matrix();
        void print_euclidean_matrix();
        
        void save_positions_teams(ofstream& file_out);
    };
    
#endif
    