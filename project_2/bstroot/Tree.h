#ifndef _TREE_H_
#define _TREE_H_

#include <stdexcept>
#include "KVPair.h"
#include <queue>

namespace cop3530 {
	template<typename key_t, typename value_t, bool (*cmp_func)(const key_t &, const key_t &), bool (*eq_func)(const key_t &, const key_t &)>
		class Tree {
	protected:
		class Node {
		public:
			key_t key;
			value_t value;
			Node *left;
			Node *right;

			Node(key_t key, value_t value) {
				this->key = key;
				this->value = value;
				this->left = nullptr;
				this->right = nullptr;
			}

			bool operator ==(const Node &other) {
				return eq_func(this->key, other.key);
			}

			bool operator ==(const key_t &other) {
				return eq_func(this->key, other);
			}

			bool operator !=(const Node &other) {
				return ! eq_func(this->key, other.key);
			}

			bool operator !=(const key_t &other) {
				return ! eq_func(this->key, other);
			}

			bool operator >(const Node &other) {
				return cmp_func(this->key, other.key);
			}

			bool operator >(const key_t &other) {
				return cmp_func(this->key, other);
			}

			int count() {
				int r = (this->right) ? this->right->count() : 0;
				int l = (this->left) ? this->left->count() : 0;
				return r + l + 1;
			}

			int height() {
				if (this->left && this->right) {
					int r = this->right->height() + 1;
					int l = this->left->height() + 1;
					return (r > l) ? r : l;
				}
				if (this->right) {
					return this->right->height() + 1;
				}
				if (this->left) {
					return this->left->height() + 1;
				}
				return 1;
			}

			int balance() {
				int r = (this->right) ? this->right->height() : 0;
				int l = (this->left) ? this->left->height() : 0;
				return l - r;
			}

			void clear() {
				if (this->left) {
					this->left->clear();
					this->left = nullptr;
				}
				if (this->right) {
					this->right->clear();
					this->right = nullptr;
				}
				delete(this);
			}

			Node *duplicate() {
				Node *node = new Node(this->key, this->value);
				if (this->right)
					node->right = this->right->duplicate();
				if (this->left)
					node->left = this->left->duplicate();
				return node;
			}
		};
		
		Node *root;

		void insert_at_leaf(key_t key, value_t value) {
			Node **iter = &(this->root);
			while (*iter) {
				if (**iter == key) {
					(*iter)->value = value;
					return;
				} else if (**iter > key) {
					iter = &((*iter)->left);
				} else {
					iter = &((*iter)->right);
				}
			}
			*iter = new Node(key, value);
		}

		void insert_at_root(Node *&root, key_t key, value_t value) {
			if (root == nullptr) {
				root = new Node(key, value);
			} else if (*root > key) {
				this->insert_at_root(root->left, key, value);
				this->rotate_right(root);
			} else {
				this->insert_at_root(root->right, key, value);
				this->rotate_left(root);
			}
		}

		void rotate_left(Node *&root) {
			if (root == nullptr || root->right == nullptr)
				throw std::runtime_error("rotate_left: No right child to take place");
			Node *tmp = root;
			root = root->right;
			tmp->right = root->left;
			root->left = tmp;
		}
		
		void rotate_right(Node *&root) {
			if (root == nullptr || root->left == nullptr)
				throw std::runtime_error("rotate_right: No left child to take place");
			Node *tmp = root;
			root = root->left;
			tmp->left = root->right;
			root->right = tmp;
		}

		
		void remove_from_tree(const key_t &key) {
			if (this->is_empty()) {
				throw std::runtime_error("remove: Map is empty");
			}
			// having a double pointer allows us to change where the
			// variable in the previous link points to without keeping
			// another variable
			Node **iter = &(this->root);
			while (*iter) {
				if (**iter == key) {
					break;
				} else if (**iter > key) {
					iter = &((*iter)->left);
				} else {
					iter = &((*iter)->right);
				}
			}
			if (*iter == nullptr)
				throw std::runtime_error("remove: Key not found");
			if ((*iter)->left == nullptr && (*iter)->right == nullptr) {
				// if the item has no children, simply delete the link
				*iter = nullptr;
				delete *iter;
			} else if ((*iter)->right && (*iter)->left) {
				// if the item has two children, find the node who's
				// IOP is the node to delete, swap it's value, and
				// delete the found node
				if ((*iter)->right->left) {
					// walk down one to the right, and then keep going
					// on the left trees until you reach the bottom
					// node, and remove it
					Node *to_replace = *iter;
					iter = &((*iter)->right->left);
					while ((*iter)->left) {
						iter = &((*iter)->left);
					}
					to_replace->value = (*iter)->value;
					to_replace->key = (*iter)->key;
					Node *tmp = *iter;
					*iter = (*iter)->right;
					delete tmp;
				} else {
					// when there is no left child, you can simply
					// make that node replace the deleted one
					Node *tmp = *iter;
					*iter = (*iter)->right;
					(*iter)->left = tmp->left;
					delete tmp;
				}
			} else {
				// if the node has one child, set it's child to
				// replace the deleted node
				Node *tmp = *iter;
				if ((*iter)->right) {
					*iter = (*iter)->right;
				} else {
					*iter = (*iter)->left;
				}
				delete tmp;
			}
		}

		
	public:
		class iterator {
		private:
			const Tree *parent;
			bool done;
			std::queue<Node *> queue;
			Node *current;

			using pair_t = KVPair<key_t, value_t>;
		public:
		iterator(const Tree *parent, bool done) : parent(parent) {
				this->done = done;
				this->current = nullptr;
				if (! done) {
					this->current = parent->root;
					this->queue.push(parent->root);
				}
				
			}

			iterator &operator ++ () {
				if (this->done) {
					throw std::runtime_error("iterator: Exhausted");
				}
				if (this->queue.empty()) {
					this->done = true;
				} else {
					this->current = this->queue.front();
					this->queue.pop();
					if (this->current->left)
						this->queue.push(this->current->left);
					if (this->current->right)
						this->queue.push(this->current->right);
				}
				return *this;
			}

			iterator &operator ++ (int) {
				iterator tmp = *this;
				++(*this);
				return tmp;
			}

			bool operator == (const iterator &other) {
				if (this->parent != other.parent)
					return false;
				if (this->done)
					return other.done;
				if (other.done)
					return this->done;
				return this->current == other.current;
			}

			bool operator != (const iterator &other) {
				return ! operator==(other);
			}

			iterator &operator = (const pair_t &other) {
				if (this->done) {
					throw std::runtime_error("iterator: Exhausted");
				}
				this->current->key = other.key;
				this->current->value = other.value;
				return *this;
			}
			
			pair_t operator * () {
				if (this->done) {
					throw std::runtime_error("iterator: Exhausted");
				}
				pair_t p(this->current->key, this->current->value);
				return p;
			}
			
		};
		
		using size_t = std::size_t;
		using key_type = key_t;
		using value_type = value_t;
		using const_iterator = iterator;
		
		Tree () {
			this->root = nullptr;
		}
		
		virtual void insert(const key_t, const value_t) = 0;
		virtual void remove(const key_t &) = 0;

		iterator begin() {
			return iterator(this, false);
		}

		iterator end() {
			return iterator(this, true);
		}
		
		value_t &lookup(const key_t &key) const {
			if (this->is_empty()) {
				throw std::runtime_error("lookup: Map is empty");
			}
			Node *iter = this->root;
			while (iter) {
				if (*iter == key) {
					return iter->value;
				} else if (*iter > key) {
					iter = iter->left;
				} else {
					iter = iter->right;
				}
			}
			throw std::runtime_error("lookup: Key not found");
		}
		
		bool contains(const key_t &key) const {
			if (this->is_empty()) {
				return false;
			}
			Node *iter = this->root;
			while (iter) {
				if (*iter == key) {
					return true;
				} else if (*iter > key) {
					iter = iter->left;
				} else {
					iter = iter->right;
				}
			}
			return false;
		}

		bool is_full() const {
			return false;
		}
		
		virtual int size() const {
			if (this->is_empty())
				return 0;
			return this->root->count();
		}

		int height() const {
			if (this->is_empty())
				return 0;
			return this->root->height();
		}

		int balance() const {
			if (this->is_empty())
				return 0;
			return this->root->balance();
		}
			
		bool is_empty() const {
			return this->root == nullptr;
		}
		
		virtual void clear() {
			if (this->is_empty())
				return;
			this->root->clear();
			this->root = nullptr;
		}
		
		~Tree() {
			this->clear();
		}
	};
}

#endif
