/*
 * MuonTest.h
 *
 *		模拟muon成像系统
 *			--成像系统模型
 *				--系统包含MRPC*4，探测区域，铅块，铅块承载平台
 *				--探测器部分：计数器，角分辨率，天顶角分布，铅块散射路程分布，散射角
 *			--提供muon入射模拟
 *			--提供muon铅块入射模拟
 *			MORE:
 *			--提供muon铅块散射模型
 *			--提供PoCA图像重建？需要考虑对实际实验的适配
 *
 *  Created on: 2014年11月18日
 *      Author: torrence
 */


#ifndef MUONTOMOGRAPHY_SRC_MUONTEST_H_
#define MUONTOMOGRAPHY_SRC_MUONTEST_H_

#include "Muon.h"
#include "MuonTracer.h"

#include "util/Histo1.h"
#include "util/LeadCuboid.h"
#include "util/Vector.h"
#include "util/Rand.h"

#include "image/ImagePoCA.h"

typedef Vector _Point;

#include <iostream>
#include<fstream>
#include<cmath>
#include<time.h>
#include<stdlib.h>
#include<vector>


class MuonTest {
public:
	//Constructor
	MuonTest();
	MuonTest(double,double);

	//Reset the System - DEFAULT
	void reset();

	//Setter
	//MORE: 考虑添加参数判定，校验输入合法性，返回bool
	void set_SYSTEM_d(double);
	void set_SYSTEM_h(double);
	void set_PLATFORM_H(double);
	void set_MRPC_Lx(double);
	void set_MRPC_Ly(double);
	void set_MRPC_dx(double);
	void set_MRPC_dy(double);
	void set_theta_h1(double from,double to ,int size);
	void set_through_L_h1(double from,double to ,int size);
	bool add_lead(LeadCuboid *lead);
	bool initial_muon_out(Muon* _m_in , Muon* _m_out);


	//Getter
	double get_SQRT_L();
	double get_SYSTEM_d();
	double get_SYSTEM_h( );
	double get_PLATFORM_H( );
	double get_MRPC_Lx( );
	double get_MRPC_Ly( );
	double get_MRPC_dx( );
	double get_MRPC_dy( );
	double get_delta_theta();
	long get_count_system();
	long get_count_lead();
	Histo1* get_theta_h1();
	Histo1* get_through_L_h1();
	vector<LeadCuboid>* get_lead();
	void ImagePlot();
	void ImageSimplePlot();

	//Simulation
	void muon_in(long times ,vector<short>* probes);
	enum Function
		{THETA_HIST ,
			DELTA_THETA ,
			POINT_OUT ,
			THROUGH_LEAD ,
			MULTIPLE_SCATTER,
			PoCA_IMAGING};

private:
	//计算单次入射角分辨率
	double delta_theta_in(Muon *m );

	//模拟muon穿过探测区域内长方体铅块
	//计算穿过铅块的路程
	double through_lead_cuboid(Muon *m,LeadCuboid lead);
	double through_lead(Muon *m);
	void add_default_lead();
	/**
	 * 模拟铅块内多次散射
	 * Input:
	 * 			Muon*			-入射μ子
	 * 			LeadCuboid	-散射铅块
	 * 	Output:
	 * 			Muon*			-出射μ子
	 * */
	void multiple_scatter(Muon* m_in, Muon* m_out,LeadCuboid lead);

	//Tracer - Detector
	//Muon径迹追踪器
	MuonTracer* _M_tracer;

	/**
	 * 系统参量定义为类全局变量
	 * */
	//MRPC尺寸参量mm
	double _X_SCALE;
	double _Y_SCALE;
	//MRPC精度参量mm
	double _X_DELTA;
	double _Y_DELTA;

	//成像系统尺寸参量mm
	double _MRPCs_D ;  //MRPC_#1#2或#3#4板间距，单位mm
	double _DETECT_H ; //_DETECT_H，探测区域高度，单位mm

	//承重平台相对探测区域高度
	/*MORE: 考虑承重平台与铅块，探测区域高度之间的限制关系
	 	 写出更健壮的代码*/
	double _PLATFORM_H;

	//铅块
	/**
	 * MORE: 拒绝直接提供_Lead，rewrite一组本地函数实现需要的功能
	 * 		--健壮性考虑
	 * */
	vector<LeadCuboid>* _Lead;

	/**
	 * 数据存储区
	 *  MORE： 根据系统参数设计足够强大的初始化函数
	 * */

	/**
	 * 系统入射模型
	 * */
	//有效入射事例，muon穿过系统计数
	long count_system;

	//有效入射muon天顶角分布直方图
	Histo1* theta_h1;

	//有效入射muon平均角分辨率
	//	不提供set
	double delta_theta;

	/**
	 * 铅块入射模型
	 * */
	//muon穿过铅块计数
	long count_lead;

	//原始入射铅块长度开放平均值
	double sqrt_L;

	//muon穿过铅块内部距离直方图
	Histo1* through_L_h1;

	//图像重建类
	Image_PoCA* _image;

	/**
	 * 	统一数据流
	 * 		MORE: 考虑多文件输出需求，扩充为一组数据流
	 * */
	fstream point_out;
	fstream fout;

};

#endif /* MUONTOMOGRAPHY_SRC_MUONTEST_H_ */
