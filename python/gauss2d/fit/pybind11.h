/*
 * This file is part of gauss2dfit.
 *
 * Developed for the LSST Data Management System.
 * This product includes software developed by the LSST Project
 * (https://www.lsst.org).
 * See the COPYRIGHT file at the top-level directory of this distribution
 * for details of code ownership.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef GAUSS2D_FIT_PYBIND11
#define GAUSS2D_FIT_PYBIND11

#include <pybind11/pybind11.h>

namespace py = pybind11;

void bind_centroidparameters(py::module &m);
void bind_channel(py::module &m);
void bind_component(py::module &m);
void bind_data(py::module &m);
void bind_ellipseparameters(py::module &m);
void bind_ellipticalcomponent(py::module &m);
void bind_fractionalintegralmodel(py::module &m);
void bind_gaussiancomponent(py::module &m);
void bind_integralmodel(py::module &m);
void bind_linearintegralmodel(py::module &m);
void bind_model(py::module &m);
void bind_observation(py::module &m);
void bind_param_filter(py::module &m);
void bind_parameters(py::module &m);
void bind_parametric(py::module &m);
void bind_parametricmodel(py::module &m);
void bind_psfmodel(py::module &m);
void bind_source(py::module &m);

#endif