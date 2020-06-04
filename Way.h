//#pragma once //для VS
#ifndef _WAH_H_
#define _WAH_H_

#include <vector>
#include <cmath>
#include "Point.h"
#include "Shop.h"
#include "Store.h"
#include "Line.h"

using namespace std;

class Way {
public:
	vector<Shop> shops;
	vector<Line> lines;
	Store store;

	void setStore(Store newStore) {
		store = newStore;
	}

	void setShop(Shop newShop) {
		shops.push_back(newShop);
	}

	void setLine(Line newLine) {
		lines.push_back(newLine);
	}

	void printShops() {
		for (Shop i : shops) {
			cout << "[" << i.i << ";" << i.j << "]" << endl;
		}
	}

	void printStore() {
		cout << "[" << store.i << ";" << store.j << "]" << endl;
	}

	void printLines() {
		for (Line i : lines) {
			cout << i.begin.toString() << "->" << i.end.toString() << endl;
		}
	}

	float shortestLine(Point begin, Point end) {
		return sqrt(pow((begin.i-end.i),2)+ pow((begin.j - end.j),2));
	}
};

#endif // _WAH_H_
