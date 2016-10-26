#include <limits>
#include <iostream>
#include <string>

using namespace std;
struct overflow_exception {
	overflow_exception(string const& text) : message(text) { };
	string const& message_text() const {
		return message; 
	}
private:
	string message;
};

struct signed_tag {};
struct unsigned_tag {};

template<typename T, bool C>
struct overflow_traits;

template<typename T>
struct overflow_traits<T, 1> {
	typedef typename signed_tag tag;
};

template<typename T> 
struct overflow_traits<T, 0> {
	typedef typename unsigned_tag tag;
};

template<typename T> 
struct overflowChecking {
	overflowChecking() {};
	overflowChecking(T val) : value(val) {};
	//---------
	//add
	//---------
	template<typename U>
	friend overflowChecking<U> add_impl(overflowChecking<U> const&, overflowChecking<U> const&, signed_tag);
	template<typename U>
	friend overflowChecking<U> add_impl(overflowChecking<U> const&, overflowChecking<U> const&, unsigned_tag);
	template<typename U>
	friend overflowChecking<U> operator+(overflowChecking<U> const&, overflowChecking<U> const&);

	//--------
	//sub
	//--------
	template<typename U>
	friend overflowChecking<U> sub_impl(overflowChecking<U> const&, overflowChecking<U> const&, signed_tag);
	template<typename U>
	friend overflowChecking<U> sub_impl(overflowChecking<U> const&, overflowChecking<U> const&, unsigned_tag);
	template<typename U>
	friend overflowChecking<U> operator-(overflowChecking<U> const&, overflowChecking<U> const&);
	//--------
	//mul
	//--------
	template<typename U>
	friend overflowChecking<U> mul_impl(overflowChecking<U> const&, overflowChecking<U> const&, signed_tag);
	template<typename U>
	friend overflowChecking<U> mul_impl(overflowChecking<U> const&, overflowChecking<U> const&, unsigned_tag);
	template<typename U>
	friend overflowChecking<U> operator*(overflowChecking<U> const&, overflowChecking<U> const&);

	//--------
	//div
	//--------
	template<typename U>
	friend overflowChecking<U> div_impl(overflowChecking<U> const&, overflowChecking<U> const&, signed_tag);
	template<typename U>
	friend overflowChecking<U> div_impl(overflowChecking<U> const&, overflowChecking<U> const&, unsigned_tag);
	template<typename U>
	friend overflowChecking<U> operator/(overflowChecking<U> const&, overflowChecking<U> const&);


	overflowChecking<T>& operator=(overflowChecking<T> const& right) {
		this->value = right.value;
		return *this;
	}
	overflowChecking<T> negate(signed_tag) const {
		if (this->value == numeric_limits<T>::min()) {
			throw overflow_exception("signed number negate overflow \n");
		}
		return this->value * (T)-1;
	}
	overflowChecking<T> negate(unsigned_tag) const {
		throw overflow_exception("can't negate unsigned number \n");
	}
	overflowChecking<T> operator-() const {
		return this->negate(typename overflow_traits<T, numeric_limits<T>::is_signed>::tag());
	}

private: 
	T value;
};

//---------
//add
//---------
template<typename T>
overflowChecking<T> add_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, signed_tag) {
	if ((right.value < 0 && left.value < numeric_limits<T>::min() - right.value)
		|| (right.value > 0 && left.value > numeric_limits<T>::max() - right.value)) {
		throw overflow_exception("signed numbers add overflow \n");
	}
	return overflowChecking<T>(left.value + right.value);
}
template<typename T>
overflowChecking<T> add_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, unsigned_tag) {
	if (right.value > numeric_limits<T>::max() - left.value) {
		throw overflow_exception("unsigned numbers add overflow \n");
	}
	return overflowChecking<T>(left.value + right.value);
}
template<typename T>
overflowChecking<T> operator+(overflowChecking<T> const& left, overflowChecking<T> const& right) {
	return add_impl(left, right, typename overflow_traits<T, numeric_limits<T>::is_signed>::tag());
}

//--------
//sub
//--------
template<typename T>
overflowChecking<T> sub_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, signed_tag) {
	if ((right.value > 0 && left.value < numeric_limits<T>::min() + right.value)
		|| (right.value < 0 && left.value > numeric_limits<T>::max() + right.value)) {
		throw overflow_exception("signed numbers subtract overflow \n");
	}
	return overflowChecking<T>(left.value + right.value);
}
template<typename T>
overflowChecking<T> sub_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, unsigned_tag) {
	if (right.value > left.value) {
		throw overflow_exception("unsigned numbers subtract overflow \n");
	}
	return overflowChecking<T>(left.value + right.value);
}
template<typename T>
overflowChecking<T> operator-(overflowChecking<T> const& left, overflowChecking<T> const& right) {
	return sub_impl(left, right, typename overflow_traits<T, numeric_limits<T>::is_signed>::tag());
}

//--------
//mul
//--------
template<typename T>
overflowChecking<T> mul_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, signed_tag) {
	if (left.value != 0 && right.value != 0 &&
		(((right.value == -1) && (left.value == numeric_limits<T>::min())) 
			|| ((left.value == -1) && (right.value == numeric_limits<T>::min()))
			|| (abs(left.value) > numeric_limits<T>::max() / abs(right.value)))) {

		throw overflow_exception("singed numbers multiply overflow \n");
	}
	return overflowChecking<T>(left.value * right.value);
}
template<typename T>
overflowChecking<T> mul_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, unsigned_tag) {
	if (left.value != 0 && right.value != 0 &&
		(left.value > numeric_limits<T>::max() / right.value)) {
		throw overflow_exception("unsigned numbers multiply overflow \n");
	}
	return overflowChecking<T>(left.value * right.value);
}
template<typename T>
overflowChecking<T> operator*(overflowChecking<T> const& left, overflowChecking<T> const& right) {
	return mul_impl(left, right, typename overflow_traits<T, numeric_limits<T>::is_signed>::tag());
}

//--------
//div
//--------
template<typename T>
overflowChecking<T> div_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, signed_tag) {
	if (right.value == -1 && left.value == numeric_limits<T>::min()) {
		throw overflow_exception("signed numbers division overflow \n");
	}
	return overflowChecking<T>(left.value / right.value);
}
template<typename T>
overflowChecking<T> div_impl(overflowChecking<T> const& left, overflowChecking<T> const& right, unsigned_tag) {
	return overflowChecking<T>(left.value / right.value);

}
template<typename T>
overflowChecking<T> operator/(overflowChecking<T> const& left, overflowChecking<T> const& right) {
	return div_impl(left, right, typename overflow_traits<T, numeric_limits<T>::is_signed>::tag());
}


int main() {
	try {
		typedef int32_t type;
		{
			overflowChecking<type> a((int32_t) 566239);
			overflowChecking<type> b(numeric_limits<type>::min());
			a + b;
			//a - b;
			a * b;
			b / a;
			-a;
			a = b;
			-a;
		}
	} 
	catch (overflow_exception& e) {
		cout << e.message_text();
	}
	system("pause");
	return 0;
}