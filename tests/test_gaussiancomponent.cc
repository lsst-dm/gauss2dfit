#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include "doctest.h"

#include "channel.h"
#include "gaussiancomponent.h"

namespace g2 = gauss2d;
namespace g2f = gauss2d::fit;

TEST_CASE("GaussianComponent") {
    const auto & C = g2f::Channel::NONE();
    auto comp = std::make_shared<g2f::GaussianComponent>();

    g2f::ParamCRefs params{};
    // 2 centroid, 3 ellipse, 1 integral
    CHECK(comp->get_parameters_const(params).size() == 6);

    auto gaussians = comp->get_gaussians(C);
    CHECK(gaussians->size() == 1);
    const auto & g0 = gaussians->at(0);
    const g2::Gaussian g1{};
    CHECK(g0.get_centroid_const() == g1.get_centroid_const());
    CHECK(g0.get_ellipse_const() == g1.get_ellipse_const());
    CHECK(g0.get_integral_const() == g1.get_integral_const());

    CHECK(gaussians->at_const(0) == g1);
}
