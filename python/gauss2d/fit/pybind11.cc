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

#include <pybind11/pybind11.h>

#include "pybind11.h"

PYBIND11_MODULE(_gauss2d_fit, m)
{
    m.doc() = "Gauss2DFit Python bindings";
    py::module::import("gauss2d");

    // Abstract types MUST go first
    bind_parametric(m);
    bind_parametricmodel(m);
    bind_component(m);
    bind_integralmodel(m);

    bind_centroidparameters(m);
    bind_channel(m);
    bind_data(m);
    bind_ellipseparameters(m);
    bind_ellipticalcomponent(m);
    bind_fractionalintegralmodel(m);
    bind_gaussiancomponent(m);
    bind_linearintegralmodel(m);
    bind_model(m);
    bind_observation(m);
    bind_param_filter(m);
    bind_parameters(m);
    bind_psfmodel(m);
    bind_source(m);
}
