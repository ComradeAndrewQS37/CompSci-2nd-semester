#include "Client.h"

void main_menu()
{
	try
	{
		while (true)
		{

			std::cout << "What would you like to do?" << std::endl;
			std::cout << "1) Work with binary search trees" << std::endl;
			std::cout << "2) Work with n-trees" << std::endl;
			std::cout << "3) Run automatic tests" << std::endl;
			std::cout << "4) Exit" << std::endl;

			switch (Choice(1, 4))
			{
			case 1:
				BST_client();
				break;
			case 2:
				NTree_Client();
				break;
			case 3:
				BST_test_all();
				NTRee_test_all();
				break;
			default:
				exit(0);
			}
		}
	}
	catch (...)
	{
		std::cout << "\nUnexpected Error\n";
		exit(0);
	}
}

int Choice(int min, int max)
{
	int choice = -1;
	while (true)
	{
		std::cout << ">>> ";
		std::cin >> choice;
		if (std::cin.fail() || choice<min || choice>max)
		{

			std::cout << "Choose valid option, please" << std::endl;
			std::cin.clear();
			std::cin.ignore(1000, '\n');
			continue;
		}

		break;
	}

	return choice;
}

void BST_client()
{
	int type = 0;
	BinarySearchTree<int>* int_tree = nullptr;
	BinarySearchTree<double>* double_tree = nullptr;

	std::cout << "Firstly, we need to create a new tree\n";
	void* new_tree = CreateBST(type);
	if(type == 1)
	{
		int_tree = static_cast<BinarySearchTree<int>*>(new_tree);
	}else
	{
		double_tree = static_cast<BinarySearchTree<double>*>(new_tree);
	}
	while (true)
	{
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "1) Create a new tree" << std::endl;
		std::cout << "2) Print the tree" << std::endl;
		std::cout << "3) Remove an element" << std::endl;
		std::cout << "4) Add an element" << std::endl;
		std::cout << "5) Test map-reduce" << std::endl;
		std::cout << "6) Go to main menu" << std::endl;
		std::cout << "7) Exit" << std::endl;


		void* tmp = nullptr;
		switch (Choice(1, 7))
		{
		case 1:
			tmp = CreateBST(type);
			if (type == 1)
			{
				int_tree = static_cast<BinarySearchTree<int>*>(tmp);
			}
			else
			{
				double_tree = static_cast<BinarySearchTree<double>*>(tmp);
			}
			break;
		case 2:
			std::cout << "Would you like to use default print format?\n";
			std::cout << "1) Yes, use <K>(L)[R] format\n";
			std::cout << "2) No, input other format\n";
			if (Choice(1, 2) == 2)
			{
				while(true)
				{
					std::cout << "Format string must be \".K..L..R.\", where each . is any symbol, characters K,L,R can be swapped\n";
					try
					{
						std::string format;
						std::cout << ">>> ";
						std::cin >> format;

						if (type == 1)
						{
							std::cout << int_tree->ToString(format) << std::endl;
						}
						else
						{
							std::cout << double_tree->ToString(format) << std::endl;
						}
					}catch(std::invalid_argument& e)
					{
						continue;
					}

					break;
				}
			}else
			{
				if (type == 1)
				{
					std::cout << int_tree->ToString() << std::endl;
				}else
				{
					std::cout << double_tree->ToString() << std::endl;
				}
			}
			
			break;
		case 3:
			// remove
			std::cout << "What element you'd like to remove (type value) ?\n";
			if (type == 1)
			{
				int to_remove = Choice(INT_MIN, INT_MAX);
				if(!int_tree->Remove(to_remove))
				{
					std::cout << "No nodes with key: " << to_remove << " were found\n";
				}
			}else
			{
				double to_remove;
				std::cout << ">>> ";

				std::cin >> to_remove;
				if (!double_tree->Remove(to_remove))
				{
					std::cout << "No nodes with key: " << to_remove << " were found\n";
				}
			}
			
			break;
		case 4:
			// add
			std::cout << "What element you'd like to add (type value) ?\n";
			if (type == 1)
			{
				int to_add = Choice(INT_MIN, INT_MAX);
				int_tree->Add(to_add);
			}
			else
			{
				double to_add;
				std::cout << ">>> ";

				std::cin >> to_add;
				double_tree->Add(to_add);
			}
			break;
		case 5:
			// map-reduce
			std::cout << "Firstly, let's test map\n";
			if(type==1)
			{
				std::cout << "We have function which replaces value x with (10-x)\n";
					std::cout << "If we use map with this function on the tree, we'll get:\n";
				std::cout << int_tree->Map(TenMinus)->ToString()<<std::endl;
			}else
			{
				std::cout << "We have function which replaces value x with (x/7)\n";
				std::cout << "If we use map with this function on the tree, we'll get:\n";
				std::cout << double_tree->Map(DivideBySeven)->ToString()<<std::endl;
			}

			std::cout << "Now we'll test reduce function. It will found the product of all keys\n";
			if (type == 1)
			{
				std::cout << int_tree->Reduce(MultElements, 1) << std::endl;
			}
			else
			{
				std::cout << double_tree->Reduce(MultElements, 1.0) << std::endl;
			}
			break;
		case 6:
			return;
		default:
			exit(0);
		}
	}
}
void NTree_Client()
{
	int type = 0;
	NTree<int>* int_tree = nullptr;
	NTree<double>* double_tree = nullptr;

	std::cout << "Firstly, we need to create a new tree\n";
	void* new_tree = CreateNTree(type);
	if (type == 1)
	{
		int_tree = static_cast<NTree<int>*>(new_tree);
	}
	else
	{
		double_tree = static_cast<NTree<double>*>(new_tree);
	}
	while (true)
	{
		std::cout << "What would you like to do?" << std::endl;
		std::cout << "1) Create a new tree" << std::endl;
		std::cout << "2) Print the tree" << std::endl;
		std::cout << "3) Remove an element" << std::endl;
		std::cout << "4) Add an element" << std::endl;
		std::cout << "5) Go to main menu" << std::endl;
		std::cout << "6) Exit" << std::endl;


		void* tmp = nullptr;
		switch (Choice(1, 6))
		{
		case 1:
			tmp = CreateNTree(type);
			if (type == 1)
			{
				int_tree = static_cast<NTree<int>*>(tmp);
			}
			else
			{
				double_tree = static_cast<NTree<double>*>(tmp);
			}
			break;
		case 2:
			if(type==1)
			{
				std::cout << int_tree->ToString() << std::endl;
			}else
			{
				std::cout << double_tree->ToString() << std::endl;

			}

			break;
		case 3:
			// remove
			std::cout << "What element you'd like to remove (type value) ?\n";
			if (type == 1)
			{
				int to_remove = Choice(INT_MIN, INT_MAX);
				if (!int_tree->Remove(to_remove))
				{
					std::cout << "No nodes with key: " << to_remove << " were found\n";
				}
			}
			else
			{
				double to_remove;
				std::cout << ">>> ";

				std::cin >> to_remove;
				if (!double_tree->Remove(to_remove))
				{
					std::cout << "No nodes with key: " << to_remove << " were found\n";
				}
			}

			break;
		case 4:
			// add
			std::cout << "What element you'd like to add (type value) ?\n";
			if (type == 1)
			{
				int to_add = Choice(INT_MIN, INT_MAX);
				int_tree->Add(to_add);
			}
			else
			{
				double to_add;
				std::cout << ">>> ";

				std::cin >> to_add;
				double_tree->Add(to_add);
			}
			break;
		
		case 5:
			return;
		default:
			exit(0);
		}
	}
}

void* CreateBST(int& type)
{
	void* to_return = nullptr;
	std::cout << "What type would tree be?\n";
	std::cout << "1) int" << std::endl;
	std::cout << "2) double" << std::endl;
	type = Choice(1, 2);

	bool input_correct = false;
	while (!input_correct) 
	{
		
		std::cout << "Type the tree elements, divided with whitespaces and add \"x\" in the end\n";

		if(type == 1)
		{
			BinarySearchTree<int>* tree = nullptr;
			std::string elem = "";
			while (true)
			{
				std::cin >> elem;
				if(elem == "x")
				{
					if(tree)
					{
						input_correct = true;
					}
					break;
				}
				
				if(!tree)
				{
					
					tree = new BinarySearchTree<int>(std::stoi(elem));
				}else
				{
					tree->Add(std::stoi(elem));
				}
			}
			to_return = tree;
		}else
		{

			BinarySearchTree<double>* tree = nullptr;
			std::string elem = "";
			while (true)
			{
				std::cin >> elem;
				if (elem == "x")
				{
					if (tree)
					{
						input_correct = true;
					}
					break;
				}

				if (!tree)
				{

					tree = new BinarySearchTree<double>(std::stod(elem));
				}
				else
				{
					tree->Add(std::stod(elem));
				}
			}
			to_return = tree;

		}
	}

	return to_return;
	
}
void* CreateNTree(int& type)
{
	void* to_return = nullptr;
	std::cout << "What type would tree be?\n";
	std::cout << "1) int" << std::endl;
	std::cout << "2) double" << std::endl;
	type = Choice(1, 2);

	bool input_correct = false;

	std::cout << "What arity would tree have?\n";
	int arity = Choice(1, INT_MAX);
	
	std::cout << "Type tree elements in following format:\n";
	std::cout << "<R>{1}{2}..., where R is node key and 1,2 etc. are children(if no child leave brackets empty)\n>>> ";
	std::string tree_str = "";
	std::cin >> tree_str;


	if (type == 1)
	{
		auto* tree = new NTree<int>(arity, tree_str);
		
		to_return = tree;
	}
	else
	{
		auto* tree = new NTree<double>(arity, tree_str);

		to_return = tree;

	}


	return to_return;
}

double MultElements(double a, double b)
{
	return a * b;
}

int MultElements(int a, int b)
{
	return a * b;
}
double DivideBySeven(double a)
{
	return a / 7;
}