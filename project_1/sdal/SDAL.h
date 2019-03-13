#ifndef _SDAL_H
#define _SDAL_H

#include <stdexcept>
#include <stdio.h>
#include "List.h"

namespace cop3530 {
	template <typename T>
	class SDAL : public List<T> {
	public:
		class iterator {
		private:
			SDAL *const parent;
			// `here` refers to the absolute index in the array, which
			// also corresponds to the position in the list
			int here;
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			iterator(SDAL *parent, int where)
				: parent(parent) {
				this->here = where;
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
				this->here ++;
				return *this;
			}

			iterator &operator ++ (int) {
				iterator tmp(*this);
				++(*this);
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
			iterator i(this, this->tail);
			return i;
		}

		const iterator end() const {
			const iterator i(this, this->tail);
			return i;
		}

		const iterator begin() const {
			const iterator i(this, 0);
			return i;
		}
		
	SDAL(int size) : starting_capacity(size) {
			this->size = size;
			this->tail = 0;
			this->arr = new T[size];
		}

		SDAL () : SDAL(50) {}

		SDAL (const SDAL &other) : starting_capacity(other.starting_capacity) {
			this->size = other.size;
			this->tail = other.tail;
			this->arr = new T[this->size];
			for (int i = 0; i < this->tail; ++i) {
				this->arr[i] = other.arr[i];
			}
		}

		SDAL (SDAL &&other) : starting_capacity(other.starting_capacity) {
			this->tail = other.tail;
			this->size = other.size;
			this->arr = other.arr;
			other.arr = nullptr;
		}

		SDAL &operator = (const SDAL &other) {
			this->size = other.size;
			this->tail = other.tail;
			this->arr = new T[this->size];
			for (int i = 0; i < this->tail; ++i) {
				this->arr[i] = other.arr[i];
			}
			return *this;
		}

		SDAL &operator = (SDAL &&other) {
			this->starting_capacity = other.starting_capacity;
			this->tail = other.tail;
			this->size = other.size;
			this->arr = other.arr;
			other.arr = nullptr;
			return *this;
		}
		
		void insert(T item, int position) {
			if (position < 0 || position > this->tail) {
				throw std::runtime_error("insert: Position out of range");
			}
			// if we are out of space, make more room
			while (position >= this->size || this->tail + 1 >= this->size) {
				this->size_change_by_ratio(3, 2);
			}
			// shift the array to the right
			for (int i = this->tail - 1; i >= position; i--) {
				this->arr[i + 1] = this->arr[i];
			}
			this->arr[position] = item;
			++this->tail;
		}
		
		void push_front(T item) {
			this->insert(item, 0);
		}
		
		void push_back(T item) {
			this->insert(item, this->tail);
		}
		
		void replace(T item, int position) {
			if (position < 0 || position > this->tail) {
				throw std::runtime_error("replace: Position out of range");
			}
			this->arr[position] = item;
		}
		
		T remove(int position) {
			if (position < 0 || position >= this->tail) {
				throw std::runtime_error("remove: Out of range");
			}
			T ret = this->arr[position];
			this->tail --;
			// shift the array to the left
			for (int i = position; i < this->tail; i++) {
				this->arr[i] = this->arr[i + 1];
			}
			// if we have too much space, re-allocate
			if (this->size > this->starting_capacity && this->tail * 2 <= this->size) {
				this->size_change_by_ratio(3, 4);
			}
			return ret;
		}
		
		T pop_front() {
			return this->remove(0);
		}
		
		T pop_back() {
			return this->remove(this->tail - 1);
		}
		
		T& item_at(int position) const {
			if (position < 0 || position > this->tail) {
				throw std::runtime_error("item_at: Out of range");
			}
			return this->arr[position];
		}
		
		T& peek_front() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_front: List is empty");
			}
			return this->arr[0];
		}
		
		T& peek_back() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_back: List is empty");
			}
			return this->arr[this->tail - 1];
		}
		
		bool is_empty() const {
			return this->tail == 0;
		}
		
		bool is_full() const {
			return false;
		}
		
		int length() const {
			return this->tail;
		}
		
		void clear() {
			this->tail = 0;
		}
		
		bool contains(T item, bool (*equal_function)(const T&, const T&)) const {
			for (int i = 0; i < this->tail; i++) {
				if (equal_function(item, this->arr[i]))
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
			int i;
			for (i = 0; i < this->tail - 1; i++) {
				stream << this->arr[i] << ",";
			}
			stream << this->arr[i] << "]";
			return stream;
		}
		
		T* contents() const {
			T *ret = new T[this->tail];
			for (int i = 0; i < this->tail; i++) {
				ret[i] = this->arr[i];
			}
			return ret;
		}
		
		~SDAL() {
			delete [] this->arr;
		}
	private:
		T *arr;
		int size;
		int tail;
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
				this->arr[i] = old_arr[i];
			}
			delete [] old_arr;
		}
	};
}

#endif
