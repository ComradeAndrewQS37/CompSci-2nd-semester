#pragma once

#include <string>

#include "ListSequence.h"
#include "Concepts.h"

template <class T>
class BinaryNode
{
private:
	T key_;
	BinaryNode<T>* parent_;
	BinaryNode<T>* left_;
	BinaryNode<T>* right_;

public:

	BinaryNode(T key, BinaryNode<T>* parent = nullptr, BinaryNode<T>* left = nullptr, BinaryNode<T>* right = nullptr);
	BinaryNode(BinaryNode* old_node);
	~BinaryNode();

	T GetKey();
	void SetKey(T new_key);

	BinaryNode<T>* Left();
	BinaryNode<T>* Right();
	BinaryNode<T>* Parent();

	bool AddLeft(T new_key);
	bool AddRight(T new_key);
	void DeleteBranch();

	std::string BranchToString(std::string f);
	bool IsNodeFromThisBranch(BinaryNode<T>* node);
	void RemoveNode();
	bool IsSubBranch(BinaryNode<T>* node);

	
};

template<class T>
BinaryNode<T>::BinaryNode(T key, BinaryNode<T>* parent, BinaryNode<T>* left, BinaryNode<T>* right)
{
	key_ = key;
	parent_ = parent;


	if (left)
	{
		left_ = new BinaryNode<T>(left->key_, this, left->left_, left->right_);
	}
	else
	{
		left_ = nullptr;
	}

	if (right)
	{
		right_ = new BinaryNode<T>(right->key_, this, right->left_, right->right_);
	}
	else
	{
		right_ = nullptr;
	}
}

template<class T>
BinaryNode<T>::BinaryNode(BinaryNode* old_node)
{
	parent_ = nullptr;
	key_ = old_node->key_;

	if (old_node->left_)
	{
		left_ = new BinaryNode<T>(old_node->left_->key_, this, old_node->left_->left_, old_node->left_->right_);
	}
	else
	{
		left_ = nullptr;
	}

	if (old_node->right_)
	{
		right_ = new BinaryNode<T>(old_node->right_->key_, this, old_node->right_->left_, old_node->right_->right_);
	}
	else
	{
		right_ = nullptr;
	}
}

template<class T>
BinaryNode<T>::~BinaryNode()
{
	if (parent_)
	{
		if (this == parent_->left_)
		{
			parent_->left_ = nullptr;
		}
		else
		{
			parent_->right_ = nullptr;
		}
	}
}

template<class T>
T BinaryNode<T>::GetKey()
{
	return key_;
}

template<class T>
void BinaryNode<T>::SetKey(T new_key)
{
	key_ = new_key;
}

template<class T>
BinaryNode<T>* BinaryNode<T>::Left()
{
	return left_;
}

template<class T>
BinaryNode<T>* BinaryNode<T>::Right()
{
	return right_;
}

template<class T>
BinaryNode<T>* BinaryNode<T>::Parent()
{
	return parent_;
}

template<class T>
bool BinaryNode<T>::AddLeft(T new_key)
{
	if (left_)
	{
		return false;
	}

	left_ = new BinaryNode<T>(new_key, this);
	return true;

}

template<class T>
bool BinaryNode<T>::AddRight(T new_key)
{
	if (right_)
	{
		return false;
	}

	right_ = new BinaryNode<T>(new_key, this);
	return true;

}

template<class T>
void BinaryNode<T>::DeleteBranch()
{
	auto* current_node = this;
	BinaryNode<T>* node_to_delete;
	while (current_node)
	{
		if (current_node->left_)
		{
			current_node = current_node->left_;
		}
		else if (current_node->right_)
		{
			current_node = current_node->right_;
		}
		else
		{
			node_to_delete = current_node;
			current_node = current_node->parent_;
			delete node_to_delete;
		}
	}
}

template <OrderedType T>
class BinarySearchTree
{

private:
	BinaryNode<T>* root_;

public:
	// constructors
	explicit BinarySearchTree(BinaryNode<T>* old_branch);
	explicit BinarySearchTree(BinarySearchTree<T>* old_tree);
	BinarySearchTree(T value);
	BinarySearchTree(T* values, int number_of_elements);

	BinarySearchTree& operator=(const BinarySearchTree& tree);

	// destructor
	~BinarySearchTree();

	// cast
	operator BinaryNode<T>() { return BinaryNode<T>(root_); }

	// decomposition
	BinaryNode<T>* Root() { return root_; }
	BinarySearchTree<T>* GetSubTree(BinaryNode<T>* branch);
	bool IsNodeFromThisTree(BinaryNode<T>* node);

	// to string
	std::string ToString(std::string format = "<K>(L)[R]");
	
	// operations
	bool Add(T value, BinaryNode<T>* node);
	void Add(T value);

	
	BinaryNode<T>* Find(T value, BinaryNode<T>* start_node = nullptr);
	BinaryNode<T>* Find(bool (*predicate)(T), BinaryNode<T>* start_node = nullptr);
	ListSequence<BinaryNode<T>*>* FindAll(T value, BinaryNode<T>* start_node = nullptr, ListSequence<BinaryNode<T>*>* saved_nodes = nullptr);
	ListSequence<BinaryNode<T>*>* FindAll(bool (*predicate)(T), BinaryNode<T>* start_node = nullptr, ListSequence<BinaryNode<T>*>* saved_nodes = nullptr);

	bool Remove(T value);
	bool Remove(bool (*predicate)(T));
	int RemoveAll(T value);
	int RemoveAll(bool (*predicate)(T));

	template <OrderedType U>
	BinarySearchTree<U>* Map(U (*transform)(T));
	T Reduce(T(*red_function)(T, T), const T& start);
	BinarySearchTree<T>* Where(bool (*filter)(T));

	bool IsSubTree(BinarySearchTree<T>* other_tree);
	
};


template <OrderedType T>
BinarySearchTree<T>::BinarySearchTree(BinaryNode<T>* old_branch)
{
	root_ = new BinaryNode<T>(old_branch);
}

template <OrderedType T>
BinarySearchTree<T>::BinarySearchTree(BinarySearchTree<T>* old_tree)
{
	root_ = new BinaryNode<T>(old_tree->root_);
}

template <OrderedType T>
BinarySearchTree<T>::BinarySearchTree(T value)
{
	root_ = new BinaryNode<T>(value);
}

template <OrderedType T>
BinarySearchTree<T>::BinarySearchTree(T* values, int number_of_elements)
{
	if (number_of_elements <= 0)
	{
		throw std::exception("Invalid number of elements: must be positive");
	}

	root_ = new BinaryNode<T>(*values);
	for (int i = 1; i < number_of_elements; i++)
	{
		Add(*(values + i));
	}
}

template <OrderedType T>
BinarySearchTree<T>& BinarySearchTree<T>::operator=(const BinarySearchTree& tree)
{
	if (root_ == tree.root_)
	{
		return *this;
	}
	if (root_)
	{
		root_->DeleteBranch();
	}

	root_ = new BinaryNode<T>(tree.root_);

	return *this;
}

template <OrderedType T>
BinarySearchTree<T>::~BinarySearchTree()
{
	root_->DeleteBranch();
}

template <OrderedType T>
BinarySearchTree<T>* BinarySearchTree<T>::GetSubTree(BinaryNode<T>* branch)
{
	if(this->IsNodeFromThisTree(branch))
	{
		return new BinarySearchTree<T>(branch);
	}

	throw std::invalid_argument("Passed node does not belong the tree");
}

template <class T>
bool BinaryNode<T>::IsNodeFromThisBranch(BinaryNode<T>* node)
{
	if (this == node)
	{
		return true;
	}


	bool is_in_left_branch;
	bool is_in_right_branch;


	if (Left())
	{
		is_in_left_branch = Left()->IsNodeFromThisBranch(node);

	}
	else
	{
		is_in_left_branch = false;
	}
	if (Right())
	{
		is_in_right_branch = Right()->IsNodeFromThisBranch(node);

	}
	else
	{
		is_in_right_branch = false;
	}

	return is_in_left_branch || is_in_right_branch;
}

template <OrderedType T>
bool BinarySearchTree<T>::IsNodeFromThisTree(BinaryNode<T>* node)
{
	return root_->IsNodeFromThisBranch(node);

}

template <class T>
std::string BinaryNode<T>::BranchToString(std::string f)
{
	std::string str = "";
	
	for (int i = 1; i < 9;) 
	{
		if (f[i] == 'K')
		{
			str.push_back(f[i - 1]);
			str.append(std::to_string(key_));
			str.push_back(f[i + 1]);

		}
		else if(f[i] == 'L' && left_)
		{
			str.push_back(f[i - 1]);
			str.append(left_->BranchToString(f));
			str.push_back(f[i + 1]);

			
		}else if(f[i] == 'R' && right_)
		{
			str.push_back(f[i - 1]);
			str.append(right_->BranchToString(f));
			str.push_back(f[i + 1]);

		}
		
		i += 3;
	}

	return str;
}

template <OrderedType T>
std::string BinarySearchTree<T>::ToString(std::string format)
{
	if (format.length() > 9 ||
		format[1] != 'K' && format[1] != 'L' && format[1] != 'R' ||
		format[4] != 'K' && format[4] != 'L' && format[4] != 'R' ||
		format[7] != 'K' && format[7] != 'L' && format[7] != 'R' )
	{
		throw std::invalid_argument("Invalid format:\
			format string must be \".K..L..R.\", where each . is any symbol, characters K,L,R can be swapped");
	}
	if(!root_)
	{
		throw std::invalid_argument("The tree was empty");
	}
	return this->root_->BranchToString(format);
}


template <OrderedType T>
bool BinarySearchTree<T>::Add(T value, BinaryNode<T>* node)
{
	if(!node)
	{
		return false;
	}
	auto node_key = node->GetKey();
	if (value < node_key)
	{
		if (node->Left())
		{
			return false;
		}

		node->AddLeft(new BinaryNode<T>(value));
		return true;
	}
	else
	{
		if (node->Right())
		{
			return false;
		}

		node->AddRight(new BinaryNode<T>(value));
		return true;
	}
	
}


template <OrderedType T>
void BinarySearchTree<T>::Add(T value)
{
	if(!root_)
	{
		root_ = new BinaryNode<T>(value);
		return;
	}
	
	auto current_node = root_;

	while (true)
	{
		if (value < current_node->GetKey())
		{
			if(current_node->Left())
			{
				current_node = current_node->Left();
			}
			else
			{
				current_node->AddLeft(value);
				return;
			}
		}else
		{
			if (current_node->Right())
			{
				current_node = current_node->Right();
			}
			else
			{
				current_node->AddRight(value);
				return;
			}
		}

	}
}



template <OrderedType T>
BinaryNode<T>* BinarySearchTree<T>::Find(T value, BinaryNode<T>* start_node)
{
	if (!root_)
	{
		return nullptr;
	}
	if (!start_node)
	{
		start_node = root_;
	}

	if (start_node->GetKey() == value)
	{
		return start_node;
	}

	if (value < start_node->GetKey() && start_node->Left())
	{
		return Find(value, start_node->Left());
	}

	if(start_node->Right())
	{
		return Find(value, start_node->Right());
	}

	return nullptr;
}

template <OrderedType T>
BinaryNode<T>* BinarySearchTree<T>::Find(bool (*predicate)(T), BinaryNode<T>* start_node)
{
	if (!root_)
	{
		return nullptr;
	}
	
	if (!start_node)
	{
		start_node = root_;
	}

	if (predicate(start_node->GetKey()))
	{
		return start_node;
	}

	if(start_node->Left())
	{
		auto* new_node = Find(predicate, start_node->Left());
		if (new_node)
		{
			return new_node;
		}
	}
	
	if(start_node->Right())
	{
		auto* new_node = Find(predicate, start_node->Right());
		return new_node;

	}

	return nullptr;
}

template <OrderedType T>
ListSequence<BinaryNode<T>*>* BinarySearchTree<T>::FindAll(T value, BinaryNode<T>* start_node, ListSequence<BinaryNode<T>*>* saved_nodes)
{
	if (!root_)
	{
		return nullptr;
	}
	
	if (!saved_nodes)
	{
		saved_nodes = new ListSequence<BinaryNode<T>*>();
	}
	if (!start_node)
	{
		start_node = root_;
	}
	if (start_node->GetKey() == value)
	{
		saved_nodes->Append(start_node);
	}

	if (value < start_node->GetKey() && start_node->Left())
	{
		FindAll(value, start_node->Left(), saved_nodes);
	}
	else if(start_node->Right())
	{
		FindAll(value, start_node->Right(), saved_nodes);
	}

	return saved_nodes;

}


template <OrderedType T>
ListSequence<BinaryNode<T>*>* BinarySearchTree<T>::FindAll(bool (*predicate)(T), BinaryNode<T>* start_node, ListSequence<BinaryNode<T>*>* saved_nodes)
{
	if (!root_)
	{
		return nullptr;
	}
	if (!saved_nodes)
	{
		saved_nodes = new ListSequence<BinaryNode<T>*>();
	}
	if (!start_node)
	{
		start_node = root_;
	}
	if (predicate(start_node->GetKey()))
	{
		saved_nodes->Append(start_node);
	}

	if(start_node->Left())
	{
		FindAll(predicate, start_node->Left(), saved_nodes);
	}
	if (start_node->Right()) 
	{
		FindAll(predicate, start_node->Right(), saved_nodes);
	}

	return saved_nodes;
}


template <class T>
void BinaryNode<T>::RemoveNode()
{
	if(!left_ && !right_)
	{
		delete this;
	}
	else if(!left_ && right_)
	{
		auto* to_delete = right_;
		key_ = to_delete->key_;
		right_ = to_delete->right_;
		left_ = to_delete->left_;
		if (right_)
		{
			right_->parent_ = this;
		}

		if (left_)
		{
			left_->parent_ = this;
		}

		to_delete->parent_ = nullptr;
		delete to_delete;
	}
	else if (left_ && !right_)
	{
		auto* to_delete = left_;
		key_ = to_delete->key_;
		right_ = to_delete->right_;
		left_ = to_delete->left_;
		if (right_)
		{
			right_->parent_ = this;
		}

		if (left_)
		{
			left_->parent_ = this;
		}

		to_delete->parent_ = nullptr;
		delete to_delete;
	}
	else
	{
		auto* successor = right_;
		while(true)
		{
			if(successor->left_)
			{
				successor = successor->left_;
			}else
			{
				break;
			}
		}
		key_ = successor->key_;
		successor->RemoveNode();
	}

}


template <OrderedType T>
bool BinarySearchTree<T>::Remove(T value)
{
	auto* found = Find(value);
	if (found) 
	{
		if(!root_->Left() && !root_->Right())
		{
			root_ = nullptr;
		}
		found->RemoveNode();
		return true;
	}
	return false;
}
template <OrderedType T>
bool BinarySearchTree<T>::Remove(bool (*predicate)(T))
{
	auto* found = Find(predicate);
	if (found)
	{
		if (!root_->Left() && !root_->Right())
		{
			root_ = nullptr;
		}
		found->RemoveNode();
		return true;
	}
	return false;
}
template <OrderedType T>
int BinarySearchTree<T>::RemoveAll(T value)
{
	int removed = 0;
	auto* found = Find(value);
	while(found)
	{
		if (!root_->Left() && !root_->Right())
		{
			root_ = nullptr;
		}
		found->RemoveNode();
		removed++;
		found = Find(value);
	}
	return removed;

}
template <OrderedType T>
int BinarySearchTree<T>::RemoveAll(bool (*predicate)(T))
{
	int removed = 0;
	auto* found = Find(predicate);
	while (found)
	{
		if (!root_->Left() && !root_->Right())
		{
			root_ = nullptr;
		}
		found->RemoveNode();
		removed++;
		found = Find(predicate);
	}
	return removed;
}

template <OrderedType T>
template <OrderedType U>
BinarySearchTree<U>* BinarySearchTree<T>::Map(U(*transform)(T))
{
	auto* visited = new ListSequence<BinaryNode<T>*>();
	
	auto* new_tree = new BinarySearchTree<U>(transform(root_->GetKey()));
	auto* current_node_old = root_;

	while (current_node_old)
	{
		if (current_node_old->Left() && !visited->Contains(current_node_old->Left()))
		{
			current_node_old = current_node_old->Left();
			new_tree->Add(transform(current_node_old->GetKey()));
			visited->Append(current_node_old);
		}
		else if (current_node_old->Right() && !visited->Contains(current_node_old->Right()))
		{
			current_node_old = current_node_old->Right();
			new_tree->Add(transform(current_node_old->GetKey()));
			visited->Append(current_node_old);
		}
		else
		{
			current_node_old = current_node_old->Parent();
		}
	}

	return new_tree;
}
template <OrderedType T>
T BinarySearchTree<T>::Reduce(T(*red_function)(T, T), const T& start)
{
	auto* visited = new ListSequence<BinaryNode<T>*>();
	T accum = red_function(start, root_->GetKey());
	auto* current_node = root_;

	while (current_node)
	{
		if (current_node->Left() && !visited->Contains(current_node->Left()))
		{
			current_node = current_node->Left();
			accum = red_function(accum, current_node->GetKey());
			visited->Append(current_node);
		}
		else if (current_node->Right() && !visited->Contains(current_node->Right()))
		{
			current_node = current_node->Right();
			accum = red_function(accum, current_node->GetKey());
			visited->Append(current_node);
		}
		else
		{
			current_node = current_node->Parent();
		}
	}

	return accum;
}

template <OrderedType T>
BinarySearchTree<T>* BinarySearchTree<T>::Where(bool (*filter)(T))
{
	auto* visited = new ListSequence<BinaryNode<T>*>();

	auto* new_tree = new BinarySearchTree<T>(this);
	auto* current_node = new_tree->Root();
	if (filter(current_node->GetKey()))
	{
		current_node->RemoveNode();
	}

	while (current_node)
	{
		
		if (current_node->Left() && !visited->Contains(current_node->Left()))
		{
			if(filter(current_node->Left()->GetKey()))
			{
				current_node->Left()->RemoveNode();
			}
			else
			{
				current_node = current_node->Left();
				visited->Append(current_node);
			}
			
		}
		else if (current_node->Right() && !visited->Contains(current_node->Right()))
		{
			if (filter(current_node->Right()->GetKey()))
			{
				current_node->Right()->RemoveNode();
			}
			else
			{
				current_node = current_node->Right();
				visited->Append(current_node);
			}

		}
		else
		{
			current_node = current_node->Parent();
		}
	}

	return new_tree;
}

template<class T>
bool AreEqual(BinaryNode<T>* branch1, BinaryNode<T>* branch2)
{
	if(!branch1 && !branch2)
	{
		return true;
	}

	if(!branch1 || !branch2)
	{
		return false;
	}
	
	return branch1->GetKey() == branch2->GetKey() &&
		AreEqual(branch1->Left(), branch2->Left()) &&
		AreEqual(branch1->Right(), branch2->Right());
}

template <class T>
bool BinaryNode<T>::IsSubBranch(BinaryNode<T>* node)
{
	if(!node)
	{
		return false;
	}

	if(AreEqual(this, node))
	{
		return true;
	}

	bool left_b = false;
	bool right_b = false;
	if(left_)
	{
		left_b = left_->IsSubBranch(node);
		if(left_b)
		{
			return true;
		}
	}

	if(right_)
	{
		right_b = right_->IsSubBranch(node);
	}
	return left_b || right_b;
	
}


template<OrderedType T>
bool BinarySearchTree<T>::IsSubTree(BinarySearchTree<T>* other_tree)
{
	if(!other_tree->root_)
	{
		return false;
	}
	return root_->IsSubBranch(other_tree->root_);
}
