/*
 * MuonTracer.cpp
 *
 *  Created on: 2014年11月23日
 *      Author: torrence
 */

#include "MuonTracer.h"

MuonTracer::MuonTracer(Muon* muon_in) {
	this->_M_in = muon_in;
	this->_M_out = NULL;

	this->_Lead_in = NULL;
	this->_Lead_out = NULL;

	this->_L_through_lead = -1.0;		//用负值表示原始穿越长度处于未计算状态

	this->_MRPC_record = NULL;

}

MuonTracer::~MuonTracer() {
	// TODO Auto-generated destructor stub
}

void MuonTracer::initial_MRPC(){
	if(! this->_MRPC_record)
		this->_MRPC_record = new vector<_Point>;
}

void MuonTracer::initial_Lead(){
	if(! this->_Lead_in)
		this->_Lead_in = new vector<_Point>;
	if(! this->_Lead_out)
		this->_Lead_out = new vector<_Point>;
}

//Getter
double MuonTracer::get_through_lead_L(){
	return this->_L_through_lead;
}
vector<_Point>* MuonTracer::get_MRPC_record(){
	if(! this->_MRPC_record)
		this->_MRPC_record = new vector<_Point>;
	return this->_MRPC_record;
}
vector<_Point>* MuonTracer::get_Lead_in(){
	return this->_Lead_in;
}
vector<_Point>* MuonTracer::get_Lead_out(){
	return this->_Lead_out;
}

//Setter
bool MuonTracer::autoset_MRPC_record(double _d , double _h){
	if(this->_M_out == NULL)
		return false;

	this->initial_MRPC();

	_Point p1(_M_in->get_x(),_M_in->get_y(),
			2*_d + _h);
	this->add_MRPC_record(&p1);

	double* point = _M_in->flight_point(_d);

	_Point p2(point[0],point[1],_d + _h);
	this->add_MRPC_record(&p2);

	point = _M_out->flight_point(_M_out->get_z() - _d );
	_Point p3(point[0],point[1],_d);
	this->add_MRPC_record(&p3);

	point = _M_out->flight_point(_M_out->get_z());
	_Point p4(point[0],point[1],0);
	this->add_MRPC_record(&p4);

	return true;
}
bool MuonTracer::add_MRPC_record(_Point* _mrpc){
	if(!this->_MRPC_record)
		this->_MRPC_record = new vector<_Point>;
	this->_MRPC_record->push_back(*_mrpc);
	return true;
}
bool MuonTracer::add_Lead_in(_Point* in){
	if(!this->_Lead_in)
		this->_Lead_in = new vector<_Point>;
	this->_Lead_in->push_back(*in);
	return true;
}
bool MuonTracer::add_Lead_out(_Point* out){
	if(! this->_Lead_out)
		this->_Lead_out = new vector<_Point>;
	this->_Lead_out->push_back(*out);
	return true;
}
bool MuonTracer::set_muon_out(Muon* _out){
	if(this->_M_out)
		return false;
	this->_M_out = _out;
	return true;
}
bool MuonTracer::set_through_lead_L(double _length){
	if(this->_L_through_lead > 0.0)
		return false;
	this->_L_through_lead = _length;
	return true;
}
