#pragma once


#include "BST Tests.h"
#include "NTree Tests.h"

#include <sstream>


void main_menu();
int Choice(int min, int max);

void BST_client();
void NTree_Client();

void* CreateBST(int& type);
void* CreateNTree(int& type);

double MultElements(double a, double b);
int MultElements(int a, int b);

double DivideBySeven(double a);