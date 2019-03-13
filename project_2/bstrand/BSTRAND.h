#ifndef _BSTRAND_H_
#define _BSTRAND_H_

#include "Tree.h"
#include <time.h>
#include <stdlib.h>

namespace cop3530 {
	template<typename key_t, typename value_t, bool (*cmp_func)(const key_t &, const key_t &), bool (*eq_func)(const key_t &, const key_t &)>
		class BSTRAND : public Tree<key_t, value_t, cmp_func, eq_func> {
	protected:
		using node_t = typename Tree<key_t, value_t, cmp_func, eq_func>::Node;
	private:
		int count;
	public:
		
		BSTRAND() {
			srand(time(NULL));
			this->count = 0;
		}

		BSTRAND(const BSTRAND &other) {
			this->root = other.root->duplicate();
		}

		BSTRAND(const BSTRAND &&other) {
			this->root = other.root;
		}

		BSTRAND &operator = (const BSTRAND &other) {
			this->root = other.root->duplicate();
			return *this;
		}
		
		BSTRAND &operator = (const BSTRAND &&other) {
			this->root = other.root;
			return *this;
		}
		
		void insert(const key_t key, const value_t value) {
			if (this->count == 0) {
				this->insert_at_root(this->root, key, value);
			} else if (rand() % this->count) {
				this->insert_at_leaf(key, value);
			} else {
				this->insert_at_root(this->root, key, value);
			}
			++this->count;
		}

		void remove(const key_t &key) {
			this->remove_from_tree(key);
			--this->count;
		}

		void clear() override {
			if (this->is_empty())
				return;
			this->root->clear();
			this->root = nullptr;
			this->count = 0;
		}
		
		int size() const override {
			return this->count;
		}
	};
}

#endif
