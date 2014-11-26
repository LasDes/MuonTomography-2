/*
 * Histogram.h
 *
 *  Created on: 2014年11月21日
 *      Author: torrence
 */

#ifndef UTIL_HISTOGRAM_H_
#define UTIL_HISTOGRAM_H_


class Histogram {
public:
	virtual ~Histogram();
	virtual double* locate(double* value);
	virtual void fill(double* value);
};


#endif /* UTIL_HISTOGRAM_H_ */
