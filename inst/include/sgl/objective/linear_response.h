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

#ifndef LINEAR_RESPONSE_H_
#define LINEAR_RESPONSE_H_

class LinearResponse : public elements < LinearResponse > {

private:
  vector const linear_predictors;

public:

  LinearResponse(vector const& linear_predictors) :
    linear_predictors(linear_predictors) {}

  //Needed so that we can use fields
  LinearResponse() :
    linear_predictors(null_vector) {}

  LinearResponse const& operator=(LinearResponse const& s) {

    const_cast< vector & >(this->linear_predictors) = s.linear_predictors;

    return * this;
	}

  rList as_rList() const {

    rList list;
    list.attach(linear_predictors, "link");

    return list;
  }

};

#endif /* LINEAR_RESPONSE_H_ */
