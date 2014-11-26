/*
 * ImagePoCA.h
 *
 *  Created on: 2014年11月24日
 *      Author: torrence
 */
/***************
*
* PoCA图像重建类 - class ：(public Histogram?
* ClassName : PoCA_Image
*
* 数据域：
*  整体信息:
*   重建区域尺寸
*	_X_SCALE _Y_SCALE
*   区域块数量
*	_X_NUM _Y_NUM
*   区域块数组_Block**
*
*  区域块 struct - Block
*   散射角记录
*	vector<double>* scatter_angle
*   散射角均方根
*	scatter_angle_rms
*
*  RMS-FLAG
* 	_is_rmsed
*
* 函数域
*  构造器
*	Image()
*  析构器
*	~Image()
*  Getter & Setter
*  初始化
*	initial()
*  PoCA散射重建
*	PoCA(vector<_Point>*)
*  数据采集
*	locateX(double)
*	locateY(double)
*	fill(_Point*,double)
*  均方根处理
*	rms()
*  绘图
*	plot()
**********/
#ifndef IMAGE_IMAGEPOCA_H_
#define IMAGE_IMAGEPOCA_H_

#include<vector>
#include<iostream>
#include<cmath>

#include "../util/Vector.h"
typedef Vector _Point;

using namespace std;

class Image_PoCA {
public:
	Image_PoCA();
	virtual ~Image_PoCA();
	void initial();
	//Setter & Getter
	void set_DETECT_H(double _h){
		this->_DETECT_H = _h;
	}
	void set_MRPCs_D(double _d){
		this->_MRPCs_D = _d;
	}

	//散射重建PoCA算法
	bool PoCA(vector<_Point>*);

	//数据采集
	int locateX(double x_value);
	int locateY(double y_value);
	//int locateZ(double z_value);		//预留三维重建
	bool fill2(_Point*,double _angle);
	//bool fill3(_Point*,double _angle); //预留三维重建

	//均方根处理
	void rms_calc();

	//绘图
	void plot();
	void plot_simple();
private:
	//成像系统尺寸
	double _MRPCs_D;
	double _DETECT_H;
	//重建区域尺寸
	double _X_SCALE;
	double _Y_SCALE;
	//double _Z_SCALE;	//预留三维重建
	//区域块数量
	int _X_NUM;
	int _Y_NUM;
	//int _Z_NUM;			//预留三维重建
	//区域块结构体
	struct Block {
		vector<double>* scatter_angle; 	//散射角记录
		double scatter_angle_rms ;		//散射角均方根
	};
	bool _is_rmsed;	//均方根计算布尔量
	Block** _Block2;
	//Block*** _Block3;			//预留三维重建
};

#endif /* IMAGE_IMAGEPOCA_H_ */
