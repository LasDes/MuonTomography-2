/*
 * ImagePoCA.cpp
 *
 *  Created on: 2014年11月24日
 *      Author: torrence
 */

#include "ImagePoCA.h"

ImagePoCA::ImagePoCA() {
	this->_MRPCs_D = 400;
	this->_DETECT_H = 200;
	this->_X_SCALE = 400;
	this->_Y_SCALE = 400;
	this->_X_NUM = 80;
	this->_Y_NUM = 80;
	_Block2 = new Block*[_X_NUM];
	this->_is_rmsed = false;
	this->initial();
}

ImagePoCA::~ImagePoCA() {
	// TODO Auto-generated destructor stub
}
void ImagePoCA::initial(){
	for(int x_index = 0 ; x_index < _X_NUM ; x_index ++){
		_Block2[x_index] = new Block[_Y_NUM];
		for(int y_index = 0 ; y_index < _Y_NUM ; y_index ++){
			_Block2[x_index][y_index].scatter_angle = new vector<double>;
			_Block2[x_index][y_index].scatter_angle_rms = 0.0;
		}
	}
}

bool ImagePoCA::PoCA(vector<_Point>* _MRPC){
	if(_MRPC->size() != 4)
		return false;

	_Point p0 = (*_MRPC)[0];
	_Point p1 = (*_MRPC)[1];
	_Point q0 = (*_MRPC)[2];
	_Point q1 = (*_MRPC)[3];

	Vector u = (p1 - p0);
	Vector v = (q1 - q0);
	Vector w = (p0 - q0);

	double a = u*u;
	double b = u*v;
	double c = v*v;
	double d = u*w;
	double e = v*w;

	if(a*c-b*b == 0)
		return false;
	Vector _pc = (b*e-c*d)  / (a*c-b*b) * u;
	Vector pc = (p0 + _pc);
	Vector _qc =  (a*e-b*d)/(a*c-b*b)*v;
	Vector qc = (q0 + _qc);

	double scatter_angle = acos(
		fabs(u*v)/(sqrt(u * u) * sqrt(v * v))
		);
	_Point PoCA(pc.x / 2.0 + qc.x / 2.0 ,
			pc.y / 2.0 + qc.y / 2.0 ,
			pc.z / 2.0 + qc.z / 2.0);

	if(fabs(scatter_angle) > 0.001){
		if(this->fill2(&PoCA , scatter_angle)){
		//debug
		//cout<<"!!!Scttering Dected!!!"<<endl;
		//PoCA.plot();
		//cout<<scatter_angle<<endl;
		//cout<<"--------------------------"<<endl;
		//debug
		return true;
		}
	}
	return false;
}

int ImagePoCA::locateX(double x_value){
	if(x_value == this->_X_SCALE)
		return this->_X_NUM - 1;
	return (int)(
	  this->_X_NUM * x_value / this->_X_SCALE
	);
}
int ImagePoCA::locateY(double y_value){
	if(y_value == this->_Y_SCALE)
		return this->_Y_NUM - 1;
	return (int)(
	  ((double)this->_Y_NUM) * y_value / this->_Y_SCALE
	);
}
/**
 * 图像填充 - 散射信息记录
 *		加入初级噪声处理
 * 		--拒绝超出探测区域的PoCA
 * */
bool ImagePoCA::fill2(_Point* _PoCA , double _scatter_angle){
	/*
	if(_PoCA->z < this->_MRPCs_D ||
			_PoCA->z > this->_DETECT_H + this->_MRPCs_D)
		return false;
	 */
	int x_index = locateX(_PoCA->x);
	int y_index = locateY(_PoCA->y);
	if(x_index < 0 || x_index >= _X_NUM)
		return false;
	if(y_index < 0 || y_index >= _Y_NUM)
		return false;
	Block& block =
	  this->_Block2[x_index][y_index];
	block.scatter_angle->push_back(_scatter_angle);
	return true;
}
void ImagePoCA::rms_calc(){
	if(this->_is_rmsed) return;

	for(int i = 0 ; i < this->_X_NUM ; i++){
	  for(int j = 0 ; j < this->_Y_NUM ; j ++){
	    vector<double>& angle =
		*(_Block2[i][j].scatter_angle);
	    double& rms =
	    		_Block2[i][j].scatter_angle_rms;

	    if(! angle.size())
	    	continue;
	    for(vector<double>::iterator itr = angle.begin() ;
		itr != angle.end(); itr ++)
	    	rms += *itr;
	    rms = sqrt(rms / (double)angle.size());
	  }
	}
	this->_is_rmsed = true;
}
void ImagePoCA::plot(){
	this->rms_calc();
	cout<<endl
			<<"=======Image========"
			<<endl;
	for(int i = 0 ; i < this->_X_NUM ; i++){
		for(int j = 0 ; j < this->_Y_NUM ; j ++){
			vector<double>& angle =
					*(_Block2[i][j].scatter_angle);
			double& rms =
					_Block2[i][j].scatter_angle_rms;
			cout<<"("
					<<rms<<","<<angle.size()
					<<")"<<" ";
		}//for_x_i
		cout<<endl;
	}//for_y_j
	cout<<"=======Image========"
			<<endl;
}
void ImagePoCA::plot_simple(fstream& _fout){
	this->rms_calc();

	_fout<<endl
			<<"=======Image========"
			<<endl;
	for(int i = 0 ; i < this->_X_NUM ; i++){
		for(int j = 0 ; j < this->_Y_NUM ; j ++){
			vector<double>& angle =
					*(_Block2[i][j].scatter_angle);
			double& rms =
					_Block2[i][j].scatter_angle_rms;

			if(angle.empty() || rms < 0.001)
				_fout<<"□";
			else
				_fout<<"■";
			_fout<<" ";
		}//for_x_i
		_fout<<endl;
	}//for_y_j
	_fout<<"=======Image========"
			<<endl;
}
