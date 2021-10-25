
#include "figure.hpp"

using namespace std;



const unsigned int GEXAGON_TYPE_ID = 3;

template<class T>
struct TGexagon : public IFigure {
	pair<T, T> Cord;
	T Side;

	TGexagon(const pair<T, T>& cord, T side) : Cord(cord), Side(side) {}

	

	void Write(FILE* out) override {
		fwrite(&GEXAGON_TYPE_ID, sizeof(unsigned int), 1, out);
		fwrite(&Cord.first, sizeof(T), 1, out);
		fwrite(&Cord.second, sizeof(T), 1, out);
		fwrite(&Side, sizeof(T), 1, out);
	}

	void Print() override {
		std::cout << *this << std::endl;
	}
};



template<class U>
std::ostream& operator << (std::ostream& out, const TGexagon<U>& TGexagon) {
	out << "Gexagon {";
	U a = 0.8660 * TGexagon.Side;
	U b = 0.5 * TGexagon.Side;
	out << std::pair<U, U>(TGexagon.Cord.first, TGexagon.Cord.second + TGexagon.Side) << ", ";
	out << std::pair<U, U>(TGexagon.Cord.first - a, TGexagon.Cord.second + b) << ", ";
	out << std::pair<U, U>(TGexagon.Cord.first - a, TGexagon.Cord.second - b) << ", ";
	out << std::pair<U, U>(TGexagon.Cord.first + b, TGexagon.Cord.second - TGexagon.Side) << ", ";
	out << std::pair<U, U>(TGexagon.Cord.first + a, TGexagon.Cord.second - b) << ", ";
	out << std::pair<U, U>(TGexagon.Cord.first + a, TGexagon.Cord.second + b) << ", ";
	out << "}";
	return out;
}