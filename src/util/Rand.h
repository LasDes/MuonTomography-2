/*
 * Rand.h
 *
 *  Created on: 2014年11月26日
 *      Author: torrence
 */

#ifndef UTIL_RAND_H_
#define UTIL_RAND_H_

#include<stdlib.h>
#include<cmath>

class Rand {
public:
	Rand();
	virtual ~Rand();
	/**
	 * Rand::Uniform
	 *
	 * 			-标准分布为(0,1)区间内均匀
	 * 			- 生成均匀分布随机数
	 **/
	static double Uniform(double from , double to){
		return from +Rand::Uniform(to - from) ;
	}
	static double Uniform(double _scale){
		return _scale * Rand::Uniform();
	}
	static double Uniform(){
		return 1.0 * rand() / double(RAND_MAX);
	}
	/**
	 * Rand::Gaussian
	 *
	 * 			-生成高斯分布随机数
	 * 			- 1.Maraglia舍选法
	 */
	static double Gaussian(double _sigma , double _miu){
		return Rand::Gaussian(_sigma) + _miu;
	}
	static double Gaussian(double _sigma){
		return _sigma * Rand::Gaussian() ;
	}
	static double Gaussian(){
		double u,v,w,z;
		do{
			u = Rand::Uniform();
			v = Rand::Uniform();
			w = (2.0*u-1.0)*(2.0*u-1.0) + (2.0*v-1.0)*(2.0*v-1.0);
		}while( w > 1);
		z = sqrt(-log(w)/w);
		if(Rand::Uniform() < 0.5)
			return -u*z;
		return u*z;
	}
	/**
	 * 	Rand::Rejection
	 *
	 * 		简单舍选抽样
	 * */
	static double Rejection(double _f(double) ,
			double _fmax,
			double _from , double _to){

		double r,z,u;
		do{
			r = Rand::Uniform(_from , _to);
			z = _f(r);
			u = Rand::Uniform(_fmax);
		}while(u > z);

		return r;
	}
};

#endif /* UTIL_RAND_H_ */
