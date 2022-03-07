#pragma once
#include "unit_test.hpp"
#include <core/core.hpp>

static void TestSparseSet()
{
    using namespace legion;

    LEGION_SUBTEST("Index Of")
    {
        sparse_set<size_type> sparseSet;
        sparseSet.insert(3);
        sparseSet.insert(7);
        sparseSet.insert(5);
        sparseSet.insert(9);

        auto opt3 = sparseSet.index_of(3);
        L_CHECK(opt3.has_value());
        L_CHECK(opt3.value() == 0);

        auto opt7 = sparseSet.index_of(7);
        L_CHECK(opt7.has_value());
        L_CHECK(opt7.value() == 1);

        auto opt5 = sparseSet.index_of(5);
        L_CHECK(opt5.has_value());
        L_CHECK(opt5.value() == 2);

        auto opt9 = sparseSet.index_of(9);
        L_CHECK(opt9.has_value());
        L_CHECK(opt9.value() == 3);

        L_CHECK(!sparseSet.index_of(1).has_value());
    }
}

LEGION_TEST("core::containers::sparse_set")
{
    Test(TestSparseSet);
}
