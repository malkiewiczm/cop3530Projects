#ifndef _BSTLEAF_H_
#define _BSTLEAF_H_

#include "Tree.h"

namespace cop3530 {
	template<typename key_t, typename value_t, bool (*cmp_func)(const key_t &, const key_t &), bool (*eq_func)(const key_t &, const key_t &)>
		class BSTLEAF : public Tree<key_t, value_t, cmp_func, eq_func> {
	protected:
		using node_t = typename Tree<key_t, value_t, cmp_func, eq_func>::Node;
	public:
		
		BSTLEAF() {
		}

		BSTLEAF(const BSTLEAF &other) {
			this->root = other.root->duplicate();
		}

		BSTLEAF(const BSTLEAF &&other) {
			this->root = other.root;
		}

		BSTLEAF &operator = (const BSTLEAF &other) {
			this->root = other.root->duplicate();
			return *this;
		}
		
		BSTLEAF &operator = (const BSTLEAF &&other) {
			this->root = other.root;
			return *this;
		}
		
		void insert(const key_t key, const value_t value) {
			this->insert_at_leaf(key, value);
		}

		void remove(const key_t &key) {
			this->remove_from_tree(key);
		}
	};
}

#endif
