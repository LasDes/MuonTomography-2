/*
 * Vector.cpp
 *
 *  Created on: 2014年11月21日
 *      Author: torrence
 */

#include "../util/Vector.h"

Vector::Vector(double _x,double _y ,double _z) {
	this->x = _x;
	this->y = _y;
	this->z = _z;
}

Vector::~Vector() {
	// TODO Auto-generated destructor stub
}

Vector operator +(Vector& _v1, Vector& _v2){
	Vector _v(_v1.x + _v2.x , _v1.y + _v2.y ,_v1.z + _v2.z);
	return _v;
}
Vector operator -(Vector& _v1, Vector& _v2){
	Vector _v(_v1.x - _v2.x , _v1.y - _v2.y ,_v1.z - _v2.z);
	return _v;
}
Vector operator *(double cons,Vector& _v){
	Vector _V(cons * _v.x , cons * _v.y , cons * _v.z);
	return _V;
}
Vector operator *(Vector& _v,double cons){
	Vector _V(cons * _v.x , cons * _v.y , cons * _v.z);
	return _V;
}
double operator *(Vector& _v1, Vector& _v2){
	return (_v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z);
}
Vector& Vector::operator=(Vector& _v){
	Vector _V(_v.x,_v.y,_v.z);
	return _V;
}
void Vector::plot(){
	cout<<
			this->x<<" "<<this->y<<" "<<this->z
			<<endl;
}

