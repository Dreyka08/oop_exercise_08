#include <iostream>
#include <vector>
#include "figure.hpp"

using namespace std;

const unsigned int PENTAGON_TYPE_ID = 2;

template<class T>
struct TPentagon : public IFigure {
	pair<T, T> Cord;
	T Side;

	TPentagon(const pair<T, T>& cord, T side) : Cord(cord), Side(side) {}

	void Write(FILE* out) override {
		fwrite(&ROMB_TYPE_ID, sizeof(unsigned int), 1, out);
		fwrite(&Cord.first, sizeof(T), 1, out);
		fwrite(&Cord.second, sizeof(T), 1, out);
		fwrite(&Side, sizeof(T), 1, out);
	}

	void Print() override {
		std::cout << *this << std::endl;
	}
};

template<class T>
ostream& operator << (ostream& out, const TPentagon<T>& TPentagon) {
	out << "Pentagon {";
	T vis = 0.85065 * TPentagon.Side;
	out << pair<T, T>(TPentagon.Cord.first, TPentagon.Cord.second + vis) << ", ";
	T a = 0.30901 * vis;
	T b = 0.95105 * vis;
	out << pair<T, T>(TPentagon.Cord.first - b, TPentagon.Cord.second + a) << ", ";
	T v = 0.80901 * vis;
	T u = 0.58778 * vis;
	out << pair<T, T>(TPentagon.Cord.first - u, TPentagon.Cord.second - v) << ", ";

	out << pair<T, T>(TPentagon.Cord.first + u, TPentagon.Cord.second - v) << ", ";

	out << pair<T, T>(TPentagon.Cord.first + b, TPentagon.Cord.second + a);
	out << "}";
	return out;
}