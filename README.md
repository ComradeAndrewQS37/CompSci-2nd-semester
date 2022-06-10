# Programming Course 2nd semester
## Лабораторная работа №1  
Реализация абстрактного типа данных – «полиморфной» коллекции на основе динамического массива на языке C. 
Под полиморфным, в данном случае, подразумевается такой массив, который может хранить (и обрабатывать) значения различных типов. 
Для реализации используется концепция `void*`.
В данном случае реализованы коллекции для хранения целых чисел и строк.
## Лабораторная работа №2
Реализация полиморфного абстрактного типа данных (АТД) – с помощью нескольких уровней абстракции, на языке С++ 
На нижнем уровне реализуются структуры для организации данных в памяти – [DynamicArray](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab2%20Sequence/DynamicArray.h) и [LinkedList](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab2%20Sequence/LinkedList.h). 
Уровнем выше располагается абстракция ([Sequence](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab2%20Sequence/Sequence.h)), обеспечивающая возможности унифицированной работы со всеми структурами нижнего уровня. 
Целевой АТД, реализация которого и является конечной целью задания, располагается на самом верхнем уровне и использует АТД последовательность для реализации большинства операций. 
В данном случае в качестве главного типа данных реализована [матрица](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab2%20Sequence/Matrix.h) и основные операции на ней.
## Лабораторная работа №3
Реализация абстрактного типа данных на основе структур данных типа дерево на языке C++. В данном варианте реализованы [бинарное дерево поиска](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab3%20Trees/BinaryTree.h) с реализацией обхода по заданному шаблону и [n-арное дерево](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab3%20Trees/NTree.h). 
В данной работе используются типы данных, написанные в предыдущей лабораторной работе (например, [ArraySequence](https://github.com/ComradeAndrewQS37/CompSci-2nd-semester/blob/master/Lab3%20Trees/ArraySequence.h)).
