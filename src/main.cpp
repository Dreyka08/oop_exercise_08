#include <ctime>
#include <functional>
#include <thread>
#include <vector>
#include <string>

#include "factory.hpp"
#include "pub_sub.hpp"

TPubSubMQ< std::vector< std::shared_ptr<IFigure> > > mq;
const std::string FOLDER = "files/";
FILE* file = NULL;

std::string genFileName(size_t n) {
	std::string res;
	for (size_t i = 0; i < n; ++i) {
		res.push_back('a' + std::rand() % 26);
	}
	return res;
}

void ThreadFunc() {
	using functionType = std::function<void(std::shared_ptr<IFigure> fig)>;
	functionType PrintToStdout = [](std::shared_ptr<IFigure> fig) {
		fig->Print();
	};
	functionType WriteToFile = [](std::shared_ptr<IFigure> fig) {
		fig->Write(file);
	};
	std::vector<functionType> funcs({PrintToStdout, WriteToFile});
	bool awake = true;
	while (awake) {
		if (!mq.Empty()) {
			std::vector< std::shared_ptr< IFigure > > message = mq.Front();
			if (message.empty()) {
				awake = false;
				break;
			}
			file = fopen((FOLDER + genFileName(16)).c_str(), "wb");
			while (!message.empty()) {
				std::shared_ptr< IFigure > figPtr = message.back();
				message.pop_back();
				for (auto func : funcs) {
					func(figPtr);
				}
			}
			fclose(file);
			mq.Pop();
		}
	}
}

using SCALAR_TYPE = int;

int main(int argc, char** argv) {
	if (argc < 2) {
		std::cout << "Missing argument!" << std::endl;
		return -1;
	}
	size_t bufferSize;
	try {
		bufferSize = std::stoi(std::string(argv[1]));
	} catch (std::exception & ex) {
		std::cout << ex.what() << std::endl;
		return -1;
	}
	std::srand(time(NULL));
	std::thread myThread(ThreadFunc);
	std::vector< std::shared_ptr<IFigure> > figures;
	unsigned int type;
	while (std::cin >> type) {
		if (type == ROMB_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TRomb<SCALAR_TYPE> >::CreateFigure());
		} else if (type == PENTAGON_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TPentagon<SCALAR_TYPE> >::CreateFigure());
		} else if (type == GEXAGON_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TGexagon<SCALAR_TYPE> >::CreateFigure());
		}
		if (figures.size() == bufferSize) {
			mq.Push(figures);
			bool threadWorkDone = false;
			while (!threadWorkDone) {
				if (mq.Empty()) {
					threadWorkDone = true;
				}
			}
			figures.clear();
		}
	}
	mq.Push(figures);
	figures.clear();
	mq.Push(figures);
	myThread.join();
}