#pragma once
#include "Vector.h"
#include "Matrix.h"
#include "TestAll.h"

struct matrices;
struct vectors;


void MainMenu();

int Choice(int min=INT_MIN, int max = INT_MAX);

void MatrixClient(matrices* saved_matrices);

void PrintOneMatrix(void* matrix, int type);
void PrintMatricesInMemory(matrices* saved_matrices);
void* ChooseMatrix(matrices* saved_matrices, int& type);
void* CreateMatrix(matrices* saved_matrices, int& m_type, int p_type = 0, int p_height = 0, int p_width = 0);


int GetWidth(void* matrix, int type);
int GetHeight(void* matrix, int type);


void VectorClient(vectors* saved_vectors);

void PrintOneVector(void* vector, int type);
void PrintVectorsInMemory(vectors* saved_vectors);
void* ChooseVector(vectors* saved_vectors, int& type);
void* CreateVector(vectors* saved_vectors, int& v_type, int p_type = 0, int p_dim = 0);


int GetDim(void* vector, int type);


std::complex<double>* RandComplexSeq(int size);