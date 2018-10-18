/*
 Lightweight tools for R and c++ integration.
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

#ifndef ROBJECT_DECL_H_
#define ROBJECT_DECL_H_

class rList;

template<typename T>
class elements;

class rObject {

private:

	SEXP exp; //sould only be set in the constructor
	int number_of_protects; //sould only be set in the constructor

	bool * const unprotect_on_destruction;
	int * const exp_counter;

	void takeover_protection(); //unprotect

public:

	//Copy constructor
	rObject(rObject const& s);

	//Assignmnet operator
	rObject operator=(rObject const& s);

	//Constructors
	rObject(SEXP exp);

	rObject(arma::u32 value, bool no_protect = false);
	rObject(double value, bool no_protect = false);
  rObject(int value, bool no_protect = false);
	rObject(bool value, bool no_protect = false);

	rObject(arma::Mat<double> const& m, bool no_protect = false);
	rObject(arma::Mat<arma::u32> const& m, bool no_protect = false);
  rObject(arma::Mat<int> const& m, bool no_protect = false);

  rObject(arma::Col<double> const& v, bool no_protect = false);
  rObject(arma::Col<arma::u32> const& v, bool no_protect = false);
  rObject(arma::Col<int> const& v, bool no_protect = false);

	rObject(arma::sp_mat const& m, bool no_protect = false);

  rObject(std::vector<int> const& v, bool no_protect = false);
  rObject(std::vector<std::string> const& v, bool no_protect = false);
  rObject(std::string const& str, bool no_protect = false);

	template<typename T>
	rObject(arma::field<T> const& field, bool no_protect = false);

	rObject(rList const& list, bool no_protect = false);

	template<typename T>
	rObject(elements<T> const& elms, bool no_protect = false);

	~rObject();

	operator SEXP() const;
	SEXP getSEXP() const;

	int n_protects() const;

};

#endif /* ROBJECT_DECL_H_ */
