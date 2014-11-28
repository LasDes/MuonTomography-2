/*
 * Muon.cpp
 *
 *  Created on: 2014年11月17日
 *      Author: torrence
 */

#include "Muon.h"

using namespace std;

Muon::Muon() {
	_X_SCALE = 400;
	_Y_SCALE = 400;
	theta_gen = NULL;
	generate();
}
Muon::Muon(double Lx,double Ly){
	_X_SCALE = Lx;
	_Y_SCALE = Ly;
	theta_gen = NULL;
	generate();
}
Muon::Muon(double f()){
	_X_SCALE = 400;
	_Y_SCALE = 400;
	theta_gen = f;
	generate();
}

Muon::~Muon() {
}
/*默认theta抽样函数
* -简单第一类舍选抽样，存在效率提高的可能
*/
double Muon::theta_gen_DEFAULT(){
	double r,z,u;
	do{
		r = Rand::Uniform(asin(1));
		z = cos(r)*cos(r);
		u = Rand::Uniform();
	}while(u > z);
	return r;
}

void Muon::generate(){
	//x,y对给定范围内均匀抽样
	x = Rand::Uniform(_X_SCALE);
	y = Rand::Uniform(_Y_SCALE);
	//theta根据给定生成函数抽样，默认对cos[theta]^2简单舍选
	if(theta_gen)
		theta = theta_gen();
	else
		theta = this->theta_gen_DEFAULT();
	//phi对[0,2\pi]均匀抽样
	phi = Rand::Uniform(4*asin(1));
}

double* Muon::flight_point(double down_h){
	double* point;
	point = new double[2];
	point[0] = x - down_h * tan(theta)*cos(phi);
	point[1] = y - down_h * tan(theta)*sin(phi);
	return point;
}

double Muon::get_x(){
	return x;
}

double Muon::get_y(){
	return y;
}

double Muon::get_theta(){
	return theta;
}

double Muon::get_phi(){
	return phi;
}
