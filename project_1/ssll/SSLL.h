#ifndef _SSLL_H
#define _SSLL_H

#include <stdexcept>
#include "List.h"
#include <stdio.h>

namespace cop3530 {
	template <typename T>
	class SSLL : public List<T> {
	protected:
		class Node {
		public:
			Node() {
				this->next = nullptr;
			}

			Node (T value) {
				this->value = value;
				this->next = nullptr;
			}
			
			T value;
			
			Node *next;
		};

		virtual Node *alloc_node(T value) {
			return new Node(value);
		}

		virtual void node_dispose(Node *node) {
			delete node;
		}
		
		Node *head;
		Node *tail;
	public:
		class iterator {
		private:
			const SSLL<T> *parent;
			Node *here;
		public:
			using value_type = T;
			using reference = T&;
			using pointer = T*;
			using difference_type = std::ptrdiff_t;
			using iterator_category = std::forward_iterator_tag;
			
			iterator (SSLL<T> *parent, Node *where)
				: parent(parent), here(where) {}

			iterator (const iterator &other)
				: parent(other.parent), here(other.here) {}
		
			bool operator != (const iterator &other) const {
				return ! (operator == (other)); 
			}

			bool operator == (const iterator &other) const {
				return this->parent == other.parent && this->here == other.here;
			}

			iterator &operator ++ () {
				this->here = this->here->next;
				return *this;
			}

			iterator &operator ++ (int) {
				iterator tmp(*this);
				++(*this);
				return tmp;
			}

			T operator * () const {
				return this->here->value;
			}

			iterator &operator = (T value) {
				this->here->value = value;
				return *this;
			}
		};

		iterator begin() {
			iterator i(this, this->head);
			return i;
		}

		iterator end() {
			iterator i(this, nullptr);
			return i;
		}

		const iterator begin() const {
			const iterator i(this, this->head);
			return i;
		}

		const iterator end() const {
			const iterator i(this, nullptr);
			return i;
		}
		
		SSLL() {
			this->head = nullptr;
			this->tail = nullptr;
		}

		SSLL(const SSLL &other) {
			this->head = nullptr;
			this->tail = nullptr;
			Node *walk = other.head;
			Node *iter = this->head;
			// walk through the entire list and copy all nodes 
			while (walk) {
				if (iter) {
					iter->value = walk->value;
				} else {
					iter = new Node(walk->value);
					this->head = iter;
				}
				if (walk->next) {
					walk = walk->next;
					iter->next = new Node();
					iter = iter->next;
				} else {
					break;
				}
			}
		}

		SSLL(SSLL &&other) {
			this->head = other.head;
			other.head = nullptr;
			this->tail = other.tail;
			other.tail = nullptr;
		}

		SSLL &operator = (const SSLL &other) {
			this->head = nullptr;
			this->tail = nullptr;
			Node *walk = other.head;
			Node *iter = this->head;
			// walk through the entire list and copy all nodes 
			while (walk) {
				if (iter) {
					iter->value = walk->value;
				} else {
					iter = new Node(walk->value);
					this->head = iter;
				}
				if (walk->next) {
					walk = walk->next;
					iter->next = new Node();
					iter = iter->next;
				} else {
					break;
				}
			}
			return *this;
		}

		SSLL &operator = (SSLL &&other) {
			this->head = other.head;
			other.head = nullptr;
			this->tail = other.tail;
			other.tail = nullptr;
			return *this;
		}
		
		void insert(T value, int position) {
			if (this->is_empty() || position == 0) {
				this->push_front(value);
				return;
			}
			int i = 0;
			Node *iter = this->head;
			while (iter) {
				// put the new node at the position, and hook it in
				// the chain
				if (i == position) {
					T tmp = iter->value;
					iter->value = value;
					Node *new_node = this->alloc_node(tmp);
					new_node->next = iter->next;
					iter->next = new_node;
					return;
				}
				iter = iter->next;
				i++;
			}
			if (i == position) {
				this->push_back(value);
				return;
			}

			throw std::runtime_error("insert: Position out of range");
		}

		void push_front(T value) {
			// if there are no items in the list, set the head to be
			// the new node;; otherwise 
			if (this->is_empty()) {
				this->head = this->alloc_node(value);
				this->tail = this->head;
			} else {
				Node *new_node = this->alloc_node(value);
				new_node->next = this->head;
				this->head = new_node;
			}
		}
		
		void push_back(T value) {
			// if there are no items in the list, set the head to be
			// the new node
			if (this->is_empty()) {
				this->head = this->alloc_node(value);
				this->tail = this->head;
			} else {
				Node *new_node = alloc_node(value);
				this->tail->next = new_node;
				this->tail = new_node;
			}
		}
	
		void replace(T value, int position) {
			if (this->is_empty()) {
				throw std::runtime_error("replace: List is empty");
			}
			Node *iter = this->head;
			int i = 0;
			while (iter) {
				// set the value of the correct position to the new
				// value
				if (i == position) {
					iter->value = value;
					return;
				}
				iter = iter->next;
				i++;
			}
			throw std::runtime_error("replace: Out of range");
		}
		
		T remove(int position) {
			if (this->is_empty()) {
				throw std::runtime_error("remove: List is empty");
			}
			// if the front item is requested, there is no need to
			// start to iterate through the list
			if (position == 0) {
				return this->pop_front();
			}
			Node **iter = &(this->head);
			int i = 0;
			while (*iter) {
				if (i == position) {
					Node *tmp = *iter;
					T ret = tmp->value;
					// hook the previous node to the next node
					*iter = (*iter)->next;
					this->node_dispose(tmp);
					return ret;
				}
				iter = &((*iter)->next);
				i++;
			}
			throw std::runtime_error("remove: Out of range");
		}
		
		T pop_back() {
			if (this->is_empty()) {
				throw std::runtime_error("pop_back: List is empty");
			}
			T ret = this->tail->value;
			Node *iter = this->head;
			// even with a tail pointer, we must still go through the
			// entire list in order to ensure that the node before
			// doesn't link to a de-allocated node
			while (iter->next != this->tail) {
				iter = iter->next;
			}
			this->node_dispose(this->tail);
			this->tail = iter;
			this->tail->next = nullptr;
			return ret;
		}
		
		T pop_front() {
			if (this->is_empty()) {
				throw std::runtime_error("pop_front: List is empty");
			}
			T ret = this->head->value;
			// put the head at the node after;; if the node after is
			// null, then the head becomes null
			Node *new_head = this->head->next;
			this->node_dispose(this->head);
			this->head = new_head;
			return ret;
		}
		
		T& item_at(int position) const {
			if (this->is_empty()) {
				throw std::runtime_error("item_at: List is empty");
			}
			int i = 0;
			Node *iter = this->head;
			// walk through the list and grab the item at the position
			while (iter) {
				if (i == position)
					return iter->value;
				iter = iter->next;
				i++;
			}
			throw std::runtime_error("item_at: Out of range");
		}
		
		T& peek_front() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_front: List is empty");
			}
			return this->head->value;
		}
		
		T& peek_back() const {
			if (this->is_empty()) {
				throw std::runtime_error("peek_back: List is empty");
			}
			return this->tail->value;
		}
		
		bool is_empty() const {
			return this->head == nullptr;
		}
		
		bool is_full() const {
			return false;
		}
		
		int length() const {
			int len = 0;
			Node *iter = this->head;
			while (iter) {
				len++;
				iter = iter->next;
			}
			return len;
		}
		
		void clear() {
			Node *iter = this->head;
			while (iter) {
				Node *target = iter;
				iter = iter->next;
				this->node_dispose(target);
			}
			this->head = nullptr;
			this->tail = nullptr;
		}
		
		bool contains(T value, bool (*equal_function)(const T&, const T&)) const {
			Node *iter = this->head;
			while (iter) {
				if (equal_function(value, iter->value))
					return true;
				iter = iter->next;
			}
			return false;
		}
		
		std::ostream &print(std::ostream &stream) const {
			if (this->is_empty()) {
				stream << "<empty list>";
				return stream;
			}
			stream << '[';
			Node *iter = this->head;
			while (iter) {
				stream << iter->value;
				if (iter->next)
					stream << ",";
				iter = iter->next;
			}
			stream << ']';
			return stream;
		}
		
		T* contents() const {
			int len = this->length();
			T *arr = new T[len];
			int i = 0;
			Node *iter = this->head;
			while (iter) {
				arr[i] = iter->value;
				iter = iter->next;
				i++;
			}
			return arr;
		}
		
		virtual ~SSLL() {
			this->clear();
		}
	};
}


#endif
