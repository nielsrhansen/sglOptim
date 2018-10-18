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

#ifndef SGL_GL_LOSS_DENSE_H_
#define SGL_GL_LOSS_DENSE_H_


template < typename T >
class GenralizedLinearLossDense: public GenralizedLinearLossBase < T , sgl::matrix > {

public:

	typedef sgl::matrix matrix_type;

	typedef typename GenralizedLinearLossBase < T , matrix_type >::data_type data_type;
	typedef typename GenralizedLinearLossBase < T , matrix_type >::hessian_type hessian_type;

	using GenralizedLinearLossBase < T , matrix_type >::dim_config;

private:

	using GenralizedLinearLossBase < T , matrix_type >::X;

	using GenralizedLinearLossBase < T , matrix_type >::n_samples;
	using GenralizedLinearLossBase < T , matrix_type >::n_feature_parameters;
	using GenralizedLinearLossBase < T , matrix_type >::n_features;

	using GenralizedLinearLossBase < T , matrix_type >::partial_hessian;
	using GenralizedLinearLossBase < T , matrix_type >::hessian_diag_mat_computed;
	using GenralizedLinearLossBase < T , matrix_type >::hessian_diag_mat;
	using GenralizedLinearLossBase < T , matrix_type >::current_parameters;

	using GenralizedLinearLossBase < T , matrix_type >::x_norm;
	using GenralizedLinearLossBase < T , matrix_type >::x_norm_max;
	using GenralizedLinearLossBase < T , matrix_type >::partial_hessian_norm;
	using GenralizedLinearLossBase < T , matrix_type >::level0_bound;
	using GenralizedLinearLossBase < T , matrix_type >::recompute_hessian_norm;

public:

	GenralizedLinearLossDense(data_type const& data, sgl::DimConfig const& dim_config);

	//Hessian
	sgl::matrix const hessian_diag(sgl::natural block_index) const;

	void hessian_update(sgl::natural block_index, sgl::parameter_block_vector const& z);

};

template < typename T >
GenralizedLinearLossDense < T >::GenralizedLinearLossDense(data_type const& data,
		sgl::DimConfig const& dim_config)
		: GenralizedLinearLossBase < T , sgl::matrix >(data, dim_config)
{
}

template < typename T >
inline sgl::matrix const GenralizedLinearLossDense < T >::hessian_diag(
		sgl::natural block_index) const
{

	TIMER_START;

	if (hessian_diag_mat_computed(block_index) != 0)
	{
		return hessian_diag_mat(block_index);
	}

	T::compute_hessians();

	hessian_diag_mat(block_index).zeros(
			dim_config.block_dim(block_index), dim_config.block_dim(block_index));

	sgl::natural n_cols = (dim_config.block_end_index(block_index) - dim_config.block_start_index(block_index)) / n_feature_parameters + 1;
	sgl::natural X_col_offset = dim_config.block_start_index(block_index) / n_feature_parameters;

	typename hessian_type::representation J(hessian_type::zero_representation_of(T::hessians(0)));

	for (sgl::natural j = 0; j < n_cols; ++j)
	{
		for (sgl::natural k = j; k < n_cols; ++k)
		{

			matrix_type::const_col_iterator j_col = X.begin_col(j + X_col_offset);
			matrix_type::const_col_iterator k_col = X.begin_col(k + X_col_offset);

			hessian_type::scalar_mult_set(J, (*j_col) * (*k_col), T::hessians(0));
			//typename hessian_type::representation J(hessian_type::scalar_mult((*j_col) * (*k_col), T::hessians(0)));

			++j_col;
			++k_col;

			for (sgl::natural i = 1; i < n_samples; ++i, ++j_col, ++k_col)
			{
				hessian_type::scalar_mult_add(J, (*j_col) * (*k_col), T::hessians(i));
			}

			hessian_type::diag(hessian_diag_mat(block_index), j, k, n_feature_parameters, J);

		}
	}

	hessian_diag_mat(block_index) = symmatu(hessian_diag_mat(block_index));

	hessian_diag_mat_computed(block_index) = 1;

	return hessian_diag_mat(block_index);
}

template < typename T >
void GenralizedLinearLossDense < T >::hessian_update(
		sgl::natural block_index, sgl::parameter_block_vector const& z) {

	TIMER_START
	DEBUG_ENTER

	//Update
	T::compute_hessians();

	if(hessian_type::is_constant) {

		sgl::matrix tmp2(z - current_parameters.block(block_index));
		tmp2.reshape(n_feature_parameters, dim_config.block_dim(block_index) / n_feature_parameters);

		partial_hessian += hessian_type::update(
			T::hessians(0),
			tmp2*trans(X.cols(
				dim_config.block_start_index(block_index) / n_feature_parameters,
				dim_config.block_end_index(block_index) / n_feature_parameters)), 1.0);

	}

	else {


		sgl::matrix tmp1(
			reshape(z - current_parameters.block(block_index),
					n_feature_parameters,
					dim_config.block_dim(block_index) / n_feature_parameters)
			* trans(X.cols(
					dim_config.block_start_index(block_index) / n_feature_parameters,
					dim_config.block_end_index(block_index) / n_feature_parameters)));

		for (sgl::natural i = 0; i < n_samples; ++i)
		{
			partial_hessian.col(i) += hessian_type::update(T::hessians(i), tmp1.col(i), 1.0);
			//partial_hessian.col(i) += T::hessians(i) * tmp1.col(i);
		}

	}

	recompute_hessian_norm = true;

	//Update current x
	current_parameters.set_block(block_index, z);
		}

#endif /* SGL_GL_LOSS_DENSE_H_ */
