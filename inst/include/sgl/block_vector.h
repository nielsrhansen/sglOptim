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

#ifndef BLOCKVECTOR_H_
#define BLOCKVECTOR_H_


template<typename MATRIX, typename VECTOR>
class BlockVector {

  private:

    MATRIX matrix;

    arma::uvec const block_pos;

    static arma::uvec
    compute_block_pos(arma::u32 unit_size, arma::uvec block_sizes);

  public:

    arma::uvec const block_sizes;

    arma::u32 const n_blocks;
    arma::u32 const n_elem;

    arma::u32 const n_nonzero;
    arma::u32 const n_nonzero_blocks;

    BlockVector<MATRIX, VECTOR>();

    BlockVector<MATRIX, VECTOR>(
      arma::u32 unit_size,
      arma::uvec block_sizes)
    ;

    BlockVector<MATRIX, VECTOR>(BlockVector<MATRIX, VECTOR> const& source);

    // BlockVector<MATRIX, VECTOR>(
    //   MATRIX const& source,
    //   const sgl::DimConfig & dim);

    void set_dim(
      arma::u32 unit_size,
      arma::uvec block_sizes)
    ;

    void copy_size(BlockVector<MATRIX, VECTOR> const& source);

    VECTOR const block(arma::u32 block_index) const;

    void set_block(
      arma::u32 block_index,
      arma::sp_vec const& block);

    void set_block_zero(arma::u32 block_index);

    void zeros();

    bool is_block_zero(arma::u32 block_index) const;

    operator MATRIX const&() const { return matrix; }

    MATRIX const& as_matrix() const { return matrix; }

    BlockVector<MATRIX, VECTOR> const&
      operator =(BlockVector<MATRIX, VECTOR> const& source);

    BlockVector<MATRIX, VECTOR> const&
      operator +=(BlockVector<MATRIX, VECTOR> const& x);

    BlockVector<MATRIX, VECTOR> const&
      operator -=(BlockVector<MATRIX, VECTOR> const& x);

    BlockVector<MATRIX, VECTOR> const&
      operator *=(typename MATRIX::elem_type const& s);

    BlockVector<MATRIX, VECTOR>
      operator -();

    double norm2sq() const;

    double norm2sq(arma::vec const& weights) const;
  };

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>::BlockVector() :
      matrix(),
      block_pos(),
      block_sizes(),
      n_blocks(0),
      n_elem(0),
      n_nonzero(0),
      n_nonzero_blocks(0) {}

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>::BlockVector(
    arma::u32 unit_size,
    arma::uvec block_sizes) :
      matrix(unit_size, sum(block_sizes) / unit_size),
      block_pos(compute_block_pos(unit_size, block_sizes)),
      block_sizes(block_sizes),
      n_blocks(block_sizes.n_elem),
      n_elem(sum(block_sizes)),
      n_nonzero(0),
      n_nonzero_blocks(0) {}

template<typename MATRIX, typename VECTOR>
    BlockVector<MATRIX, VECTOR>::BlockVector(const BlockVector<MATRIX, VECTOR> & source) :
      matrix(source.as_matrix()),
      block_pos(compute_block_pos(matrix.n_rows, source.block_sizes)),
      block_sizes(source.block_sizes),
      n_blocks(source.n_blocks),
      n_elem(source.n_elem),
      n_nonzero(source.n_nonzero),
      n_nonzero_blocks(source.n_nonzero_blocks) {}

// template<typename MATRIX, typename VECTOR>
//   BlockVector<MATRIX, VECTOR>::BlockVector(
//
//     const MATRIX & source,
//     const sgl::DimConfig & dim) :
//
//     matrix(source),
//     block_pos(compute_block_pos(matrix.n_rows, dim.block_dim)),
//     block_sizes(dim.block_dim),
//     n_blocks(dim.n_blocks),
//     n_elem(dim.dim),
//     n_nonzero(source.n_nonzero),
//     n_nonzero_blocks(0) {
//
//     //Compute n_nonzero_blocks
//
//     arma::u32 nonzero_blocks = 0;
//     for (arma::u32 i = 0; i < n_blocks; ++i)
//       {
//         if (!is_cols_zero(matrix, block_pos(i), block_pos(i + 1) - 1))
//           {
//             ++nonzero_blocks;
//           }
//       }
//
//     const_cast<arma::u32&>(this->n_nonzero_blocks) = nonzero_blocks;
//
//   }

template<typename MATRIX, typename VECTOR>
  void
  BlockVector<MATRIX, VECTOR>::set_dim(
    arma::u32 unit_size,
    arma::uvec block_sizes) {

    arma::u32 dim = sum(block_sizes);

    matrix.set_size(unit_size, dim / unit_size);
    matrix.zeros();

    const_cast<arma::uvec&>(this->block_sizes) = block_sizes;
    const_cast<arma::u32&>(this->n_blocks) = block_sizes.n_elem;
    const_cast<arma::u32&>(this->n_elem) = dim;
    const_cast<arma::u32&>(this->n_nonzero) = 0;
    const_cast<arma::u32&>(this->n_nonzero_blocks) = 0;

    const_cast<arma::uvec&>(this->block_pos)
      = compute_block_pos(
        matrix.n_rows,
        block_sizes
      );
  }

template<typename MATRIX, typename VECTOR>
  void
  BlockVector<MATRIX, VECTOR>::copy_size(
      const BlockVector<MATRIX, VECTOR> & source)
  {
    matrix.copy_size(source.as_matrix());
    matrix.zeros();

    const_cast<arma::uvec&>(this->block_sizes) = source.block_sizes;
    const_cast<arma::u32&>(this->n_blocks) = source.n_blocks;
    const_cast<arma::u32&>(this->n_elem) = source.n_elem;
    const_cast<arma::u32&>(this->n_nonzero) = 0;
    const_cast<arma::u32&>(this->n_nonzero_blocks) = 0;

    const_cast<arma::uvec&>(this->block_pos) = compute_block_pos(matrix.n_rows,
        block_sizes);
  }

template<typename MATRIX, typename VECTOR>
  const BlockVector<MATRIX, VECTOR> &
  BlockVector<MATRIX, VECTOR>::operator =(
      const BlockVector<MATRIX, VECTOR> & source)
  {
    matrix = source.matrix;

    const_cast<arma::uvec&>(this->block_sizes) = source.block_sizes;
    const_cast<arma::u32&>(this->n_blocks) = source.n_blocks;
    const_cast<arma::u32&>(this->n_elem) = source.n_elem;
    const_cast<arma::u32&>(this->n_nonzero) = source.n_nonzero;
    const_cast<arma::u32&>(this->n_nonzero_blocks) = source.n_nonzero_blocks;

    const_cast<arma::uvec&>(this->block_pos) = compute_block_pos(matrix.n_rows,
        block_sizes);

    return *this;
  }

template<typename MATRIX, typename VECTOR>
  void
  BlockVector<MATRIX, VECTOR>::zeros()
  {
    matrix.zeros();

    const_cast<arma::u32&>(this->n_nonzero) = 0;
    const_cast<arma::u32&>(this->n_nonzero_blocks) = 0;
  }

template<typename MATRIX, typename VECTOR>
  const BlockVector<MATRIX, VECTOR> &
  BlockVector<MATRIX, VECTOR>::operator +=(
      const BlockVector<MATRIX, VECTOR> & x)
  {
    matrix += x.as_matrix();

    const_cast<arma::u32&>(this->n_nonzero) = matrix.n_nonzero;

    arma::u32 nonzero_blocks = 0;
    for (arma::u32 i = 0; i < n_blocks; ++i)
      {
        if (!is_cols_zero(matrix, block_pos(i), block_pos(i + 1) - 1))
          {
            ++nonzero_blocks;
          }
      }

    const_cast<arma::u32&>(this->n_nonzero_blocks) = nonzero_blocks;

    return *this;
  }

template<typename MATRIX, typename VECTOR>
  const BlockVector<MATRIX, VECTOR> &
  BlockVector<MATRIX, VECTOR>::operator -=(
      const BlockVector<MATRIX, VECTOR> & x)
  {
    matrix -= x.as_matrix();

    const_cast<arma::u32&>(this->n_nonzero) = matrix.n_nonzero;

    arma::u32 nonzero_blocks = 0;
    for (arma::u32 i = 0; i < n_blocks; ++i)
      {
        if (!is_cols_zero(matrix, block_pos(i), block_pos(i + 1) - 1))
          {
            ++nonzero_blocks;
          }
      }

    const_cast<arma::u32&>(this->n_nonzero_blocks) = nonzero_blocks;

    return *this;
  }

template<typename MATRIX, typename VECTOR>
  const BlockVector<MATRIX, VECTOR> &
  BlockVector<MATRIX, VECTOR>::operator *=(const typename MATRIX::elem_type & s)
  {

    if (s == 0)
      {
        zeros();
        return *this;
      }

    matrix *= s;
    return *this;
  }

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>
  BlockVector<MATRIX, VECTOR>::operator -()
  {
    matrix = -matrix;
    return *this;
  }

template<typename MATRIX, typename VECTOR>
  double
  BlockVector<MATRIX, VECTOR>::norm2sq() const
  {
    return square(norm(matrix, 2));
  }

template<typename MATRIX, typename VECTOR>
  double
  BlockVector<MATRIX, VECTOR>::norm2sq(const arma::vec & weights) const
  {

    double r = 0;

    for (arma::u32 i = 0; i < n_blocks; ++i)
      {
        r += weights(i)
            * accu(square(matrix.cols(block_pos(i), block_pos(i + 1) - 1)));
      }

    return r;
  }

template<typename MATRIX, typename VECTOR>
  typename MATRIX::elem_type
  max(BlockVector<MATRIX, VECTOR> const& a)
  {
    return max(static_cast<MATRIX>(a));
  }

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>
  operator +(BlockVector<MATRIX, VECTOR> const& a
      , BlockVector<MATRIX, VECTOR> const& b)
  {
    BlockVector<MATRIX, VECTOR> x(a);
    x += b;
    return x;
  }

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>
  operator -(BlockVector<MATRIX, VECTOR> const& a
      , BlockVector<MATRIX, VECTOR> const& b)
  {
    BlockVector<MATRIX, VECTOR> x(a);
    x -= b;
    return x;
  }

template<typename MATRIX, typename VECTOR>
  BlockVector<MATRIX, VECTOR>
  operator *(typename MATRIX::elem_type const& a,
      BlockVector<MATRIX, VECTOR> const& b)
  {
    BlockVector<MATRIX, VECTOR> x(b);
    x *= a;
    return x;
  }

template<typename MATRIX, typename VECTOR>
  typename MATRIX::elem_type
  dot(BlockVector<MATRIX, VECTOR> const& a
      , BlockVector<MATRIX, VECTOR> const& b)
  {
    return sum(a.as_matrix() % b.as_matrix());
  }

template<typename MATRIX, typename VECTOR>
  typename MATRIX::elem_type
  dot(arma::vec const& a, BlockVector<MATRIX, VECTOR> const& b)
  {
    arma::u32 n_cols = b.as_matrix().n_cols;
    arma::u32 n_rows = b.as_matrix().n_rows;

    MATRIX tmp(a);
    tmp.reshape(n_rows, n_cols);
    return arma::as_scalar(accu(tmp % b.as_matrix()));
  }

template<typename MATRIX, typename VECTOR>
  VECTOR const
  BlockVector<MATRIX, VECTOR>::block(arma::u32 block_index) const
  {
    MATRIX tmp(
        matrix.cols(block_pos(block_index), block_pos(block_index + 1) - 1));

    tmp.reshape(block_sizes(block_index), 1);

    return VECTOR(tmp);
  }

template<typename MATRIX, typename VECTOR>
  bool
  BlockVector<MATRIX, VECTOR>::is_block_zero(arma::u32 block_index) const
  {
    return is_cols_zero(matrix, block_pos(block_index),
        block_pos(block_index + 1) - 1);
  }

template<typename MATRIX, typename VECTOR>
  void
  BlockVector<MATRIX, VECTOR>::set_block_zero(arma::u32 block_index)
  {

    if (is_block_zero(block_index))
      {
        return;
      }

    matrix.cols(block_pos(block_index), block_pos(block_index + 1) - 1).zeros();

    const_cast<arma::u32&>(this->n_nonzero) = matrix.n_nonzero;
    const_cast<arma::u32&>(this->n_nonzero_blocks) = n_nonzero_blocks - 1;

  }

template<typename MATRIX, typename VECTOR>
  void
  BlockVector<MATRIX, VECTOR>::set_block(arma::u32 block_index,
      arma::sp_vec const& block)
  {

    if (block.n_nonzero == 0)
      {
        set_block_zero(block_index);
        return;
      }

    if (is_block_zero(block_index))
      {
        const_cast<arma::u32&>(this->n_nonzero_blocks) = n_nonzero_blocks + 1;
      }

    for (arma::u32 i = block_pos(block_index); i < block_pos(block_index + 1);
        ++i)
      {
        matrix.col(i) = block.rows(matrix.n_rows * (i - block_pos(block_index)),
            matrix.n_rows * (i + 1 - block_pos(block_index)) - 1);
      }

    const_cast<arma::u32&>(this->n_nonzero) = matrix.n_nonzero;
  }

template<typename MATRIX, typename VECTOR>
  arma::uvec
  BlockVector<MATRIX, VECTOR>::compute_block_pos(arma::u32 unit_size,
      arma::uvec block_sizes)
  {
    arma::uvec block_pos(block_sizes.n_elem + 1);

    block_pos(0) = 0;
    for (arma::u32 i = 1; i < block_pos.n_elem; ++i)
      {
        block_pos(i) = block_pos(i - 1) + block_sizes(i - 1) / unit_size;
      }

    return block_pos;
  }

#endif /* BLOCKVECTOR_H_ */
