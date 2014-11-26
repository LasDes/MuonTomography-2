/*
 * MuonTracer.h
 *
 *  Created on: 2014年11月23日
 *      Author: torrence
 */

#ifndef MUONTRACER_H_
#define MUONTRACER_H_

#include "Muon.h"
#include "util/Vector.h"
#include "util/Rand.h"

#include<vector>

typedef Vector _Point;

class MuonTracer {
public:
	MuonTracer(Muon*);
	virtual ~MuonTracer();

	//Getter
	vector<_Point>* get_MRPC_record();
	vector<_Point>* get_Lead_in();
	vector<_Point>* get_Lead_out();

	//Setter
	void intial_MRPC();
	void initial_Lead();
	bool autoset_MRPC_record(double _MRPC_D,double _DETECT_H);
	bool add_MRPC_record(_Point* _mrpc);
	bool add_Lead_in(_Point*);
	bool add_Lead_out(_Point*);
	bool set_muon_out(Muon*);

private:
	Muon* _M_in;
	Muon* _M_out;
	vector<_Point>* _MRPC_record;
	vector<_Point>* _Lead_in;
	vector<_Point>* _Lead_out;
};

#endif /* MUONTRACER_H_ */
