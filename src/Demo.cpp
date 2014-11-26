//============================================================================
// Name        : Demo.cpp
// Author      : Yitao Wu
// Version     : 1.0
// Copyright   : Copyright Reserved@Yitao Wu
// Description : 对已有类进行测试，包含包内其他类的Demo函数
//============================================================================

#include "MuonTest.h"

#include<iostream>
#include<fstream>
#include<math.h>
#include<time.h>
#include<stdlib.h>
#include<vector>

#include "util/Histo1.h"
#include "util/Vector.h"
#include "util/Rand.h"


using namespace std;

Histo1 *h1;
fstream fout;

void Histo1Demo(){
	h1 = new Histo1(0,asin(1),100);
	for(long i = 0 ; i < 1000000; i ++){
		h1->fill(Rand::Rejection(cos,1,0,asin(1)));
	}
	h1->plot();
}

void VectorDemo(){
	Vector p1(0,0,2);
	Vector p2(0,1,2);
	(p1+p2).plot();
}

void RandDemo(){
	h1 = new Histo1(1,5,1000);
	for(int count = 0 ; count < 1000000 ; count ++)
		h1->fill(Rand::Gaussian(0.1,1));
	h1->plot();
	delete h1;
}

/**
 * 模拟成像系统测试函数
 * */
void MuonTestDemo(){
/**
 * 系统声明 - 相关参数预定义<default>
 * */
	MuonTest SYSTEM;
	//设置MRPC参数-尺寸-位置测量精度
	SYSTEM.set_MRPC_Lx(400);
	SYSTEM.set_MRPC_Ly(400);
	SYSTEM.set_MRPC_dx(5);
	SYSTEM.set_MRPC_dy(5);
	//设置成像系统参数-MRPC间距-探测区域高度
	SYSTEM.set_SYSTEM_d(400);
	SYSTEM.set_SYSTEM_h(200);

/**
 * SYSTEM调用参数
 *
 * 		- 模拟入射次数
 * 		- 系统功能设定
 * */
	int simulate_times = (int)pow(10,5);
	vector<short> probe;

/**
 * 系统角分辨率计算
 * */
	//probe.push_back(SYSTEM.DELTA_THETA);

/**
 * 系统角分布直方图
 * */
	//SYSTEM.set_theta_h1(0,asin(1.0),100);
	//probe.push_back(SYSTEM.THETA_HIST);

/**
 * 系统中心截面入射点分布
 *
 * 		--MORE: 增加截面选择接口<截面高度>
 * */
	//probe.push_back(SYSTEM.POINT_OUT);

/**
 * 铅块原始入射模型测试
 * */
	//SYSTEM.set_through_L_h1(0,140,1400);
	//probe.push_back(SYSTEM.THROUGH_LEAD);

/**
 * PoCA图像重建模型测试
 * */
	LeadCuboid* lead_1;
	lead_1 = new LeadCuboid(100,100,100,100,100);
	SYSTEM.add_lead(lead_1);
	LeadCuboid* lead_2;
	lead_2 = new LeadCuboid(100,100,100,300,300);
	SYSTEM.add_lead(lead_2);
	probe.push_back(SYSTEM.PoCA_IMAGING);

	SYSTEM.muon_in(simulate_times , &probe);

	/*计算角分辨率随MRPC间距变化
	fout.open("d_theta-vs-d.txt",ios::out);
	fout<<"=====d_theta vs d========"<<endl;
	for(int _d = 200 ; _d <= 1500 ; _d+=50){
		SYSTEM.set_SYSTEM_d(_d);
		SYSTEM.muon_in(simulate_times , &probe);
		fout<<_d<<" "
				<<1000 * SYSTEM.get_delta_theta() / SYSTEM.get_count_system()
				<<" "
				<<100 * SYSTEM.get_count_system() / (double)simulate_times
				<<endl;
		SYSTEM.reset();
	}*/

	cout<<"-----------HistogramPlot---------------"<<endl;

	//SYSTEM.ImagePlot();
	SYSTEM.ImageSimplePlot();

	//SYSTEM.get_through_L_h1()->plot();
	//cout<<"--------------"<<endl
	//		<<SYSTEM.get_SQRT_L()
	//		<<endl;

	//SYSTEM.get_theta_h1()->plot();
	cout<<"-----------HistogramEnd---------------"<<endl;

	//参数输出
	cout<<"/***********************/"<<endl
			<<"MRPCs_D : "<<SYSTEM.get_SYSTEM_d()<<endl
			<<"Dectect_h : "<<SYSTEM.get_SYSTEM_h()<<endl
			<<"Platform_h : "<<SYSTEM.get_PLATFORM_H()
			<<endl;

	//计数器数据输出
	cout<<"==========="<<endl
			<<"Counters : "<<endl
			<<"System : "<<SYSTEM.get_count_system()<<endl
			<<"Lead : "<<SYSTEM.get_count_lead()<<endl;

	//平均值和比例数据输出
	cout<<"============"<<endl
			<<"Average & Ratio : "<<endl
			<<"Delta Theta : "<<SYSTEM.get_delta_theta() / SYSTEM.get_count_system()
			<<endl
			<<"Detection Ratio : "<<SYSTEM.get_count_system() / (double)simulate_times<<endl;


}

int main() {
	/*初始化随机数生成标准时钟*/
	srand(time(NULL));

	//Histo1Demo();

	//VectorDemo();

	//RandDemo();

	MuonTestDemo();

	return 0;
}

