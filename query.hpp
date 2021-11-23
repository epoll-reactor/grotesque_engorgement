#ifndef QUERY_HPP
#define QUERY_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_map>
#include <optional>
#include <cassert>
#include <functional>

namespace query {
namespace container_traits {

template<typename T>
class has_iterator final {
private:
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_iterator(typename C::iterator*);
  template<typename C> static constexpr no  test_for_iterator(...);

public:
  static constexpr bool value = sizeof(test_for_iterator<T>(nullptr)) == sizeof(yes);
};

template <typename T>
class has_mapped_type final {
private:
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_mapped_type(typename C::mapped_type*);
  template<typename C> static constexpr no  test_for_mapped_type(...);

public:
  static constexpr bool value = sizeof(test_for_mapped_type<T>(nullptr)) == sizeof(yes);
};

template <typename T>
class has_traits_type final {
private:
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test_for_traits_type(typename C::traits_type*);
  template<typename C> static constexpr no  test_for_traits_type(...);

public:
  static constexpr bool value = sizeof(test_for_traits_type<T>(nullptr)) == sizeof(yes);
};

template <typename C>
class has_three_way_comparator final {
public:
  template <typename T> static constexpr auto check(T*) -> typename
    std::is_same<
      decltype(std::operator<=>(std::declval<T>(), std::declval<T>())),
      std::strong_ordering
  >::type;
  template <typename> static constexpr std::false_type check(...);

  using type = decltype(check<C>(nullptr));

  static constexpr bool value = type::value;
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
    container_traits::is_associative_container<from_type>::value  ,
    "Source type is not container");
  static_assert(
    container_traits::is_sequence_container   <to_type>::value   ||
    container_traits::is_associative_container<to_type>::value   ||
    container_traits::is_basic_string         <to_type>::value    ,
    "Target type is neither container nor string");

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_sequence_container<  to_type>::value &&
     !container_traits::is_basic_string      <  to_type>::value
    ) || (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_associative_container<  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return same_to_same(from); }

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_basic_string      <  to_type>::value )
  { return seq_to_string(from); }

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_sequence_container   <  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return ass_to_seq(from); }

  to_type operator()(const from_type& from)
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

  to_type same_to_same(const from_type& sequence) {
    return {std::begin(sequence), std::end(sequence)};
  }

  auto seq_to_string(const from_type& sequence) {
    std::string result;
    for (auto&& element : sequence) {
      result += to_string(element) += ' ';
    }
    if (!result.empty()) {
      result.pop_back();
    }
    return result;
  }

  to_type ass_to_seq(const from_type& associative) {
    to_type result;
    for (auto&&[k, v] : associative) {
      container_traits::any_push(result, k);
      container_traits::any_push(result, v);
    }
    return result;
  }

  auto ass_to_string(const from_type& associative) {
    std::string result;
    for (const auto&[k, v] : associative) {
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
    ),
    "Both sequence or associative containers expected"
  );

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

} // namespace query

namespace query {

template <typename Comparator, typename T>
class gate final {
public:
  using      value_type = T;
  using comparator_type = Comparator;

  static_assert(std::is_move_constructible_v<value_type>, "T requires to be move constructible");

  constexpr gate(comparator_type comparator, value_type left, value_type right) noexcept(std::is_nothrow_constructible_v<comparator_type>)
    : comparator_   (std::forward<Comparator>(comparator))
    , left_         (std::move(left))
    , right_        (std::move(right))
    , logical_value_(comparator_(left_, right_)) {}

  constexpr gate(comparator_type comparator, value_type value) noexcept(std::is_nothrow_constructible_v<comparator_type>)
    : comparator_   (std::forward<comparator_type>(comparator))
    , left_         (std::move(value))
    , right_        ()
    , logical_value_(false) {}

  constexpr explicit operator bool() const noexcept(is_noexcept_comparator_) {
    return logical_value_;
  }

  constexpr bool operator &&(const gate& other) const noexcept(is_noexcept_comparator_) {
    return logical_value_ && other.logical_value_;
  }

  constexpr bool operator ||(const gate& other) const noexcept(is_noexcept_comparator_) {
    return logical_value_ || other.logical_value_;
  }

  constexpr bool compare_with(const T& value) const noexcept(is_noexcept_comparator_) {
    return comparator_(value, left_);
  }

private:
  static constexpr bool is_noexcept_comparator_ = noexcept(comparator_type()(std::declval<T>(), std::declval<T>()));
  const comparator_type comparator_;
  const T               left_;
  const T               right_;
  bool                  logical_value_;
};

} // namespace query

namespace query {
// This hell actually needs to be simplified...
template <typename Buffer>
class where final {
public:
  using buffer_type = Buffer;

  static_assert(
    container_traits::is_sequence_container   <buffer_type>::value ||
    container_traits::is_associative_container<buffer_type>::value  ,
    "Both sequence or associative containers expected");

  enum struct select_policy { by_key, by_value, none };

  where(buffer_type& buffer, ssize_t to_take) : buffer_(buffer), to_take_(to_take) {}

  template <typename Gate>
  void by_gate(const Gate& logical_gate) {
    sequence_impl([&](const auto& element) { return logical_gate.compare_with(element); });
  }

  template <typename Field, typename Gate>
  void by_gate(Field field, const Gate& logical_gate) {
    sequence_impl([&](const auto& element) { return logical_gate.compare_with(element.*field); });
  }

  template <typename Lambda>
  void by_lambda(Lambda lambda) {
    sequence_impl([&](const auto& element) { return lambda(element); });
  }

  template <typename Field, typename Lambda>
  void by_lambda(Field field, Lambda lambda) {
    sequence_impl([&](const auto& element) { return lambda(element.*field); });
  }

  template <typename Gate>
  void by_gate(enum select_policy policy, const Gate& logical_gate) {
    associative_impl([&](const auto& key, const auto& value) {
      return logical_gate.compare_with((policy == select_policy::by_key) ? key : value);
    });
  }

  template <typename Field, typename Gate>
  void by_gate(enum select_policy policy, Field field, const Gate& logical_gate) {
    associative_impl([&](const auto& key, const auto& value) {
      return logical_gate.compare_with((policy == select_policy::by_key) ? key.*field : value.*field);
    });
  }

  template <typename Lambda>
  void by_lambda(enum select_policy policy, Lambda lambda) {
    associative_impl([&](const auto& key, const auto& value) {
      return (policy == select_policy::by_key) ? lambda(key) : lambda(value);
    });
  }

  template <typename Field, typename Lambda>
  void by_lambda(enum select_policy policy, Field field, Lambda lambda) {
    associative_impl([&](const auto& key, const auto& value) {
      return (policy == select_policy::by_key) ? lambda(key.*field) : lambda(value.*field);
    });
  }

private:
  template <typename Comparator>
  void sequence_impl(Comparator comparator) {
    Buffer new_buffer;
    ssize_t total_found = 0;
    for (const auto& element : buffer_) {
      if (total_found == to_take_) {
        break;
      }
      if (comparator(element)) {
        container_traits::any_push(new_buffer, element);
        ++total_found;
      }
    }
    buffer_ = std::move(new_buffer);
  }

  template <typename Comparator>
  void associative_impl(Comparator comparator) {
    Buffer new_buffer;
    ssize_t total_found = 0;
    for (const auto&[key, value] : buffer_) {
      if (total_found == to_take_) {
        break;
      }
      if (comparator(key, value)) {
        container_traits::any_push(new_buffer, key, value);
        ++total_found;
      }
    }
    buffer_ = std::move(new_buffer);
  }

  buffer_type& buffer_;
  ssize_t to_take_;
};

} // namespace query

namespace query {

template <typename Buffer>
class set_operation final {
public:
  using buffer_type = Buffer;
  
  set_operation(buffer_type& buffer) : buffer_(buffer) {}

  static_assert(
    container_traits::is_sequence_container   <buffer_type>::value ||
    container_traits::is_associative_container<buffer_type>::value,
    "Both sequence or associative containers expected"
  );

  /// Сделать итераторы для любых контейнеров
  template <typename Target>
  void union_with(const Target& with) {
    buffer_type new_buffer;
    std::set_union(buffer_.begin(), buffer_.end(), with.begin(), with.end(), std::back_inserter(new_buffer));
    buffer_ = std::move(new_buffer);
  }

  template <typename Target>
  void intersect_with(const Target& with) {
    buffer_type new_buffer;
    std::set_intersection(buffer_.begin(), buffer_.end(), with.begin(), with.end(), std::back_inserter(new_buffer));
    buffer_ = std::move(new_buffer);
  }

  template <typename Target>
  void difference_with(const Target& with) {
    buffer_type new_buffer;
    std::set_difference(buffer_.begin(), buffer_.end(), with.begin(), with.end(), std::back_inserter(new_buffer));
    buffer_ = std::move(new_buffer);
  }

private:
  buffer_type& buffer_;
};

} // namespace query

namespace query {

//template <typename Buffer>
//class numeric final {
//public:
//  using buffer_type = Buffer;
//  using  value_type = typename Buffer::value_type;
//  using   size_type = size_t;
//
//  static_assert(
//    container_traits::is_sequence_container<buffer_type>::value,
//    "Sequence container expected"
//  );
//
//  numeric(buffer_type& buffer) : buffer_(buffer) {}
//
//  value_type min() {
//
//  }
//
//  value_type max() {
//
//  }
//
//  size_type sum() {
//
//  }
//
//private:
//  buffer_type& buffer_;
//}

} // namespace query

namespace query {

template <
  typename SourceContainer,
  typename TemporaryHolderContainer                         = SourceContainer,
  template <typename          > typename WherePolicy        = where,
  template <typename, typename> typename MergePolicy        = merge,
//  typename OrderPolicy,
  template <typename          > typename SetOperationPolicy = set_operation,
  template <typename, typename> typename CastPolicy         = convert_detail::container_cast
//  typename NumericPolicy
>
class from final {
public:
  using source_container = SourceContainer;
  using temporary_holder = TemporaryHolderContainer;
  using     where_policy = WherePolicy<temporary_holder>;
  template <typename T1, typename T2>
  using     merge_policy = MergePolicy<T1, T2>;
  template <typename T1, typename T2>
  using      cast_policy = CastPolicy<T1, T2>;
  using       set_policy = SetOperationPolicy<temporary_holder>;

  explicit from(const source_container& container) : container_(container), elements_to_take_(-1) {}

  template <typename T, typename Comparator>
  from& where(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(logical_gate);
    return *this;
  }

  template <typename Field, typename T, typename Comparator>
  from& where(Field field, gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(field, logical_gate);
    return *this;
  }

  template <typename T, typename Comparator>
  from& where_key(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_key, logical_gate);
    return *this;
  }

  template <typename Field, typename T, typename Comparator>
  from& where_key(Field field, gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_key, field, logical_gate);
    return *this;
  }

  template <typename T, typename Comparator>
  from& where_value(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_value, logical_gate);
    return *this;
  }

  template <typename Lambda>
  from& where(Lambda lambda) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_lambda(lambda);
    return *this;
  }

  template <typename Field, typename Lambda>
  from& where(Field field, Lambda lambda) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_lambda(field, lambda);
    return *this;
  }

  from& take(ssize_t to_take) noexcept {
    elements_to_take_ = to_take;
    return *this;
  }

  template <typename Target>
  from& merge(const Target& with) {
    merge_impl(with);
    return *this;
  }

  template <typename T>
  from& merge(std::initializer_list<T> with) {
    merge_impl(with);
    return *this;
  }

  from& order_by_ascending() {
//    OrderPolicy::by_ascending(buffer_);
    return *this;
  }

  from& order_by_descending() {
//    OrderPolicy::by_descending(buffer_);
    return *this;
  }

  from& reverse() {
//    OrderPolicy::reverse(buffer_);
    return *this;
  }

  template <typename Target>
  from& union_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.union_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  from& intersect_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.intersect_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  from& difference_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.difference_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  Target to(Target) {
    if (buffer_.empty()) {
      cast_policy<source_container , Target> policy;
      return policy(container_);
    } else {
      cast_policy<temporary_holder, Target> policy;
      return policy(buffer_);
    }
  }

  auto min() {
//    return NumericPolicy::min(buffer_);
  }

  auto max() {
//    return NumericPolicy::max(buffer_);
  }

  auto sum() {
//    return NumericPolicy::sum(buffer_);
  }

private:
  template <typename Target>
  void merge_impl(const Target& with) {
    populate_buffer_if_empty();
    merge_policy<temporary_holder, Target> policy;
    policy(buffer_, with);
  }

  void populate_buffer_if_empty() {
    if (buffer_.empty()) {
      merge_policy<temporary_holder, source_container> policy;
      policy(buffer_, container_);
    }
  }

  const source_container& container_;
  temporary_holder        buffer_;
  ssize_t                 elements_to_take_;
};

} // namespace query

/*
 *  class select {
 *    where, take, merge, order_by_(descending, ascending), reverse,
 *    union, intersect, difference, to, min, max, sum
 *  };
 */

#endif // QUERY_HPP
