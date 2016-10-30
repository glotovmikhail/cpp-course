#include "my_vector.h"

#include <vector>

struct store {
	store();
	store(store const& x);
	int cnt;
	std::vector<int> vec;
};


my_vector::my_vector() : num(0), sz(0), st(nullptr) { }

my_vector::~my_vector() {
	if (st != nullptr) {
		st->cnt--;
		if (st->cnt == 0) delete st;
	}
}

void my_vector::copy() {
	if (st == nullptr) {
		return;
	}
	if (st->cnt > 1) {
		st->cnt--;
		store *old = st;
		st = new store(*old);
		if (old->cnt == 0) {
			delete old;
		}
	}
}

void my_vector::push_back(int x) {
	copy();
	sz++;
	if (sz == 1 && st == nullptr) num = x;
	if (sz > 1 && st == nullptr) {
		st = new store();
		st->vec.push_back(num);
	}
	if (st != nullptr) st->vec.push_back(x);
}

void my_vector::pop_back() {
	copy();
	sz--;
	if (st != nullptr) st->vec.pop_back();
}

int my_vector::back() const {
	if (st == nullptr) return num; else return st->vec.back();
}

int my_vector::size() const {
	return sz;
}

void my_vector::resize(int x) {
	copy();
	if (x > 1 && st == nullptr) {
		st = new store();
		st->vec.resize(x);
		if (sz == 1) (st->vec)[0] = num;
		sz = x;
	}
	if (x <= 1 && st == nullptr) {
		sz = x;
	}
	if (st != nullptr) {
		st->vec.resize(x);
	}
}

bool my_vector::empty() const {
	return (sz == 0);
}

int& my_vector::operator[](int x) {
	copy();
	if (st == nullptr) {
		return num;
	}
	else {
		return (st->vec)[x];
	}
}

int my_vector::operator[](int x) const {
	if (st == nullptr) {
		return num;
	}
	else {
		return (st->vec)[x];
	}
}

void my_vector::clear() {
	copy();
	sz = 0;
	num = 0;
	if (st != nullptr) st->vec.clear();
}

my_vector& my_vector::operator=(my_vector const& other) {
	num = other.num;
	sz = other.sz;
	if (st != nullptr) {
		st->cnt--;
		if (st->cnt == 0) delete st;
	}
	st = other.st;
	if (other.st != nullptr) st->cnt++;
	return *this;
}

my_vector& my_vector::operator=(std::vector<int> const& other) {
	copy();
	sz = other.size();
	if (st != nullptr) st->vec = other; else
		if (other.size() == 1) num = other[0]; else {
			if (st == nullptr) st = new store();
			st->vec = other;
		}
	return *this;
}

store::store() : cnt(1) { }

store::store(store const& x) {
	vec = x.vec;
	cnt = 1;
}