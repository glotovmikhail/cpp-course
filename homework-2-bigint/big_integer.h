#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>

struct big_integer
{
    big_integer();
    big_integer(big_integer const& other);
    big_integer(int a);
    big_integer(std::string const& str);
    ~big_integer();

    big_integer& operator=(big_integer const& other);

    big_integer& operator+=(big_integer const& rhs);
    big_integer& operator-=(big_integer const& rhs);
    big_integer& operator*=(big_integer const& rhs);
    big_integer& operator/=(big_integer const& rhs);
    big_integer& operator%=(big_integer const& rhs);

    big_integer& operator&=(big_integer const& rhs);
    big_integer& operator|=(big_integer const& rhs);
    big_integer& operator^=(big_integer const& rhs);

    big_integer& operator<<=(int rhs);
    big_integer& operator>>=(int rhs);

    big_integer operator+() const;
    big_integer operator-() const;
    big_integer operator~() const;

    big_integer& operator++();
    big_integer operator++(int);

    big_integer& operator--();
    big_integer operator--(int);

    big_integer& operator/=(int32_t v);
    big_integer operator/(int32_t v);
    big_integer& operator*=(int32_t v);
    big_integer operator*(int32_t v);

    friend bool operator==(big_integer const& a, big_integer const& b);
    friend bool operator!=(big_integer const& a, big_integer const& b);
    friend bool operator<(big_integer const& a, big_integer const& b);
    friend bool operator>(big_integer const& a, big_integer const& b);
    friend bool operator<=(big_integer const& a, big_integer const& b);
    friend bool operator>=(big_integer const& a, big_integer const& b);

    friend std::string to_string(big_integer const& a);
    friend std::ostream& operator<<(std::ostream& stream, big_integer const& v);
private:
    std::vector<int32_t> numbers;
    int sign;

    void clear_zeroes();
    big_integer abs() const;
    //friend void docheck(const big_integer &a, const big_integer &b, big_integer &dv, big_integer &md);
    //friend void divmod(const big_integer &a1, const big_integer &b1, big_integer &dv, big_integer &md);
	friend big_integer divide(const big_integer &a1, const big_integer &b1);
    int operator%(int32_t v);


    void convert_to_addcode(std::vector<int32_t>& a);
    void convert_to_normal(std::vector<int32_t>& a);
};

big_integer operator+(big_integer a, big_integer const& b);
big_integer operator-(big_integer a, big_integer const& b);
big_integer operator*(big_integer a, big_integer const& b);
big_integer operator/(big_integer a, big_integer const& b);
big_integer operator%(big_integer a, big_integer const& b);

big_integer operator&(big_integer a, big_integer const& b);
big_integer operator|(big_integer a, big_integer const& b);
big_integer operator^(big_integer a, big_integer const& b);

big_integer operator<<(big_integer a, int b);
big_integer operator>>(big_integer a, int b);

bool operator==(big_integer const& a, big_integer const& b);
bool operator!=(big_integer const& a, big_integer const& b);
bool operator<(big_integer const& a, big_integer const& b);
bool operator>(big_integer const& a, big_integer const& b);
bool operator<=(big_integer const& a, big_integer const& b);
bool operator>=(big_integer const& a, big_integer const& b);


#endif