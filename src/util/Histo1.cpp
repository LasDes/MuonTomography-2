/*
 * Histo1.cpp
 *
 *  Created on: 2014年11月19日
 *      Author: torrence
 */

#include "../util/Histo1.h"

using namespace std;

Histo1::Histo1(double from,double to,int size) {
	this->_FROM = from;
	this->_TO = to;
	this->_SIZE = size;
	this->count = new int[size];
	initial();
}

Histo1::~Histo1() {}

void Histo1::initial(){
	for( int index = 0 ; index < this->_SIZE ; index ++){
		count[index]=0;
	}
}

int Histo1::locate(double value){
	if(value == this->_TO)
		return this->_SIZE -1;
	return (int)((double)this->_SIZE*
			(value - this->_FROM) / (this->_TO-this->_FROM)) ;
}

void Histo1::fill(double value){
	if(value <= this->_TO && value >= this->_FROM)
		count[this->locate(value)]++;
}
void Histo1::plot(){
	cout<<"Histogram1D"<<endl<<
			"Size = "<<this->_SIZE<<endl
			<<"From "<<this->_FROM<<" to "<<this->_TO
			<<endl<<"Value Count"<<endl;
	for( int index = 0 ; index < this->_SIZE ; index ++){
		cout<<this->_FROM + index * (this->_TO - this->_FROM) / this->_SIZE
				<<" "<<count[index]<<endl;
	}

}
