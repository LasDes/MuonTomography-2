/*
 * Vector.h
 *
 * Vector & Point(看做原点指出的向量)
 *
 *	构建三维空间的向量，均具有加法减法乘法
 *
 *
 *  Created on: 2014年11月21日
 *      Author: torrence
 */

#ifndef PUBLIC_Vector_H_
#define PUBLIC_Vector_H_

#include<iostream>

using namespace std;


class Vector {
public:
	Vector(double _x , double _y, double _z);
	virtual ~Vector();

	void plot();

	friend Vector operator -(Vector& _v1, Vector& _v2);
	friend Vector operator +(Vector& _v1, Vector& _v2);
	friend Vector operator *(double cons, Vector& _v);
	friend Vector operator *(Vector& _v,double cons);
	friend double operator *(Vector& _v1, Vector& _v2);
	Vector& operator=(Vector& _v);
	double x;
	double y;
	double z;
};


#endif /* PUBLIC_Vector_H_ */
