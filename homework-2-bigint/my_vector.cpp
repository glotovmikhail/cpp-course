#include "my_vector.h"

#include <vector>

using namespace std;

void my_vector::cow_copy() {
	if (store.unique()) return;
	store = make_shared<vector<int32_t> >(*store);
}

void my_vector::copy_vector(my_vector const& vec) {
	this->is_small_num = vec.is_small_num;
	if (!is_small_num) {
		this->store = vec.store;
	}
	else {
		this->num = vec.num;
	}

}


int32_t& my_vector::operator[](size_t index) {
	if (is_small_num) return num;
	cow_copy();
	return store->at(index);
}

const int32_t my_vector::operator[](size_t index) const {
	if (is_small_num) return num;
	return store->at(index);
}

my_vector::my_vector(my_vector const& vec) {
	copy_vector(vec);
}

void my_vector::push_back(const int32_t& x) {
	if (is_small_num == 1) {
		store = make_shared<vector<int32_t>>();
		store->push_back(num);
		store->push_back(x);
		is_small_num = 0;
	}
	else if (is_small_num == 2)
	{
		is_small_num = 1;
		num = x;
	}
	else {
		cow_copy();
		store->push_back(x);
	}

}

void my_vector::pop_back() {
	if (is_small_num)
		is_small_num = 2;
	else
	{
		cow_copy();
		store->pop_back();
	}
}

size_t my_vector::back() const {
	if (is_small_num) return num;
	return store->back();
}

size_t my_vector::size() const {
	if (is_small_num == 1) return 1;
	else if (is_small_num == 2) return 0;
	return store->size();
}

void my_vector::resize(size_t x) {
	if (is_small_num >= 2 && !x)
	{
		is_small_num = 2;
		return;
	}
	if (is_small_num >= 1) {
		if (x <= 1) return;
		int32_t temp = num;
		store = make_shared<vector<int32_t>>(x);
		if (is_small_num == 1)
			store->at(0) = temp;
		is_small_num = 0;
		return;
	}
	cow_copy();
	store->resize(x);
}

bool my_vector::empty() const {
	if (is_small_num == 2 || (!is_small_num && !store->size())) return true;
	return false;
}


void my_vector::clear() {
	if (!is_small_num)
	{
		cow_copy();
		store->clear();
	}
	else if (is_small_num)
		is_small_num = 2, num = 0;
}

my_vector& my_vector::operator=(my_vector const& other) {
	copy_vector(other);
	return *this;
}

my_vector& my_vector::operator=(std::vector<int32_t> const& other) {
	/* if (other.size() == 1) {
		is_small_num = true;
		num = other.at(0);
	}
	else {
		is_small_num = false;
		for (size_t i = 0; i < other.size(); i++) {
			store->at(i) = other.at(i);
		}
	}
	*/
	*store = other;
	return *this;
}
