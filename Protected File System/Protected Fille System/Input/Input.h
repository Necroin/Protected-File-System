#pragma once
#ifndef INPUT_H
#define INPUT_H
#include <iostream>
#include <functional>
#include <initializer_list>

template <class T, class ...F>
int get_object(std::istream& input,T& object, F... functions) {
	int res, err_flag;
	do {
		err_flag = 0;
		input >> object;
		if (std::cin.eof()) {
			return 0;
		}
		if (std::cin.good()) {
			res = 1;
		}
		else {
			res = -1;
		}
		if (res > 0) {
			for (auto&& p : std::initializer_list<std::function<int(T)>>{ functions... }) {
				err_flag += p(object);
			}
		}

		if (res < 0) {
			err_flag = 1;
			std::cout << "You are wrong" << std::endl;
		}
		if (err_flag) {
			std::cout << "Try again -->";
		}
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	} while (err_flag);
	return 1;
}
#endif


