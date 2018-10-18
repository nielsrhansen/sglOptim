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

#ifndef DIMCONFIG_H_
#define DIMCONFIG_H_

class DimConfig {

private:

	sgl::natural_vector index;

public:

	sgl::vector const L1_penalty_weights;

	sgl::vector const L2_penalty_weights;

	/*
	 * Dimension of each block (i.e this is a vector of length n_blocks)
	 */
	const sgl::natural_vector block_dim;

	/*
	 * Number of blocks
	 */
	const sgl::natural n_blocks;

	/*
	 * Total dimension i.e. sum block_dim
	 */
	const sgl::natural dim;

	/*
	 * Block unit dimension - all blocks has size = 0 mod block_unit_dim
	 */
	const sgl::natural block_unit_dim;

	DimConfig() :
		index(static_cast<sgl::natural>(0)),
		L1_penalty_weights(static_cast<sgl::natural>(0)),
		L2_penalty_weights(static_cast<sgl::natural>(0)),
		block_dim(static_cast<sgl::natural>(0)),
		n_blocks(0),
		dim(0),
		block_unit_dim(0) {}

	DimConfig(
		sgl::natural_vector const& block_dim,
		sgl::natural unit_dim,
		sgl::vector const& L1_penalty_weights,
		sgl::vector const& L2_penalty_weights) :
			index(block_dim.n_elem + 1),
			L1_penalty_weights(L1_penalty_weights),
			L2_penalty_weights(L2_penalty_weights),
			block_dim(block_dim),
			n_blocks(block_dim.n_elem),
			dim(sum(block_dim)),
			block_unit_dim(unit_dim) {

		//Domain checks
		if (!sgl::is_non_negative(L1_penalty_weights)) {
			throw std::domain_error("L1 weights are negative");
		}
		if (!sgl::is_non_negative(L2_penalty_weights)) {
			throw std::domain_error("L2 weights are negative");
		}
		if (sum(block_dim) != L1_penalty_weights.n_elem) {
			throw std::logic_error("L1 weights dimension mismatch");
		}
		if (block_dim.n_elem != L2_penalty_weights.n_elem) {
			throw std::logic_error("L2 weights dimension mismatch");
		}

		//block_dim
		for(sgl::natural i = 1; i < block_dim.n_elem; ++i) {
			if(block_dim(i) == 0) {
				throw std::logic_error("dimension of a block is zero");
			}

			if(block_dim(i) % block_unit_dim != 0) {
				throw std::logic_error("dimension of a block is not a multiple of unit block size");
			}
		}

		//Initialise index
		index(0) = 0;
		for (sgl::natural i = 1; i < index.n_elem; i++) {
			index(i) = index(i - 1) + block_dim(i-1);
		}
	}

	sgl::natural block_start_index(sgl::natural block_index) const;

	sgl::natural block_end_index(sgl::natural block_index) const;

	sgl::vector L1_penalty_weight(sgl::natural block_index) const;

	sgl::numeric L2_penalty_weight(sgl::natural block_index) const;

	sgl::vector::const_iterator L1_penalty_weight_block_begin(sgl::natural block_index) const;

};

inline sgl::natural DimConfig::block_start_index(sgl::natural block_index) const {
	return index(block_index);
}

inline sgl::natural DimConfig::block_end_index(sgl::natural block_index) const {
	return index(block_index + 1) - 1;
}

inline sgl::vector DimConfig::L1_penalty_weight(sgl::natural block_index) const {
	return L1_penalty_weights.subvec(block_start_index(block_index), block_end_index(block_index));
}

inline sgl::vector::const_iterator DimConfig::L1_penalty_weight_block_begin(sgl::natural block_index) const {
	return L1_penalty_weights.begin_row(block_start_index(block_index));
}

inline sgl::numeric DimConfig::L2_penalty_weight(sgl::natural block_index) const {
	return L2_penalty_weights(block_index);
}

DimConfig createDimConfig(sgl::natural_vector const& block_dim, sgl::vector const& blocks_weights, sgl::matrix const& parameter_weights) {

	//Create L1 weights
	sgl::vector penalty_weight_L1 = reshape(parameter_weights, parameter_weights.n_elem, 1);

	if(block_dim.n_elem != blocks_weights.n_elem) {
		throw std::logic_error("L2 weights dimension mismatch");
	}

	if(sum(block_dim) != penalty_weight_L1.n_elem) {
		throw std::logic_error("L1 weights dimension mismatch");
	}

	return DimConfig(block_dim, parameter_weights.n_rows, penalty_weight_L1, blocks_weights);
}

#endif /* DIMCONFIG_H_ */
