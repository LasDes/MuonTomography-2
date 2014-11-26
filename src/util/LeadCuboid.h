/*
 * LeadCuboid.h
 *
 *  Created on: 2014年11月21日
 *      Author: torrence
 */

#ifndef UTIL_LEADCUBOID_H_
#define UTIL_LEADCUBOID_H_


class LeadCuboid {
public:
	LeadCuboid(double l,double w,double h,
			double x, double y){
		this->length = l;
		this->width = w;
		this->height = h;
		this->x_center = x;
		this->y_center = y;
	}
	virtual ~LeadCuboid();

	double length;
	double width;
	double height;

	double x_center;
	double y_center;
};


#endif /* UTIL_LEADCUBOID_H_ */
