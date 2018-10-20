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

#ifndef ARMA_ADDITIONS_H_
#define ARMA_ADDITIONS_H_

bool is_cols_zero(arma::sp_mat const& a, arma::u32 col1, arma::u32 col2) {

if(a.col_ptrs[col1] - a.col_ptrs[col2+1] == 0) {
    return true;
}

return false;
}

namespace arma {


  template<typename eT>
  inline
  const SpMat<eT>
  join_rows(const SpMat<eT>& x, const SpMat<eT>& y)
  {
          const uword x_n_rows = x.n_rows;
          const uword x_n_cols = x.n_cols;
          const uword x_n_nonzero = x.n_nonzero;
          const uword y_n_rows = y.n_rows;
          const uword y_n_cols = y.n_cols;
          const uword y_n_nonzero = y.n_nonzero;

          arma_debug_check
            (
            ( (y_n_rows != x_n_rows) && ( (y_n_rows > 0) || (y_n_cols > 0) ) && ( (x_n_rows > 0) || (x_n_cols > 0) ) ),
            "join_rows(): number of rows must be the same"
            );

          SpMat<eT> tmp(x_n_rows, x_n_cols+y_n_cols);

           //Mem

          uword* new_row_indices = memory::acquire<uword>(x_n_nonzero + y_n_nonzero + 1);
          eT* new_values         = memory::acquire<eT>(x_n_nonzero + y_n_nonzero + 1);

          arrayops::copy(new_values, x.values, x_n_nonzero);
          arrayops::copy(new_values+x_n_nonzero, y.values, y_n_nonzero+1);

          arrayops::copy(new_row_indices, x.row_indices, x_n_nonzero);
          arrayops::copy(new_row_indices+x_n_nonzero, y.row_indices, y_n_nonzero+1);

          arrayops::copy(access::rwp(tmp.col_ptrs), x.col_ptrs, x_n_cols);
          arrayops::copy(access::rwp(tmp.col_ptrs) + x_n_cols, y.col_ptrs, y_n_cols+2);
          arrayops::inplace_plus(access::rwp(tmp.col_ptrs)+x_n_cols, x_n_nonzero, y_n_cols+1);

          memory::release(tmp.values);
          memory::release(tmp.row_indices);

          access::rw(tmp.values)       = new_values;
          access::rw(tmp.row_indices)  = new_row_indices;

          // Update counts and such.
          access::rw(tmp.n_nonzero) = x_n_nonzero+y_n_nonzero;
          access::rw(tmp.n_cols) = x_n_cols+y_n_cols;

          return(tmp);
  }
}  // namespace arma


template<class E, class F>
  static void
  copy_cast(E* target, const F* source, arma::uword size)
  {

    for (arma::uword i = 0; i < size; ++i)
      {
        target[i] = static_cast<E>(source[i]);
      }

  }

#endif /* ARMA_ADDITIONS_H_ */
