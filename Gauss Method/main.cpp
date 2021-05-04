#include <iostream>
#include <string>
#include <fstream>

template<typename T>
void print(T& stream, double** a, double* y, size_t n)
{
	for(size_t i = 0; i < n; i++)
	{
		for(size_t j = 0; j < n; j++)
		{
			stream << a[i][j] << "*x" << j;
			if(j < n - 1)
			{
				stream << " + ";
			}

		}
		stream << " = " << y[i] << "\n";
	}
	return;
}

template<typename T>
void print(T& stream, double* a, size_t size)
{
	for(size_t i = 0; i < size; i++)
	{
		stream << "x[" << i << "]=" << a[i] << "\n";
	}
}

double* gauss(double** a, double* y, int size, double eps = 0.00001)
{
	double* x;
	double max;
	int k;
	int index;
	x = new double[size];
	k = 0;
	while(k < size)
	{
		// Поиск строки с максимальным a[i][k]
		max = fabs(a[k][k]);
		index = k;
		for(int i = k + 1; i < size; i++)
		{
			if(fabs(a[i][k]) > max)
			{
				max = fabs(a[i][k]);
				index = i;
			}
		}
		// Перестановка строк
		if(max < eps)
		{
			// нет ненулевых диагональных элементов
			std::cout << "Решение получить невозможно из-за нулевого столбца ";
			std::cout << index << " матрицы A\n";
			std::abort();
			return 0;
		}
		for(int j = 0; j < size; j++)
		{
			double temp = a[k][j];
			a[k][j] = a[index][j];
			a[index][j] = temp;
		}
		double temp = y[k];
		y[k] = y[index];
		y[index] = temp;
		// Нормализация уравнений
		for(int i = k; i < size; i++)
		{
			double temp = a[i][k];
			if(abs(temp) < eps) continue; // для нулевого коэффициента пропустить
			for(int j = 0; j < size; j++)
				a[i][j] = a[i][j] / temp;
			y[i] = y[i] / temp;
			if(i == k)  continue; // уравнение не вычитать само из себя
			for(int j = 0; j < size; j++)
				a[i][j] = a[i][j] - a[k][j];
			y[i] = y[i] - y[k];
		}
		k++;
	}
	// обратная подстановка
	for(k = size - 1; k >= 0; k--)
	{
		x[k] = y[k];
		for(int i = 0; i < k; i++)
			y[i] = y[i] - a[i][k] * x[k];
	}
	return x;
}

template<typename T>
T getInputNumberFromUser()
{
	T size;
	std::string userInput;
	bool isCorrect = false;
	while(isCorrect == false)
	{
		std::cin >> userInput;
		try
		{
			system("cls");
			size = stod(userInput);
			isCorrect = true;
		}
		catch(const std::exception& name)
		{
			std::cout << "Нельзя вводить символы!\n" << name.what() << "\n";
		}
	}
	return (T)size;
}

size_t getSizeFromUser()
{
	std::cout << "Введите количество уравнений: ";
	int size;
	bool isCorrect = false;
	while(isCorrect == false)
	{
		size = getInputNumberFromUser<size_t>();
		if(size >= 2)
		{
			isCorrect = true;
		}
		else
		{
			std::cout << "Введите корректную размерность!\n";
		}
	}
	return size;
}

template<typename T>
void allocateMemory(T*& arr, size_t size)
{
	arr = new T[size];
}

template<typename T>
void allocateMemory(T**& arr, size_t size)
{
	arr = new T * [size];
	for(size_t i = 0; i < size; i++)
	{
		arr[i] = new T[size];
	}
}

void inputMatrixFromUser(double**& matrix, size_t size)
{
	for(size_t i = 0; i < size; i++)
	{
		for(size_t j = 0; j < size; j++)
		{
			std::cout << "A[" << i << "][" << j << "]= ";
			matrix[i][j] = getInputNumberFromUser<double>();
		}
	}
}

void inputFreeMemberFromUser(double*& arr, size_t size)
{
	for(size_t i = 0; i < size; i++)
	{
		std::cout << "y[" << i << "]= ";
		arr[i] = getInputNumberFromUser<double>();
	}
}

using namespace std;
int main(int arg, char** argc)
{
	setlocale(LC_ALL, "rus");
	double** matrixA;
	double* freeMembers;
	double* result;
	size_t size;
	size = getSizeFromUser();

	allocateMemory(matrixA, size);
	allocateMemory(freeMembers, size);

	inputMatrixFromUser(matrixA, size);
	inputFreeMemberFromUser(freeMembers, size);

	print(cout, matrixA, freeMembers, size);
	result = gauss(matrixA, freeMembers, size);

	print(cout, result, size);
	ofstream out;
	out.open("answer.txt");
	print(out, matrixA, freeMembers, size);
	print(out, result, size);

	out.close();

	system("pause");
	return 0;
}