/* 
 * File:   DoubleMatrix.hpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

// matriz bidimensional com valores double
#if !defined(__DOUBLE_MATRIX_HPP__)
    #define __DOUBLE_MATRIX_HPP__

    #include "ControlExperiment.hpp"    
    #include <iostream>    
    #include <iomanip>
    #include <fstream>
    using namespace std;
    
    class DoubleMatrix{  
        int dimension;      // numero de valores da matriz = dimension x dimension
        double** matrix;    // matriz
        
    public:    
        DoubleMatrix(int dimension);
        ~DoubleMatrix();
        void set_value(int i, int j, double value);
        double get_value(int i, int j);
        double* get_col_matrix (int col);
        void print_matrix();
        void save_matrix();
    };

#endif
