//#pragma once

#ifndef _POINT_H_
#define _POINT_H_

using namespace std;

class Point {
public:
	int i, j;

	string toString() {
		return "[" + to_string(i) + ";" + to_string(j) + "]";
	}
};

#endif // _POINT_H_
