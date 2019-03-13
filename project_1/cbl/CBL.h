#ifndef _CBLL_H
#define _CBL_H

#include <stdexcept>
#include <stdio.h>
#include "List.h"

namespace cop3530 {
	template <typename T>
	class CBL : public List<T> {
	public:
		class iterator {
		private:
			const CBL *const parent;
			int here;
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;

			// `position` as passed to the array is supposed to be
			// relative to the head, so position = 0 would point at
			// the head
			iterator (const CBL *parent, int position)
				: parent(parent) {
				// `here` is an absolute measure of the array, it is
				// relative to the start of the array, not the head
				this->here = (this->parent->head + position) % this->parent->size;
			}

			iterator (const iterator &other) : parent(other.parent) {
				this->here = other.here;
			}
			
			bool operator == (const iterator &other) const {
				return this->parent == other.parent && other.here == this->here;
			}
			
			bool operator != (const iterator &other) const {
				return ! (operator == (other));
			}

			iterator &operator ++ () {
				++this->here;
				if (this->here == this->parent->size) {
					this->here = 0;
				}
				return *this;
			}

			iterator &operator ++ (int) {
				iterator tmp(*this);
				++(*this);
				return tmp;
			}

			iterator &operator -- () {
				if (this->here == 0) {
					this->here = this->parent->size;
				}
				--this->here;
				return *this;
			}

			iterator &operator -- (int) {
				iterator tmp(*this);
				--(*this);
				return tmp;
			}

			T &operator * () const {
				return this->parent->arr[this->here];
			}

			T *operator & () const {
				return this->parent->arr + this->here;
			}

			iterator &operator = (const T &value) {
				this->parent->arr[this->here] = value;
				return *this;
			}
		};

		iterator begin() {
			iterator i(this, 0);
			return i;
		}

		iterator end() {
			iterator i(this, this->length());
			return i;
		}

		const iterator end() const {
			const iterator i(this, this->length());
			return i;
		}

		const iterator begin() const {
			const iterator i(this, 0);
			return i;
		}
		
	CBL(int size) : starting_capacity(size + 1) {
			++size;
			this->size = size;
			this->arr = new T[size];
			this->head = size / 2;
			this->tail = this->head;
		}

		CBL () : CBL(50) {}

		CBL (const CBL &other) : starting_capacity(other.starting_capacity) {
			this->size = other.size;
			this->head = other.head;
			this->tail = other.tail;
			this->arr = new T[size];
			for (int i = 0; i < size; ++i) {
				this->arr[i] = other.arr[i];
			}
		}

		CBL (CBL &&other) : starting_capacity(other.starting_capacity) {
			this->size = other.size;
			this->head = other.head;
			this->tail = other.tail;
			this->arr = other.arr;
			other.arr = nullptr;
		}

		CBL &operator = (const CBL &other) {
			this->staring_capacity = other.starting_capacity;
			this->size = other.size;
			this->head = other.head;
			this->tail = other.tail;
			this->arr = new T[size];
			for (int i = 0; i < size; ++i) {
				this->arr[i] = other.arr[i];
			}
			return *this;
		}

		CBL &operator = (CBL &&other) {
			this->size = other.size;
			this->head = other.head;
			this->tail = other.tail;
			this->arr = other.arr;
			other.arr = nullptr;
		}
		
		void insert(T item, int position) {
			if (position < 0 || position > this->length()) {
				throw std::runtime_error("insert: Position out of range");
			}
			if (this->size <= this->length() + 1) {
				this->size_change_by_ratio(3, 2);
			}
			if (this->is_empty()) {
				this->arr[this->tail] = item;
				this->increment_pointer(&(this->tail));
				return;
			}
			// if the position is near the front of the list, move the
			// head and shift all the items;; otherwise move the tail
			// back and shift the items the other way
			if (position < this->length() / 2) {
				// move head back
				this->decrement_pointer(&(this->head));
				iterator target(this, position);
				for (iterator iter = this->begin(); iter != target; ++iter) {
					iterator tmp(iter);
					++tmp;
					iter = *tmp;
				}
				*target = item;
			} else {
				// move tail back
				iterator target(this, position);
				this->increment_pointer(&(this->tail));
				for (iterator iter = this->end(); iter != target; --iter) {
					iterator tmp(iter);
					--tmp;
					iter = *tmp;
				}
				*target = item;
			}
		}
		
		void push_front(T item) {
			this->insert(item, 0);
		}
		
		void push_back(T item) {
			this->insert(item, this->length());
		}
		
		void replace(T item, int position) {
			if (position < 0 || position > this->length()) {
				throw std::runtime_error("replace: Position out of range");
			}
			this->arr[(this->head + position) % this->size] = item;
		}
		
		T remove(int position) {
			if (position < 0 || position >= this->length()) {
				throw std::runtime_error("remove: Out of range");
			}
			iterator iter(this, position);
			T ret = *iter;
			if (position < this->length() / 2) {
				iterator begin = this->begin();
				for ( ; iter != begin; --iter) {
					iterator tmp(iter);
					--tmp;
					iter = *tmp;
				}
				this->increment_pointer(&(this->head));
			} else {
				this->decrement_pointer(&(this->tail));
				iterator end = this->end();
				for ( ; iter != end; ++iter) {
					iterator tmp(iter);
					++tmp;
					iter = *tmp;
				}
			}
			if (this->size > this->starting_capacity && this->length() * 2 <= this->size) {
				this->size_change_by_ratio(3, 4);
			}
			return ret;
		}
		
		T pop_front() {
			if (this->is_empty()) {
				throw std::runtime_error("pop_front: Out of range");
			}
			T item = this->arr[this->head];
			this->increment_pointer(&(this->head));
			return item;
		}
		
		T pop_back() {
			if (this->is_empty()) {
				throw std::runtime_error("pop_back: Out of range");
			}
			T item = this->peek_back();
			this->decrement_pointer(&(this->tail));
			return item;
		}
		
		T& item_at(int position) const {
			if (position < 0 || position > this->length()) {
				throw std::runtime_error("item_at: Out of range");
			}
			return this->arr[(this->head + position) % this->size];
		}
		
		T& peek_front() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_front: List is empty");
			}
			return this->arr[this->head];
		}
		
		T& peek_back() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_back: List is empty");
			}
			// tail is one past the end of the array so when tail is
			// zero, it should wrap to the end of the backing array
			if (this->tail == 0)
				return this->arr[this->size - 1];
			else
				return this->arr[this->tail - 1];
		}
		
		bool is_empty() const {
			return this->head == this->tail;
		}
		
		bool is_full() const {
			return false;
		}
		
		int length() const {
			// if tail is not ahead of head, then that means that it
			// is wrapping around to the front of the array
			if (this->tail >= this->head) {
				return this->tail - this->head;
			} else {
				return this->size - this->head + this->tail;
			}
		}
		
		void clear() {
			this->head = this->size / 2;
			this->tail = this->head;
		}
		
		bool contains(T item, bool (*equal_function)(const T&, const T&)) const {
			iterator iter = this->begin();
			iterator end = this->end();
			for ( ; iter != end; ++iter) {
				if (equal_function(item, *iter))
					return true;
			}
			return false;
		}
		
		std::ostream &print(std::ostream &stream) const {
			if (this->is_empty()) {
				stream << "<empty list>";
				return stream;
			}
			stream << "[";
			iterator iter = this->begin();
			iterator end = this->end();
			// comma separate all of them except the last, so go to
			// end - 1
			--end;
			while (iter != end) {
				stream << *iter << ",";				
				++iter;
			}
			stream << *iter << "]";
			return stream;
		}
		
		T* contents() const {
			T *ret = new T[this->length()];
			iterator end = this->end();
			int i = 0;
			for (iterator iter = this->begin(); iter != end; ++iter, ++i) {
				ret[i] = *iter;
			}
			return ret;
		}
		
		~CBL() {
			delete [] this->arr;
		}
	private:
		T *arr;
		int size;
		int tail;
		int head;
		const int starting_capacity;

		// changes the size by a fraction
		void size_change_by_ratio(int num, int div) {
			int old_size = this->size;
			this->size = old_size * num / div;
			T *old_arr = this->arr;
			this->arr = new T[this->size];
			// if we are increasing the size, copy the entire current
			// array; otherwise copy until the size of the new array
			int to = (num > div) ? old_size : this->size;
			for (int i = 0; i < to; i++) { 
				this->arr[i] = old_arr[(this->head + i) % old_size];
			}
			this->head = 0;
			this->tail = to - 1;
			delete [] old_arr;
		}

		// allows circling once the back of the array is reached
		// only to be used on the head and tail pointers !
		inline void increment_pointer(int *ptr) const {
			++*ptr;
			if (*ptr == this->size) {
				*ptr = 0;
			}
		}

		// allows circling once the front of the array is reached
		// only to be used on the head and tail pointers !
		inline void decrement_pointer(int *ptr) const {
			if (*ptr == 0) {
				*ptr = this->size;
			}
			--*ptr;
		}
	};
}

#endif
