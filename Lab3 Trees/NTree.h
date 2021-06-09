#pragma once

#include <sstream>
#include <string>

#include "ListSequence.h"
#include "ArraySequence.h"

template <class T>
class NNode
{
private:
	T value_;
	ArraySequence<NNode<T>*>* children_;
	NNode<T>* parent_;
public:
	
	NNode(int arity, T value, ArraySequence<NNode<T>*>* children = nullptr, NNode<T>* parent = nullptr);
	NNode(NNode<T>* old_node);
	
	~NNode();

	int GetArity();
	T GetValue();
	NNode<T>* GetChild(int index);
	NNode<T>* Parent();
	bool AddChild(T new_value, int index = -1);

	void SetValue(T new_value);
	void SetChild(T new_value, int index);
	void SetChildNode(NNode<T>* new_node, int index);
	bool AddChildToBranch(T new_value);

	void DeleteBranch();

	std::string BranchToString();
	
};

template <class T>
NNode<T>::NNode(int arity, T value, ArraySequence<NNode<T>*>* children , NNode<T>* parent)
{
	value_ = value;
	parent_ = parent;
	children_ = new ArraySequence<NNode<T>*>(static_cast<NNode<T>*>(nullptr), arity);

	if (children)
	{
		if (children->GetCount() != arity)
		{
			throw std::invalid_argument("Number of passed children does not match the tree arity");
		}

		for (int i = 0; i < arity; i++)
		{
			if (children->Get(i))
			{
				children_->operator[](i) = new NNode(arity, children->Get(i)->value_, children->Get(i)->children_, this);
			}
		}
	}

}

template<class T>
NNode<T>::NNode(NNode<T>* old_node)
{
	value_ = old_node->value_;
	parent_ = nullptr;
	int arity = old_node->children_->GetCount();
	children_ = new ArraySequence<NNode<T>*>(static_cast<NNode<T>*>(nullptr), arity);
	for (int i = 0; i < arity; i++)
	{
		if (old_node->children_->Get(i))
		{
			children_->operator[](i) = new NNode(arity, old_node->children_->Get(i)->value_, old_node->children_->Get(i)->children_, this);
		}
	}
}



template<class T>
NNode<T>::~NNode()
{
	if (parent_)
	{
		for (int i = 0; i < GetArity(); i++)
		{
			if (this == parent_->children_->Get(i))
			{
				parent_->children_->Set(i, nullptr);
				return;
			}
		}
	}
}

template<class T>
int NNode<T>::GetArity()
{
	return children_->GetCount();
}

template<class T>
T NNode<T>::GetValue()
{
	return value_;
}

template<class T>
void NNode<T>::SetValue(T new_value)
{
	value_ = new_value;
}

template<class T>
NNode<T>* NNode<T>::GetChild(int index)
{
	if (index < 0 || index >= children_->GetCount())
	{
		throw std::invalid_argument("Index was out of range");
	}
	return children_->Get(index);
}

template<class T>
NNode<T>* NNode<T>::Parent()
{
	return parent_;
}

template<class T>
bool NNode<T>::AddChild(T new_value, int index)
{
	if (index == -1)
	{
		for (int i = 0; i < children_->GetCount(); i++)
		{
			if (!children_->Get(i))
			{
				index = i;
				break;
			}
		}

		if (index == -1)
		{
			return false;
		}
	}

	if (index < 0 || index >= children_->GetCount())
	{
		throw std::invalid_argument("Index was out of range");
	}
	if (children_->Get(index))
	{
		return false;
	}

	children_->Set(index, new NNode<T>(children_->GetCount(), new_value, nullptr, this));
	return true;
}

template<class T>
void NNode<T>::SetChild(T new_value, int index)
{
	if (index < 0 || index >= children_->GetCount())
	{
		throw std::invalid_argument("Index was out of range");
	}
	children_->Get(index)->SetValue(new_value);
}

template<class T>
void NNode<T>::SetChildNode(NNode<T>* new_node, int index)
{
	if (index < 0 || index >= children_->GetCount())
	{
		throw std::invalid_argument("Index was out of range");
	}
	children_->Set(index, new_node);
}

template<class T>
bool NNode<T>::AddChildToBranch(T new_value)
{
	if (AddChild(new_value))
	{
		return true;
	}

	for (int i = 0; i < children_->GetCount(); i++)
	{
		if (children_->Get(i)->AddChildToBranch(new_value))
		{
			return true;
		}
	}

	return false;

}

template<class T>
void NNode<T>::DeleteBranch()
{
	for (int i = 0; i < GetArity(); i++)
	{
		if (children_->Get(i))
		{
			children_->Get(i)->DeleteBranch();
		}
	}

	delete this;

}

template <class T>
std::string NNode<T>::BranchToString()
{
	std::string str = "<" + std::to_string(value_) + ">";

	bool no_children = true;
	int start_pos = str.length();
	for (int i = 0; i < children_->GetCount(); i++)
	{
		if (GetChild(i))
		{
			str.append("{" + GetChild(i)->BranchToString() + "}");
			no_children = false;
		}
		else
		{
			str.append("{}");
		}

	}
	if (no_children)
	{
		str.erase(start_pos, children_->GetCount() * 2);
	}

	return str;
}


template<class T>
NNode<T>* BranchFromString(int arity, std::string str, NNode<T>* parent = nullptr)
{
	// <R>{1}{2}...
	// <a>{<b>{<v>}{t}}{<c>}{<k>{<r>}}
	/*
	 *							a
	 *						   /|\
	 *						  / | \
	 *						 /  |  \
	 *						b	c   k
	 *					   / \	    |
	 *					  v   t     r
	 */

	std::stringstream ss;
	NNode<T>* new_node = nullptr;
	std::string elem_str = "";
	int child_cnt = 0;
	int start_pos = 0;
	int bracket_cnt = 0;
	bool is_bracket_opened = false;
	bool main_value_saved = false;
	for (int i = 1; i < str.length(); i++)
	{

		if (str[i] == '>' && !main_value_saved)
		{
			if (elem_str.length() == 0) { return nullptr; }
			ss.str(elem_str);
			T new_value;
			ss >> new_value;
			if(elem_str.length() == 0)
			{
				return nullptr;
			}
			new_node = new NNode<T>(arity, new_value, nullptr, parent);
			elem_str = "";
			main_value_saved = true;
		}
		else if (str[i] == '{')
		{
			if (!is_bracket_opened)
			{
				is_bracket_opened = true;
				start_pos = i + 1;
			}else
			{
				elem_str += str[i];
			}
			bracket_cnt++;
		}
		else if (str[i] == '}')
		{
			bracket_cnt--;
			if (is_bracket_opened && bracket_cnt == 0)
			{
				is_bracket_opened = false;
				auto* new_child = BranchFromString(arity, elem_str, new_node);
				new_node->SetChildNode(new_child, child_cnt);

				elem_str = "";
				child_cnt++;
			}else
			{
				elem_str += str[i];
			}
		}
		else
		{
			elem_str += str[i];
		}
	}

	return new_node;
}

template <class T>
class NTree
{
private:
	int arity_;
	NNode<T>* root_;
public:
	// constructors
	NTree(NNode<T>* old_branch);
	explicit NTree(NTree<T>* old_tree);
	NTree(int arity, const T& value);
	NTree(int arity, std::string str);

	~NTree();

	NTree& operator=(const NTree& tree);

	NNode<T>* Root() { return root_; }

	std::string ToString();

	bool Add(const T& value, NNode<T>* node);
	void Add(const T& value);

	NNode<T>* Find(T value, NNode<T>* start_node = nullptr);
	NNode<T>* Find(bool (*predicate)(T), NNode<T>* start_node = nullptr);
	ListSequence<NNode<T>*>* FindAll(T value, NNode<T>* start_node = nullptr, ListSequence<NNode<T>*>* saved_nodes = nullptr);
	ListSequence<NNode<T>*>* FindAll(bool (*predicate)(T), NNode<T>* start_node = nullptr, ListSequence<NNode<T>*>* saved_nodes = nullptr);

	bool Remove(T value);
	bool Remove(bool (*predicate)(T));
	int RemoveAll(T value);
	int RemoveAll(bool (*predicate)(T));
};

template <class T>
bool NTree<T>::Add(const T& value, NNode<T>* node)
{
	if(!node)
	{
		return false;
	}

	if(node->AddChild(value))
	{
		return true;
	}

	return false;
}

template <class T>
void  NTree<T>::Add(const T& value)
{
	root_->AddChildToBranch(value);
}

template <class T>
NNode<T>* NTree<T>::Find(T value, NNode<T>* start_node)
{
	if (!root_)
	{
		return nullptr;
	}
	if (!start_node)
	{
		start_node = root_;
	}
	
	if (start_node->GetValue() == value)
	{
		return start_node;
	}

	for(int i = 0;i<arity_;i++)
	{
		if(start_node->GetChild(i))
		{
			auto* found = Find(value, start_node->GetChild(i));
			if(found)
			{
				return found;
			}
		}
	}

	return nullptr;
}
template <class T>
NNode<T>* NTree<T>::Find(bool (*predicate)(T), NNode<T>* start_node)
{
	if (!root_)
	{
		return nullptr;
	}
	if (!start_node)
	{
		start_node = root_;
	}

	if (predicate(start_node->GetValue()))
	{
		return start_node;
	}

	auto* found;
	for (int i = 0; i < arity_; i++)
	{
		if (start_node->GetChild(i))
		{
			found = Find(predicate, start_node->GetChild(i));
			if (found)
			{
				return found;
			}
		}
	}

	return nullptr;
}

template <class T>
ListSequence<NNode<T>*>* NTree<T>::FindAll(T value, NNode<T>* start_node, ListSequence<NNode<T>*>* saved_nodes )
{
	if (!root_)
	{
		return nullptr;
	}

	if (!saved_nodes)
	{
		saved_nodes = new ListSequence<NNode<T>*>();
	}
	if (!start_node)
	{
		start_node = root_;
	}
	if (start_node->GetValue() == value)
	{
		saved_nodes->Append(start_node);
	}

	for (int i = 0; i < arity_; i++)
	{
		if (start_node->GetChild(i))
		{
			FindAll(value, start_node->GetChild(i), saved_nodes);
		}
	}

	return saved_nodes;
}
template <class T>
ListSequence<NNode<T>*>* NTree<T>::FindAll(bool (*predicate)(T), NNode<T>* start_node, ListSequence<NNode<T>*>* saved_nodes)
{
	if (!root_)
	{
		return nullptr;
	}

	if (!saved_nodes)
	{
		saved_nodes = new ListSequence<NNode<T>*>();
	}
	if (!start_node)
	{
		start_node = root_;
	}
	if (predicate(start_node->GetValue()))
	{
		saved_nodes->Append(start_node);
	}

	for (int i = 0; i < arity_; i++)
	{
		if (start_node->GetChild(i))
		{
			FindAll(predicate, start_node->GetChild(i), saved_nodes);
		}
	}

	return saved_nodes;
}
template <class T>
bool  NTree<T>::Remove(T value)
{
	auto* found = Find(value);
	if (found)
	{
		found->DeleteBranch();
		return true;
	}
	return false;
}
template <class T>
bool  NTree<T>::Remove(bool (*predicate)(T))
{
	auto* found = Find(predicate);
	if (found)
	{
		found->DeleteBranch();
		return true;
	}
	return false;
}
template <class T>
int  NTree<T>::RemoveAll(T value)
{
	int deleted = 0;
	auto* found = Find(value);
	while (found)
	{
		found->DeleteBranch();
		deleted++;
		found = Find(value);
	}
	return deleted;
}
template <class T>
int  NTree<T>::RemoveAll(bool (*predicate)(T))
{
	int deleted = 0;
	auto* found = Find(predicate);
	while (found)
	{
		found->DeleteBranch();
		deleted++;
		found = Find(predicate);
	}
	return deleted;
}


template <class T>
NTree<T>::NTree(NNode<T>* old_branch)
{
	arity_ = old_branch->GetArity();
	root_ = new NNode<T>(old_branch);
}

template <class T>
NTree<T>::NTree(NTree<T>* old_tree)
{
	arity_ = old_tree->arity_;
	root_ = new NNode<T>(old_tree->root_);
}

template <class T>
NTree<T>::NTree(int arity, const T& value)
{
	arity_ = arity;
	root_ = new NNode<T>(arity, value);
}

template <class T>
NTree<T>::NTree(int arity, std::string str)
{
	try
	{
		arity_ = arity;
		root_ = BranchFromString<T>(arity, str);
	}
	catch(...)
	{
		throw std::invalid_argument("Passed string has invalid format");
	}
}

template <class T>
NTree<T>::~NTree()
{
	root_->DeleteBranch();
}

template <class T>
NTree<T>& NTree<T>::operator=(const NTree& tree)
{
	if (root_ == tree.root_)
	{
		return *this;
	}

	if (root_)
	{
		root_->DeleteBranch();
	}

	root_ = new NNode<T>(tree.root_);

	return *this;
}

template <class T>
std::string NTree<T>::ToString()
{
	// <R>{1}{2}...

	if (!root_)
	{
		throw std::invalid_argument("The tree was empty");
	}
	return root_->BranchToString();
}
