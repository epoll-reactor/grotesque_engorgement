#ifndef LINQ_HPP
#define LINQ_HPP

/*  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |            Operation |                 Description |                                                 Example |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                where |   Filters sequence based on | std::vector<int> elements = { 1, 2, 3 };                |
 *  |                      |                   predicate | auto range = from(elements)                             |
 *  |                      |                             |             || .where([](auto&& e) { return e > 1; })   |
 *  |                      |                             |             || .where(gate(std::greater<>{}, 1))        |
 *  |                      |                             |                    && gate(std::less<>{}   , 3))        |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 2, 3 };                                    |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                 take |   Creates new sequence with | std::vector<int> elements = { 1, 1, 1, 1, 2, 3 };       |
 *  |                      |         given max number of | auto range = from(elements)                             |
 *  |                      |                    elements |                .where([](auto&& e) { return e == 1; })  |
 *  |                      |                             |                .take(2)                                 |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 1, 1 };                                    |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                merge |        Merge two containers | std::vector<int> elements = { 1, 2, 3 };                |
 *  |                      |     sequence ->    sequence | auto range = from(elements)                             |
 *  |                      |  associative -> associative |                .merge({1, 2, 3})                        |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 1, 2, 3, 1, 2, 3 };                        |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |   order_by_ascending |             Ascending order | std::vector<int> elements = { 3, 2, 1 };                |
 *  |                      |                             | auto range = from(elements)                             |
 *  |                      |                             |                .order_by_ascending()                    |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 1, 2, 3 };                                 |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |  order_by_descending |            Descending order | std::vector<int> elements = { 1, 2, 3 };                |
 *  |                      |                             | auto range = from(elements)                             |
 *  |                      |                             |                .order_by_descending()                   |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 3, 2, 1 };                                 |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |              reverse |            Reverse elements | std::vector<int> elements = { 5, 1, 3 };                |
 *  |                      |                             | auto range = from(elements)                             |
 *  |                      |                             |                .reverse()                               |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 3, 1, 5 };                                 |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                union |    Produce union with given | std::vector<int> part_1 = { 1, 2, 3, 4, 5 };            |
 *  |                      |                       range | std::vector<int> part_2 = {       3, 4, 5, 6, 7 };      |
 *  |                      |                             | auto range = from(part_1)                               |
 *  |                      |                             |                .union(part_2)                           |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 1, 2, 3, 4, 5, 6, 7 };                     |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |            intersect |      Produce intersect with | std::vector<int> part_1 = { 1, 2, 3, 4, 5 };            |
 *  |                      |                 given range | std::vector<int> part_2 = {    2,       5 };            |
 *  |                      |                             | auto range = from(part_1)                               |
 *  |                      |                             |                .intersect(part_2)                       |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 2, 5 };                                    |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |           difference |  Produce difference between | std::vector<int> part_1 = { 1, 2, 5, 5, 5,    9 };      |
 *  |                      |      source and given range | std::vector<int> part_2 = {    2, 5,       7    };      |
 *  |                      |                             | auto range = from(part_1)                               |
 *  |                      |                             |                .difference(part_2)                      |
 *  |                      |                             |                .to<std::vector<int>>();                 |
 *  |                      |                             | // range = { 1, 5, 5, 9 };                              |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                   to |    Create given as template | std::( vector                                           |
 *  |                      |                   container |      | list                                             |
 *  |                      |              -------------- |      | deque                                            |
 *  |                      |     sequence ->    sequence |      | set                                              |
 *  |                      |     sequence ->      string |      | multiset )<int> values = { 1, 2 };               |
 *  |                      |  associative -> associative | auto seq_transformed = from(values)                     |
 *  |                      |  associative ->    sequence |                         .to<std::( vector               |
 *  |                      |  associative ->      string |                                  | list                 |
 *  |                      |                             |                                  | deque                |
 *  |                      |                             |                                  | set                  |
 *  |                      |                             |                                  | multiset )<int>>();  |
 *  |                      |                             |                                                         |
 *  |                      |                             | std::( vector                                           |
 *  |                      |                             |      | list                                             |
 *  |                      |                             |      | deque                                            |
 *  |                      |                             |      | set                                              |
 *  |                      |                             |      | multiset )<int> values = { 1, 2 };               |
 *  |                      |                             | auto string = from(values).to<std::string>();           |
 *  |                      |                             |                                                         |
 *  |                      |                             | std::( map                                              |
 *  |                      |                             |      | multimap                                         |
 *  |                      |                             |      | unordered_map                                    |
 *  |                      |                             |      | unordered_multimap )<int> values = { {1,2} };    |
 *  |                      |                             | auto ass_to_ass = from(values)                          |
 *  |                      |                             |                     .to<std::( map                      |
 *  |                      |                             |                              | multimap                 |
 *  |                      |                             |                              | unordered_map            |
 *  |                      |                             |                              | unordered_multimap )>;   |
 *  |                      |                             |                                                         |
 *  |                      |                             | std::( map                                              |
 *  |                      |                             |      | multimap                                         |
 *  |                      |                             |      | unordered_map                                    |
 *  |                      |                             |      | unordered_multimap )<int> values = { {1,2} };    |
 *  |                      |                             | auto ass_to_seq = from(values)                          |
 *  |                      |                             |                     .to<std::( vector                   |
 *  |                      |                             |                              | list                     |
 *  |                      |                             |                              | deque                    |
 *  |                      |                             |                              | set                      |
 *  |                      |                             |                              | multiset )<int>>();      |
 *  |                      |                             | std::( map                                              |
 *  |                      |                             |      | multimap                                         |
 *  |                      |                             |      | unordered_map                                    |
 *  |                      |                             |      | unordered_multimap )<int> values = { {1,2} };    |
 *  |                      |                             | auto ass_to_string = from(values)                       |
 *  |                      |                             |                        .to<std::string>();              |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *  |                  sum | Produce sum of all elements | std::vector<int> to_sum = { 1, 2, 3, 4 };               |
 *  |                      |                             | int sum = from(to_sum)                                  |
 *  |                      |                             |             .where([](auto i) { return i % 2 == 0; })   |
 *  |                      |                             |             .sum() | min() | max();                     |
 *  +----------------------+-----------------------------+                                                         |
 *  |                  min |   Find minimum in container | // sum == 10                                            |
 *  +----------------------+-----------------------------+ // min == 1                                             |
 *  |                  max |   Find maximum in container | // max == 4                                             |
 *  +----------------------+-----------------------------+---------------------------------------------------------+
 *
 *  Complex example: select maximum 3 persons older than 50 years.
 *
 *  Naive:
 *    struct person { size_t age, salary; };
 *    std::vector<person> persons = { {36, 1900}, {57, 2200}, {51, 3900} };
 *    std::vector<person> rest = { {39, 2100} };
 *    std::vector<person> out;
 *    for (const auto&[age, salary] : persons) {
 *      if (age > 50 && salary > 2000) {
 *        out.push_back({age, salary});
 *      }
 *    }
 *    std::vector<person> union;
 *    std::set_union(
 *      out .begin(), out .end(),
 *      rest.begin(), rest.end(),
 *      std::back_inserter(union));
 *
 *  Using wrapper:
 *    struct person { size_t age, salary; };
 *    std::vector<person> persons = { {36, 1900}, {57, 2200}, {51, 3900} };
 *    std::vector<person> rest = { {39, 2100} };
 *    std::vector<person> out = from(persons).where(
 *      cond(&person::age,    std::greater<>{},   50) &&
 *      cond(&person::salary, std::greater<>{}, 2000)  )
 *      .union(rest).to<std::vector<person>>();
 */

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_map>
#include <cassert>

namespace query {
namespace container_traits {

template<typename T>
struct has_iterator final {
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_iterator(typename C::iterator*);
  template<typename C> static constexpr no  test_for_iterator(...);

  static constexpr bool value = sizeof(test_for_iterator<T>(nullptr)) == sizeof(yes);
};

template <typename T>
struct has_mapped_type final {
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_mapped_type(typename C::mapped_type*);
  template<typename C> static constexpr no  test_for_mapped_type(...);

  static constexpr bool value = sizeof(test_for_mapped_type<T>(nullptr)) == sizeof(yes);
};

template <typename T>
struct has_traits_type final {
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_traits_type(typename C::traits_type*);
  template<typename C> static constexpr no  test_for_traits_type(...);

  static constexpr bool value = sizeof(test_for_traits_type<T>(nullptr)) == sizeof(yes);
};

template <typename T>
struct is_sequence_container final : std::integral_constant<bool, has_iterator<T>::value && !has_mapped_type<T>::value> {};
template <typename T>
struct is_associative_container final : std::integral_constant<bool, has_iterator<T>::value && has_mapped_type<T>::value> {};
template <typename T>
struct is_basic_string final : std::integral_constant<bool, has_iterator<T>::value && has_traits_type<T>::value> {};

template <typename... Args> struct need_emplace_param final : std::false_type {};
template <typename... Args> struct need_emplace_param<std::vector<Args...>> final : std::true_type {};
template <typename... Args> struct need_emplace_param<std::list<Args...>> final : std::true_type {};
template <typename... Args> struct need_emplace_param<std::deque<Args...>> final : std::true_type {};

template <typename... Args> struct has_not_clear_method final : std::false_type {};
template <typename... Args> struct has_not_clear_method<std::stack<Args...>> final : std::true_type {};
template <typename... Args> struct has_not_clear_method<std::queue<Args...>> final : std::true_type {};

template <typename... Args> struct need_push_back final : std::false_type {};
template <typename... Args> struct need_push_back<std::basic_string<Args...>> final : std::true_type {};

template <typename Container, typename... Args>
void any_push(Container& container, Args&&... values) {
  if constexpr (need_emplace_param<std::decay_t<Container>>::value){
    container.emplace(container.end(), std::forward<Args>(values)...);
  } else if constexpr (need_push_back<std::decay_t<Container>>::value) {
    container.push_back(std::forward<Args>(values)...);
  } else {
    container.emplace(std::forward<Args>(values)...);
  }
}

template <typename Container>
void any_clear(Container& container) noexcept {
  if constexpr (!has_not_clear_method<std::decay_t<Container>>::value) {
    container.clear();
  }
}

}// namespace container_traits
}// namespace query

namespace query {
namespace convert_detail {

template <typename From, typename To>
class container_cast final {
public:
  using from_type = From;
  using   to_type = To;

  static_assert(
    container_traits::is_sequence_container   <from_type>::value ||
    container_traits::is_associative_container<from_type>::value  , "Source type is not container");
  static_assert(
    container_traits::is_sequence_container   <to_type>::value   ||
    container_traits::is_associative_container<to_type>::value   ||
    container_traits::is_basic_string         <to_type>::value    , "Target  type is not container or string");

  To operator()(const From& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_sequence_container<  to_type>::value &&
     !container_traits::is_basic_string      <  to_type>::value
    ) || (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_associative_container<  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return same_to_same(from, To{}); }

  To operator()(const From& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_basic_string      <  to_type>::value )
  { return seq_to_string(from); }

  To operator()(const From& from)
    requires (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_sequence_container   <  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return ass_to_seq(from, To{}); }

  To operator()(const From& from)
    requires (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_basic_string         <  to_type>::value )
  { return ass_to_string(from); }

private:
  template <typename T>
  auto to_string(const T& element) {
    std::string output;
    if constexpr (std::is_integral_v<std::decay_t<T>>) {
      output += std::to_string(element);
    } else {
      output += element;
    }
    return output;
  }

  To same_to_same(const From& sequence, To result) {
    return (result = {std::begin(sequence), std::end(sequence)});
  }

  auto seq_to_string(const From& sequence) {
    std::string result;
    for (auto&& element : sequence) {
      result += to_string(element) += ' ';
    }
    if (!result.empty()) {
      result.pop_back();
    }
    return result;
  }

  To ass_to_seq(const From& associative, To result) {
    for (auto&&[k, v] : associative) {
      container_traits::any_push(result, k);
      container_traits::any_push(result, v);
    }
    return result;
  }

  auto ass_to_string(const From& associative) {
    std::string result;
    for (auto&&[k, v] : associative) {
      result += '(';
      result += to_string(k);
      result += ", ";
      result += to_string(v);
      result += ')';
    }
    return result;
  }
};

} // namespace convert_detail
} // namespace query

namespace query {
namespace merge_detail {

template <typename Target, typename ToMerge>
class merge final {
public:
  using   target_type = Target;
  using to_merge_type = ToMerge;

  static_assert(
  	(
  	  container_traits::is_sequence_container<  target_type>::value &&
  	  container_traits::is_sequence_container<to_merge_type>::value
  	) || (
  	  container_traits::is_associative_container<  target_type>::value &&
 	  container_traits::is_associative_container<to_merge_type>::value
    )
    , "Both sequence or associative containeres expected");

  void operator()(target_type& target, const to_merge_type& to_merge)
    requires (
      container_traits::is_sequence_container<  target_type>::value &&
      container_traits::is_sequence_container<to_merge_type>::value )
  { merge_sequence(target, to_merge); }

  void operator()(target_type& target, const to_merge_type& to_merge)
    requires (
      container_traits::is_associative_container<  target_type>::value &&
      container_traits::is_associative_container<to_merge_type>::value )
  { merge_associative(target, to_merge); }

private:
  void merge_sequence(target_type& target, const to_merge_type& to_merge) {
  	for (const auto& value : to_merge) {
  		container_traits::any_push(target, value);
  	}
  }

  void merge_associative(target_type& target, const to_merge_type& to_merge) {
  	for (const auto&[key, value] : to_merge) {
  		container_traits::any_push(target, key, value);
  	}
  }
};

} // namespace merge_detail
} // namespace query

namespace query {
namespace conditional {

template <typename Comparator, typename T>
class gate final {
public:
  using      value_type = T;
  using comparator_type = Comparator;

  static_assert(std::is_move_constructible_v<value_type>, "T requires to be move constructible");

  constexpr gate(Comparator comparator, T left, T right) noexcept(
    std::is_nothrow_constructible_v<std::decay_t<Comparator>>
  )
    : comparator_   (std::forward<Comparator>(comparator))
    , left_         (std::move(left))
    , right_        (std::move(right))
    , logical_value_(comparator_(left_, right_)) {}

  constexpr explicit operator bool() const noexcept(noexcept(Comparator()(std::declval<T>(), std::declval<T>()))) {
    return logical_value_;
  }

  constexpr bool operator &&(const gate& other) const noexcept(noexcept(Comparator()(std::declval<T>(), std::declval<T>()))) {
    return logical_value_ && other.logical_value_;
  }

  constexpr bool operator ||(const gate& other) const noexcept(noexcept(Comparator()(std::declval<T>(), std::declval<T>()))) {
    return logical_value_ || other.logical_value_;
  }

  const Comparator comparator_;
  const T left_;
  const T right_;
  const bool logical_value_;
};

} //namespace conditional
} // namespace query

#endif // LINQ_HPP
