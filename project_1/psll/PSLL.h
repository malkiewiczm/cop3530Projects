#ifndef _PSLL_H
#define _PSLL_H

#include <stdexcept>
#include "SSLL.h"

namespace cop3530 {
	template <typename T>
	class PSLL : public SSLL<T> {
	protected:
		using node_t = typename SSLL<T>::Node;
		
		virtual node_t *alloc_node(T value) {
			// policy when the base class tries to allocate a node;;
			// check the pull and if it's not empty, grab one from
			// there
			if (this->pool_count) {
				node_t *node = this->pool;
				// the linked list essentially functions as a stack
				this->pool = this->pool->next;
				--this->pool_count;
				node->value = value;
				node->next = nullptr;
				return node;
			} else {
				return new node_t(value);
			}
		}

		virtual void node_dispose(node_t *node) {
			// policy when the base class tries to de-allocafe a
			// node;; put it in a pull
			node->next = this->pool;
			this->pool = node;
			++this->pool_count;
			int len = this->length();
			// when the pool is too large, delete the extras
			if (len >= 100 && this->pool_count * 2 > len) {
				int target = this->pool_count / 2;
				while (this->pool_count != target) {
					node_t *tmp = this->pool;
					this->pool = pool->next;
					delete tmp;
					--this->pool_count;
				}
			}
		}
	private:
		node_t *pool;
		int pool_count;
	public:
		PSLL() {
			this->pool = nullptr;
			this->pool_count = 0;
		}

		~PSLL() {
			this->clear();
			// must de-allocate the pool when we are finished
			node_t *iter = this->pool;
			while (iter) {
				node_t *tmp = iter;
				iter = iter->next;
				delete tmp;
			}
		}
	};
}

#endif
