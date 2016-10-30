#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <vector>

struct store;

struct my_vector {
	my_vector();
	~my_vector();
	void push_back(int x);
	void pop_back();
	int back() const;
	int size() const;
	void resize(int x);
	bool empty() const;
	void clear();
	int& operator[](int x);
	int operator[](int x) const;
	my_vector& operator=(my_vector const& other);
	my_vector& operator=(std::vector<int> const& other);

private:
	int num;
	int sz;
	store* st;
	void copy();
};

#endif MY_VECTOR_H