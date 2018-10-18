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

#ifndef GET_VALUE_H_
#define GET_VALUE_H_

template<typename type>
type get_value(SEXP exp) {
	//deafult get_value
	return type(exp);
}

template<>
double get_value(SEXP exp) {
	return static_cast<double>(*REAL(exp));
}

template<>
int get_value(SEXP exp) {
    return static_cast<int>(*INTEGER(exp));
}

template<>
arma::u32 get_value(SEXP exp) {
	return static_cast<arma::u32>(*INTEGER(exp));
}

template<>
bool get_value(SEXP exp) {
	return static_cast<bool>(*LOGICAL(exp));
}

template<>
std::string get_value(SEXP exp) {
	return CHAR(STRING_ELT(exp,0));
}

template<>
arma::mat get_value(SEXP exp) {

	double * ptr = REAL(exp);

	SEXP dim = Rf_getAttrib(exp, R_DimSymbol);

	unsigned int n_rows = INTEGER(dim)[0];
	unsigned int n_cols = INTEGER(dim)[1];

	return arma::mat(ptr, n_rows, n_cols, true, true);
}

template<>
arma::umat get_value(SEXP exp) {

	int * ptr = INTEGER(exp);

	SEXP dim = Rf_getAttrib(exp, R_DimSymbol);

	unsigned int n_rows = INTEGER(dim)[0];
	unsigned int n_cols = INTEGER(dim)[1];

	return arma::conv_to< arma::umat >::from(
		arma::imat(ptr, n_rows, n_cols, true, true)
	);
}

template<>
arma::imat get_value(SEXP exp) {

	int * ptr = INTEGER(exp);

	SEXP dim = Rf_getAttrib(exp, R_DimSymbol);

	unsigned int n_rows = INTEGER(dim)[0];
	unsigned int n_cols = INTEGER(dim)[1];

	return arma::imat(ptr, n_rows, n_cols, true, true);
}

template<>
arma::Col<double> get_value(SEXP exp) {

	double *ptr = REAL(exp);

	return arma::vec(ptr, Rf_length(exp), true, true);
}

template<>
arma::Col<arma::u32> get_value(SEXP exp) {

	int *ptr = INTEGER(exp);

	return arma::conv_to< arma::Col<arma::u32> >::from(
		arma::Col<int>(ptr, Rf_length(exp), true, true)
	);
}

template<>
arma::Col<arma::s32> get_value(SEXP exp) {

	int *ptr = INTEGER(exp);

	return arma::Col<int>(ptr, Rf_length(exp), true, true);
}

template<>
arma::sp_mat get_value(SEXP exp) {

	SEXP dim = VECTOR_ELT(exp, 0);
	unsigned int n_rows = INTEGER(dim)[0];
	unsigned int n_cols = INTEGER(dim)[1];

	SEXP col_ptrs = VECTOR_ELT(exp, 1);
	SEXP row_idx = VECTOR_ELT(exp, 2);
	SEXP values = VECTOR_ELT(exp, 3);

	//Create and return sparse matrix
	return arma::sp_mat(
		get_value<arma::uvec>(row_idx),
	 	get_value<arma::uvec>(col_ptrs),
		get_value<arma::vec>(values),
		n_rows, n_cols);
}

template<typename type>
arma::field<type> get_field(SEXP exp) {

	arma::field<type> res(static_cast<arma::u32>(Rf_length(exp)));

	for (arma::u32 i = 0; i < static_cast<arma::u32>(Rf_length(exp)); ++i) {
		SEXP elm = VECTOR_ELT(exp, i);
		res(i) = get_value<type>(elm);
	}

	return res;
}

template<typename type>
std::vector<type> get_vector(SEXP exp) {

	std::vector<type> res(static_cast<arma::u32>(Rf_length(exp)));

	for (arma::u32 i = 0; i < static_cast<arma::u32>(Rf_length(exp)); ++i) {
		SEXP elm = VECTOR_ELT(exp, i);
		res[i] = get_value<type>(elm);
	}

	return res;
}


#endif /* GET_VALUE_H_ */
