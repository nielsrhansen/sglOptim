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

#ifndef SGL_GL_LOSS_SPARSE_H_
#define SGL_GL_LOSS_SPARSE_H_


// Sparse matrix specializations

template < typename T >
class GenralizedLinearLossSparse: public GenralizedLinearLossBase < T , sgl::sparse_matrix > {

public:

	typedef sgl::sparse_matrix matrix_type;

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

	GenralizedLinearLossSparse(data_type const& data, sgl::DimConfig const& dim_config);

	//Hessian
	sgl::matrix const hessian_diag(sgl::natural block_index) const;

	void hessian_update(sgl::natural block_index, sgl::parameter_block_vector const& z);

};

template < typename T >
GenralizedLinearLossSparse < T >::GenralizedLinearLossSparse(data_type const& data,
		sgl::DimConfig const& dim_config)
		: GenralizedLinearLossBase < T , sgl::sparse_matrix >(data, dim_config)
{
}

template < typename T >
void GenralizedLinearLossSparse < T >::hessian_update(sgl::natural block_index,
		sgl::parameter_block_vector const& z)
{

	TIMER_START;

	T::compute_hessians();

	sgl::matrix tmp(z - current_parameters.block(block_index));
	tmp.reshape(n_feature_parameters, dim_config.block_dim(block_index) / n_feature_parameters);

	sgl::vector tmp2(n_feature_parameters);

	for (sgl::natural i = dim_config.block_start_index(block_index) / n_feature_parameters;
			i < dim_config.block_end_index(block_index) / n_feature_parameters + 1; ++i)
	{
		tmp2 = tmp.col(i - dim_config.block_start_index(block_index) / n_feature_parameters);

		for (sgl::natural j = X.col_ptrs[i]; j < X.col_ptrs[i + 1]; ++j)
		{
			sgl::natural row = X.row_indices[j];

			partial_hessian.col(row) += hessian_type::update(T::hessians(row), tmp2, X.values[j]);
			//partial_hessian.col(row) += T::hessians(row) * tmp2 * X.values[j];
		}
	}

	recompute_hessian_norm = true;

	//Update current x
	current_parameters.set_block(block_index, z);
}

template < typename T >
inline sgl::matrix const GenralizedLinearLossSparse < T >::hessian_diag(
		sgl::natural block_index) const
{

	if (hessian_diag_mat_computed(block_index) != 0)
		{
			return hessian_diag_mat(block_index);
		}

		T::compute_hessians();

		hessian_diag_mat(block_index).zeros(dim_config.block_dim(block_index),
				dim_config.block_dim(block_index));

		sgl::sparse_matrix tmp(
				X.cols(dim_config.block_start_index(block_index) / n_feature_parameters,
						dim_config.block_end_index(block_index) / n_feature_parameters));

		for (sgl::natural j = 0; j < tmp.n_cols; ++j)
		{
			for (sgl::natural k = j; k < tmp.n_cols; ++k)
			{

				typename hessian_type::representation J(hessian_type::zero_representation_of(T::hessians(0)));

				for (sgl::natural i1 = tmp.col_ptrs[j]; i1 < tmp.col_ptrs[j + 1]; ++i1)
				{

					sgl::natural row1 = tmp.row_indices[i1];

					sgl::numeric vi2 = 0;

					for (sgl::natural i2 = tmp.col_ptrs[k]; i2 < tmp.col_ptrs[k + 1]; ++i2)
					{

						sgl::natural row2 = tmp.row_indices[i2];

						if (row1 != row2)
						{
							continue;
						}

						vi2 += tmp.values[i2];
					}

					if (vi2 != 0)
					{
						hessian_type::scalar_mult_add(J, vi2*tmp.values[i1], T::hessians(row1));
						//J += vi2 * tmp.values[i1]*T::hessians(row1);
					}
				}

				hessian_type::diag(hessian_diag_mat(block_index), j, k, n_feature_parameters, J);
			}
		}

		hessian_diag_mat(block_index) = symmatu(hessian_diag_mat(block_index));

		hessian_diag_mat_computed(block_index) = 1;

		return hessian_diag_mat(block_index);

}

#endif /* SGL_GL_LOSS_SPARSE_H_ */
