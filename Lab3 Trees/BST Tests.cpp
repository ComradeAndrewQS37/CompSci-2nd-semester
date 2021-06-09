#include "BST Tests.h"

#include <cassert>


void BST_test_all()
{
	BST_test_construction();
	BST_test_operations();
	BST_test_output();
	std::cout << "All binary search tree tests passed successfully" << std::endl;
}

void BST_test_construction()
{
	int nums1[7] = {3, 13,1,-4,5,6,9 };
	auto tree1 = BinarySearchTree<int>(nums1, 7);
	assert(tree1.Root()->GetKey() == 3);
	assert(tree1.Root()->Left()->GetKey() == 1);
	assert(tree1.Root()->Left()->Left()->GetKey() == -4);
	assert(tree1.Root()->Right()->GetKey() == 13);
	assert(tree1.Root()->Right()->Left()->GetKey() == 5);
	assert(tree1.Root()->Right()->Left()->Right()->GetKey() == 6);
	assert(tree1.Root()->Right()->Left()->Right()->Right()->GetKey() == 9);

	auto tree2 = BinarySearchTree<int>(&tree1);
	assert(tree2.Root()->GetKey() == 3);
	assert(tree2.Root()->Left()->GetKey() == 1);
	assert(tree2.Root()->Left()->Left()->GetKey() == -4);
	assert(tree2.Root()->Right()->GetKey() == 13);
	assert(tree2.Root()->Right()->Left()->GetKey() == 5);
	assert(tree2.Root()->Right()->Left()->Right()->GetKey() == 6);
	assert(tree2.Root()->Right()->Left()->Right()->Right()->GetKey() == 9);

	auto node1 = static_cast<BinaryNode<int>>(tree2);
	assert(node1.GetKey() == 3);
	assert(node1.Left()->GetKey() == 1);
	assert(node1.Left()->Left()->GetKey() == -4);
	assert(node1.Right()->GetKey() == 13);
	assert(node1.Right()->Left()->GetKey() == 5);
	assert(node1.Right()->Left()->Right()->GetKey() == 6);
	assert(node1.Right()->Left()->Right()->Right()->GetKey() == 9);

	auto tree3 = BinarySearchTree<int>(tree1.Root()->Right());
	assert(tree3.Root()->GetKey() == 13);
	assert(tree3.Root()->Left()->GetKey() == 5);
	assert(tree3.Root()->Left()->Right()->GetKey() == 6);
	assert(tree3.Root()->Left()->Right()->Right()->GetKey() == 9);

	auto* tree4 = tree1.GetSubTree(tree1.Root()->Left());
	assert(tree4->Root()->GetKey() == 1);
	assert(tree4->Root()->Left()->GetKey() == -4);

	try
	{
		auto* tr = tree1.GetSubTree(tree2.Root()->Left());
		assert(0);
	}catch(std::invalid_argument& e)
	{
		assert(strcmp(e.what(), "Passed node does not belong the tree") == 0);
	}catch(...)
	{
		assert(0);
	}

	assert(tree3.IsNodeFromThisTree(tree3.Root()->Left()));
	assert(!tree3.IsNodeFromThisTree(tree1.Root()->Right()));


}
void BST_test_operations()
{
	// add
	auto tree1 = BinarySearchTree<int>(3);
	tree1.Add(13);
	tree1.Add(1);
	tree1.Add(-4);
	tree1.Add(5);
	tree1.Add(6);
	tree1.Add(9);
	assert(tree1.Root()->GetKey() == 3);
	assert(tree1.Root()->Left()->GetKey() == 1);
	assert(tree1.Root()->Left()->Left()->GetKey() == -4);
	assert(tree1.Root()->Right()->GetKey() == 13);
	assert(tree1.Root()->Right()->Left()->GetKey() == 5);
	assert(tree1.Root()->Right()->Left()->Right()->GetKey() == 6);
	assert(tree1.Root()->Right()->Left()->Right()->Right()->GetKey() == 9);


	// find
	auto* n1 = tree1.Root()->Right()->Left();
	assert(n1 == tree1.Find(5));

	auto* n2 = tree1.Root()->Left()->Left();
	assert(n2 == tree1.Find([](int a) {return a < 0; }));

	auto* l1 = new ListSequence<BinaryNode<int>*>();
	l1->Append(n2);
	l1->Append(n1->Right());
	auto* l2 = tree1.FindAll([](int a) {return a % 2 == 0; });
	assert(l2->GetCount() == 2);
	assert(l1->Get(0) == l2->Get(0) && l1->Get(1) == l2->Get(1));
	l1->Remove(0);
	l1->Remove(0);
	l1->Append(tree1.Root()->Right());
	auto* l3 = tree1.FindAll(13);
	assert(l3->GetCount() == 1);
	assert(l1->Get(0) == l3->Get(0));

	
	// remove
	auto tree2 = BinarySearchTree<int>(&tree1);
	tree2.Remove(-4);
	assert(!tree2.Root()->Left()->Left() && !tree2.Root()->Left()->Right());

	tree2.Remove(13);
	assert(tree2.Root()->Right()->GetKey() == 5 && tree2.Root()->Right()->Right()->GetKey() == 6);

	auto tree3 = BinarySearchTree<int>(&tree1);
	tree3.RemoveAll([](int a) {return a > 7; });
	assert(tree3.Root()->Right()->GetKey() == 5 && tree3.Root()->Right()->Right()->GetKey() == 6\
		&& !tree3.Root()->Right()->Right()->Right());

	// map
	int nums4[9] = { 7,-2,6,9,-4,-5,0,11,10 };
	auto tree4 = BinarySearchTree<int>(nums4, 9);
	auto* tree5 = tree4.Map(TenMinus);
	assert(tree5->Root()->GetKey() == 3);
	assert(tree5->Root()->Left()->GetKey() == 1);
	assert(tree5->Root()->Left()->Left()->GetKey() == -1);
	assert(tree5->Root()->Left()->Left()->Right()->GetKey() == 0);
	assert(tree5->Root()->Right()->GetKey() == 12);
	assert(tree5->Root()->Right()->Left()->GetKey() == 4);
	assert(tree5->Root()->Right()->Left()->Right()->GetKey() == 10);
	assert(tree5->Root()->Right()->Right()->GetKey() == 14);
	assert(tree5->Root()->Right()->Right()->Right()->GetKey() == 15);


	// reduce
	int sum4 = 0;
	for (auto i : nums4)
	{
		sum4 += i;
	}
	assert(sum4 == tree4.Reduce(SumTwo, 0));


	// where
	auto* tree6 = tree4.Where(IsEven);
	assert(tree6->Root()->GetKey() == 7);
	assert(tree6->Root()->Left()->GetKey() == -5);
	assert(tree6->Root()->Right()->GetKey() == 9);
	assert(tree6->Root()->Right()->Right()->GetKey() == 11);


	// subtree
	int nums7[3] = { 9,11,10 };
	auto tree7 = BinarySearchTree<int>(nums7, 3);
	assert(tree4.IsSubTree(&tree7));
	int nums8[4] = { 1,11,6,0 };
	auto tree8 = BinarySearchTree<int>(nums8, 4);
	assert(!tree4.IsSubTree(&tree8));
	
}
void BST_test_output()
{
	int nums1[8] = { 5, 89,-98,0,4,12,1,8 };
	auto tree1 = BinarySearchTree<int>(nums1, 8);
	auto tree1_str = tree1.ToString();
	assert(tree1_str =="<5>(<-98>[<0>[<4>(<1>)]])[<89>(<12>(<8>))]");
	tree1_str = tree1.ToString("*K*|R|\\L/");
	assert(tree1_str == "*5*|*89*\\*12*\\*8*//|\\*-98*|*0*|*4*\\*1*/||/");

}

int TenMinus(int a) { return 10 - a; }
int SumTwo(int a, int b) { return a + b; }
bool IsEven(int a) { return a % 2 == 0; }