/*
 * Muon.h
 *
 *  Created on: 2014年11月17日
 *      Author: torrence
 */


#ifndef MUONTEST_MUON_H_
#define MUONTEST_MUON_H_
#include<iostream>
#include<cmath>
#include<time.h>
#include<stdlib.h>

#include "../util/Rand.h"

class Muon {
public:
	Muon();
	Muon(double,double);
	Muon(double(*)());
	virtual ~Muon();
	double get_x();
	double get_y();
	double get_theta();
	double get_phi();
	double get_z(){
		return this->_z;
	}
	void set_z(double _H){
		this->_z = _H;
	}
	void set_x(double _x){
		this->x = _x;
	}
	void set_y(double _y){
		this->y = _y;
	}
	void set_theta(double _angle){
		this->theta = _angle;
	}
	void set_phi(double _angle){
		this->phi = _angle;
	}
	//飞行过程垂直距离down_h与水平面的坐标
	double* flight_point(double);

private:
	double theta_gen_DEFAULT();
	void generate();

	double x;
	double y;
	double _z;		//出射muon生成高度
	double theta;
	double phi;
	double _X_SCALE;
	double _Y_SCALE;
	double(*theta_gen)();
};

#endif /* MUONTEST_MUON_H_ */

