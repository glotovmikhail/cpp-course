
#include <functional>
#include <iostream>
#include <string>

template<int N>
struct place_holder {};
constexpr place_holder<0> _1{};
constexpr place_holder<1> _2{};
constexpr place_holder<2> _3{};
constexpr place_holder<3> _4{};

template<typename F, typename... Args>
struct bind_t {

	template<typename Ff, typename... Args_>
	bind_t(Ff &&func, Args_ &&... args) :
		func(std::forward<Ff>(func)),
		base_args(std::forward<Args_>(args)...)
	{};

	template<typename... newArgs>
	auto operator()(newArgs&&... new_args) {
		return call_func(typename generate<std::tuple_size<tuple>::value>::type(), std::forward<newArgs>(new_args)...);
	}

private:
	typename std::decay<F>::type func;

	typedef std::tuple<std::decay_t<Args>...> tuple;

	tuple base_args;

	template<int... r>
	struct range {};

	template<int r, int... n>
	struct generate {
		typedef typename generate<r - 1, r - 1, n ...>::type type;
	};

	template<int... n>
	struct generate<0, n...> {
		typedef range<n...> type;
	};

	template <typename prevArgs, typename ... newArgs>
	decltype(auto) arg_get(prevArgs &base_arg, newArgs &... new_args) {
		return base_arg;
	};

	template <int r, typename ... newArgs>
	decltype(auto) arg_get(place_holder<r> &plh, newArgs &... new_args) {
		return std::get<r>(std::forward_as_tuple(new_args...));
	};

	template <typename Ff, typename ... prevArgs, typename ...newArgs>
	decltype(auto) arg_get(bind_t<Ff, prevArgs...>& b, newArgs&... new_args) {
		return b(new_args ...);
	};

	template <typename ... newArgs, int ... r>
	auto call_func(const range<r...> & s, newArgs&& ... new_args) {
		return func(arg_get(std::get<r>(base_args), new_args...)...);
	};

};

template<typename F, typename ... Args>
bind_t<F, Args...> bind(F&& func, Args&& ... args) {
	return bind_t<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
};

/*
int triple_sum(int a, int b, int c) {
	return a + b + c;
}

int triple_mul(int a, int b, int c) {
	return a * b * c;
}

double triple_mull(double a, double b, int c) {
	return a * b * c;
}

int smth(int a, int b, int c) {
	return a + b - c;
}

int main() {
	std::cout << bind(&triple_sum, _2, _2, _1)(1, 2) << std::endl;
	std::cout << bind(&triple_mul, _2, _1, _2)(2, 1) << std::endl;
	std::cout << bind(&triple_mull, _2, _2, _1)(4, 0.5) << std::endl;
	std::cout << bind(&smth, _2, _3, _1)(5, 10, 12) << std::endl;
	return 0;
}
*/