/* 
 * File:   Node.cpp
 * Author: Denilson Fagundes Barbosa
 * E-mail: denilsonfag@yahoo.com.br
 * Created on 22 de Novembro de 2014, 12:20
 */

#include "Node.hpp"

Node::Node(){}
void Node::create(double x, double y){
        cx = x;
        cy = y;
}
Node::~Node(){}
double Node::x(){ return cx; }
double Node::y(){ return cy; }
