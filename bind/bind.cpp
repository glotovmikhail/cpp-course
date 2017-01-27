#include <string>
#include <functional>
#include <iostream>

template <int N>
struct placeholder
{
};

placeholder<0> _1;
placeholder<1> _2;
placeholder<2> _3;
placeholder<3> _4;

template <typename F, typename... Args>
struct bind_t {

	template <typename... newArgs>
	auto operator()(newArgs&&... new_args) {
		return call(std::forward<newArgs>(new_args)...);
	}

private:
	F func;
	std::tuple<Args...> args;
	template <typename F_, typename... Args_>
	bind_t(F_&& func, Args_&&... args) : 
		func(std::forward<F_>(func)),
		args(std::forward<Args_>(args)...)
	{};

	template <typename prevArgs, typename... newArgs>
	auto arg_get(prevArgs& prev_arg, newArgs&...) {
		return prev_arg;
	};

	template <typename F_, typename... prevArgs, typename... newArgs>
	auto arg_get(bind_t<F_, prevArgs...>& b, newArgs&... new_args) {
		return b(new_args...);
	}

	template <int n, typename... newArgs>
	auto arg_get(placeholder<n>&, newArgs&... new_args) {
		return std::get<n>(std::forward_as_tuple(new_args...));
	}

	template<int... n>
	struct range {};

	template <int r, int... n> 
	struct generate {
		typedef typename generate<r - 1, r - 1, n...>::type type;
	};

	template <int... n>
	struct generate<0, n...> {
		typedef range<n...> type;
	};

	template <typename... newArgs, int... r>
	auto call_r(const range<r...>&, newArgs&&... new_args) {
		return func(arg_get(std::get<r>(args), new_args...)...);
	}

	template <typename... newArgs>
	auto call(newArgs&&... new_args) {
		return call_r(typename generate<std::tuple_size<std::tuple<Args...>>::value>::type(),
			std::forward<newArgs>(new_args)...);
	}
	
	template <typename F_, typename... Args_>
	friend bind_t<F_, Args_...> bind(F_&& func, Args_&&... args);

};

template <typename F, typename... Args>
bind_t<F, Args...> bind(F&& func, Args&&... args) {
	return bind_t<F, Args...>(std::forward<F>(func), std::forward<Args>(args)...);
}

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
	system("pause");
	return 0;
}