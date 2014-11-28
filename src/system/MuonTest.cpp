/*
 * MuonTest.cpp
 *
 *  Created on: 2014年11月18日
 *      Author: torrence
 *
 *
 */

//============================================================================
// Name        : MuonTest.cpp
// Author      : Yitao Wu
// Version     : 1.2
// Copyright   : Copyright Reserved@Yitao Wu
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "MuonTest.h"

using namespace std;
/**
 * 成像系统默认构造函数
 * */
MuonTest::MuonTest(){
	this->fout = NULL;
	/*
	 * 系统参数设定
	 * */
	this->_DETECT_H = 200;		//_DETECT_H，探测区域高度，单位mm
	this->_MRPCs_D = 400;		//MRPC_#1#2或#3#4板间距，单位mm

	//注意承重平台高度受探测区域高度限制
	//缺省为1/4
	this->_PLATFORM_H = _DETECT_H / 4.0 ;

	//MRPC板的尺寸mm
	this->_X_SCALE = 400;
	this->_Y_SCALE = 400;

	//MRPC板的精度mm
	this->_X_DELTA = 5;
	this->_Y_DELTA = 5;

	//铅块放置信息设定
	this->_Lead = new vector<LeadCuboid>;

	/**
	 * 初始化观测量
	 * */
	//初始化计数器
	this->count_system = 0;
	this->count_lead = 0;

	//初始化系统角分辨率
	this->delta_theta = 0.0;

	//重置theta分布直方图
	this->theta_h1 = NULL;
	//重置原始入射长度根号平均值
	this->sqrt_L = 0.0;
	//重置muon穿过铅块内部路程L分布直方图
	this->through_L_h1 = NULL;

	//初始化point_out输出流
	if(this->point_out)
		point_out.close();

	//初始化_M_tracer
	this->_M_tracer = NULL;

	//初始化图像重建类
	this->_image = NULL;
}

/**
 * Reset The System
 *
 * 		初始化/重设成像系统到默认状态
 * 				-参数重设
 * 				-清空指针NULL
 * 				-关闭文件流
 * */
void MuonTest::reset(){
	/*
	 * 系统参数设定
	 * */
	this->set_SYSTEM_d(400);  //MRPC_#1#2或#3#4板间距，单位mm
	this->set_SYSTEM_h(200); //_DETECT_H，探测区域高度，单位mm
	//注意承重平台高度受探测区域高度限制
	//缺省为1/4
	this->set_PLATFORM_H(_DETECT_H / 4.0);

	//MRPC板的尺寸mm
	this->set_MRPC_Lx(400);
	this->set_MRPC_Ly(400);

	//MRPC板的精度mm
	this->set_MRPC_dx(5);
	this->set_MRPC_dy(5);

	//铅块放置信息设定
	this->_Lead = new vector<LeadCuboid>;

	/**
	 * 初始化观测量
	 * */
	//初始化计数器
	this->count_system = 0;
	this->count_lead = 0;

	//初始化系统角分辨率
	this->delta_theta = 0.0;

	//重置theta分布直方图
	this->theta_h1 = NULL;
	//重置原始入射长度根号平均值
	this->sqrt_L = 0.0;
	//重置muon穿过铅块内部路程L分布直方图
	this->through_L_h1 = NULL;

	//初始化point_out输出流
	if(this->point_out)
		point_out.close();

	//初始化图像重建类
	this->_image = NULL;
}

/*
 * 计算单次入射角分辨率,MRPC_#01-02
*/
double MuonTest::delta_theta_in(Muon *m){
	double d_t_i;
	d_t_i = pow(this->_X_DELTA,2) * pow(cos(m->get_phi()),2)
			+ pow(this->_Y_DELTA,2) * pow(sin(m->get_phi()),2);
	d_t_i = sqrt(2.0 * d_t_i) * cos(m->get_theta()) * cos(m->get_theta());
	return d_t_i / _MRPCs_D ;
}
/*模拟muon穿过长方体铅块
 * 尺寸length*width*height(mm^3)
 * 默认为100mm立方体
*	计算穿过铅块的路程L
*	 -基本思路:层析法
*	 铅块均匀分1000层水平面
*	 计算muon的分层坐标
*	 判断是否在该层的铅块内
*
*	 2014.11.1:
*	 	铅块尺寸及分布设定为系统参数vector<LeadCuboid>*
*	 	函数调用该参数进行分析
*	 	提供外部设置接口，提供多点分布可能
*/
void MuonTest::add_default_lead(){
	if(this->_Lead->empty()){
		cout<<"!!!No lead in the system!!!"<<endl
				<<"!!!AutoSet a default cube in the center!!!"<<endl;
		LeadCuboid lead(100.0,100.0,100.0,
				this->_X_SCALE / 2.0 ,this->_Y_SCALE / 2.0);
		this->_Lead->push_back(lead);
	}
}
double MuonTest::through_lead(Muon* muon){

	double through_L = 0.0;

	for(vector<LeadCuboid>::iterator itr = this->_Lead->begin();
			itr != this->_Lead->end() ; itr++)
		through_L += this->through_lead_cuboid(muon,*itr);
	if(through_L > 0){
		this->count_lead ++;
		this->sqrt_L += sqrt(through_L);
	}
	return through_L;
}
double MuonTest::through_lead_cuboid(Muon* muon,LeadCuboid lead){

	double delta_H = 0.0;

	const double LAYERS_NUM = 1000;
	const double dH = lead.height / LAYERS_NUM; //对高度细分1000层

	//下降dH的水平坐标偏移量
	const double dx = dH*tan(muon->get_theta())*cos(muon->get_phi());
	const double dy = dH*tan(muon->get_theta())*sin(muon->get_phi());

	//muon实时水平坐标
	//初始平面为铅块上平面
	double x =muon->flight_point(
			this->_MRPCs_D+this->_DETECT_H - this->_PLATFORM_H - lead.height / 2)[0];
	double y =muon->flight_point(
			this->_MRPCs_D+this->_DETECT_H - this->_PLATFORM_H - lead.height / 2)[1];

	this->_M_tracer->initial_Lead();

	_Point in(x,y,
			this->_MRPCs_D + this->_PLATFORM_H + lead.height);

	bool into_lead = false;

	//逐层判断muon是否经过铅块
	//后期增加提前结束循环判断，减少运算量
	for(int i = 0 ; i < LAYERS_NUM ; i++){
		if( x < lead.x_center + lead.length / 2.0
				&& x > lead.x_center - lead.length / 2.0 ){
			if( y < lead.y_center + lead.width / 2.0
					&& y > lead.y_center - lead.width / 2.0 ){
				into_lead =true;
				delta_H += dH;
			}//if_y
		}//if_x
		//调整入射点的位置
		if( ! into_lead){
			in.x = x;
			in.y = y;
			in.z -= dH;
		}//if_muon_not_in_lead
		x += dx; y += dy;
	}//for_i
	if(into_lead){
		this->_M_tracer->add_Lead_in(&in);
		_Point out(x , y ,
				this->_MRPCs_D + this->_PLATFORM_H + lead.height - delta_H);
		this->_M_tracer->add_Lead_out(&out);
	}
	return delta_H / cos(muon->get_theta());
}

/**
 * 散射模型
 * */
void MuonTest::multiple_scatter(Muon *m_in , Muon* m_out,
		LeadCuboid lead){
	double L_origin = this->through_lead_cuboid(m_in,lead);
	//无散射
	if( L_origin < 0.01){
		return;
/**Debug11261807
		double* point = m_in->flight_point(
				this->_DETECT_H + this->_MRPCs_D - this->_PLATFORM_H);
		m_out->set_x(point[0]);
		m_out->set_y(point[1]);
		m_out->set_z(this->_MRPCs_D + this->_PLATFORM_H);
		m_out->set_theta(m_in->get_theta());
		m_out->set_phi(m_in->get_phi());
**/
	}
	//穿过铅块发生散射
	//多次散射模型
	else{
		this->count_lead ++;
		/*
		 * 束流Moliere模型
		 * */
		_Point& p_in = *(this->_M_tracer->get_Lead_in()->begin());
		_Point& p_out = *(this->_M_tracer->get_Lead_out()->begin());
		m_out->set_x(p_in.x * 0.5 + p_out.x * 0.5 +Rand::Uniform(-3,3));
		m_out->set_y(p_in.y * 0.5 + p_out.y * 0.5+Rand::Uniform(-3,3) );
		m_out->set_z(p_in.z * 0.5 + p_out.z * 0.5+Rand::Uniform(-3,3) );
		m_out->set_theta(fabs(Rand::Gaussian(0.00271 * sqrt(L_origin))));
		m_out->set_phi(Rand::Uniform(4*asin(1)));
	}
}

/*
 * 模拟单个muon穿过探测系统
 *
 * 已有功能：
 * 		基础：有效入射计数 - count_system
 * 		1.入射天顶角分布
 * 		2.入射角分辨率统计
 * 		3.指定横截面入射点muon数分布
 * 		4.muon穿越铅块路程分布
 *
 * 2014.11.21: 加入switch-enum机制
 * 	提供外部接口对探测功能进行选择控制
*/
void MuonTest::muon_in(long _total_times ,vector<_Task>* probes){

	Muon* muon;

	double* point;

	for(int times = 0 ; times < _total_times ; times ++){
		if(! (times % (_total_times / 10)))
			cout<<"Processing : "<<times / (_total_times / 10)<<"/10."<<endl;
		muon  = new Muon( this->_X_SCALE, this->_Y_SCALE);

		//debug束流检测
		muon->set_theta(0.0);
		//debug束流检测

		this->_M_tracer = new MuonTracer(muon);
/*
 * 判断区
 * 	- 确认入射muon有效
 * */
			//获得muon飞行至MRPC_#4平面坐标
			point = muon ->flight_point(_MRPCs_D * 2 + _DETECT_H);
			//判断该事件是否有效，muon经过整个成系统，即经过MRPC_#4
			if(point[0]<_X_SCALE && point[0]>0)
				if(point[1]<_Y_SCALE && point[1]>0){

					//基础功能：有效入射muon计数
					this->count_system++;

					//附加功能
					for(vector<short>::iterator probeItr = probes->begin() ;
							probeItr != probes->end() ; probeItr ++){
/*
 * 数据收集与初步处理区
 *  - 已加入switch-enum组合控制任务
 * */
						switch(*probeItr){

						//统计有效入射天顶角
						case THETA_OUT:
							//天顶角数据记录
							if( fout != &_fout[MuonTest::THETA_OUT]){
								this->_fout[THETA_OUT].open("Theta-In.dat",ios::out);
								this->fout = &_fout[THETA_OUT];
							}
							*fout<<muon->get_theta()<<endl;
							//天顶角统计直方图
							if( this->theta_h1)
								this->theta_h1 = new Histo1(0,asin(1),100);
							this->theta_h1->fill(muon ->get_theta());

						break;


						//建立muon铅块内部路程L分布直方图
						case THROUGH_LEAD:
							//确定建立输出文件流
							if( this->fout != &this->_fout[MuonTest::THROUGH_LEAD]){
								this->_fout[MuonTest::THROUGH_LEAD].open("Length_inLead.txt",ios::out);
								this->fout = &this->_fout[MuonTest::THROUGH_LEAD];
							}
							//判断是否添加默认铅块
							this->add_default_lead();
							//输出铅块内路程统计到文件
							if(this->_M_tracer->get_through_lead_L() < 0.0)
								this->_M_tracer->set_through_lead_L(this->through_lead(muon));
							if(this->_M_tracer->get_through_lead_L() > 0.0)
								*fout<<this->_M_tracer->get_through_lead_L()<<endl;

						break;


						//输出muon入射到系统某横截平面入射点分布
						case POINT_OUT:
							if( this->fout != &this->_fout[MuonTest::POINT_OUT]){
								this->_fout[MuonTest::POINT_OUT].open("Point-in.dat",ios::out);
								this->fout = &this->_fout[MuonTest::POINT_OUT];
							}
							point = muon ->flight_point(
									this->_MRPCs_D+this->_DETECT_H  / 2.0);
							*fout<<point[0]<<" "<<point[1]<<endl;
						break;

						//累加有效muon的单次入射角分辨率
						case DELTA_THETA:
							this->delta_theta += delta_theta_in(muon );
						break;

						/**
						 * PoCA图像重建模型
						 * */
						case PoCA_IMAGING:
							if(! this->_image)
								this->_image = new ImagePoCA;
							_image->set_DETECT_H(this->_DETECT_H);
							_image->set_MRPCs_D(this->_MRPCs_D);


							this->add_default_lead();
							Muon* m_out = new Muon;

							this->initial_muon_out(muon,m_out);

							for(vector<LeadCuboid>::iterator itr = this->_Lead->begin();
									itr != this->_Lead->end() ; itr++)
								this->multiple_scatter(muon,m_out,*itr);

							this->_M_tracer->set_muon_out(m_out);

							this->_M_tracer->autoset_MRPC_record(
									this->_MRPCs_D, this->_DETECT_H);
							this->_image->PoCA(this->_M_tracer->get_MRPC_record());
						break;
						}//switch_功能选择
					}//for_probeItr_多功能选择控制循环
				}//if_判断入射muon是否穿过MRPC板
			delete muon;muon = NULL;
			delete this->_M_tracer;this->_M_tracer = NULL;
		}//for_times

	cout<<"Simulation Completed!"<<endl;
}

/*
 * Getter & Setter Functions
 * */

//Getter
double MuonTest::get_SQRT_L(){
	return this->sqrt_L / (double)this->count_lead;
}
double MuonTest::get_SYSTEM_d(){
	return this->_MRPCs_D;
}
double MuonTest::get_SYSTEM_h( ){
	return this->_DETECT_H;
}
double MuonTest::get_PLATFORM_H( ){
	return this->_PLATFORM_H;
}
double MuonTest::get_MRPC_Lx( ){
	return this->_X_SCALE;
}
double MuonTest::get_MRPC_Ly( ){
	return this->_Y_SCALE;
}
double MuonTest::get_MRPC_dx( ){
	return this->_X_DELTA;
}
double MuonTest::get_MRPC_dy( ){
	return this->_Y_DELTA;
}
double MuonTest::get_delta_theta(){
	return this->delta_theta;
}
vector<LeadCuboid>* MuonTest::get_lead(){
	return this->_Lead;
}
long MuonTest::get_count_system(){
	return this->count_system;
}
long MuonTest::get_count_lead(){
	return this->count_lead;
}
Histo1* MuonTest::get_theta_h1(){
	return this->theta_h1;
}
Histo1* MuonTest::get_through_L_h1(){
	return this->through_L_h1;
}

void MuonTest::ImagePlot(){
	if( !this->_image){
		cout<<"!!!No image!!!"<<endl;
		return;
	}
	_image->plot();
}
void MuonTest::ImageSimplePlot(fstream& _fout){
	if( !this->_image){
		cout<<"!!!No image!!!"<<endl;
		return;
	}
	_image->plot_simple(_fout);
}

//Setter
/**
 * 无散射情况下的Muon，即默认出射Muon
 * */
bool MuonTest::initial_muon_out(Muon* _m_in , Muon* _m_out){
	if(!_m_in || !_m_out)
		return false;

	double* point = _m_in->flight_point(
			this->_DETECT_H + this->_MRPCs_D - this->_PLATFORM_H);
	_m_out->set_x(point[0]);
	_m_out->set_y(point[1]);
	_m_out->set_z(this->_MRPCs_D + this->_PLATFORM_H);
	_m_out->set_theta(_m_in->get_theta());
	_m_out->set_phi(_m_in->get_phi());
	return true;
}

/**
 * Function: 判断添加的铅块lead尺寸是否合法
 * 		-1.高度超过整个探测区域 - return false
 * 		MORE:
 * 		-2.高度超过平台上方区域，自动调整平台高度(输出Warning)
 * 		-3.与已有铅块有重合部分
 * 			-3.1 拒绝添加 -输出Error - return false
 * 			-3.2 尝试重新安排已有铅块
 * 				-3.2.1 不可重排 -输出Error - return false
 * 				-3.2.2 铅块可被重排， 调整 - 输出Warning
 * 					-打印铅块分布信息
 * */
bool MuonTest::add_lead(LeadCuboid *_lead){
	if( _lead->height + this->_PLATFORM_H > this->_DETECT_H )
		return false;
	this->_Lead->push_back(*_lead);
	return true;
}
void MuonTest::set_PLATFORM_H(double h){
	if(h > this->_DETECT_H)
		h = this->_DETECT_H / 4.0;
	this->_PLATFORM_H = h;
}
void MuonTest::set_theta_h1(double from , double to ,int size){
	this->theta_h1 = new Histo1(from,to,size);
}
void MuonTest::set_through_L_h1(double from,double to,int size){
	this->through_L_h1 = new Histo1(from,to,size);
}
void MuonTest::set_SYSTEM_d(double d){
	this->_MRPCs_D = d;
}
void MuonTest::set_SYSTEM_h(double h){
	this->_DETECT_H = h;
}
void MuonTest::set_MRPC_Lx(double Lx){
	this->_X_SCALE = Lx;
}
void MuonTest::set_MRPC_Ly(double Ly){
	this->_Y_SCALE = Ly;
}
void MuonTest::set_MRPC_dx(double dx){
	this->_X_DELTA = dx;
}
void MuonTest::set_MRPC_dy(double dy){
	this->_Y_DELTA = dy;
}


