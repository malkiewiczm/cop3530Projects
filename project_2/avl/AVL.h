#ifndef _AVL_H_
#define _AVL_H_

#include "Tree.h"
#include <stack>

namespace cop3530 {
	template<typename key_t, typename value_t, bool (*cmp_func)(const key_t &, const key_t &), bool (*eq_func)(const key_t &, const key_t &)>
		class AVL : public Tree<key_t, value_t, cmp_func, eq_func> {
	protected:
		using node_t = typename Tree<key_t, value_t, cmp_func, eq_func>::Node;
	public:
		
		AVL() {
		}

		AVL(const AVL &other) {
			this->root = other.root->duplicate();
		}

		AVL(const AVL &&other) {
			this->root = other.root;
		}

		AVL &operator = (const AVL &other) {
			this->root = other.root->duplicate();
			return *this;
		}
		
		AVL &operator = (const AVL &&other) {
			this->root = other.root;
			return *this;
		}

		void insert(const key_t key, const value_t value) {
			node_t **iter = &(this->root);
			// this stack will store the nodes we visited, so we can
			// go back and fix the balance factors later
			std::stack<node_t **> visited;
			while (*iter) {
				if (**iter == key) {
					(*iter)->value = value;
					return;
				}
				visited.push(iter);
				if (**iter > key) {
					iter = &((*iter)->left);
				} else {
					iter = &((*iter)->right);
				}
			}
			*iter = new node_t(key, value);
			// if we visted one or less nodes, then there are no
			// parent or grandparent to perform rotations on
			if (visited.size() <= 1)
				return;
			int root_bf;
			// rotations are performed on the parent and grandparent
			node_t **grand_parent = nullptr;
			node_t **parent = nullptr;
			while (! visited.empty()) {
				parent = grand_parent;
				grand_parent = visited.top();
				visited.pop();
				root_bf = (*grand_parent)->balance();
				// exit the loop once the balance factor is off
				if (root_bf == 2 || root_bf == -2)
					break;
			}
			int child_bf = (*parent)->balance();
			// do the rotation depending on the grandparent and it's
			// child's balance factors
			if (root_bf == 2) {
				if (child_bf == 1) {
					// LL
					this->rotate_right(*grand_parent);
				} else {
					// LR
					this->rotate_left(*parent);
					this->rotate_right(*grand_parent);
				}
			} else if (root_bf == -2) {
				if (child_bf == 1) {
					// RL
					this->rotate_right(*parent);
					this->rotate_left(*grand_parent);
				} else {
					// RR
					this->rotate_left(*grand_parent);
				}
			}
		}
		
		void remove(const key_t &key) {
			if (this->is_empty()) {
				throw std::runtime_error("remove: Map is empty");
			}
			// having a double pointer allows us to change where the
			// variable in the previous link points to without keeping
			// another variable
			node_t **iter = &(this->root);
			// this stack will store the nodes we visited, so we can
			// go back and fix the balance factors later
			std::stack<node_t **> visited;
			while (*iter) {
				if (**iter == key) {
					break;
				}
				visited.push(iter);
				if (**iter > key) {
					iter = &((*iter)->left);
				} else {
					iter = &((*iter)->right);
				}
			}
			if (*iter == nullptr)
				throw std::runtime_error("remove: Key not found");
			if ((*iter)->left == nullptr && (*iter)->right == nullptr) {
				// if the item has no children, simply delete the link
				delete *iter;
				*iter = nullptr;
			} else if ((*iter)->right && (*iter)->left) {
				// if the item has two children, find the node who's
				// IOP is the node to delete, swap it's value, and
				// delete the found node
				visited.push(&((*iter)->right));
				if ((*iter)->right->left) {
					// walk down one to the right, and then keep going
					// on the left trees until you reach the bottom
					// node, and remove it
					node_t *to_replace = *iter;
					iter = &((*iter)->right->left);
					visited.push(iter);
					while ((*iter)->left) {
						iter = &((*iter)->left);
						visited.push(iter);
					}
					to_replace->value = (*iter)->value;
					to_replace->key = (*iter)->key;
					node_t *tmp = *iter;
					*iter = (*iter)->right;
					delete tmp;
				} else {
					// when there is no left child, you can simply
					// make that node replace the deleted one
					node_t *tmp = *iter;
					*iter = (*iter)->right;
					(*iter)->left = tmp->left;
					delete tmp;
				}
			} else {
				// if the node has one child, set it's child to
				// replace the deleted node
				node_t *tmp = *iter;
				if ((*iter)->right) {
					*iter = (*iter)->right;
				} else {
					*iter = (*iter)->left;
				}
				delete tmp;
			}
			// remove the last visited node from the stack, since it
			// is deleted now
			visited.pop();
			// if we visted one or less nodes, then there are no
			// parent or grandparent to perform rotations on
			if (visited.size() <= 1)
				return;
			int root_bf;
			// rotations are performed on the parent and grandparent
			node_t **grand_parent = nullptr;
			node_t **parent = nullptr;
			while (! visited.empty()) {
				parent = grand_parent;
				grand_parent = visited.top();
				visited.pop();
				root_bf = (*grand_parent)->balance();
				// exit the loop once the balance factor is off
				if (root_bf == 2 || root_bf == -2)
					break;
			}
			if (parent == nullptr) {
				if (root_bf == 2) {
					parent = &((*grand_parent)->left);
				} else {
					parent = &((*grand_parent)->right);
				}
			}
			int child_bf = (*parent)->balance();
			// do the rotation depending on the grandparent and it's
			// child's balance factors
			if (root_bf == 2) {
				if (child_bf == 1) {
					// LL
					this->rotate_right(*grand_parent);
				} else {
					// LR
					this->rotate_left(*parent);
					this->rotate_right(*grand_parent);
				}
			} else if (root_bf == -2) {
				if (child_bf == 1) {
					// RL
					this->rotate_right(*parent);
					this->rotate_left(*grand_parent);
				} else {
					// RR
					this->rotate_left(*grand_parent);
				}
			}
		}
	};
}

#endif
