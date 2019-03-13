#ifndef _LIST_H
#define _LIST_H

#include <ostream>

namespace cop3530 {
template <typename T>
class List {
public:
	virtual void insert(T, int) = 0;
	virtual void push_front(T) = 0;
	virtual void push_back(T) = 0;
	virtual void replace(T, int) = 0;
	virtual T remove(int) = 0;
	virtual T pop_front() = 0;
	virtual T pop_back() = 0;
	virtual T& item_at(int) const = 0;
	virtual T& peek_front() const = 0;
	virtual T& peek_back() const = 0;
	virtual bool is_empty() const = 0;
	virtual bool is_full() const = 0;
	virtual int length() const = 0;
	virtual void clear() = 0;
	virtual bool contains(T, bool (*equal_function)(const T&, const T&)) const = 0;
	virtual std::ostream& print(std::ostream &) const = 0;
	virtual T* contents() const = 0;
	
	virtual ~List() {};
};
}

#endif
