//#pragma once

#ifndef _LINE_H_
#define _LINE_H_

class Line {
public:
	Point begin, end;

	Line() {

	}

	Line(Point begin, Point end) {
		this->begin = begin;
		this->end = end;
	}

	void setBegin(Point begin) {
		this->begin = begin;
	}

	void setEnd(Point end) {
		this->end = end;
	}
};

#endif //_LINE_H_
