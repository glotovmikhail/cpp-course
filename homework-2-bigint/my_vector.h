#ifndef MY_VECTOR_H
#define MY_VECTOR_H

#include <vector>
#include <memory>

struct store;

struct my_vector {
	my_vector() : num(0), is_small_num(2)
	{ };
	~my_vector()
	{ };
	my_vector(my_vector const& other);
	void push_back(const int64_t&);
	void pop_back();
	size_t back() const;
	size_t size() const;
	void resize(size_t sz);
	bool empty() const;
	void clear();
	int64_t& operator[](size_t x);
	const int64_t operator[](size_t x) const;
	my_vector& operator=(my_vector const& other);
	my_vector& operator=(std::vector<int64_t> const& other);

private:
	int64_t num;
	int8_t is_small_num;
	std::shared_ptr<std::vector<int64_t>> store;
	void copy_vector(my_vector const& other);
	void cow_copy();

};

#endif //MY_VECTOR_H