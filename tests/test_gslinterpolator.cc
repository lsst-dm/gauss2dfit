#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#ifdef GAUSS2D_FIT_HAS_GSL

#include "gsl.h"
#include "gslinterpolator.h"

namespace g2f = gauss2d::fit;

std::vector<double> x = {0., 1., 2., 2.5, 3.};
std::vector<double> y = {-1., 1., 0., -1., 0.};

const size_t size_x = x.size();

void check_interp(g2f::GSLInterpolator& interp) {
    CHECK(interp.size() == size_x);
    CHECK(interp.get_knot_x(0) == x[0]);
    CHECK(interp.get_knot_y(size_x - 1) == y[size_x - 1]);
    CHECK_THROWS_AS(interp.get_knot_x(size_x), std::out_of_range);

    auto type = interp.interp_type;
    if (type == g2f::InterpType::linear) {
        CHECK(interp.eval((x[0] + x[1]) / 2.) == (y[0] + y[1]) / 2.);
    } else {
        double interp_0 = interp.eval((x[0] + x[1]) / 2.);
        CHECK(interp_0 > y[0]);
        CHECK(interp_0 < y[1]);
        // It's not so obvious how a polynomial interpolator should behave
        // (best not to use them with so few knots in the first place)
        if (type != g2f::InterpType::polynomial) {
            CHECK(interp_0 > (y[0] + y[1]) / 2.);
        }
    }
}

TEST_CASE("GSLInterpolator Default") {
    g2f::GSLInterpolator interp(x, y);
    CHECK(y.size() == size_x);
    CHECK(y[0] == -1);
    CHECK(interp.size() == size_x);
    CHECK(interp.interp_type == g2f::GSLInterpolator::INTERPTYPE_DEFAULT);
    check_interp(interp);
}

TEST_CASE("GSLInterpolator Linear") {
    g2f::GSLInterpolator interp(x, y, g2f::InterpType::linear);
    check_interp(interp);
}

TEST_CASE("GSLInterpolator Polynomial") {
    g2f::GSLInterpolator interp(x, y, g2f::InterpType::polynomial);
    check_interp(interp);
}

TEST_CASE("GSLInterpolator Cubic Spline") {
    g2f::GSLInterpolator interp(x, y, g2f::InterpType::cspline);
    check_interp(interp);
}

TEST_CASE("GSLInterpolator Akima Spline") {
    g2f::GSLInterpolator interp(x, y, g2f::InterpType::akima);
    check_interp(interp);
}

#endif