#include "query.hpp"

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

//namespace merge {
//
//template <typename Sequence1, typename Sequence2>
//void test_merge_seq_impl() {
//  using namespace query::merge_detail;
//  Sequence1 seq_1 = { 1, 2, 3 };
//  Sequence2 seq_2 = { 4, 5, 6 };
//  Sequence1 assert = { 1, 2, 3, 4, 5, 6 };
//  query::merge_detail::merge<Sequence1, Sequence2> merge_object;
//  merge_object(seq_1, seq_2);
//  assert(seq_1 == assert);
//}
//
//void test_merge_seq() {
//  test_merge_seq_impl<std::vector<int>, std::vector<int>>();
//  test_merge_seq_impl<std::vector<int>, std::deque<int>>();
//  test_merge_seq_impl<std::vector<int>, std::list<int>>();
//  test_merge_seq_impl<std::vector<int>, std::set<int>>();
//
//  test_merge_seq_impl<std::deque<int>, std::vector<int>>();
//  test_merge_seq_impl<std::deque<int>, std::deque<int>>();
//  test_merge_seq_impl<std::deque<int>, std::list<int>>();
//  test_merge_seq_impl<std::deque<int>, std::set<int>>();
//
//  test_merge_seq_impl<std::list<int>, std::vector<int>>();
//  test_merge_seq_impl<std::list<int>, std::deque<int>>();
//  test_merge_seq_impl<std::list<int>, std::list<int>>();
//  test_merge_seq_impl<std::list<int>, std::set<int>>();
//
//  test_merge_seq_impl<std::set<int>, std::vector<int>>();
//  test_merge_seq_impl<std::set<int>, std::deque<int>>();
//  test_merge_seq_impl<std::set<int>, std::list<int>>();
//  test_merge_seq_impl<std::set<int>, std::set<int>>();
//}
//
//template <typename Associative1, typename Associative2>
//void test_merge_ass_impl() {
//  using namespace query::merge_detail;
//  Associative1 ass_1 = { {1, 1}, {2, 2}, {3, 3} };
//  Associative2 ass_2 = { {4, 4}, {5, 5}, {6, 6} };
//  Associative1 assert = { {1, 1}, {2, 2}, {3, 3}, {4, 4}, {5, 5}, {6, 6} };
//  query::merge_detail::merge<Associative1, Associative2> merge_object;
//  merge_object(ass_1, ass_2);
//  assert(ass_1 == assert);
//}
//
//void test_merge_ass() {
//  test_merge_ass_impl<std::map<int, int>, std::map<int, int>>();
//  test_merge_ass_impl<std::map<int, int>, std::unordered_map<int, int>>();
//  test_merge_ass_impl<std::map<int, int>, std::multimap<int, int>>();
//  test_merge_ass_impl<std::map<int, int>, std::unordered_multimap<int, int>>();
//
//  test_merge_ass_impl<std::map<int, int>, std::map<int, int>>();
//  test_merge_ass_impl<std::unordered_map<int, int>, std::unordered_map<int, int>>();
//  test_merge_ass_impl<std::unordered_map<int, int>, std::multimap<int, int>>();
//  test_merge_ass_impl<std::unordered_map<int, int>, std::unordered_multimap<int, int>>();
//
//  test_merge_ass_impl<std::multimap<int, int>, std::map<int, int>>();
//  test_merge_ass_impl<std::multimap<int, int>, std::unordered_map<int, int>>();
//  test_merge_ass_impl<std::multimap<int, int>, std::multimap<int, int>>();
//  test_merge_ass_impl<std::multimap<int, int>, std::unordered_multimap<int, int>>();
//
//  test_merge_ass_impl<std::unordered_multimap<int, int>, std::map<int, int>>();
//  test_merge_ass_impl<std::unordered_multimap<int, int>, std::unordered_map<int, int>>();
//  test_merge_ass_impl<std::unordered_multimap<int, int>, std::multimap<int, int>>();
//  test_merge_ass_impl<std::unordered_multimap<int, int>, std::unordered_multimap<int, int>>();
//}
//
//void test_merge() {
//  test_merge_seq();
//  test_merge_ass();
//}
//
//} // namespace merge

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
  constexpr gate _6(std::equal_to<>{}, 2);
  static_assert( _6.compare_with(2));
  static_assert(!_6.compare_with(1));
}

} // namespace conditional

namespace from {

template <typename From, typename To>
void from_tests_seq_cast() {
  From container = { 1, 2, 3 };
    To    assert = { 1, 2, 3 };
    To converted = query::from<From>(container).to(To{});
  assert(converted == assert);
}

template <typename From, typename To>
void from_tests_ass_cast() {
  From container = { {1,1}, {2,2}, {3,3} };
    To    assert = { {1,1}, {2,2}, {3,3} };
    To converted = query::from<From>(container).to(To{});
  assert(converted == assert);
}

void from_tests() {
  from_tests_seq_cast<std::vector<int>, std::vector<int>>();
  from_tests_seq_cast<std::vector<int>, std::deque<int>>();
  from_tests_seq_cast<std::vector<int>, std::list<int>>();
  from_tests_seq_cast<std::vector<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::vector<int>, std::set<int>>();
  from_tests_seq_cast<std::vector<int>, std::multiset<int>>();

  from_tests_seq_cast<std::deque<int>, std::vector<int>>();
  from_tests_seq_cast<std::deque<int>, std::deque<int>>();
  from_tests_seq_cast<std::deque<int>, std::list<int>>();
  from_tests_seq_cast<std::deque<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::deque<int>, std::set<int>>();
  from_tests_seq_cast<std::deque<int>, std::multiset<int>>();

  from_tests_seq_cast<std::list<int>, std::vector<int>>();
  from_tests_seq_cast<std::list<int>, std::deque<int>>();
  from_tests_seq_cast<std::list<int>, std::list<int>>();
  from_tests_seq_cast<std::list<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::list<int>, std::set<int>>();
  from_tests_seq_cast<std::list<int>, std::multiset<int>>();

  from_tests_seq_cast<std::forward_list<int>, std::vector<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::deque<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::list<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::set<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::multiset<int>>();

  from_tests_seq_cast<std::set<int>, std::vector<int>>();
  from_tests_seq_cast<std::set<int>, std::deque<int>>();
  from_tests_seq_cast<std::set<int>, std::list<int>>();
  from_tests_seq_cast<std::set<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::set<int>, std::set<int>>();
  from_tests_seq_cast<std::set<int>, std::multiset<int>>();

  from_tests_seq_cast<std::multiset<int>, std::vector<int>>();
  from_tests_seq_cast<std::multiset<int>, std::deque<int>>();
  from_tests_seq_cast<std::multiset<int>, std::list<int>>();
  from_tests_seq_cast<std::multiset<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::multiset<int>, std::set<int>>();
  from_tests_seq_cast<std::multiset<int>, std::multiset<int>>();

  from_tests_ass_cast<std::map<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::multimap<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::unordered_map<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::unordered_multimap<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::unordered_multimap<int, int>>();
}

} // namespace from

namespace merge {

template <typename Container, typename MergeWith>
void from_tests_merge_seq() {
  Container to_merge_1 = { 1, 2, 3 };
  MergeWith to_merge_2 = { 4, 5, 6 };
  Container     assert = { 1, 2, 3, 4, 5, 6 };
  Container     merged = query::from<Container>(to_merge_1).merge(to_merge_2).to(Container{});
  assert(merged == assert);
}

template <typename Container>
void from_tests_merge_by_il() {
  Container to_merge_1 = { 1, 2, 3 };
  Container     assert = { 1, 2, 3, 4, 5, 6 };
  Container     merged = query::from<Container>(to_merge_1).merge({ 4, 5, 6 }).to(Container{});
  assert(merged == assert);
}

void merge_tests() {
  from_tests_merge_seq<std::vector<int>, std::vector<int>>();
  from_tests_merge_seq<std::vector<int>, std::deque<int>>();
  from_tests_merge_seq<std::vector<int>, std::list<int>>();
  from_tests_merge_seq<std::vector<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::vector<int>, std::set<int>>();
  from_tests_merge_seq<std::vector<int>, std::multiset<int>>();

  from_tests_merge_seq<std::deque<int>, std::vector<int>>();
  from_tests_merge_seq<std::deque<int>, std::deque<int>>();
  from_tests_merge_seq<std::deque<int>, std::list<int>>();
  from_tests_merge_seq<std::deque<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::deque<int>, std::set<int>>();
  from_tests_merge_seq<std::deque<int>, std::multiset<int>>();

  from_tests_merge_seq<std::list<int>, std::vector<int>>();
  from_tests_merge_seq<std::list<int>, std::deque<int>>();
  from_tests_merge_seq<std::list<int>, std::list<int>>();
  from_tests_merge_seq<std::list<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::list<int>, std::set<int>>();
  from_tests_merge_seq<std::list<int>, std::multiset<int>>();

  from_tests_merge_seq<std::set<int>, std::vector<int>>();
  from_tests_merge_seq<std::set<int>, std::deque<int>>();
  from_tests_merge_seq<std::set<int>, std::list<int>>();
  from_tests_merge_seq<std::set<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::set<int>, std::set<int>>();
  from_tests_merge_seq<std::set<int>, std::multiset<int>>();

  from_tests_merge_seq<std::multiset<int>, std::vector<int>>();
  from_tests_merge_seq<std::multiset<int>, std::deque<int>>();
  from_tests_merge_seq<std::multiset<int>, std::list<int>>();
  from_tests_merge_seq<std::multiset<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::multiset<int>, std::set<int>>();
  from_tests_merge_seq<std::multiset<int>, std::multiset<int>>();

  from_tests_merge_by_il<std::vector<int>>();
  from_tests_merge_by_il<std::deque<int>>();
  from_tests_merge_by_il<std::list<int>>();
  from_tests_merge_by_il<std::set<int>>();
  from_tests_merge_by_il<std::multiset<int>>();
}

} // namespace merge

namespace where {

template <typename Container>
void where_lambda_test_seq_impl() {
  Container values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  Container assert = {    2,    4,    6,    8    };
  Container select =
    query::from<Container>(values).where(
      [](const auto& element) { return element % 2 == 0; }
    ).to(Container{});
  assert(select == assert);
}

template <typename Container>
void where_gate_test_seq_impl() {
  Container values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  Container assert = {       3, 4, 5, 6, 7, 8, 9 };
  Container select =
    query::from<Container>(values).where(query::conditional::gate(std::greater_equal<>{}, 3)).to(Container{});
  assert(select == assert);
}

void where_tests() {
  where_lambda_test_seq_impl<std::vector<int>>();
  where_lambda_test_seq_impl<std::deque<int>>();
  where_lambda_test_seq_impl<std::list<int>>();
  where_lambda_test_seq_impl<std::set<int>>();
  where_lambda_test_seq_impl<std::multiset<int>>();

  where_gate_test_seq_impl<std::vector<int>>();
  where_gate_test_seq_impl<std::deque<int>>();
  where_gate_test_seq_impl<std::list<int>>();
  where_gate_test_seq_impl<std::set<int>>();
  where_gate_test_seq_impl<std::multiset<int>>();
}

} // namespace where

void complex_test() {
  using query::conditional::gate;
  {
    std::vector<int> values_1 = { 9,  7,  5,  3,  1};
    std::vector<int> values_2 = { 2,  4,  6,  8, 10};
    std::set<int> result =
      query::from<std::vector<int>>(values_1)
        .merge(values_2)
        .where(gate(std::greater_equal<>{}, 5))
        .merge({11, 12, 13, 14, 15, 16})
        .to(std::set<int>{});
    std::set<int> assert = {
       5,   6,   7,   8,
       9,  10,  11,  12,
      13,  14,  15,  16
    };
    assert(result == assert);
  } {
    /// ...
  }
}

} // namespace test

int main() {
  test::container_traits::test_push();
  test::container_traits::test_clear();
  test::convert::test_convert();
  test::conditional::test_cond();
  test::from::from_tests();
  test::merge::merge_tests();
  test::where::where_tests();
  test::complex_test();
}
