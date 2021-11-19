#include "select.hpp"

namespace test {
namespace container_traits {

void test_push() {
  using namespace query::container_traits;
  std::vector<std::string> v;                             any_push(v, "1");
  std::list<std::string> l;                               any_push(l, "1");
  std::deque<std::string> d;                              any_push(d, "1");
  std::queue<std::string> q;                              any_push(q, "1");
  std::set<std::string> s;                                any_push(s, "1");
  std::stack<std::string> st;                             any_push(st, "1");
  std::multiset<std::string> ms;                          any_push(ms, "1");
  std::map<std::string, std::string> m;                   any_push(m, "1", "2");
  std::multimap<std::string, std::string> mm;             any_push(mm, "1", "2");
  std::unordered_map<std::string, std::string> um;        any_push(um, "1", "2");
  std::unordered_multimap<std::string, std::string> umm;  any_push(um, "1", "2");
}

void test_clear() {
  using namespace query::container_traits;
  std::vector<std::string> v;                             any_clear(v);
  std::list<std::string> l;                               any_clear(l);
  std::deque<std::string> d;                              any_clear(d);
  std::queue<std::string> q;                              any_clear(q);
  std::set<std::string> s;                                any_clear(s);
  std::stack<std::string> st;                             any_clear(st);
  std::multiset<std::string> ms;                          any_clear(ms);
  std::map<std::string, std::string> m;                   any_clear(m);
  std::multimap<std::string, std::string> mm;             any_clear(mm);
  std::unordered_map<std::string, std::string> um;        any_clear(um);
  std::unordered_multimap<std::string, std::string> umm;  any_clear(um);
}

} // namespace container_traits

namespace convert {

void test_traits() {
  using namespace query::container_traits;
  static_assert( has_mapped_type<std::unordered_map<int, int>>::value);
  static_assert( has_mapped_type<std::map<int, int>>::value);
  static_assert( has_mapped_type<std::multimap<int, int>>::value);
  static_assert( has_mapped_type<std::unordered_multimap<int, int>>::value);
  static_assert(!has_mapped_type<std::set<int>>::value);
  static_assert(!has_mapped_type<std::vector<int>>::value);

  static_assert( has_iterator<std::unordered_map<int, int>>::value);
  static_assert( has_iterator<std::map<int, int>>::value);
  static_assert( has_iterator<std::multimap<int, int>>::value);
  static_assert( has_iterator<std::unordered_multimap<int, int>>::value);
  static_assert( has_iterator<std::set<int>>::value);
  static_assert( has_iterator<std::vector<int>>::value);
  static_assert(!has_iterator<int>::value);
  static_assert(!has_iterator<char>::value);
  static_assert(!has_iterator<std::pair<int, int>>::value);
}

template <typename Container>
void test_seq_to_seq_impl() {
  using namespace query::convert_detail;
  std::vector<int> v = { 1, 2, 3 };
  Container assert = { 1, 2, 3 };
  container_cast<decltype(v), Container> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

void test_seq_to_seq() {
  test_seq_to_seq_impl<std::vector<int>>();
  test_seq_to_seq_impl<std::deque<int>>();
  test_seq_to_seq_impl<std::forward_list<int>>();
  test_seq_to_seq_impl<std::list<int>>();
  test_seq_to_seq_impl<std::set<int>>();
  test_seq_to_seq_impl<std::multiset<int>>();
}

void test_seq_to_string() {
  using namespace query::convert_detail;
  std::deque<int> v = { 1, 2, 3 };
  std::string assert = "1 2 3";
  container_cast<decltype(v), std::string> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_string() {
  using namespace query::convert_detail;
  std::map<int, int> v = { {1, 1}, {2, 2}, {3, 3} };
  std::string assert = "(1, 1)(2, 2)(3, 3)";
  container_cast<decltype(v), std::string> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

template <typename Container>
void test_ass_to_ass_impl() {
  using namespace query::convert_detail;
  std::map<int, int> v = { {1, 1}, {2, 2}, {3, 3} };
  Container assert = { {1, 1}, {2, 2}, {3, 3} };
  container_cast<decltype(v), Container> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_ass() {
  test_ass_to_ass_impl<std::map<int, int>>();
  test_ass_to_ass_impl<std::unordered_map<int, int>>();
  test_ass_to_ass_impl<std::multimap<int, int>>();
  test_ass_to_ass_impl<std::unordered_multimap<int, int>>();
}

template <typename Associative ,typename Sequence>
void test_ordered_ass_to_seq_impl() {
  using namespace query::convert_detail;
  Associative v = { {1, 1}, {2, 2}, {3, 3} };
  Sequence assert = { 1, 1, 2, 2, 3, 3 };
  container_cast<Associative, Sequence> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

template <typename Associative ,typename Sequence>
void test_ordered_multi_ass_to_seq_impl() {
  using namespace query::convert_detail;
  Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  Sequence assert = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
  container_cast<Associative, Sequence> cast;
  Sequence cv = cast(v);
  assert(cv == assert);
}

template <typename Associative, typename Sequence>
void test_unordered_ass_to_seq_impl() {
  using namespace query::convert_detail;
  Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  Sequence assert = { 1, 1, 2, 2, 3, 3 };
  container_cast<Associative, Sequence> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

template <typename Associative, typename Sequence>
void test_unordered_multi_ass_to_seq_impl() {
  using namespace query::convert_detail;
  Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  Sequence assert = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
  container_cast<Associative, Sequence> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_seq() {
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::vector<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::deque<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::list<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::set<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::multiset<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::vector<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::deque<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::list<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::set<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::multiset<int>>();

  /// Easy test for sorted sets only
  test_unordered_ass_to_seq_impl<std::unordered_map<int, int>, std::set<int>>();
  test_unordered_ass_to_seq_impl<std::unordered_multimap<int, int>, std::set<int>>();
  test_unordered_ass_to_seq_impl<std::unordered_map<int, int>, std::multiset<int>>();
  test_unordered_multi_ass_to_seq_impl<std::unordered_multimap<int, int>, std::multiset<int>>();
}

void test_convert() {
  test_traits();
  test_seq_to_seq();
  test_seq_to_string();
  test_ass_to_string();
  test_ass_to_ass();
  test_ass_to_seq();
}

}// namespace convert

namespace merge {

template <typename Sequence1, typename Sequence2>
void test_merge_seq_impl() {
  using namespace query::merge_detail;
  Sequence1 seq_1 = { 1, 2, 3 };
  Sequence2 seq_2 = { 4, 5, 6 };
  Sequence1 assert = { 1, 2, 3, 4, 5, 6 };
  query::merge_detail::merge<Sequence1, Sequence2> merge_object;
  merge_object(seq_1, seq_2);
  assert(seq_1 == assert);
}

void test_merge_seq() {
  test_merge_seq_impl<std::vector<int>, std::vector<int>>();
  test_merge_seq_impl<std::vector<int>, std::deque<int>>();
  test_merge_seq_impl<std::vector<int>, std::list<int>>();
  test_merge_seq_impl<std::vector<int>, std::set<int>>();

  test_merge_seq_impl<std::deque<int>, std::vector<int>>();
  test_merge_seq_impl<std::deque<int>, std::deque<int>>();
  test_merge_seq_impl<std::deque<int>, std::list<int>>();
  test_merge_seq_impl<std::deque<int>, std::set<int>>();

  test_merge_seq_impl<std::list<int>, std::vector<int>>();
  test_merge_seq_impl<std::list<int>, std::deque<int>>();
  test_merge_seq_impl<std::list<int>, std::list<int>>();
  test_merge_seq_impl<std::list<int>, std::set<int>>();

  test_merge_seq_impl<std::set<int>, std::vector<int>>();
  test_merge_seq_impl<std::set<int>, std::deque<int>>();
  test_merge_seq_impl<std::set<int>, std::list<int>>();
  test_merge_seq_impl<std::set<int>, std::set<int>>();
}

template <typename Associative1, typename Associative2>
void test_merge_ass_impl() {
  using namespace query::merge_detail;
  Associative1 ass_1 = { {1, 1}, {2, 2}, {3, 3} };
  Associative2 ass_2 = { {4, 4}, {5, 5}, {6, 6} };
  Associative1 assert = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6} };
  query::merge_detail::merge<Associative1, Associative2> merge_object;
  merge_object(ass_1, ass_2);
  assert(ass_1 == assert);
}

void test_merge_ass() {
  test_merge_ass_impl<std::map<int, int>, std::map<int, int>>();
  test_merge_ass_impl<std::map<int, int>, std::unordered_map<int, int>>();
  test_merge_ass_impl<std::map<int, int>, std::multimap<int, int>>();
  test_merge_ass_impl<std::map<int, int>, std::unordered_multimap<int, int>>();

  test_merge_ass_impl<std::map<int, int>, std::map<int, int>>();
  test_merge_ass_impl<std::unordered_map<int, int>, std::unordered_map<int, int>>();
  test_merge_ass_impl<std::unordered_map<int, int>, std::multimap<int, int>>();
  test_merge_ass_impl<std::unordered_map<int, int>, std::unordered_multimap<int, int>>();

  test_merge_ass_impl<std::multimap<int, int>, std::map<int, int>>();
  test_merge_ass_impl<std::multimap<int, int>, std::unordered_map<int, int>>();
  test_merge_ass_impl<std::multimap<int, int>, std::multimap<int, int>>();
  test_merge_ass_impl<std::multimap<int, int>, std::unordered_multimap<int, int>>();

  test_merge_ass_impl<std::unordered_multimap<int, int>, std::map<int, int>>();
  test_merge_ass_impl<std::unordered_multimap<int, int>, std::unordered_map<int, int>>();
  test_merge_ass_impl<std::unordered_multimap<int, int>, std::multimap<int, int>>();
  test_merge_ass_impl<std::unordered_multimap<int, int>, std::unordered_multimap<int, int>>();
}

void test_merge() {
  test_merge_seq();
  test_merge_ass();
}

} // namespace merge

namespace set_operations {

//template <typename SetFunction, typename Container>
//void set_test_impl(SetFunction set_function, Container expected_range) {
//  using namespace query::set_operations;
//  Container elements_1 = { 1, 2, 3 };
//  Container elements_2 = {    2, 3, 4 };
//  Container result;
//  default_set(
//    elements_1.begin(), elements_1.end(),
//    elements_2.begin(), elements_2.end(),
//    std::back_inserter(result),
//    set_function);
//  assert(result == expected_range);
//}

void set_tests() {
//  std::vector<int> union_expected = { 1, 2, 3, 4 };
//  set_test_impl(std::set_union<
//    decltype(union_expected.begin()),
//    decltype(union_expected.end()),
//    decltype(std::back_insert_iterator(union_expected))>, union_expected);
//
//  std::vector<int> intersect_expected = { 2, 3 };
//  set_test_impl(std::set_intersection<
//    decltype(intersect_expected.begin()),
//    decltype(intersect_expected.end()),
//    decltype(std::back_insert_iterator(intersect_expected))>, intersect_expected);
//
//  std::vector<int> difference_expected = { 1 };
//  set_test_impl(std::set_difference<
//    decltype(difference_expected.begin()),
//    decltype(difference_expected.end()),
//    decltype(std::back_insert_iterator(difference_expected))>, difference_expected);
}

} // namespace set_operations

namespace conditional {

void test_cond() {
  using namespace query::conditional;
  constexpr gate _1(std::equal_to<>{}, 1, 0);
  static_assert(!_1);
  struct equal_comparator {
    constexpr bool operator()(int l, int r) const {
      return l == r;
    }
  };
  constexpr gate _2(equal_comparator{}, 1, 1);
  static_assert(_2);
  constexpr gate
    _3(std::equal_to<>{}, 1, 1),
    _4(std::equal_to<>{}, 2, ~0),
    _5(std::equal_to<>{}, 3, 3);
  static_assert(!(_3 && _4) || _5);
  static_assert(gate(std::equal_to<>{}, 1, 1));
  static_assert(
    (
         !gate(std::equal_to<>{}, 1, 1)
      || !gate(std::equal_to<>{}, 1, 0)
    ) && (
         !gate(std::less_equal<>{}, 1, 1)
      || !gate(std::less<>{}, 1, 1)
    )
  );
}

} // namespace conditional
} // namespace test

void container_select_tests() {
  test::container_traits::test_push();
  test::container_traits::test_clear();
  test::convert::test_convert();
  test::merge::test_merge();
  test::set_operations::set_tests();
  test::conditional::test_cond();
}

void access_by_field() {
  struct x { int key, value; };
  auto compare_by_field = [](auto x, auto field, auto value) {
    if (std::equal_to<>{}(x.*field, value)) {
      std::cout << "OK\n";
    } else {
      std::cout << "X\n";
    }
  };
  x value { 1, 2 };
  compare_by_field(value, &x::key, 1);
  compare_by_field(value, &x::value, 1);
}

int main() {
  container_select_tests();
}
