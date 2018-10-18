/*
 Sgl template library for optimizing sparse group lasso penalized objectives.
 Copyright (C) 2012 Martin Vincent
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>
 */

#ifndef SIMPLE_TIMER_H_
#define SIMPLE_TIMER_H_

#ifdef DO_TIMING

class SimpleTimer {

private:
	int total;
	int s;

	int times;

	const std::string func;
	const std::string file;
	const int line;

public:

	SimpleTimer(std::string func, std::string file, int line) : total(0), s(0), times(0), func(func), file(file), line(line) {}

	~SimpleTimer() {
		//Rcpp::Rcout << func << " " << static_cast<double>(total) / CLOCKS_PER_SEC <<  " seconds - x"<< times << "." << " (in " << file << " at line "<< line << ") " << std::endl;
		std::cout << func << " " << static_cast<double>(total) / CLOCKS_PER_SEC <<  " seconds - x"<< times << "." << " (in " << file << " at line "<< line << ") " << std::endl;
	}

	void start() {
		++times;
		s = clock();
	}

	void end() {
		total += clock() - s;
	}
};

class TimerScope {

private:
	SimpleTimer & timer;
public:

	TimerScope(SimpleTimer & timer) : timer(timer) {
		timer.start();
	}

	~TimerScope() {
		timer.end();
	}
};

#define TIMER_START static SimpleTimer timer(__func__, __FILE__, __LINE__); TimerScope timer_scope(timer);
#else
#define TIMER_START
#endif

#ifdef FUNC_ENTER

class EnterFunction {


private:
	const std::string func;
	const std::string file;
	const int line;

public:

	EnterFunction(std::string func, std::string file, int line) : func(func), file(file), line(line) {
		 std::cout << "Enter : " << func << " in " << file << " line " << line <<  std::endl;
	}

	~EnterFunction() {
		 std::cout << "Leave : " << func << std::endl;
	}

};

#define DEBUG_ENTER EnterFunction enter(__func__, __FILE__, __LINE__);
#else
#define DEBUG_ENTER
#endif

#endif /* SIMPLE_TIMER_H_ */
