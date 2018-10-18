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

#ifndef RLIST_H_
#define RLIST_H_

#include <vector>

class rList {

private:

    std::vector < rObject > objects;
    std::vector < std::string > names;

public:

	rList()	: objects(), names() {}

	rList(SEXP list) : objects(), names() {

		SEXP SEXP_names = Rf_getAttrib(list, R_NamesSymbol);

		for (int i = 0; i < Rf_length(list); ++i)
		{
			attach(VECTOR_ELT(list, i), CHAR(STRING_ELT(SEXP_names, i)));
		}

	}

	template<typename T>
	rList(arma::field<T> const& field) {
		for (unsigned int i = 0; i < field.n_elem; i++) {
				// attaching

			std::stringstream ss;
			ss << i;

			attach(field(i), ss.str());
		}
	}

	~rList()
	{
	}

  void attach(rObject const& object, std::string const& name) {
		objects.push_back(object);
		names.push_back(name);
	}

	rObject get(unsigned int index) const {
		return objects[index];
	}

  std::string getName(unsigned int index) const {
		return names[index];
	}

	unsigned int length() const {
		return objects.size();
	}

  int getIndex(std::string const& name) const
	{

        for (unsigned int index = 0; index < objects.size(); ++index)
		{
			if (name.compare(names[index]) == 0)
			{
				return index;
			}
		}

		return -1;

	}

};

template<typename T>
class elements {
public:
  rList as_rList() const {
    return static_cast<T const * >(this) -> as_rList();
  }
};

#endif /* RLIST_H_ */
