#include "TestTools.h"

#include "Localization/Localization.h"
#include "Localization/Combiner/FilteredMeanCombiner.h"
#include "Localization/Combiner/MeanCombiner.h"
#include "Localization/Combiner/MedianCombiner.h"
#include "Localization/Combiner/TriangleCombiner.h"
#include "Localization/Combiner/TimeSumCombiner.h"
#include "Geometric/Points/SpherePoint.h"

TEST(DIRECT_DETECT, IN_TRIANGLE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-5, -5, 15.640312423743286},
                                                           {5,  -5, 15.640312423743286},
                                                           {0,  5,  15.6}},
                                                          10),
                           pl::TimePoint<>(0, -1, 15))
}

TEST(DIRECT_DETECT, OUT_TRIANGLE_1) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{1,  1, 11.17851130197758},
                                                           {10, 2, 10.424918292799399},
                                                           {12, 3, 10.25}},
                                                          12),
                           pl::TimePoint<>(15, 3, 10))
}

TEST(DIRECT_DETECT, OUT_TRIANGLE_2) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-10, -15, 13.77307714089059},
                                                           {-2,  -5,  12.927361345041716},
                                                           {-8,  -20, 14.174159928789397}},
                                                          12),
                           pl::TimePoint<>(-5, 30, 10))
}

TEST(DIRECT_DETECT, DETECTOR_LINE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-1, -1, 4.58113883008419},
                                                           {-2, -2, 4.414213562373095},
                                                           {-4, -4, 5}},
                                                          2),
                           std::nullopt)
}

TEST(DIRECT_DETECT, ON_EDGE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-1, 0, 3.5},
                                                           {1,  0, 3.5},
                                                           {0,  2, 4}},
                                                          2),
                           pl::TimePoint<>(0, 0, 3))
}

TEST(DIRECT_DETECT, BEHIND_DETECTOR) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-1, 0, 5.061552812808831},
                                                           {1,  0, 5.061552812808831},
                                                           {0,  2, 4}},
                                                          2),
                           std::nullopt)
}

TEST(DIRECT_DETECT, FOUR_DETECTORS) {
    LOCALIZATION_RESULT_EQ(pl::localizationByDirectMethod({{-1, 0, 5.061552812808831},
                                                           {1,  0, 5.061552812808831},
                                                           {0,  2, 4},
                                                           {4,  2, 5.23606797749979}},
                                                          2),
                           pl::TimePoint<>(0, 4, 3))
}

TEST(QP_DETECT, IN_TRIANGLE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-5, -5, 15.640312423743286},
                                                       {5,  -5, 15.640312423743286},
                                                       {0,  5,  15.6}},
                                                      10),
                           pl::TimePoint<>(0, -1, 15))
}

TEST(QP_DETECT, OUT_TRIANGLE_1) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{1,  1, 11.17851130197758},
                                                       {10, 2, 10.424918292799399},
                                                       {12, 3, 10.25}},
                                                      12),
                           pl::TimePoint<>(15, 3, 10))
}

TEST(QP_DETECT, OUT_TRIANGLE_2) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-10, -15, 13.77307714089059},
                                                       {-2,  -5,  12.927361345041716},
                                                       {-8,  -20, 14.174159928789397}},
                                                      12),
                           pl::TimePoint<>(-5, 30, 10))
}

TEST(QP_DETECT, DETECTOR_LINE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-1, -1, 4.58113883008419},
                                                       {-2, -2, 4.414213562373095},
                                                       {-4, -4, 5}},
                                                      2),
                           std::nullopt)
}

TEST(QP_DETECT, ON_EDGE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-1, 0, 3.5},
                                                       {1,  0, 3.5},
                                                       {0,  2, 4}},
                                                      2),
                           pl::TimePoint<>(0, 0, 3))}

TEST(QP_DETECT, BEHIND_DETECTOR) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-1, 0, 5.061552812808831},
                                                       {1,  0, 5.061552812808831},
                                                       {0,  2, 4}},
                                                      2),
                           std::nullopt)
}

TEST(QP_DETECT, FOUR_DETECTORS) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQPMethod({{-1, 0, 5.061552812808831},
                                                       {1,  0, 5.061552812808831},
                                                       {0,  2, 4},
                                                       {4,  2, 5.23606797749979}},
                                                      2),
                           pl::TimePoint<>(0, 4, 3))
}

TEST(QUAD_DETECT, IN_QUAD) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-5, -5, 11.781024967590666},
                                                               {-5, 5,  11.640312423743286},
                                                               {6,  5,  11.721110255092798},
                                                               {6,  -5, 11.848528137423857}},
                                                              10),
                           pl::TimePoint<>(0, 1, 11))
}

TEST(QUAD_DETECT, OUT_QUAD_1) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-3, -4, 5.279508497187473},
                                                               {-4, -3, 5.375},
                                                               {0,  2,  6.908042190309218},
                                                               {5,  -4, 7.253469547164993}},
                                                              4),
                           pl::TimePoint<>(-4, -4.5, 5))
}

TEST(QUAD_DETECT, OUT_QUAD_2) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-3, -4, 8.553167600887974},
                                                               {-4, -3, 8.535533905932738},
                                                               {0,  2,  6.9525624189766635},
                                                               {5,  -4, 7.761340254296815}},
                                                              4),
                           pl::TimePoint<>(6, 7, 5))
}

TEST(QUAD_DETECT, BEHIND_DETECTOR) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-5, -5, 12.555634918610405},
                                                               {-5, 5,  12.104536101718725},
                                                               {5,  5,  11.14142135623731},
                                                               {5,  -4, 12.004987562112088}},
                                                              10),
                           pl::TimePoint<>(6, 6, 11))
}

TEST(QUAD_DETECT, ON_EDGE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-5, -5, 9.666666666666666},
                                                               {-5, 5,  7.666666666666667},
                                                               {5,  5,  10.399346342395189},
                                                               {5,  -4, 11.068851871911235}},
                                                              3),
                           pl::TimePoint<>(-5, 3, 7))
}

TEST(QUAD_DETECT, FIVE_DETECTORS) {
    LOCALIZATION_RESULT_EQ(pl::localizationByQuadrangleMethod({{-3, -4, 8.553167600887974},
                                                               {-4, -3, 8.535533905932738},
                                                               {0,  2,  6.9525624189766635},
                                                               {5,  -4, 7.761340254296815},
                                                               {7,  6,  5.353553390593274}},
                                                              4),
                           pl::TimePoint<>(6, 7, 5))
}

TEST(ELDER_MEAD_DETECT, IN_TRIANGLE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByElderMeadMethod({{-5, -5, 15.640312423743286},
                                                              {5,  -5, 15.640312423743286},
                                                              {0,  5,  15.6}},
                                                             10,
                                                             {0, 0, 0},
                                                             1000),
                           pl::TimePoint<>(0, -1, 15))
}

TEST(ELDER_MEAD_DETECT, OUT_TRIANGLE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByElderMeadMethod({{-10, -15, 13.77307714089059},
                                                              {-2,  -5,  12.927361345041716},
                                                              {-8,  -20, 14.174159928789397}},
                                                             12,
                                                             {0, 0, 0},
                                                             1000),
                           pl::TimePoint<>(-5, 30, 10))
}

TEST(ELDER_MEAD_DETECT, ON_EDGE) {
    LOCALIZATION_RESULT_EQ(pl::localizationByElderMeadMethod({{-1, 0, 3.5},
                                                              {1,  0, 3.5},
                                                              {0,  2, 4}},
                                                             2,
                                                             {0, 0, 0},
                                                             1000),
                           pl::TimePoint<>(0, 0, 3))
}

TEST(ELDER_MEAD_DETECT, IN_QUAD) {
    LOCALIZATION_RESULT_EQ(pl::localizationByElderMeadMethod({{-5, -5, 11.781024967590666},
                                                              {-5, 5,  11.640312423743286},
                                                              {6,  5,  11.721110255092798},
                                                              {6,  -5, 11.848528137423857}},
                                                             10,
                                                             {0, 0, 0},
                                                             1000),
                           pl::TimePoint<>(0, 1, 11))
}

TEST(MEAN_COMBINER, INHERITANCE) {
    pl::MeanCombiner combiner;
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    combiner.reset();
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 1));
}

TEST(MEAN_COMBINER, MULTY) {
    pl::MeanCombiner combiner;
    combiner.add(pl::TimePoint<>(1, 0, 0), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(5, 1, 2), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 0, 3), std::vector<pl::TimePoint<>>());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(2, 0.5, 1.5));
}

TEST(MEDIAN_COMBINER, INHERITANCE) {
    pl::MedianCombiner combiner;
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    combiner.reset();
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 1));
}

TEST(MEDIAN_COMBINER, MULTY) {
    pl::MedianCombiner combiner;
    combiner.add(pl::TimePoint<>(1, 0, 0), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(5, 1, 2), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 0, 3), std::vector<pl::TimePoint<>>());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 2));
}

TEST(FILTERED_MEAN_COMBINER, INHERITANCE) {
    pl::FilteredMeanCombiner combiner;
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    combiner.reset();
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    EXPECT_TRUE(combiner.result().has_value());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 1));
}

TEST(FILTERED_MEAN_COMBINER, MULTY) {
    pl::FilteredMeanCombiner combiner;
    combiner.add(pl::TimePoint<>(1, 0, 0), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 1, 1), std::vector<pl::TimePoint<>>());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 0.5, 0.5));
    combiner.add(pl::TimePoint<>(5, 1, 2), std::vector<pl::TimePoint<>>());
    combiner.add(pl::TimePoint<>(1, 0, 3), std::vector<pl::TimePoint<>>());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 0.5, 1.5));
}

TEST(FILTERED_TRIANGLE_COMBINER, INHERITANCE) {
    pl::TriangleCombiner combiner;
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), {
        pl::TimePoint<>{-5, 0},
        pl::TimePoint<>{5, 0},
        pl::TimePoint<>{0, 8.66}
    });
    EXPECT_TRUE(combiner.result().has_value());
    combiner.reset();
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1), {
        pl::TimePoint<>{-5, 0},
        pl::TimePoint<>{5, 0},
        pl::TimePoint<>{0, 8.66}
    });
    EXPECT_TRUE(combiner.result().has_value());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 1));
}

TEST(FILTERED_TRIANGLE_COMBINER, MULTY) {
    pl::TriangleCombiner combiner;
    combiner.add(pl::TimePoint<>(0, 0, 0),{
        pl::TimePoint<>{-5, 0},
        pl::TimePoint<>{5, 0},
        pl::TimePoint<>{0, 8.66}
    });
    combiner.add(pl::TimePoint<>(1, 1, 1),{
        pl::TimePoint<>{0, 0},
        pl::TimePoint<>{5, 0},
        pl::TimePoint<>{0, 8.66}
    });
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(0.21133152046128473, 0.21133152046128473, 0.21133152046128473));
}

TEST(TIME_SUM_COMBINER, INHERITANCE) {
    pl::TimeSumCombiner combiner;
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1),std::vector<pl::TimePoint<>>({
        pl::TimePoint<>{0, 0, 1},
        pl::TimePoint<>{1, 1, 1},
        pl::TimePoint<>{0, 1, 1}
    }));
    EXPECT_TRUE(combiner.result().has_value());
    combiner.reset();
    EXPECT_FALSE(combiner.result().has_value());
    combiner.add(pl::TimePoint<>(1, 1, 1),std::vector<pl::TimePoint<>>({
        pl::TimePoint<>{0, 0, 1},
        pl::TimePoint<>{1, 1, 1},
        pl::TimePoint<>{0, 1, 1}
    }));
    EXPECT_TRUE(combiner.result().has_value());
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(1, 1, 1));
}

TEST(TIME_SUM_COMBINER, MULTY) {
    pl::TimeSumCombiner combiner;
    combiner.add(pl::TimePoint<>(0, 0, 0),std::vector<pl::TimePoint<>>({
          pl::TimePoint<>{-1, 0, 1},
          pl::TimePoint<>{0, 1, 1},
          pl::TimePoint<>{1, 0, 1}
    }));
    combiner.add(pl::TimePoint<>(1, 1, 1),std::vector<pl::TimePoint<>>({
          pl::TimePoint<>{-1, 0, 2},
          pl::TimePoint<>{0, 1, 1},
          pl::TimePoint<>{0, 0, 1}
    }));
    LOCALIZATION_RESULT_EQ(combiner.result().value(), pl::TimePoint<>(0.053312170974524814, 0.053312170974524814, 0.053312170974524814));
}

TEST(PROJECTIONS, BY_EQUIRECTANGULAR) {
    double radius = 10;
    pl::SpherePoint spherePoint = {1, 20};
    auto plainPoint = pl::projectByEquirectangular(spherePoint, radius, 0);
    EXPECT_FLOAT_EQ(3.4906585, plainPoint.x);
    EXPECT_FLOAT_EQ(0.17453292, plainPoint.y);
    auto resultSpherePoint = pl::fromEquirectangular(plainPoint, radius, 0);
    EXPECT_FLOAT_EQ(spherePoint.latitude, resultSpherePoint.latitude);
    EXPECT_FLOAT_EQ(spherePoint.longitude, resultSpherePoint.longitude);
}

TEST(PROJECTIONS, BY_SINUSOIDAL) {
    double radius = 10;
    pl::SpherePoint spherePoint = {20, 10};
    auto plainPoint = pl::projectBySinusoidal(spherePoint, radius);
    EXPECT_FLOAT_EQ(1.6400731, plainPoint.x);
    EXPECT_FLOAT_EQ(3.4906585, plainPoint.y);
    auto resultSpherePoint = pl::fromSinusoidal(plainPoint, radius);
    EXPECT_FLOAT_EQ(spherePoint.latitude, resultSpherePoint.latitude);
    EXPECT_FLOAT_EQ(spherePoint.longitude, resultSpherePoint.longitude);
}
