#include "Client.h"

using complex = std::complex<double>;

struct matrices
{
	int number_of_matrices;
	ArraySequence<Matrix<int>*>* int_m;
	ArraySequence<Matrix<double>*>* double_m;
	ArraySequence <Matrix<complex>*>* complex_m;

};

struct vectors
{
	int number_of_vectors;
	ArraySequence<Vector<int>*>* int_v;
	ArraySequence<Vector<double>*>* double_v;
	ArraySequence <Vector<complex>*>* complex_v;

};
void MainMenu()
{
	matrices saved_matrices = { 0, nullptr, nullptr, nullptr };
	vectors saved_vectors = { 0, nullptr, nullptr, nullptr };

	try
	{
		while (true) 
		{
		
			std::cout << "What would you like to do?" << std::endl;
			std::cout << "1) Work with matrices" << std::endl;
			std::cout << "2) Work with vectors" << std::endl;
			std::cout << "3) Run automatic tests" << std::endl;
			std::cout << "4) Exit" << std::endl;

			switch (Choice(1, 4))
			{
			case 1:
				MatrixClient(&saved_matrices);
				break;
			case 2:
				VectorClient(&saved_vectors);
				break;
			case 3:
				test_all();
				break;
			case 4:
				exit(0);
			}
		}
	}catch(...)
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
		if(std::cin.fail() || choice<min || choice>max)
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

void MatrixClient(matrices* saved_matrices)
{
	int int_amount = saved_matrices->int_m? saved_matrices->int_m->GetLength() : 0;
	int double_amount = saved_matrices->double_m ? saved_matrices->double_m->GetLength() : 0;
	int complex_amount = saved_matrices->complex_m ? saved_matrices->complex_m->GetLength() : 0;
	
	void* current_matrix = nullptr;
	int curr_type = 0;
	int& current_type = curr_type;
	if(saved_matrices->number_of_matrices == 0)
	{
		current_matrix = CreateMatrix(saved_matrices, current_type);
		switch (current_type)
		{
		case 1:
			int_amount++;
			break;
		case 2:
			double_amount++;
			break;
		default:
			complex_amount++;
			break;
		}
	}
	else
	{
		PrintMatricesInMemory(saved_matrices);
		
		std::cout << "\nDo you want to work with these matrices or create a new one?\n";
		std::cout << "1) Work with these matrices\n";
		std::cout << "2) Create a new matrix\n";
		
		switch (Choice(1, 2))
		{
		case 1:
			std::cout << std::endl << "With which one you'd like to work?\n";
			current_matrix = ChooseMatrix(saved_matrices, current_type);
			break;
		default:
			current_matrix = CreateMatrix(saved_matrices, current_type);
			switch (current_type)
			{
			case 1:
				int_amount++;
				break;
			case 2:
				double_amount++;
				break;
			default:
				complex_amount++;
				break;
			}
		}
		
	}

	while(true)
	{
		std::cout << std::endl << "\nWhat do you want to do with the matrix?" << std::endl;
		std::cout << "1) Print" << std::endl;
		std::cout << "2) Add another matrix" << std::endl;
		std::cout << "3) Subtract another matrix" << std::endl;
		std::cout << "4) Multiply by a scalar" << std::endl;
		std::cout << "5) Find norm" << std::endl;
		std::cout << "6) Find determinant" << std::endl;
		std::cout << "7) Do elementary transformations" << std::endl;
		std::cout << "8) Go to main menu" << std::endl;
		std::cout << "9) Exit" << std::endl;

		void* other_matrix = nullptr;
		int oth_type = 0;
		int& other_type = oth_type;

		int choice1 = 0;
		int int_scalar = 0;
		double scalar = 0.0;
		switch (Choice(1, 9))
		{
		case 1:
			// print
			PrintOneMatrix(current_matrix, current_type);
			break;
		case 2:
			// add matrix
			PrintMatricesInMemory(saved_matrices);
			std::cout << "\nWould you like to use matrix in memory or create a new one?\n";
			std::cout << "1) Use one of saved matrices\n";
			std::cout << "2) Create a new matrix\n";
			if (Choice(1, 2) == 1)
			{
				while (true)
				{
					std::cout << std::endl << "Which matrix you'd like to add to the previous one?\n";
					other_matrix = ChooseMatrix(saved_matrices, other_type);
					if (current_type != other_type)
					{
						std::cout << std::endl << "These matrices have different types. Please choose matrix with the same type\n";
						continue;
					}
					break;
				}
			}
			else
			{
				other_matrix = CreateMatrix(saved_matrices, other_type, current_type, GetHeight(current_matrix, current_type), GetWidth(current_matrix, current_type));
			}
			switch (current_type)
			{
			case 1:
				current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MatrixSum(static_cast<Matrix<int>*>(other_matrix));
				break;
			case 2:
				current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MatrixSum(static_cast<Matrix<double>*>(other_matrix));
				break;
			default:
				current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MatrixSum(static_cast<Matrix<complex>*>(other_matrix));
			}
			
			PrintOneMatrix(current_matrix, current_type);

			break;
		case 3:
			// subtract matrix
			PrintMatricesInMemory(saved_matrices);
			std::cout << "\nWould you like to use matrix in memory or create a new one?\n";
			std::cout << "1) Use one of saved matrices\n";
			std::cout << "2) Create a new matrix\n";
			if (Choice(1, 2) == 1)
			{
				while (true)
				{
					std::cout << std::endl << "Which matrix you'd like to subtract?\n";
					other_matrix = ChooseMatrix(saved_matrices, other_type);
					if (current_type != other_type)
					{
						std::cout << std::endl << "These matrices have different types. Please choose matrix with the same type\n";
						continue;
					}
					break;
				}
			}
			else
			{
				other_matrix = CreateMatrix(saved_matrices, other_type, current_type, GetHeight(current_matrix, current_type), GetWidth(current_matrix, current_type));
			}
			switch (current_type)
			{
			case 1:
				current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MatrixSubtract(static_cast<Matrix<int>*>(other_matrix));
				break;
			case 2:
				current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MatrixSubtract(static_cast<Matrix<double>*>(other_matrix));
				break;
			default:
				current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MatrixSubtract(static_cast<Matrix<complex>*>(other_matrix));
			}

			PrintOneMatrix(current_matrix, current_type);

			break;
		case 4:
			// multiply by scalar
			std::cout << "\nBy which scalar should matrix be multiplied?\n";
			if(curr_type == 1)
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> int_scalar;
					if (std::cin.fail())
					{
						std::cout << "Type valid integer number, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}

					break;
				}
			}else
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> scalar;
					if (std::cin.fail())
					{
						std::cout << "Type valid number, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}

					break;
				}
			}
			switch (current_type)
			{
			case 1:
				current_matrix = &static_cast<Matrix<int>*>(current_matrix)->ScalarMult(int_scalar);
				break;
			case 2:
				current_matrix = &static_cast<Matrix<double>*>(current_matrix)->ScalarMult(scalar);
				break;
			default:
				current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->ScalarMult(scalar);
			}

			PrintOneMatrix(current_matrix, current_type);
			
			break;
		case 5:
			// find norm
			if(current_type == 1)
			{
				int int_norm = static_cast<Matrix<int>*>(current_matrix)->Norm();
				std::cout << "Norm is: " << int_norm << std::endl;
			}
			else if(current_type == 2)
			{
				double double_norm = static_cast<Matrix<double>*>(current_matrix)->Norm();
				std::cout << "Norm is: " << double_norm << std::endl;
			}
			else
			{
				complex complex_norm = static_cast<Matrix<complex>*>(current_matrix)->Norm();
				std::cout << "Norm is: " << complex_norm << std::endl;
			}
			break;
		case 6:
			// find det
			try
			{
				if (current_type == 1)
				{
					int int_det = static_cast<Matrix<int>*>(current_matrix)->Det();
					std::cout << "Determinant is: " << int_det << std::endl;
				}

				else if (current_type == 2)
				{
					double double_det = static_cast<Matrix<double>*>(current_matrix)->Det();
					std::cout << "Determinant is: " << double_det << std::endl;
				}
				else
				{
					complex complex_det = static_cast<Matrix<complex>*>(current_matrix)->Det();
					std::cout << "Determinant is: " << complex_det << std::endl;
				}

			}
			catch (std::invalid_argument& e)
			{
				std::cout << e.what() << std::endl;
			}
			break;
		case 7:
			// elementary transformations
			std::cout << "What do you want to transform?\n";
			std::cout << "1) Rows\n";
			std::cout << "2) Columns\n";

			choice1 = Choice(1, 2);
			
			if(choice1 == 1)
			{
				std::cout << "Which transformation you'd like to do?\n";
				std::cout << "1) Interchange rows\n";
				std::cout << "2) Multiply row\n";
				std::cout << "3) Multiply one row and add to another\n";

				int choice_2 = Choice(1, 3);
				
				if (choice_2 == 1)
				{
					std::cout << "Which rows you'd like to interchange (type 2 numbers in separate lines)\n";
					int row1 = Choice(1, GetHeight(current_matrix, current_type)) - 1;
					int row2 = Choice(1, GetHeight(current_matrix, current_type)) - 1;

					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->InterchangeRows(row1, row2);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->InterchangeRows(row1, row2);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->InterchangeRows(row1, row2);
					}

					PrintOneMatrix(current_matrix, current_type);
				}
				
				else if (choice_2 == 2)
				{
					std::cout << "Which row you'd like to multiply?\n";
					int row = Choice(1, GetHeight(current_matrix, current_type)) - 1;
					std::cout << "By what scalar you'd like to multiply this row?\n";
					if(current_type == 1)
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> int_scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid integer number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}else
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					
					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MultiplyRow(row, int_scalar);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MultiplyRow(row, scalar);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MultiplyRow(row, scalar);
					}

					PrintOneMatrix(current_matrix, current_type);
				}
				else
				{
					std::cout << "Which row you'd like to multiply?\n";
					int row1 = Choice(1, GetHeight(current_matrix, current_type)) - 1;
					std::cout << "By what scalar you'd like to multiply this row?\n";
					if (current_type == 1)
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> int_scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid integer number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					else
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					std::cout << "To which row you'd like to add that row?\n";
					int row2 = Choice(1, GetHeight(current_matrix, current_type)) - 1;
					
					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MultiplyRowAndAdd(row1, int_scalar, row2);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MultiplyRowAndAdd(row1, scalar, row2);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MultiplyRowAndAdd(row1, scalar, row2);
						break;
					}
					PrintOneMatrix(current_matrix, current_type);
				}
			}

			else
			{
				std::cout << "Which transformation you'd like to do?\n";
				std::cout << "1) Interchange columns\n";
				std::cout << "2) Multiply column\n";
				std::cout << "3) Multiply one column and add to another\n";

				int choice2 = Choice(1, 3);

				if (choice2 == 1)
				{
					std::cout << "Which columns you'd like to interchange (type 2 numbers in separate lines)\n";
					int column1 = Choice(1, GetWidth(current_matrix, current_type)) - 1;
					int column2 = Choice(1, GetWidth(current_matrix, current_type)) - 1;

					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->InterchangeColumns(column1, column2);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->InterchangeColumns(column1, column2);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->InterchangeColumns(column1, column2);
					}

					PrintOneMatrix(current_matrix, current_type);

				}
				else if (choice2 == 2)
				{
					std::cout << "Which column you'd like to multiply?\n";
					int column = Choice(1, GetWidth(current_matrix, current_type)) - 1;
					std::cout << "By what scalar you'd like to multiply this column?\n";
					if (current_type == 1)
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> int_scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid integer number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					else
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}

					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MultiplyColumn(column, int_scalar);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MultiplyColumn(column, scalar);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MultiplyColumn(column, scalar);
					}

					PrintOneMatrix(current_matrix, current_type);
				}
				else
				{
					std::cout << "Which column you'd like to multiply?\n";
					int column1 = Choice(1, GetWidth(current_matrix, current_type)) - 1;
					std::cout << "By what scalar you'd like to multiply this column?\n";
					if (current_type == 1)
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> int_scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid integer number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					else
					{
						while (true)
						{
							std::cout << ">>> ";
							std::cin >> scalar;
							if (std::cin.fail())
							{
								std::cout << "Type valid number, please" << std::endl;
								std::cin.clear();
								std::cin.ignore(1000, '\n');
								continue;
							}

							break;
						}
					}
					std::cout << "To which column you'd like to add that column?\n";
					int column2 = Choice(1, GetWidth(current_matrix, current_type)) - 1;


					switch (current_type)
					{
					case 1:
						current_matrix = &static_cast<Matrix<int>*>(current_matrix)->MultiplyColumnAndAdd(column1, int_scalar, column2);
						break;
					case 2:
						current_matrix = &static_cast<Matrix<double>*>(current_matrix)->MultiplyColumnAndAdd(column1, scalar, column2);
						break;
					default:
						current_matrix = &static_cast<Matrix<complex>*>(current_matrix)->MultiplyColumnAndAdd(column1, scalar, column2);
						break;
					}
					PrintOneMatrix(current_matrix, current_type);
				}

			}
			break;
		case 8:
			return;
		default:
			exit(0);
		}
	}
}
void VectorClient(vectors* saved_vectors)
{
	int int_amount = saved_vectors->int_v ? saved_vectors->int_v->GetLength() : 0;
	int double_amount = saved_vectors->double_v ? saved_vectors->double_v->GetLength() : 0;
	int complex_amount = saved_vectors->complex_v ? saved_vectors->complex_v->GetLength() : 0;


	void* current_vector = nullptr;
	int curr_type = 0;
	int& current_type = curr_type;
	if (saved_vectors->number_of_vectors == 0)
	{
		current_vector = CreateVector(saved_vectors, current_type);
		switch (current_type)
		{
		case 1:
			int_amount++;
			break;
		case 2:
			double_amount++;
			break;
		default:
			complex_amount++;
			break;
		}
	}
	else
	{
		PrintVectorsInMemory(saved_vectors);
		std::cout << "\nDo you want to work with these vectors or create a new one?\n";
		std::cout << "1) Work with these vectors\n";
		std::cout << "2) Create a new vector\n";

		switch (Choice(1, 2))
		{
		case 1:
			std::cout << std::endl << "With which one you'd like to work?\n";
			current_vector = ChooseVector(saved_vectors, current_type);
			break;
		default:
			current_vector = CreateVector(saved_vectors, current_type);
			switch (current_type)
			{
			case 1:
				int_amount++;
				break;
			case 2:
				double_amount++;
				break;
			default:
				complex_amount++;
				break;
			}
		}

	}

	while (true)
	{
		std::cout << std::endl << "\nWhat do you want to do with the vector?" << std::endl;
		std::cout << "1) Print" << std::endl;
		std::cout << "2) Add another vector" << std::endl;
		std::cout << "3) Multiply by a scalar" << std::endl;
		std::cout << "4) Find norm" << std::endl;
		std::cout << "5) Find dot product with other vector" << std::endl;
		std::cout << "6) Go to main menu" << std::endl;
		std::cout << "7) Exit" << std::endl;

		void* other_vector = nullptr;
		int oth_type = 0;
		int& other_type = oth_type;

		int choice1 = 0;
		int int_scalar = 0;
		double scalar = 0.0;
		switch (Choice(1, 7))
		{
		case 1:
			// print
			PrintOneVector(current_vector, current_type);
			break;
		case 2:
			// add vector
			PrintVectorsInMemory(saved_vectors);
			std::cout << "\nWould you like to use vector in memory or create a new one?\n";
			std::cout << "1) Use one of saved vectors\n";
			std::cout << "2) Create a new vector\n";
			if(Choice(1, 2) == 1)
			{
				while (true)
				{
					std::cout << std::endl << "Which vector you'd like to add to the previous one?\n";
					other_vector = ChooseVector(saved_vectors, other_type);
					if (current_type != other_type)
					{
						std::cout << std::endl << "These vectors have different types. Please choose vector with the same type\n";
						continue;
					}
					break;
				}
			}
			else
			{
				other_vector = CreateVector(saved_vectors, other_type, current_type, GetDim(current_vector, current_type));
			}
			switch (current_type)
			{
			case 1:
				current_vector = &static_cast<Vector<int>*>(current_vector)->VectorSum(static_cast<Vector<int>*>(other_vector));
				break;
			case 2:
				current_vector = &static_cast<Vector<double>*>(current_vector)->VectorSum(static_cast<Vector<double>*>(other_vector));
				break;
			default:
				current_vector = &static_cast<Vector<complex>*>(current_vector)->VectorSum(static_cast<Vector<complex>*>(other_vector));
			}

			PrintOneVector(current_vector, current_type);

			break;
		case 3:
			// multiply by scalar
			std::cout << "\nBy which scalar should vector be multiplied?\n";
			if (curr_type == 1)
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> int_scalar;
					if (std::cin.fail())
					{
						std::cout << "Type valid integer number, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}

					break;
				}
			}
			else
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> scalar;
					if (std::cin.fail())
					{
						std::cout << "Type valid number, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}

					break;
				}
			}
			switch (current_type)
			{
			case 1:
				current_vector = &static_cast<Vector<int>*>(current_vector)->ScalarMult(int_scalar);
				break;
			case 2:
				current_vector = &static_cast<Vector<double>*>(current_vector)->ScalarMult(scalar);
				break;
			default:
				current_vector = &static_cast<Vector<complex>*>(current_vector)->ScalarMult(scalar);
			}

			PrintOneVector(current_vector, current_type);

			break;
		case 4:
			// find norm
			if (current_type == 1)
			{
				int int_norm = static_cast<Vector<int>*>(current_vector)->Norm();
				std::cout << "Norm is: " << int_norm << std::endl;
			}
			else if (current_type == 2)
			{
				double double_norm = static_cast<Vector<double>*>(current_vector)->Norm();
				std::cout << "Norm is: " << double_norm << std::endl;
			}
			else
			{
				complex complex_norm = static_cast<Vector<complex>*>(current_vector)->Norm();
				std::cout << "Norm is: " << complex_norm << std::endl;
			}
			break;
		case 5:
			// dot product
			PrintVectorsInMemory(saved_vectors);
			std::cout << "\nWould you like to use vector in memory or create a new one?\n";
			std::cout << "1) Use one of saved vectors\n";
			std::cout << "2) Create a new vector\n";
			if (Choice(1, 2) == 1)
			{
				while (true)
				{
					std::cout << std::endl << "With which vector you'd like to find dot product?\n";
					other_vector = ChooseVector(saved_vectors, other_type);
					if (current_type != other_type)
					{
						std::cout << std::endl << "These vectors have different types. Please choose vector with the same type\n";
						continue;
					}
					break;
				}
			}else
			{
				other_vector = CreateVector(saved_vectors, other_type, current_type, GetDim(current_vector, current_type));
			}
			switch (current_type)
			{
			case 1:
				std::cout << "Dot product is: " << static_cast<Vector<int>*>(current_vector)->DotProduct(static_cast<Vector<int>*>(other_vector));
				break;
			case 2:
				std::cout << "Dot product is: " << static_cast<Vector<double>*>(current_vector)->DotProduct(static_cast<Vector<double>*>(other_vector));
				break;
			default:
				std::cout << "Dot product is: " << static_cast<Vector<complex>*>(current_vector)->DotProduct(static_cast<Vector<complex>*>(other_vector));
			}

			break;
		
		case 6:
			return;
		default:
			exit(0);
		}
	}
}

void* CreateMatrix(matrices* saved_matrices, int& m_type, int p_type, int p_height, int p_width)
{
	void* current_matrix = nullptr;

	int type;
	if(p_type)
	{
		type = p_type;
	}else
	{
		std::cout << "\nWhat type matrix should be?" << std::endl;
		std::cout << "1) int" << std::endl;
		std::cout << "2) double" << std::endl;
		std::cout << "3) complex" << std::endl;
		type = Choice(1, 3);
	}

	int height;
	if(p_height)
	{
		height = p_height;
	}else
	{
		std::cout << "What height should matrix have?" << std::endl;
		height = Choice(1);
	}

	int width;
	if(p_width)
	{
		width = p_width;
	}else
	{
		std::cout << "\nWhat width should matrix have?" << std::endl;
		width = Choice(1);
	}

	std::cout << "\nFill matrix randomly or with input data?" << std::endl;
	std::cout << "1) Randomly" << std::endl;
	std::cout << "2) Input data" << std::endl;

	int source = Choice(1,2);

	saved_matrices->number_of_matrices++;
	


	if(source == 1)
	{
		if(type==1)
		{
			auto* matrix = new Matrix<int>(height, width, RandIntSeq(height*width));
			if (!saved_matrices->int_m)
			{
				saved_matrices->int_m = new ArraySequence<Matrix<int>*>(matrix, 1);
			}
			else
			{
				saved_matrices->int_m->Append(matrix);
			}
			
			m_type = type;
			return matrix;
			
		}
		else if(type == 2)
		{
			auto* matrix = new Matrix<double>(height, width, RandDoubleSeq(height * width));
			if (!saved_matrices->double_m)
			{
				saved_matrices->double_m = new ArraySequence<Matrix<double>*>(matrix, 1);
			}
			else
			{
				saved_matrices->double_m->Append(matrix);
			}

			m_type = type;
			return matrix;
		}
		else
		{
			auto* matrix = new Matrix<complex>(height, width, RandComplexSeq(height * width));
			if (!saved_matrices->complex_m)
			{
				saved_matrices->complex_m = new ArraySequence<Matrix<complex>*>(matrix, 1);
			}
			else
			{
				saved_matrices->complex_m->Append(matrix);
			}

			m_type = type;
			return matrix;
		}
	}
	else
	{
		if (type == 1)
		{
			auto* matrix = new Matrix<int>(height, width, 0);
			current_matrix = matrix;
			if (!saved_matrices->int_m)
			{
				saved_matrices->int_m = new ArraySequence<Matrix<int>*>(matrix, 1);
			}
			else
			{
				saved_matrices->int_m->Append(matrix);
			}
		}
		else if (type == 2)
		{
			auto* matrix = new Matrix<double>(height, width, 0.0);
			current_matrix = matrix;
			if (!saved_matrices->double_m)
			{
				saved_matrices->double_m = new ArraySequence<Matrix<double>*>(matrix, 1);
			}
			else
			{
				saved_matrices->double_m->Append(matrix);
			}
		}
		else
		{
			auto* matrix = new Matrix<complex>(height, width, complex(0, 0));
			current_matrix = matrix;
			if (!saved_matrices->complex_m)
			{
				saved_matrices->complex_m = new ArraySequence<Matrix<complex>*>(matrix, 1);
			}
			else
			{
				saved_matrices->complex_m->Append(matrix);
			}
		}
		std::cout << std::endl << "Type every new matrix element in separate line" << std::endl;
	
	
		if (type == 1)
		{

			int n = 0;
			auto* matrix = static_cast<Matrix<int>*>(current_matrix);
			for (int i = 0; i < height; i++)
			{
				std::cout << "Row " << i << std::endl;
				for (int j = 0; j < width; j++)
				{
					while (true)
					{
						std::cout << ">>> ";
						std::cin >> n;
						if (std::cin.fail())
						{
							std::cout << "Type valid element, please" << std::endl;
							std::cin.clear();
							std::cin.ignore(1000, '\n');
							continue;
						}
						*matrix = matrix->ChangeElement(i, j, n);
						break;
					}

				}
			}

			m_type = type;
			return matrix;
		}
	
		else if (type == 2)
		{
			double n = 0.0;
			auto* matrix = static_cast<Matrix<double>*>(current_matrix);
			for (int i = 0; i < height; i++)
			{
				std::cout << "Row " << i << std::endl;
				for (int j = 0; j < width; j++)
				{
					while (true)
					{
						std::cout << ">>> ";
						std::cin >> n;
						if (std::cin.fail())
						{
							std::cout << "Type valid element, please" << std::endl;
							std::cin.clear();
							std::cin.ignore(1000, '\n');
							continue;
						}
						*matrix = matrix->ChangeElement(i, j, n);
						break;
					}

				}
			}

			m_type = type;
			return matrix;
		}

		else
		{
			double re = 0.0;
			double im = 0.0;
			auto* matrix = static_cast<Matrix<complex>*>(current_matrix);
			for (int i = 0; i < height; i++)
			{
				std::cout << "Row " << i << std::endl;
				for (int j = 0; j < width; j++)
				{
					while (true)
					{
						std::cout << "Type real part\n>>> ";
						std::cin >> re;
						if (std::cin.fail())
						{
							std::cout << "Type valid element, please" << std::endl;
							std::cin.clear();
							std::cin.ignore(1000, '\n');
							continue;
						}
						std::cout << "Type imaginary part\n>>> ";
						std::cin >> im;
						if (std::cin.fail())
						{
							std::cout << "Type valid element, please" << std::endl;
							std::cin.clear();
							std::cin.ignore(1000, '\n');
							continue;
						}
						*matrix = matrix->ChangeElement(i, j, complex(re, im));
						break;
					}

				}
			}

			m_type = type;
			return matrix;
		}
	}

}

void PrintOneMatrix(void* matrix, int type)
{
	if (type == 1)
	{
		std::cout << *static_cast<Matrix<int>*>(matrix);
	}
	else if (type == 2)
	{
		std::cout << *static_cast<Matrix<double>*>(matrix);
	}
	else
	{
		std::cout << *static_cast<Matrix<complex>*>(matrix);
	}
}
void PrintMatricesInMemory(matrices* saved_matrices)
{
	int int_amount = saved_matrices->int_m ? saved_matrices->int_m->GetLength() : 0;
	int double_amount = saved_matrices->double_m ? saved_matrices->double_m->GetLength() : 0;
	int complex_amount = saved_matrices->complex_m ? saved_matrices->complex_m->GetLength() : 0;

	int cnt = 1;
	std::cout << "\nThere are " << saved_matrices->number_of_matrices << " matrices in memory: " << std::endl;
	for (int i = 0; i < int_amount; i++)
	{
		auto* m = saved_matrices->int_m->Get(i);
		std::cout << cnt++ << " : int " << m->GetHeight() << "x" << m->GetWidth() << " " << std::endl;
	}

	for (int i = 0; i < double_amount; i++)
	{
		auto* m = saved_matrices->double_m->Get(i);
		std::cout << cnt++ <<  " : double " << m->GetHeight() << "x" << m->GetWidth() << " " << std::endl;
	}

	for (int i = 0; i < complex_amount; i++)
	{
		auto* m = saved_matrices->complex_m->Get(i);
		std::cout << cnt++ << " : complex " << m->GetHeight() << "x" << m->GetWidth() << " " << std::endl;
	}
}

void* ChooseMatrix(matrices* saved_matrices, int& type)
{
	int int_amount = saved_matrices->int_m ? saved_matrices->int_m->GetLength() : 0;
	int double_amount = saved_matrices->double_m ? saved_matrices->double_m->GetLength() : 0;
	int complex_amount = saved_matrices->complex_m ? saved_matrices->complex_m->GetLength() : 0;

	int choose_matrix = Choice(1, saved_matrices->number_of_matrices);
	if (choose_matrix <= int_amount)
	{
		type = 1;
		return saved_matrices->int_m->Get(choose_matrix - 1);
	}
	if (choose_matrix <= int_amount + double_amount)
	{
		type = 2;
		return saved_matrices->double_m->Get(choose_matrix - int_amount - 1);
	}
	if(choose_matrix<=saved_matrices->number_of_matrices)
	{
		type = 3;
		return saved_matrices->complex_m->Get(choose_matrix - int_amount - double_amount - 1);
	}

	throw std::exception("Unexpected Error");

}

int GetWidth(void* matrix, int type)
{
	switch (type)
	{
	case 1:
		return static_cast<Matrix<int>*>(matrix)->GetWidth();
		break;
	case 2:
		return static_cast<Matrix<double>*>(matrix)->GetWidth();
		break;
	default:
		return static_cast<Matrix<complex>*>(matrix)->GetWidth();
	}
}
int GetHeight(void* matrix, int type)
{
	switch (type)
	{
	case 1:
		return static_cast<Matrix<int>*>(matrix)->GetHeight();
		break;
	case 2:
		return static_cast<Matrix<double>*>(matrix)->GetHeight();
		break;
	default:
		return static_cast<Matrix<complex>*>(matrix)->GetHeight();
	}
}
void PrintOneVector(void* vector, int type)
{
	if (type == 1)
	{
		std::cout << *static_cast<Vector<int>*>(vector);
	}
	else if (type == 2)
	{
		std::cout << *static_cast<Vector<double>*>(vector);
	}
	else
	{
		std::cout << *static_cast<Vector<complex>*>(vector);
	}
}
void PrintVectorsInMemory(vectors* saved_vectors)
{
	int int_amount = saved_vectors->int_v ? saved_vectors->int_v->GetLength() : 0;
	int double_amount = saved_vectors->double_v ? saved_vectors->double_v->GetLength() : 0;
	int complex_amount = saved_vectors->complex_v ? saved_vectors->complex_v->GetLength() : 0;

	int cnt = 1;
	std::cout << "\nThere are " << saved_vectors->number_of_vectors << " vectors in memory: " << std::endl;
	for (int i = 0; i < int_amount; i++)
	{
		auto* v = saved_vectors->int_v->Get(i);
		std::cout <<cnt++<< " : int " << v->GetDimension()<<"-dimensional" << std::endl;
	}

	for (int i = 0; i < double_amount; i++)
	{
		auto* v = saved_vectors->double_v->Get(i);
		std::cout <<cnt++<< " : double " << v->GetDimension() << "-dimensional" << std::endl;
	}

	for (int i = 0; i < complex_amount; i++)
	{
		auto* v = saved_vectors->complex_v->Get(i);
		std::cout <<cnt++<< " : complex " << v->GetDimension() << "-dimensional" << std::endl;
	}
}
void* ChooseVector(vectors* saved_vectors, int& type)
{
	int int_amount = saved_vectors->int_v ? saved_vectors->int_v->GetLength() : 0;
	int double_amount = saved_vectors->double_v ? saved_vectors->double_v->GetLength() : 0;
	int complex_amount = saved_vectors->complex_v ? saved_vectors->complex_v->GetLength() : 0;

	int choose_vector = Choice(1, saved_vectors->number_of_vectors);
	if (choose_vector <= int_amount)
	{
		type = 1;
		return saved_vectors->int_v->Get(choose_vector - 1);
	}
	if (choose_vector <= int_amount + double_amount)
	{
		type = 2;
		return saved_vectors->double_v->Get(choose_vector - int_amount - 1);
	}
	if (choose_vector <= saved_vectors->number_of_vectors)
	{
		type = 3;
		return saved_vectors->complex_v->Get(choose_vector - int_amount - double_amount - 1);
	}

	throw std::exception("Unexpected Error");
}
void* CreateVector(vectors* saved_vectors, int& v_type, int p_type , int p_dim )
{
	void* current_vector = nullptr;
	int type;
	if(p_type)
	{
		type = p_type;
	}else
	{
		std::cout << "\nWhat type vector should be?" << std::endl;
		std::cout << "1) int" << std::endl;
		std::cout << "2) double" << std::endl;
		std::cout << "3) complex" << std::endl;
		type = Choice(1, 3);
	}
	v_type = type;

	int dim;
	if (p_dim)
	{
		dim = p_dim;
	}
	else
	{
		std::cout << "What dimension should vector be?" << std::endl;
		dim = Choice(1);
	}

	std::cout << "\nFill vector randomly or with input data?" << std::endl;
	std::cout << "1) Randomly" << std::endl;
	std::cout << "2) Input data" << std::endl;

	int source = Choice(1, 2);

	saved_vectors->number_of_vectors++;

	
	if (source == 1)
	{
		if (type == 1)
		{
			auto* vector = new Vector<int>(dim, RandIntSeq(dim));
			if (!saved_vectors->int_v)
			{
				saved_vectors->int_v = new ArraySequence<Vector<int>*>(vector, 1);
			}
			else
			{
				saved_vectors->int_v->Append(vector);
			}
			return vector;

		}
		else if (type == 2)
		{
			auto* vector = new Vector<double>(dim, RandDoubleSeq(dim));
			if (!saved_vectors->double_v)
			{
				saved_vectors->double_v = new ArraySequence<Vector<double>*>(vector, 1);
			}
			else
			{
				saved_vectors->double_v->Append(vector);
			}
			return vector;
		}
		else
		{
			auto* vector = new Vector<complex>(dim, RandComplexSeq(dim));
			if (!saved_vectors->complex_v)
			{
				saved_vectors->complex_v = new ArraySequence<Vector<complex>*>(vector, 1);
			}
			else
			{
				saved_vectors->complex_v->Append(vector);
			}
			return vector;
		}
	}
	else
	{
		if (type == 1)
		{
			auto* vector = new Vector<int>(dim, 0);
			current_vector = vector;
			if (!saved_vectors->int_v)
			{
				saved_vectors->int_v = new ArraySequence<Vector<int>*>(vector, 1);
			}
			else
			{
				saved_vectors->int_v->Append(vector);
			}
		}
		else if (type == 2)
		{
			auto* vector = new Vector<double>(dim, 0.0);
			current_vector = vector;
			if (!saved_vectors->double_v)
			{
				saved_vectors->double_v = new ArraySequence<Vector<double>*>(vector, 1);
			}
			else
			{
				saved_vectors->double_v->Append(vector);
			}
		}
		else
		{
			auto* vector = new Vector<complex>(dim, complex(0, 0));
			current_vector = vector;
			if (!saved_vectors->complex_v)
			{
				saved_vectors->complex_v = new ArraySequence<Vector<complex>*>(vector, 1);
			}
			else
			{
				saved_vectors->complex_v->Append(vector);
			}
		}
		std::cout << std::endl << "Type every new vector element in separate line" << std::endl;


		if (type == 1)
		{

			int n = 0;
			auto* vector = static_cast<Vector<int>*>(current_vector);
			for (int i = 0; i < dim; i++)
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> n;
					if (std::cin.fail())
					{
						std::cout << "Type valid element, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}
					*vector = vector->ChangeCoordinate(i, n);
					break;
				}


			}

			return vector;
		}

		else if (type == 2)
		{
			double n = 0.0;
			auto* vector = static_cast<Vector<double>*>(current_vector);
			for (int i = 0; i < dim; i++)
			{
				while (true)
				{
					std::cout << ">>> ";
					std::cin >> n;
					if (std::cin.fail())
					{
						std::cout << "Type valid element, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}
					*vector = vector->ChangeCoordinate(i, n);
					break;
				}


			}

			return vector;
		}

		else
		{
			double re = 0.0;
			double im = 0.0;
			auto* vector = static_cast<Vector<complex>*>(current_vector);
			for (int i = 0; i < dim; i++)
			{
				while (true)
				{
					std::cout << "Type real part\n>>> ";
					std::cin >> re;
					if (std::cin.fail())
					{
						std::cout << "Type valid element, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}
					std::cout << "Type imaginary part\n>>> ";
					std::cin >> im;
					if (std::cin.fail())
					{
						std::cout << "Type valid element, please" << std::endl;
						std::cin.clear();
						std::cin.ignore(1000, '\n');
						continue;
					}
					*vector = vector->ChangeCoordinate(i, complex(re, im));
					break;
				}


			}

			return vector;
		}
	}
}


int GetDim(void* vector, int type)
{
	switch (type)
	{
	case 1:
		return static_cast<Vector<int>*>(vector)->GetDimension();
		break;
	case 2:
		return static_cast<Vector<double>*>(vector)->GetDimension();
		break;
	default:
		return static_cast<Vector<complex>*>(vector)->GetDimension();
	}
}

std::complex<double>* RandComplexSeq(int size)
{
	srand(time(NULL));
	auto* seq = new std::complex<double>[size];
	for (int i = 0; i < size; i++)
	{
		*(seq + i) = std::complex<double>(static_cast<double>((rand() % 50 - 25) / 17.0), static_cast<double>((rand() % 50 - 25) / 17.0));
	}

	return seq;
}
