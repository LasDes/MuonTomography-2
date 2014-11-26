/*
 * Histo1.h
 *
 *一维生成抽样结果柱状图
 *
 *  Created on: 2014年11月19日
 *      Author: torrence
 */

#ifndef HISTO1_H_
#define HISTO1_H_

#include<vector>
#include<iostream>

using namespace std;

class Histo1 {
public:
	Histo1(double from,double to,int size);
	virtual ~Histo1();
	void fill(double);
	void plot();
private:
	void initial();
	int locate(double);
	double _FROM;
	double _TO;
	int _SIZE;
	int* count;
};


#endif /* HISTO1_H_ */
