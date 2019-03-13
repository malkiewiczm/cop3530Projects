#ifndef _CDAL_H
#define _CDAL_H

#include <stdexcept>
#include "List.h"

namespace cop3530 {
template <typename T>
class CDAL : public List<T> {
public:
	class iterator;
private:
	class Node {
	public:
		Node *next;
		T arr[50];
		Node () {
			this->next = nullptr;
		}
	};

	void dispose() {
		this->count = 0;
		Node *iter = this->head;
		while (iter) {
			Node *tmp = iter->next;
			delete iter;
			iter = tmp;
		}
	}

	Node *head;
	Node *tail;
	int count;
	int node_count;
public:
	class iterator {
	private:
		const CDAL<T> *parent;
		Node *node;
		int index;
		int abs_position;
		
		// this method only moves ahead; it's essentially the same as
		// calling increment a bunch of time, but more efficient since
		// it can skip entire nodes in the linked list
		void move_to(int position) {
			if (position < this->abs_position)
				throw std::runtime_error("CDAL: iterator: move_to: requested move backwards");
			if (position < 0 || position > this->parent->count)
				throw std::runtime_error("CDAL: iterator: move_to: out of range");
			int lists_ahead = position / 50;
			while (lists_ahead) {
				this->node = this->node->next;
				--lists_ahead;
			}
			this->index = position % 50;
			this->abs_position = position;
		}
	public:
		using value_type = T;
		using reference = T&;
		using pointer = T*;
		using difference_type = std::ptrdiff_t;
		using iterator_category = std::forward_iterator_tag;
			
		iterator (const CDAL<T> *parent)
			: parent(parent), node(parent->head), index(0), abs_position(0) {}

		iterator (const CDAL<T> *parent, int position)
			: parent(parent), node(parent->head), index(0), abs_position(0) {
			this->move_to(position);
		}
		
		iterator (const iterator &other)
			: parent(other.parent), node(other.node), index(other.index),
			abs_position(other.abs_position) {}

		static iterator make_end(const CDAL<T> *parent) {
			iterator iter(parent);
			iter.abs_position = parent->count;
			return iter;
		}

		bool operator == (const iterator &other) const {
			return this->abs_position == other.abs_position;
		}

		bool operator != (const iterator &other) const {
			return this->abs_position != other.abs_position;
		}

		T &operator * () const {
			return this->node->arr[this->index];
		}
		
		iterator &operator ++ () {
			this->index++;
			if (this->index == 50) {
				this->index = 0;
				this->node = this->node->next;
			}
			++this->abs_position;
			return *this;
		}

		iterator &operator ++ (int) {
			iterator tmp(*this);
			++(*this);
			return tmp;
		}

		T operator = (T value) {
			this->node->arr[this->index] = value;
			return value;
		}

		void print() {
			printf("%d\n", this->abs_position);
		}
	};

	iterator begin() {
		iterator i(this);
		return i;
	}

	iterator end() {
		return iterator::make_end(this);
	}

	iterator begin() const {
		iterator i(this);
		return i;
	}

	iterator end() const {
		return iterator::make_end(this);
	}
	
	CDAL() {
		this->count = 0;
		this->head = new Node();
		this->node_count = 1;
		this->tail = this->head;
	}

	CDAL (const CDAL &other) {
		this->count = 0;
		this->head = new Node();
		this->node_count = 1;
		this->tail = this->head;
		for (auto item : other) {
			this->push_back(item);
		}
	}

	CDAL (CDAL &&other) {
		this->count = other.count;
		this->head = other.head;
		this->node_count = other.node_count;
		this->tail = other.tail;
		other.head = nullptr;
		other.tail = nullptr;
	}

	CDAL &operator = (const CDAL &other) {
		this->count = 0;
		this->head = new Node();
		this->node_count = 1;
		this->tail = this->head;
		for (auto item : other) {
			this->push_back(item);
		}
		return *this;
	}

	CDAL &operator = (CDAL &&other) {
		this->count = other.count;
		this->head = other.head;
		this->node_count = other.node_count;
		this->tail = other.tail;
		other.head = nullptr;
		other.tail = nullptr;
		return *this;
	}
	
	void insert(T value, int position) {
		if (position < 0 || position > this->count)
			throw std::runtime_error("CDAL: Insert: Out of range");
		// when pushing to the tail, and there is not a linked list
		// node allocated yet
		if (! this->is_empty() && this->count % 50 == 0) {
			this->tail->next = new Node();
			++this->node_count;
			this->tail = this->tail->next;
		}
		++this->count;
		// when we push to the tail, we have no items to shift
		iterator iter0(this, position);
		if (position == this->count - 1) {
			iter0 = value;
			return;
		}
		// if we are not pushing to the tail, we must shift all items;
		// a temporary array is made to act as a queue, so all items
		// are added to the queue, and then re added to the CDAL
		iterator end = this->end();
		T *faux_queue = new T[this->count - position + 1];
		faux_queue[0] = value;
		for (int i = 1 ; iter0 != end; ++iter0, ++i) {
			faux_queue[i] = *iter0;
		}
		// putting items back in the list
		iterator iter1(this, position);
		for (int i = 0; iter1 != end; ++iter1, ++i) {
			iter1 = faux_queue[i];
		}
		delete [] faux_queue;
	}
	
	void push_front(T value) {
		this->insert(value, 0);
	}
	
	void push_back(T value) {
		this->insert(value, this->count);
	}
	
	void replace(T value, int position) {
		iterator iter(this, position);
		iter = value;
	}
	
	T remove(int position) {
		if (position < 0 || position >= this->count) {
			throw std::runtime_error("remove: Out of range");
		}
		iterator iter(this, position);
		T ret = *iter;
		// when we remove the last item, we do not need to shift any
		// items
		if (position == this->count - 1) {
			--this->count;
			return ret;
		}
		iterator ahead(this, position + 1);
		iterator end = this->end();
		// shifting items left
		for (; iter != end, ahead != end; ++ahead, ++iter) {
			iter = *ahead;
		}
		--this->count;
		// policy to check de-allocate the extra node if there is one
		if (this->node_count - (this->count / 50) >= 2) {
			int i = this->node_count;
			--this->node_count;
			Node *walk = this->head;
			// only delete the last node in the list
			while (walk) {
				--i;
				if (i == 0) {
					delete walk;
					break;
				} else {
					walk = walk->next;
				}
			}
		}
		return ret;
	}
	
	T pop_front() {
		return this->remove(0);
	}
	
	T pop_back() {
		return this->remove(this->count - 1);
	}
	
	T& item_at(int position) const {
		iterator iter(this, position);
		return *iter;
	}
	
	T& peek_front() const {
		if (this->is_empty()) {
			throw std::runtime_error("peek_front: List is empty");
		}
		return this->head->arr[0];
	}
	
	T& peek_back() const {
		if (this->is_empty()) {
			throw std::runtime_error("peek_front: List is empty");
		}
		return this->tail->arr[(this->count - 1) % 50];
	}
	
	bool is_empty() const {
		return this->count == 0;
	}
	
	bool is_full() const {
		return false;
	}
	
	int length() const {
		return this->count;
	}

	void clear() {
		this->dispose();
		this->count = 0;
		this->head = new Node();
		this->node_count = 1;
		this->tail = this->head;
	}
	
	bool contains(T value, bool (*equal_function)(const T&, const T&)) const {
		iterator i = this->begin();
		iterator end = this->end();
		for (; i != end; ++i) {
			if (equal_function(*i, value))
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
		iterator end = iterator(this, this->count - 1);
		for (iterator iter = this->begin(); iter != end; ++iter) {
			stream << *iter << ",";
		}
		stream << *end << "]";
		return stream;
	}
	
	T* contents() const {
		T *ret = new T[this->length()];
		iterator iter = this->begin();
		iterator end = this->end();
		int i = 0;
		for (; iter != end; ++iter, ++i) {
			ret[i] = *iter;
		}
		return ret;
	}
	
	~CDAL() {
		this->dispose();
	}
};
}

#endif
