#pragma once

#include <utility>
#include <cmath>
#include <concepts>

namespace shp {

namespace detail {
  template<typename CoordType, std::size_t> using coord_type = CoordType;

  template<typename shape, typename CoordType, std::size_t... Is>
  auto n_doubles_impl(std::index_sequence<Is...>)
    -> decltype(std::declval<shape>()(
      std::declval<coord_type<CoordType, Is>>()...));
}// namespace detail

template<typename T, std::size_t N, typename CoordType>
concept shape = requires(T a) {
  {
    detail::n_doubles_impl<T, CoordType>(std::make_index_sequence<N>{})
    } -> std::convertible_to<bool>;
};

namespace _combine_impl {
  template<typename Shape1, typename Shape2, size_t N, typename CoordType>
  struct combined_shape {
    [[no_unique_address]] Shape1 s1;
    [[no_unique_address]] Shape2 s2;

    [[nodiscard]] bool operator()(Double... coords) const noexcept
      requires(sizeof...(Double) == N) {
      return s1(coords...) or s2(coords...);
    }
  };
}// namespace _combine_impl

template<std::size_t N, typename CoordType>
shape<N, CoordType> auto combine(shape<N, CoordType> auto &&shp1,
  shape<N, CoordType> auto &&shp2) {
  using s1_t = std::remove_cvref_t<decltype(shp1)>;
  using s2_t = std::remove_cvref_t<decltype(shp2)>;
  return _combine_impl::combined_shape<s1_t, s2_t, N, CoordType>(
    std::forward<s1_t>(shp1), std::forward<s2_t>(shp2));
}

template<std::size_t N, typename CoordType>
shape<N, CoordType> auto intersect(shape<N, CoordType> auto &&shp1,
  shape<N, CoordType> auto &&shp2) {
  struct combined_shape {
    [[no_unique_address]] std::remove_reference_t<decltype(shp1)> s1;
    [[no_unique_address]] std::remove_reference_t<decltype(shp2)> s2;

    [[nodiscard]] bool operator()(double x, double y) const noexcept {
      return s1(x, y) and s2(x, y);
    }
  };
  return combined_shape{ std::forward<decltype(shp1)>(shp1),
    std::forward<decltype(shp2)>(shp2) };
}

class circle {
  double radius_, centre_x_, centre_y_;

public:
  circle(double radius, double x, double y)
    : radius_(radius), centre_x_(x), centre_y_(y) {}

  double &radius() noexcept { return radius_; }

  double radius() const noexcept { return radius_; }

  double &centre_x() noexcept { return centre_x_; }

  double centre_x() const noexcept { return centre_y_; }

  double &centre_y() noexcept { return centre_y_; }

  double centre_y() const noexcept { return centre_y_; }

  [[nodiscard]] bool operator()(double x, double y) const noexcept {
    return (x - centre_x_) * (x - centre_x_) + (y - centre_y_) * (y - centre_y_)
           == radius_ * radius_;
  }
};


}// namespace shp
