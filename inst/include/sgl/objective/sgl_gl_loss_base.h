/*
	Sgl template library for optimizing sparse group lasso penalized objectives.
    Copyright (C) 2014 Martin Vincent

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

#ifndef SGL_GL_LOSS_BASE_H_
#define SGL_GL_LOSS_BASE_H_

template<bool constant = false>
class hessian_diagonal {

public:

	static const bool is_constant = constant;
	typedef sgl::vector representation;

	static void scalar_mult_add(sgl::vector & J, double s, sgl::vector const& H) {
			J += s*H;
	}

	static void scalar_mult_set(sgl::vector & J, double s, sgl::vector const& H) {
			J = s*H;
	}


	static void diag(sgl::matrix & x, sgl::natural j, sgl::natural k, sgl::natural n_groups, sgl::vector const& H) {
		x.submat(j * n_groups, k * n_groups,
					(j + 1) * n_groups - 1, (k + 1) * n_groups - 1) += diagmat(H);
	}

	static sgl::matrix const update(sgl::vector const& H, sgl::matrix const& V, double s) {
		return s * diagmat(H) * V;
	}

	static representation zero_representation_of(representation const& H) {
		return sgl::vector(H.n_elem, arma::fill::zeros);
	}
};

// Hessian H = scalar * Identity
template<bool constant = false>
class hessian_identity {

public:

	static const bool is_constant = constant;
	typedef double representation;

	static void scalar_mult_add(double & J, double s, double H) {
			J += s*H;
	}

	static void scalar_mult_set(double & J, double s, double H) {
			J = s*H;
	}

	static void diag(sgl::matrix & x, sgl::natural j, sgl::natural k, sgl::natural n_groups, double H) {
		x.submat(j * n_groups, k * n_groups,
					(j + 1) * n_groups - 1, (k + 1) * n_groups - 1).diag() += H;
	}

	static sgl::matrix update(double const& H, sgl::matrix const& V, double s) {
		return s * H * V;
	}

	static representation zero_representation_of(representation const& H) {
		return 0;
	}

};

template<bool constant = false>
class hessian_full {

public:

	static const bool is_constant = constant;

	typedef sgl::matrix representation;

	static void scalar_mult_add(sgl::matrix & J, double s, sgl::matrix const& H) {
			J += s*H;
	}

	static void scalar_mult_set(sgl::matrix & J, double s, sgl::matrix const& H) {
			J = s*H;
	}

	static void diag(sgl::matrix & x, sgl::natural j, sgl::natural k, sgl::natural n_groups, sgl::matrix const& H) {
		x.submat(j * n_groups, k * n_groups,
					(j + 1) * n_groups - 1, (k + 1) * n_groups - 1) += H;
	}

	static sgl::matrix update(sgl::matrix const& H, sgl::matrix const& V, double s) {
		return s * H * V;
	}

	static representation zero_representation_of(representation const& H) {
		return sgl::matrix(H.n_rows, H.n_cols, arma::fill::zeros);
	}

};

template<bool constant = false>
class hessian_full_sparse {

public:

	static const bool is_constant = constant;

	typedef sgl::sparse_matrix representation;

	static void scalar_mult_add(sgl::sparse_matrix & J, double s, sgl::sparse_matrix const& H) {
			J += s*H;
	}

	static void scalar_mult_set(sgl::sparse_matrix & J, double s, sgl::sparse_matrix const& H) {
			J = s*H;
	}

	static void diag(sgl::matrix & x, sgl::natural j, sgl::natural k, sgl::natural n_groups, sgl::sparse_matrix const& H) {
		x.submat(j * n_groups, k * n_groups,
					(j + 1) * n_groups - 1, (k + 1) * n_groups - 1) += H;
	}

	static sgl::matrix update(sgl::sparse_matrix const& H, sgl::matrix const& V, double s) {
		return s * H * V;
	}

	static representation zero_representation_of(representation const& H) {
		return sgl::sparse_matrix(H.n_rows, H.n_cols);
	}

};

template<typename T, sgl::natural blocks, bool constant = false>
class hessian_block_diagonal {

public:

	static const bool is_constant = constant;

	typedef arma::field<T> representation;

	static void scalar_mult_add(representation & J, double s, representation const& H) {
		for(sgl::natural i = 0; i < blocks; ++i) {
			J(i) += s*H(i);
		}
	}

	static void scalar_mult_set(representation & J, double s, representation const& H) {
		for(sgl::natural i = 0; i < blocks; ++i) {
			J(i) = s*H(i);
		}
	}

	static void diag(sgl::matrix & x, sgl::natural j, sgl::natural k, sgl::natural n_groups, representation const& H) {

		sgl::natural from_row = j * n_groups;
		sgl::natural from_col = k * n_groups;
		for(sgl::natural i = 0; i < blocks; ++i) {

			x.submat(
				from_row, from_col,
				from_row + H(i).n_cols - 1, from_col + H(i).n_cols - 1
			) += H(i);

			from_row += H(i).n_cols;
			from_col += H(i).n_cols;
		}
	}

	static sgl::matrix update(representation const& H, sgl::matrix const& V, double s) {

		sgl::matrix r(V.n_rows, V.n_cols); // H is square

		sgl::natural from = 0;
		for(sgl::natural i = 0; i < blocks; ++i) {
			 r.rows(from, from + H(i).n_cols - 1) = s * H(i) * V.rows(from, from + H(i).n_cols - 1);
			 from += H(i).n_cols;
		}
				return r;
	}

	static representation zero_representation_of(representation const& H) {

		representation zr(blocks);
		for(sgl::natural i = 0; i < blocks; ++i) {
			zr(i).set_size(H(i).n_rows, H(i).n_cols);
			zr(i).zeros();
		}

		return zr;
	}

};


template < typename T , typename E >
class GenralizedLinearLossBase: public T {

public:

	typedef typename T::data_type data_type;

	typedef typename T::hessian_type hessian_type;

	sgl::DimConfig const& dim_config;

protected:

	E const& X; //design matrix - n_samples x n_features

	sgl::natural const n_samples;
	sgl::natural const n_features;
  sgl::natural const n_feature_parameters;

	mutable sgl::matrix partial_hessian;
	mutable sgl::natural_vector hessian_diag_mat_computed;
	mutable sgl::matrix_field hessian_diag_mat;

	sgl::parameter current_parameters;

	sgl::vector x_norm;
	sgl::numeric x_norm_max;
	mutable sgl::numeric partial_hessian_norm;
	mutable sgl::numeric level0_bound;
	mutable bool recompute_hessian_norm;

public:

	GenralizedLinearLossBase(data_type const& data, sgl::DimConfig const& dim_config);

	~GenralizedLinearLossBase()
	{
	}

	//Change evaluation point

	void at(sgl::parameter const& parameters);
	void at_zero();

	//Evaluation

	sgl::numeric evaluate() const;

	//Gradient

	sgl::vector const gradient() const;

	sgl::vector const compute_block_gradient(sgl::natural feature_index) const;

	sgl::numeric hessian_bound_level0() const;
	sgl::numeric hessian_bound_level1(sgl::natural block_index) const;

	static sgl::natural compute_unit_size(data_type const& data)
	{
		return data.n_responses;
	}

	static sgl::natural compute_number_of_units(data_type const& data)
	{
		return data.data_matrix.n_cols;
	}

protected:

	void compute_hessian_norm() const;
};

template < typename T , typename E >
GenralizedLinearLossBase < T , E >::GenralizedLinearLossBase(data_type const& data,
		sgl::DimConfig const& dim_config)
		: 	T(data),
		  	dim_config(dim_config),
		  	X(data.data_matrix),
		  	n_samples(data.n_samples),
		  	n_features(X.n_cols),
				n_feature_parameters(T::n_variables),
		  	partial_hessian(T::n_variables, n_samples),
		  	hessian_diag_mat_computed(dim_config.n_blocks),
		  	hessian_diag_mat(dim_config.n_blocks),
		  	current_parameters(dim_config.block_unit_dim, dim_config.block_dim),
		  	x_norm(dim_config.n_blocks),
		  	recompute_hessian_norm(true)
{

	//Dim check
	if(n_features*n_feature_parameters != dim_config.dim) {
		throw std::runtime_error("GenralizedLinearLossBase: Dimension Mismatch -- total parameters");
	}

	if(X.n_rows != n_samples) {
		throw std::runtime_error("GenralizedLinearLossBase: Dimension Mismatch -- number of samples");
	}

	if(X.n_rows == 0 || X.n_cols == 0) {
		throw std::runtime_error("GenralizedLinearLossBase: No Data");
	}


	//Initialize x_norm

	sgl::vector css(sqrt(colSumsSquare(X)));

	for (sgl::natural j = 0; j < dim_config.n_blocks; ++j)
	{

		x_norm(j) = max(css.subvec(dim_config.block_start_index(j) / n_feature_parameters,
					dim_config.block_end_index(j) / n_feature_parameters));

	}

	x_norm_max = x_norm.max();

	at_zero();
}

template < typename T , typename E >
void GenralizedLinearLossBase < T , E >::at(const sgl::parameter & parameters)
{

	current_parameters = parameters;

	//sgl::matrix lp(parameters.as_matrix()*trans(X)); //This way is very slow, too slow !!
	//T::set_lp(trans(lp));

	sgl::matrix lp(X * trans(parameters.as_matrix()));
	T::set_lp(lp);

	partial_hessian.zeros();

	if(!hessian_type::is_constant) {
		hessian_diag_mat_computed.zeros();
	}

	recompute_hessian_norm = true;
}

template < typename T , typename E >
void GenralizedLinearLossBase < T , E >::at_zero()
{

	current_parameters.zeros();
	T::set_lp_zero();

	partial_hessian.zeros();
	hessian_diag_mat_computed.zeros();

	recompute_hessian_norm = true;
}

template < typename T , typename E >
sgl::vector const GenralizedLinearLossBase < T , E >::gradient() const
{

	//TIMER_START

	return reshape(T::gradients() * X, n_features * n_feature_parameters, 1);
}

//note this function may return inf
template < typename T , typename E >
sgl::numeric GenralizedLinearLossBase < T , E >::evaluate() const
{
	return T::sum_values();
}

template < typename T , typename E >
inline sgl::vector const GenralizedLinearLossBase < T , E >::compute_block_gradient(
		sgl::natural block_index) const
{

	TIMER_START;

	sgl::index start = dim_config.block_start_index(block_index) / n_feature_parameters;
	sgl::index end = dim_config.block_end_index(block_index) / n_feature_parameters;

	return reshape(partial_hessian * X.cols(start,end), dim_config.block_dim(block_index), 1);
}

template < typename T , typename E >
inline void GenralizedLinearLossBase < T , E >::compute_hessian_norm() const
{

	TIMER_START;

	if(!recompute_hessian_norm) {
		return;
	}

	//NOTE norm configable, 2-norm, 1-norm

	partial_hessian_norm = sqrt(as_scalar(max(sum(square(partial_hessian), 1))));
	//partial_hessian_norm = as_scalar(max(sum(abs(partial_hessian), 1)));

	level0_bound = partial_hessian_norm * x_norm_max;

	recompute_hessian_norm = false;
}

template < typename T , typename E >
inline sgl::numeric GenralizedLinearLossBase < T , E >::hessian_bound_level0() const
{
	compute_hessian_norm();

	return level0_bound;
}

template < typename T , typename E >
inline sgl::numeric GenralizedLinearLossBase < T , E >::hessian_bound_level1(
		sgl::natural block_index) const
{

	compute_hessian_norm();

	return partial_hessian_norm * x_norm(block_index);
}

#endif /* SGL_GL_LOSS_BASE_H_*/
