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

/** @file numeric.h */

#ifndef NUMERIC_H_
#define NUMERIC_H_

//Forward declarations

// template<typename MATRIX, typename VECTOR>
// class BlockVector;

//index
typedef arma::uword index;

//Natural types
typedef arma::uword natural;
typedef arma::Col<natural> natural_vector;
typedef arma::Mat<natural> natural_matrix;
typedef arma::field<natural_vector> natural_vector_field;
typedef arma::field<natural_matrix> natural_matrix_field;

//Numeric types
typedef double numeric;
typedef arma::Col<numeric> vector;
typedef arma::Mat<numeric> matrix;
typedef arma::field<vector> vector_field;
typedef arma::field<matrix> matrix_field;

typedef arma::Cube<numeric> cube;

typedef arma::sp_mat sparse_matrix;
typedef arma::field<sparse_matrix> sparse_matrix_field;
typedef arma::sp_vec sparse_vector;

//typedef BlockVector<sparse_matrix, sparse_vector> block_vector;
//typedef sparse_vector parameter_block_vector;

typedef BlockVector<sparse_matrix, vector> block_vector;
typedef vector parameter_block_vector;

typedef arma::field<block_vector> block_vector_field;

typedef sparse_vector parameter_block;
typedef block_vector parameter;
typedef block_vector_field parameter_field;

//Null vectors
static const natural_vector null_natural_vector;
static const vector null_vector;
static const matrix null_matrix;
static const sparse_matrix null_sparse_matrix;

//Number of non zero elements

natural n_non_zero(vector const& a) {
	return accu(a != 0);
}

// Sign function

template<typename T>
T inline sign(T const& x) {
// non code should go here
	const T error_type_not_defined;
	&error_type_not_defined = 0;
	return false;
}

template<>
numeric inline sign(numeric const& x) {

	if (x == 0) {
		return 0;
	}

	return x > 0 ? 1 : -1;
}

template<>
vector inline sign(vector const& x) {
	return x/(abs(x) + (x == 0));
}

//abs

numeric abs(numeric const& x) {
	return fabs(x);
}

numeric inline pos(numeric const& x) {
	return x > 0 ? x : 0;
}

vector inline pos(vector const& x) {
	return arma::conv_to<vector>::from(x > 0)%x;
}

template<typename T, typename F>
numeric discrete_dist(T const& x0, F const& x1) {
  //return arma::accu((x0 == 0) % (x1 != 0));
   return arma::accu((x0 == 0) % (x1 != 0) + (x1 == 0) % (x0 != 0));
}

template<>
numeric discrete_dist(
	sgl::parameter const& x0,
	sgl::parameter const& x1) {

  sgl::numeric d = 0;

  for (sgl::natural block_index = 0; block_index < x0.n_blocks; block_index++) {
    if (!x0.is_block_zero(block_index) || !x1.is_block_zero(block_index)) {
      d = std::max(d, sgl::discrete_dist(x0.block(block_index), x1.block(block_index)));
    }
	}

  return d;
}

numeric  max_dist(
	parameter_block_vector const& x0,
	parameter_block_vector const& x1) {

	return arma::as_scalar(max(abs(x0 - x1)));
}

sgl::numeric dist(
	sgl::parameter const& x0,
	sgl::parameter const& x1) {

  sgl::numeric d = 0;

  for (sgl::natural block_index = 0; block_index < x0.n_blocks; block_index++) {

    if (!x0.is_block_zero(block_index) || !x1.is_block_zero(block_index)) {
      d += arma::as_scalar(sum(square(x0.block(block_index) - x1.block(block_index))));
    }
	}

  return d;
}

// Is decreasing

bool inline is_decreasing(vector const& x) {

	numeric x_previous = x(0);
	for (natural i = 1; i < x.n_elem; ++i) {
		if (x(i) > x_previous) {
			return false;
		}
		x_previous = x(i);
	}

	return true;
}

template<typename T>
bool inline is_increasing(T const& x) {

	numeric x_previous = x(0);
	for (natural i = 1; i < x.n_elem; ++i) {
		if (x(i) < x_previous) {
			return false;
		}
		x_previous = x(i);
	}

	return true;
}

// Is positive

bool inline is_positive(vector const& x) {

	for (natural i = 0; i < x.n_elem; ++i) {
		if (x(i) <= 0) {
			return false;
		}
	}

	return true;
}

//Is non negative

bool inline is_non_negative(vector const& x) {

	for (natural i = 0; i < x.n_elem; ++i) {
		if (x(i) < 0) {
			return false;
		}
	}

	return true;
}

template<typename T>
bool inline is_finite(T const& x) {
// non code should go here
        const T error_type_not_defined;
        &error_type_not_defined = 0;
        return false;
}

template<>
bool inline is_finite(double const& x) {
        return (! std::isnan(x)) && (! std::isinf(x));
}

template<>
bool inline is_finite(vector const& x) {
        return arma::is_finite(x);
}

template<>
bool inline is_finite(block_vector const& x) {
        return is_finite(x);
}

template<>
bool inline is_finite(matrix const& x) {
        return arma::is_finite(x);
}

template<>
bool inline is_finite(sparse_matrix const& x) {
        return arma::is_finite(x);
}


template<typename T>
inline sgl::vector colSumsSquare(T const& x) {
	sgl::vector r(trans(sum(square(x))));
	return  r;
}

//Sguare function

natural inline square(natural const& x) {
	return x * x;
}

numeric inline square(numeric const& x) {
	return x * x;
}

#endif /* NUMERIC_H_ */
