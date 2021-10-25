#ifndef FACTORY_HPP
#define FACTORY_HPP

#include <memory>

#include "romb.hpp"
#include "pentagon.hpp"
#include "gexagon.hpp"

template<class T, class FIGURE>
class TFactory;

template<class T>
class TFactory< T, TRomb<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T d1, d2;
		std::cin >> curCords.first >> curCords.second >> d1 >> d2;
		TRomb<T>* RB = new TRomb<T>(curCords, d1, d2);
		return std::shared_ptr<IFigure>(RB);
	}
};

template<class T>
class TFactory< T, TPentagon<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T side;

		std::cin >> curCords.first >> curCords.second >> side;
		TPentagon<T>* pent = new TPentagon<T>(curCords, side);
		return std::shared_ptr<IFigure>(pent);
	}
};

template<class T>
class TFactory< T, TGexagon<T> > {
public:
	static std::shared_ptr<IFigure> CreateFigure() {
		std::pair<T, T> curCords;
		T side;
		std::cin >> curCords.first >> curCords.second >> side;
		TGexagon<T>* gex = new TGexagon<T>(curCords, side);
		return std::shared_ptr<IFigure>(gex);
	}
};


#endif /* FACTORY_HPP */