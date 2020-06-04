#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>
#include <windows.h>
#include "Way.h"
#include "Shop.h"
#include "Store.h"



using namespace std;

int main(){
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	setlocale(0, "");

	int roads = 0, buildings = 0, cargo = 0, shops = 0;

	ifstream map;
	char ch = 0;
	int n=0, m=0;

	cout << "Исходная карта: " << endl << endl;

	map.open("res.txt");
	while (ch != -1) {
		ch = map.get();
		if (ch != 'я') {
			cout << ch;
		}
		if (ch == '0') {
			roads++;
			m++;
		} else if(ch == '1'){
			buildings++;
			m++;
		} else if (ch == '2') {
			shops++;
			m++;
		} else if (ch == '3') {
			cargo++;
			m++;
		}
		if (ch == '\n') {
			n++;
		}
	}
	map.close();

	m = m / n;

	cout << endl << "Дорог: " << roads << endl;
	cout << "Зданий: " << buildings << endl;
	cout << "Магазинов: " << shops << endl;
	cout << "Складов: " << cargo << endl;
	cout << "Размер карты: " << n << " x " << m << endl;

	int** intMap = new int* [n];
	for (int i = 0; i < n; i++) {
		intMap[i] = new int[m];
	}

	ifstream in;
	in.open("res.txt");
	for (int i = 0; i < n;i++) {
		for (int j = 0; j < m; j++) {
			in >> intMap[i][j];
		}
	}
	in.close();

	int stc = 0, shc = 0;
	for (int i = 0; i < n; i++) { //перебор двумерного массива целых чисел
		for (int j = 0; j < m; j++) {
			if (intMap[i][j] == 3) {
				stc++;
			}
			if (intMap[i][j] == 2) {
				shc++;
			}
			if(intMap[i][j] == 3 && intMap[i+1][j] == 1 && intMap[i-1][j] == 1 && intMap[i][j+1] == 1 && intMap[i][j-1] == 1){ //проверка на заблокированность склада
				cout << "Склад заблокирован!" << endl;
				return 0;
			} else if (intMap[i][j] == 2 && intMap[i + 1][j] == 1 && intMap[i - 1][j] == 1 && intMap[i][j + 1] == 1 && intMap[i][j - 1] == 1) {
				cout << "Один из магазинов заблокирован!" << endl;
				return 0;
			}
		}
	}

	//ещё несколько проверок
	if (stc > 1) {
		cout << "Объявлено более двух складов" << endl;
		return 0;
	} else if (stc == 0) {
		cout << "Не объявлен ни один склад" << endl;
		return 0;
	}

	if (shc == 0) {
		cout << "Нет ни одного магазина" << endl;
		return 0;
	}
	/* АЛГОРИТМ ОБХОДА */

	Way way;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (intMap[i][j] == 2) {
				Shop shop(i, j);
				way.setShop(shop);
			} else if (intMap[i][j] == 3) {
				Store store(i, j);
				way.setStore(store);
			}
		}
	}

	cout << endl << "Координаты склада: " << endl;
	way.printStore();
	cout << endl << "Координаты магазинов: " << endl;
	way.printShops();

	//определение кратчайших путей "напрямую"
	vector<Point> tempPoints;
	tempPoints.push_back(way.store);
	for (Shop i : way.shops) {
		tempPoints.push_back(i);
	}

	int iter = tempPoints.size();
	Point b, e;
	for (int i = 0; i < iter;i++) {
		Line line;
		if (i==0) {
			b = tempPoints[0];
			line.setBegin(b);

			float min = way.shortestLine(b, tempPoints[1]);

			for (int j = 1; j < tempPoints.size(); j++) {
				if (way.shortestLine(b, tempPoints[j]) < min) {
					min = way.shortestLine(b, tempPoints[j]);
				}
			}

			for (int j = 1; j < tempPoints.size(); j++) {
				if (way.shortestLine(b, tempPoints[j]) == min) {
					line.setEnd(tempPoints[j]);
					b = tempPoints[j];
					tempPoints.erase(tempPoints.begin() + j);
					break;
				}
			}
		} else if(i == iter-1){
			line.setBegin(b);
			e = tempPoints.back();
			line.setEnd(e);
		} else {
			line.setBegin(b);
			float min = way.shortestLine(b, tempPoints[iter-i-1]);
			for (int j = 1; j < iter - i; j++) {
				if (way.shortestLine(b, tempPoints[j]) < min) {
					min = way.shortestLine(b, tempPoints[j]);
				}
			}

			for (int j = 1; j < iter - i; j++) {
				if (way.shortestLine(b, tempPoints[j]) == min) {
					line.setEnd(tempPoints[j]);
					b = tempPoints[j];
					tempPoints.erase(tempPoints.begin() + j);
					break;
				}
			}
		}
		way.setLine(line);
	}

	cout << endl << "Намеченный маршрут: " << endl;
	way.printLines();
	cout << endl;

	int steps = 0;
	for (Line l : way.lines) {
		int currentI = l.begin.i, currentJ = l.begin.j;

		while ((currentI != l.end.i) || (currentJ != l.end.j)){

			if (currentI < l.end.i) {
				if (steps > m * n) {
					break;
				}
				if (intMap[currentI + 1][currentJ] != 1) {
					currentI++;
					intMap[currentI][currentJ] = 8;
					cout << "down (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI][currentJ + 1] != 1) {
					currentJ++;
					intMap[currentI][currentJ] = 8;
					cout << "right (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					if (intMap[currentI + 1][currentJ] != 1) {
						currentI++;
						intMap[currentI][currentJ] = 8;
						cout << "down (" << currentI << ";" << currentJ << ") -> ";
						steps++;
						if ((currentI == l.end.i) && (currentJ == l.end.j)) {
							intMap[currentI][currentJ] = 9;
							cout << endl << "Достигнута точка!" << endl;
							break;
						}
						continue;
					}
					continue;
				} else if (intMap[currentI - 1][currentJ] != 1) {
					currentI--;
					intMap[currentI][currentJ] = 8;
					cout << "up (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI][currentJ-1] != 1) {
					currentJ--;
					intMap[currentI][currentJ] = 8;
					cout << "left (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				}
			}
			if (currentJ < l.end.j) {
				if (steps > m * n) {
					break;
				}
				if (intMap[currentI][currentJ + 1] != 1) {
					currentJ++;
					intMap[currentI][currentJ] = 8;
					cout << "right (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI - 1][currentJ] != 1) {
					currentI--;
					intMap[currentI][currentJ] = 8;
					cout << "up (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					if (intMap[currentI][currentJ + 1] != 1) {
						currentJ++;
						intMap[currentI][currentJ] = 8;
						cout << "right (" << currentI << ";" << currentJ << ") -> ";
						steps++;
						if ((currentI == l.end.i) && (currentJ == l.end.j)) {
							intMap[currentI][currentJ] = 9;
							cout << endl << "Достигнута точка!" << endl;
							break;
						}
						continue;
					}
					continue;
				} else if (intMap[currentI + 1][currentJ] != 1) {
					currentI++;
					intMap[currentI][currentJ] = 8;
					cout << "down (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI][currentJ - 1] != 1) {
					currentJ--;
					intMap[currentI][currentJ] = 8;
					cout << "left (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				}
			}

			if (currentI > l.end.i) {
				if (steps > m * n) {
					break;
				}
				if (intMap[currentI - 1][currentJ] != 1) {
					currentI--;
					intMap[currentI][currentJ] = 8;
					cout << "up (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI][currentJ - 1] != 1) {
					currentJ--;
					intMap[currentI][currentJ] = 8;
					cout << "left (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					if (intMap[currentI - 1][currentJ] != 1) {
						currentI--;
						intMap[currentI][currentJ] = 8;
						cout << "up (" << currentI << ";" << currentJ << ") -> ";
						steps++;
						if ((currentI == l.end.i) && (currentJ == l.end.j)) {
							intMap[currentI][currentJ] = 9;
							cout << endl << "Достигнута точка!" << endl;
							break;
						}
						continue;
					}
					continue;
				} else if (intMap[currentI][currentJ + 1] != 1) {
					currentJ++;
					intMap[currentI][currentJ] = 8;
					cout << "right (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI + 1][currentJ] != 1) {
					currentI++;
					intMap[currentI][currentJ] = 8;
					cout << "down (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				}
			}
			if (currentJ > l.end.j) {
				if (steps > m * n) {
					break;
				}
				if (intMap[currentI][currentJ - 1] != 1) {
					currentJ--;
					intMap[currentI][currentJ] = 8;
					cout << "left (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				} else if (intMap[currentI + 1][currentJ] != 1) {
					currentI++;
					intMap[currentI][currentJ] = 8;
					cout << "down (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					if (intMap[currentI][currentJ - 1] != 1) {
						currentJ--;
						intMap[currentI][currentJ] = 8;
						cout << "left (" << currentI << ";" << currentJ << ") -> ";
						steps++;
						if ((currentI == l.end.i) && (currentJ == l.end.j)) {
							intMap[currentI][currentJ] = 9;
							cout << endl << "Достигнута точка!" << endl;
							break;
						}
						continue;
					}
					continue;
				} else if (intMap[currentI - 1][currentJ] != 1) {
					currentI--;
					intMap[currentI][currentJ] = 8;
					cout << "up (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута  точка!" << endl;
						break;
					}
					continue;
				}else if (intMap[currentI][currentJ + 1] != 1) {
					currentJ++;
					intMap[currentI][currentJ] = 8;
					cout << "right (" << currentI << ";" << currentJ << ") -> ";
					steps++;
					if ((currentI == l.end.i) && (currentJ == l.end.j)) {
						intMap[currentI][currentJ] = 9;
						cout << endl << "Достигнута точка!" << endl;
						break;
					}
					continue;
				}
			}
		}
	}


	cout << endl << "Маршрут построен: " << endl << endl;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (intMap[i][j] == 0) {
				cout << '.';
			} else if (intMap[i][j] == 1) {
				cout << 'П';
			} else if (intMap[i][j] == 8) {
				cout << 'o';
			} else if (intMap[i][j] == 9) {
				cout << 'X';
			}
			//cout << intMap[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl << "Где: \n. - непройденные дороги;\no - пройденные дороги;\nП - здания;\nX - посещённые торговые точки и склад;\n\n";

	cout << endl << "Путь пройден за " << steps << " ходов!" << endl << endl;

	for (int i = 0; i < n; i++) { //очистка памяти
		delete[] intMap[i];
	}
}
