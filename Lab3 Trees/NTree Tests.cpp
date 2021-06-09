#include "NTree Tests.h"


void NTRee_test_all()
{
	NTree_test_construction();
	NTree_test_operations();
	NTree_test_output();
	std::cout << "All n-tree tests passed successfully" << std::endl;
}

void NTree_test_construction()
{
	auto tree1 = NTree<int>(4, 13);
	assert(tree1.Add(7, tree1.Root()));
	assert(tree1.Add(-34, tree1.Root()));
	assert(tree1.Add(45, tree1.Root()));
	assert(tree1.Add(11, tree1.Root()));
	tree1.Add(0);
	tree1.Root()->GetChild(0)->AddChild(-45, 2);

	assert(tree1.Root()->GetChild(0)->GetValue() == 7);
	assert(tree1.Root()->GetChild(1)->GetValue() == -34);
	assert(tree1.Root()->GetChild(2)->GetValue() == 45);
	assert(tree1.Root()->GetChild(3)->GetValue() == 11);
	assert(tree1.Root()->GetChild(0)->GetChild(0)->GetValue() == 0);
	assert(tree1.Root()->GetChild(0)->GetChild(2)->GetValue() == -45);

	auto tree2 = NTree<int>(&tree1);
	assert(tree2.Root()->GetChild(0)->GetValue() == 7);
	assert(tree2.Root()->GetChild(1)->GetValue() == -34);
	assert(tree2.Root()->GetChild(2)->GetValue() == 45);
	assert(tree2.Root()->GetChild(3)->GetValue() == 11);
	assert(tree2.Root()->GetChild(0)->GetChild(0)->GetValue() == 0);
	assert(tree2.Root()->GetChild(0)->GetChild(2)->GetValue() == -45);

	auto tree3 = NTree<int>(tree1.Root());
	assert(tree3.Root()->GetChild(0)->GetValue() == 7);
	assert(tree3.Root()->GetChild(1)->GetValue() == -34);
	assert(tree3.Root()->GetChild(2)->GetValue() == 45);
	assert(tree3.Root()->GetChild(3)->GetValue() == 11);
	assert(tree3.Root()->GetChild(0)->GetChild(0)->GetValue() == 0);
	assert(tree3.Root()->GetChild(0)->GetChild(2)->GetValue() == -45);

}
void NTree_test_operations()
{
	auto tree1 = NTree<int>(3, 2);
	assert(tree1.Add(7, tree1.Root()));
	assert(tree1.Add(1, tree1.Root()));
	assert(tree1.Add(8, tree1.Root()));
	assert(tree1.Root()->GetChild(0)->AddChild(16,0));
	assert(tree1.Root()->GetChild(0)->AddChild(-3,1));
	assert(tree1.Root()->GetChild(2)->AddChild(-2, 2));

	assert(tree1.Root()->GetChild(0)->GetChild(1) == tree1.Find(-3));
	auto* list = tree1.FindAll(IsDivisibleByFour);
	assert(list->GetCount() == 2);
	assert(list->Get(0) == tree1.Root()->GetChild(0)->GetChild(0));
	assert(list->Get(1) == tree1.Root()->GetChild(2));

	tree1.Remove(-2);
	assert(!tree1.Root()->GetChild(2)->GetChild(2));
	tree1.Remove(7);
	assert(!tree1.Root()->GetChild(0));


}
void NTree_test_output()
{
	auto tree1 = NTree<int>(4, 13);
	assert(tree1.Add(7, tree1.Root()));
	assert(tree1.Add(-34, tree1.Root()));
	assert(tree1.Add(45, tree1.Root()));
	assert(tree1.Add(11, tree1.Root()));
	assert(!tree1.Add(0, tree1.Root()));
	tree1.Add(0);
	tree1.Root()->GetChild(0)->AddChild(-45, 2);
	assert(tree1.ToString() == "<13>{<7>{<0>}{}{<-45>}{}}{<-34>}{<45>}{<11>}");
	
}

bool IsDivisibleByFour(int a)
{
	return a % 4 == 0;
}