#include <ctime>
#include <functional>
#include <thread>
#include <vector>

#include "factory.hpp"
#include "pub_sub.hpp"

/*
 * Инютин М А М8О-207Б-19
 * Создать приложение, которое будет считывать из стандартного
 * ввода данные фигур, согласно варианту задания, выводить их
 * характеристики на экран и записывать в файл. Фигуры могут
 * задаваться как своими вершинами, так и другими характеристиками
 * (например, координата центра, количество точек и радиус).
 * Программа должна:
 * 1. Осуществлять ввод из стандартного ввода данных фигур,
 *    согласно варианту задания;
 * 2. Программа должна создавать классы, соответствующие введенным
 *    данных фигур;
 * 3. Программа должна содержать внутренний буфер, в который
 *    помещаются фигуры. Для создания буфера допускается
 *    использовать стандартные контейнеры STL. Размер буфера
 *    задается параметром командной строки. Например, для буфера
 *    размером 10 фигур: oop_exercise_08 10
 * 4. При накоплении буфера они должны запускаться на
 *    асинхронную обработку, после чего буфер должен очищаться;
 * 5. Обработка должна производиться в отдельном потоке;
 * 6. Реализовать два обработчика, которые должны обрабатывать
 *    данные буфера:
 *    1. Вывод информации о фигурах в буфере на экран;
 *    2. Вывод информации о фигурах в буфере в файл. Для каждого
 *       буфера должен создаваться файл с уникальным именем.
 * 7. Оба обработчика должны обрабатывать каждый введенный буфер.
 *    Т.е. после каждого заполнения буфера его содержимое должно
 *    выводиться как на экран, так и в файл.
 * 8. Обработчики должны быть реализованы в виде лямбда-функций и
 *    должны хранится в специальном массиве обработчиков. Откуда и
 *    должны последовательно вызываться в потоке – обработчике.
 * 9. В программе должно быть ровно два потока (thread). Один
 *    основной (main) и второй для обработчиков;
 * 10. В программе должен явно прослеживаться шаблон
 *    Publish-Subscribe. Каждый обработчик должен быть реализован
 *    как отдельный подписчик.
 * 11. Реализовать в основном потоке (main) ожидание обработки
 *    буфера в потоке-обработчике. Т.е. после отправки буфера на
 *    обработку основной поток должен ждать, пока поток обработчик
 *    выведет данные на экран и запишет в файл.
 */

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
		if (type == SQUARE_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TSquare<SCALAR_TYPE> >::CreateFigure());
		} else if (type == RECTANGLE_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TRectangle<SCALAR_TYPE> >::CreateFigure());
		} else if (type == TRAPEZE_TYPE_ID) {
			figures.push_back(TFactory<SCALAR_TYPE, TTrapeze<SCALAR_TYPE> >::CreateFigure());
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