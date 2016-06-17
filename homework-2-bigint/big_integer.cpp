#include "big_integer.h"
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>
using namespace std;

const int32_t main_base = (1 << 30);
int32_t base = main_base;
const int32_t digits = 9;

big_integer big_integer::abs() const {
    big_integer res = *this;
    res.sign *= res.sign;
    return res;
}

void big_integer::clear_zeroes() {
    while (!numbers.empty() && !numbers.back()) numbers.pop_back();
    if (numbers.empty()) sign = 1;
    if (numbers.size() == 1 && numbers[0] == 0) sign = 1;
}

big_integer::big_integer() : sign(1), numbers() { }

big_integer::big_integer(big_integer const& other) : sign(other.sign), numbers(other.numbers) { }

big_integer::big_integer(int a) {
    int64_t b = a;
    sign = 1;
    if (b < 0) {
        sign = -1;
        b = -b;
    }
	for (; b > 0; b = b / base) {
		numbers.push_back(b % base);
	}
}

big_integer::big_integer(std::string const& str) {
    sign = 1;
    numbers.clear();
    int32_t pos = 0;
    while (pos < (int32_t) str.size() && (str[pos] == '-' || str[pos] == '+')) {
        if (str[pos] == '-') sign = -sign;
        pos++;
    }
    for (int32_t i = (int32_t) str.size() - 1; i >= pos; i -= digits) {
        int32_t x = 0;
		for (int32_t j = max(pos, i - digits + 1); j <= i; j++) {
			x = x * 10 + str[j] - '0';
		}
        numbers.push_back(x);
    }
    clear_zeroes();
    base = 1000 * 1000 * 1000;
    int32_t new_base = main_base;
    big_integer res;
    res.sign = sign;
    *this = abs();
    while (*this > 0) {
        res.numbers.push_back(*this % new_base);
        *this /= new_base;
    }
    base = new_base;
    *this = res;
}


big_integer::~big_integer() { }

big_integer& big_integer::operator=(big_integer const& other) {
    sign = other.sign;
    numbers = other.numbers;
    return *this;
}

big_integer& big_integer::operator+=(big_integer const& rhs) {
    if (sign == rhs.sign) {
        big_integer res = rhs;
        for (int32_t i = 0, carry = 0; i < (int32_t) max(numbers.size(), rhs.numbers.size()) || carry; i++) {
            if (i == res.numbers.size()) res.numbers.push_back(0);
            res.numbers[i] += carry + (i < numbers.size() ? numbers[i] : 0);
            carry = res.numbers[i] >= base;
            if (carry) res.numbers[i] -= base;
        }
        *this = res;
    } else {
        *this -= -rhs;
    }
    return *this;
}

big_integer& big_integer::operator-=(big_integer const& rhs) {
    if (sign == rhs.sign) {
        if (abs() >= rhs.abs()) {
            big_integer res = *this;
            for (int32_t i = 0, carry = 0; i < rhs.numbers.size() || carry; i++) {
                res.numbers[i] -= carry + (i < rhs.numbers.size() ? rhs.numbers[i] : 0);
                carry = res.numbers[i] < 0;
                if (carry) res.numbers[i] += base;
            }
            res.clear_zeroes();
            *this = res;
        } else {
            big_integer buf(rhs);
            buf -= (*this);
            *this = -buf;
        }
    } else {
        *this += (-rhs);
    }
    return *this;
}

big_integer operator+(big_integer a, big_integer const& b) {
    return a += b;
}

big_integer operator-(big_integer a, big_integer const& b) {
    return a -= b;
}
big_integer& big_integer::operator*=(int32_t v) {
    if (v < 0) {
        sign = -sign;
        v = -v;
    }
    for (int32_t i = 0, carry = 0; i < numbers.size() || carry; ++i) {
		if (i == numbers.size()) {
			numbers.push_back(0);
		}
        int64_t cur = numbers[i] * (int64_t) v + carry;
        carry = (int32_t) (cur / base);
        numbers[i] = (int32_t) (cur % base);
    }
    clear_zeroes();
    return *this;
}

big_integer big_integer::operator*(int32_t v) {
    big_integer res = *this;
    res *= v;
    return res;
}

big_integer divide(const big_integer &a1, const big_integer &b1) {
	if (b1.numbers.size() == 1) {
		big_integer ret = a1;
		ret /= b1.numbers.back();
		ret.sign *= b1.sign;
		return ret;
	}
    int32_t n = base / (b1.numbers.back() + 1);
    big_integer a = a1.abs() * n;
    big_integer b = b1.abs() * n;
    big_integer ans, r;
    ans.numbers.resize(a.numbers.size());

    for (int32_t i = a.numbers.size() - 1; i >= 0; i--) {
        r *= base;
        r += a.numbers[i];
        int32_t s1 = r.numbers.size() <= b.numbers.size() ? 0 : r.numbers[b.numbers.size()];
        int32_t s2 = r.numbers.size() <= b.numbers.size() - 1 ? 0 : r.numbers[b.numbers.size() - 1];
        int32_t d = ((int64_t) base * s1 + s2) / b.numbers.back();
        r -= b * d;
        while (r < 0) {
            r += b;
            d--;
        }
        ans.numbers[i] = d;
    }
    ans.sign = a1.sign * b1.sign;
    ans.clear_zeroes();
	return ans;
}


big_integer operator/(big_integer a, big_integer const& b) {
	return divide(a, b);
}

big_integer operator%(big_integer a, big_integer const& b) {
	return (a %= b);
}

big_integer& big_integer::operator/=(big_integer const& rhs) {
    (*this) = (*this) / rhs;
    return *this;
}

big_integer& big_integer::operator%=(big_integer const& rhs) {
	*this = *this - *this / rhs * rhs;
	return *this;
}

big_integer& big_integer::operator/=(int32_t x) {
	int64_t v = x;
    if (v < 0) {
        sign = -sign;
        v = -v;
    }
    for (int32_t i = numbers.size() - 1, rem = 0; i >= 0; i--) {
        int64_t cur = numbers[i] + rem * (int64_t) base;
        numbers[i] = (int32_t) (cur / v);
        rem = (int32_t) (cur % v);
    }
    clear_zeroes();
    return *this;
}

big_integer big_integer::operator/(int32_t v) {
    big_integer res = *this;
    res /= v;
    return res;
}

int32_t big_integer::operator%(int32_t v) {
    if (v < 0)
        v = -v;
    int32_t m = 0;
	for (int32_t i = numbers.size() - 1; i >= 0; i--) {
		m = (numbers[i] + m * (int64_t)base) % v;
	}
    return m * sign;
}

bool operator<(big_integer const& a, big_integer const& b) {
    if (a.sign < b.sign) return true; 
	else if (a.sign > b.sign) return false; 
	else if (a.numbers.size() < b.numbers.size()) return true;
	else if (a.numbers.size() > b.numbers.size()) return false; 
	else {
        for (int32_t i = a.numbers.size() - 1; i >= 0; i--) {
            if (a.numbers[i] > b.numbers[i]) return false;
            if (a.numbers[i] < b.numbers[i]) return true;
        }
        return false;
    }
}

bool operator>(big_integer const& a, big_integer const& b) {
    return b < a;
}
bool operator<=(big_integer const& a, big_integer const& b) {
    return !(b < a);
}
bool operator>=(big_integer const& a, big_integer const& b) {
    return !(a < b);
}
bool operator==(big_integer const& a, big_integer const& b) {
    return !(a < b) && !(b < a);
}
bool operator!=(big_integer const& a, big_integer const& b) {
    return a < b || b < a;
}

big_integer big_integer::operator-() const {
    big_integer res = *this;
    res.sign = -sign;
    if (numbers.size() == 0) res.sign = 1;
    return res;
}

std::ostream& operator<<(ostream &stream, const big_integer &v) {
    if (v.sign == -1) stream << '-';
    int32_t new_base = 1000 * 1000 * 1000;
    big_integer buf;
    big_integer cur(v.abs());
    while (cur > 0) {
        buf.numbers.push_back(cur % new_base);
        cur /= new_base;
    }
    if (buf.numbers.empty()) stream << 0; else stream << buf.numbers.back();
    for (int32_t i = buf.numbers.size() - 2; i >= 0; i--)
        stream << setw(digits) << setfill('0') << buf.numbers[i];
    return stream;
}

std::string to_string(big_integer const& a) {
    std::stringstream ss;
    ss << a;
    string res;
    ss >> res;
    return res;
}

big_integer& big_integer::operator*=(big_integer const& rhs) {
    big_integer c;
    c.numbers.resize(numbers.size() + rhs.numbers.size());
    for (int32_t i = 0; i < numbers.size(); i++)
        for (int32_t j = 0, carry = 0; j < rhs.numbers.size() || carry; ++j) {
            int64_t cur = c.numbers[i + j] + numbers[i] * 1ll* (j < rhs.numbers.size() ? rhs.numbers[j] : 0) + carry;
            c.numbers[i + j] = int32_t(cur % base);
            carry = int32_t(cur / base);
    }
    c.sign = sign * rhs.sign;
    c.clear_zeroes();
    *this = c;
    return *this;
}

big_integer operator*(big_integer a, big_integer const& b) {
    return a *= b;
}

void big_integer::convert_to_addcode(vector<int32_t>& a) {
    a.resize(numbers.size());
    if (*this < 0) {
        (*this)++;
        if (numbers.empty()) numbers.push_back(0);
        for (int32_t i = 0; i < numbers.size(); i++)
            a[i] = ~numbers[i];
        a.push_back(1);
    } else {
        for (int32_t i = 0; i < numbers.size(); i++)
            a[i] = numbers[i];
        a.push_back(0);
    }
}

void big_integer::convert_to_normal(vector<int32_t>& a) {
    if (a.back() == 0) {
        numbers = a;
        numbers.pop_back();
    } else {
        numbers.clear();
        a.pop_back();
        sign = -1;
        for (int32_t i = 0; i < (int32_t) a.size(); i++)
            numbers.push_back(~a[i]);
        (*this)--;
    }
}

big_integer& big_integer::operator&=(big_integer const& rhs)
{
    big_integer r = rhs;
    vector<int32_t> a, b;
    convert_to_addcode(a);
    r.convert_to_addcode(b);
    while (a.size() < b.size()) a.push_back(0);
    while (b.size() < a.size()) b.push_back(0);
	for (int32_t i = 0; i < (int32_t)a.size(); i++) {
		a[i] &= b[i];
	}
    convert_to_normal(a);
    clear_zeroes();
    return *this;
}

big_integer& big_integer::operator|=(big_integer const& rhs)
{
    big_integer r = rhs;
    vector<int32_t> a, b;
    convert_to_addcode(a);
    r.convert_to_addcode(b);
    while (a.size() < b.size()) a.push_back(0);
    while (b.size() < a.size()) b.push_back(0);
	for (int32_t i = 0; i < (int32_t)a.size(); i++) {
		a[i] |= b[i];
	}
    convert_to_normal(a);
    clear_zeroes();
    return *this;
}

big_integer& big_integer::operator^=(big_integer const& rhs)
{
    big_integer r = rhs;
    vector<int32_t> a, b;
    convert_to_addcode(a);
    r.convert_to_addcode(b);
    while (a.size() < b.size()) a.push_back(0);
    while (b.size() < a.size()) b.push_back(0);
	for (int32_t i = 0; i < (int32_t)a.size(); i++) {
		a[i] ^= b[i];
	}
    convert_to_normal(a);
    clear_zeroes();
    return *this;
}

big_integer& big_integer::operator<<=(int rhs)
{
    if (*this < 0) (*this)++;
    for (int32_t i = 0; i < rhs; i++) *this *= 2;
    if (*this < 0) (*this)--;
    return *this;
}

big_integer& big_integer::operator>>=(int rhs)
{
    if (*this < 0) (*this)++;
    for (int32_t i = 0; i < rhs; i++) *this /= 2;
    if (*this < 0) (*this)--;
    return *this;
}

big_integer big_integer::operator+() const
{
    return *this;
}

big_integer big_integer::operator~() const
{
    big_integer r(*this);
    r = -r - 1;
    return r;
}
big_integer& big_integer::operator++()
{
    return (*this += big_integer(1));
}

big_integer big_integer::operator++(int)
{
    big_integer r = *this;
    ++*this;
    return r;
}

big_integer& big_integer::operator--()
{
    return (*this -= big_integer(1));
}

big_integer big_integer::operator--(int)
{
    big_integer r = *this;
    --*this;
    return r;
}

big_integer operator&(big_integer a, big_integer const& b)
{
    return a &= b;
}

big_integer operator|(big_integer a, big_integer const& b)
{
    return a |= b;
}

big_integer operator^(big_integer a, big_integer const& b)
{
    return a ^= b;
}

big_integer operator<<(big_integer a, int b)
{
    return a <<= b;
}

big_integer operator>>(big_integer a, int b)
{
    return a >>= b;
}