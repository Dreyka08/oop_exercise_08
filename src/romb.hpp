#include <iostream>
#include <vector>
#include "figure.hpp"

using namespace std;



const unsigned int ROMB_TYPE_ID = 1;

template<class T>
struct TRomb : public IFigure {
	std::pair<T, T> Cord;
	T D1, D2;

	TRomb(const std::pair<T, T>& cord, T d1, T d2) : Cord(cord), D1(d1), D2(d2) {}

	void Write(FILE* out) override {
		fwrite(&ROMB_TYPE_ID, sizeof(unsigned int), 1, out);
		fwrite(&Cord.first, sizeof(T), 1, out);
		fwrite(&Cord.second, sizeof(T), 1, out);
		fwrite(&D1, sizeof(T), 1, out);
		fwrite(&D2, sizeof(T), 1, out);
	}

	void Print() override {
		std::cout << *this << std::endl;
	}
};



template<class U>
std::ostream& operator << (std::ostream& out, const TRomb<U>& rb) {
	out << "Romb {";
	out << std::pair<U, U>(rb.Cord.first, rb.Cord.second + (rb.D1 / 2)) << ", ";
	out << std::pair<U, U>(rb.Cord.first + (rb.D2 / 2), rb.Cord.second) << ", ";
	out << std::pair<U, U>(rb.Cord.first, rb.Cord.second - (rb.D1 / 2)) << ", ";
	out << std::pair<U, U>(rb.Cord.first - (rb.D2 / 2), rb.Cord.second);
	out << "}";
	return out;
}