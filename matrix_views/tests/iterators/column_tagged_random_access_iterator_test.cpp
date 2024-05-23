#include <gtest/gtest.h>

#include "iterators/tagged_random_access_iterator.hpp"
#include "storage/const_callable_storage_proxy.hpp"

namespace tests {

using namespace matrix_views::iterators;
using namespace matrix_views::storage;
using namespace matrix_views::utils;

namespace {

auto kTag = kColumn;
auto kEchoIndex = index{};
auto kEchoStorageProxy = const_callable_storage_proxy(
    [](index index) -> index& { return kEchoIndex = index; });

using mock_tagged_random_access_iterator_t =
    tagged_random_access_iterator<decltype(kTag), decltype(kEchoStorageProxy)>;

}  // namespace

TEST(column_tagged_random_access_iterator, enforce_concept) {
  static_assert(
      std::random_access_iterator<mock_tagged_random_access_iterator_t>);
}

TEST(column_tagged_random_access_iterator, default_constructor) {
  mock_tagged_random_access_iterator_t();
};

TEST(column_tagged_random_access_iterator, constructor) {
  mock_tagged_random_access_iterator_t(kTag, kEchoIndex,
                                       std::move(kEchoStorageProxy));
};

TEST(column_tagged_random_access_iterator, iterator_traits) {
  static_assert(
      std::is_same_v<index,
                     std::iter_value_t<mock_tagged_random_access_iterator_t>>);
  static_assert(std::is_same_v<
                std::ptrdiff_t,
                std::iter_difference_t<mock_tagged_random_access_iterator_t>>);
  static_assert(std::is_same_v<
                const index&,
                std::iter_reference_t<mock_tagged_random_access_iterator_t>>);
}

TEST(column_tagged_random_access_iterator, dereference) {
  const auto it = tagged_random_access_iterator(kTag, {42, 43},
                                                std::move(kEchoStorageProxy));
  EXPECT_EQ(*it, (index{42, 43}));
  EXPECT_EQ(it->row, 42);
  EXPECT_EQ(it->column, 43);
  EXPECT_EQ(it[10], (index{52, 43}));
}

TEST(column_tagged_random_access_iterator, increment_decrement) {
  auto it =
      tagged_random_access_iterator(kTag, {0, 0}, std::move(kEchoStorageProxy));

  EXPECT_EQ(*++it, (index{1, 0}));
  EXPECT_EQ(*it, (index{1, 0}));

  EXPECT_EQ(*it++, (index{1, 0}));
  EXPECT_EQ(*it, (index{2, 0}));

  EXPECT_EQ(*--it, (index{1, 0}));
  EXPECT_EQ(*it, (index{1, 0}));

  EXPECT_EQ(*it--, (index{1, 0}));
  EXPECT_EQ(*it, (index{0, 0}));
}

TEST(column_tagged_random_access_iterator, comparison) {
  const auto it1 =
      tagged_random_access_iterator(kTag, {0, 0}, std::move(kEchoStorageProxy));
  const auto it2 =
      tagged_random_access_iterator(kTag, {5, 0}, std::move(kEchoStorageProxy));

  EXPECT_EQ(it1, it1);
  EXPECT_EQ(it2, it2);

  EXPECT_NE(it1, it2);
  EXPECT_NE(it2, it1);

  EXPECT_LT(it1, it2);
  EXPECT_GT(it2, it1);

  EXPECT_LE(it1, it1);
  EXPECT_LE(it1, it2);

  EXPECT_GE(it1, it1);
  EXPECT_GE(it2, it2);
  EXPECT_GE(it2, it2);
  EXPECT_GE(it2, it1);
}

TEST(column_tagged_random_access_iterator, addition_subtraction) {
  const auto it =
      tagged_random_access_iterator(kTag, {5, 5}, std::move(kEchoStorageProxy));

  EXPECT_EQ(*(it + 3), (index{8, 5}));
  EXPECT_EQ(*(it - 3), (index{2, 5}));
}

TEST(column_tagged_random_access_iterator, distance) {
  const auto it1 =
      tagged_random_access_iterator(kTag, {0, 0}, std::move(kEchoStorageProxy));
  const auto it2 =
      tagged_random_access_iterator(kTag, {5, 0}, std::move(kEchoStorageProxy));

  EXPECT_EQ(it2 - it1, 5);
  EXPECT_EQ(it1 - it2, -5);
}

TEST(column_tagged_random_access_iterator, inplace_addition) {
  auto it =
      tagged_random_access_iterator(kTag, {1, 1}, std::move(kEchoStorageProxy));

  EXPECT_EQ(*(it += 2), (index{3, 1}));
  EXPECT_EQ(*(it -= 2), (index{1, 1}));
}

TEST(column_tagged_random_access_iterator, add_to_difference_type) {
  const auto it =
      tagged_random_access_iterator(kTag, {1, 1}, std::move(kEchoStorageProxy));

  EXPECT_EQ(*(2 + it), (index{3, 1}));
}

}  // namespace tests
