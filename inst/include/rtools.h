/*
	Lightweight tools for R and c++ integration.
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

#ifndef RTOOLS_H_
#define RTOOLS_H_

#include "rtools/rObject_decl.h"
#include "rtools/rList.h"
#include "rtools/get_value.h"
#include "rtools/rObject_def.h"
#include "rtools/test.h"

void report_error(const char *msg) {
    Rf_error(%s msg);
}

void report_warning(const char *msg) {
    Rf_warning(%s msg);
}

#endif /* RTOOLS_H_ */
