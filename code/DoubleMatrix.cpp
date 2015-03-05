/* 
 * File:   DoubleMatrix.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "DoubleMatrix.hpp"

DoubleMatrix::DoubleMatrix(int dimension){
    this->dimension = dimension;
    matrix = new double*[dimension];
    for (int i=0; i < dimension; i++){
        matrix[i] = new double[dimension];
    }
}
DoubleMatrix::~DoubleMatrix(){
    for (int i=0; i < dimension; i++){
        delete [] matrix[i];
    }        
    delete [] matrix; 
}

void DoubleMatrix::set_value(int i, int j, double value){
    matrix[i][j] = value;
}
double DoubleMatrix::get_value(int i, int j){
    return matrix[i][j];
}
double* DoubleMatrix::get_col_matrix (int col){
    return matrix[col];
}

void DoubleMatrix::print_matrix(){
    cout << "matrix:\r\n"; 
    for (int i=0; i < dimension; i++){
        cout << "\t" << i; 
    }
    for (int i=0; i < dimension; i++){
        cout << "\r\n" << i;
        for (int j=0; j < dimension; j++){
            if (i == j){
                cout << "\t" << 0.0;
            }
            else{
                cout << "\t" << matrix[i][j];
            }
        }
    }
    cout << "\r\n";
}

void DoubleMatrix::save_matrix(){
    ofstream file_mat;
    file_mat.open("outs/matrix.txt", ios::app);
    file_mat << setiosflags (ios::fixed) << setprecision(FLOAT_PRECISION);

    file_mat << "Matrix:\r\n"; 
    for (int i=0; i < dimension; i++){
        file_mat << "\t" << i; 
    }
    for (int i=0; i < dimension; i++){
        file_mat << "\r\n" << i;
        for (int j=0; j < dimension; j++){
            file_mat << "\t" << matrix[i][j];
        }
    }
    file_mat << "\r\n\r\n";
    file_mat.close();
}
