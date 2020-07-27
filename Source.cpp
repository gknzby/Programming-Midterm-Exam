#include <iostream>
#include <vector>
#include <time.h>

using namespace std;

int** jobWorkMat;
int** temp;
int n = 0;
int total = 0;

void printMatrix()
{
	cout << endl;

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
			cout << jobWorkMat[i][j] << "\t";
		cout << endl;
	}

}

void findAndSubRow()	//step 1
{
	int min;

	for (int i = 0; i < n; i++)
	{
		min = jobWorkMat[i][0];
		for (int j = 0; j < n; j++)
		{
			if (jobWorkMat[i][j] < min)
				min = jobWorkMat[i][j];
		}
		for (int j = 0; j < n; j++)
		{
			jobWorkMat[i][j] -= min;
		}
	}
}

void findAndSubCol()
{
	int min;

	for (int i = 0; i < n; i++)
	{
		min = jobWorkMat[0][i];
		for (int j = 0; j < n; j++)
		{
			if (jobWorkMat[j][i] < min)
				min = jobWorkMat[j][i];
		}
		for (int j = 0; j < n; j++)
		{
			jobWorkMat[j][i] -= min;
		}
	}
}
void countZeros(int **zeros)
{
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < n; j++)
			zeros[i][j] = 0;
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (jobWorkMat[i][j] == 0)
			{
				zeros[0][i]++;
				zeros[1][j]++;
			}
		}
	}
}

void tmpPrint(int** zeros)
{
	cout << endl;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < n; j++)
			cout << zeros[i][j] << "\t";
		cout << endl;
	}
}

void findMax(int** zeros, int &k, int &l, int &max)
{
	max = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (max < zeros[i][j])
			{
				k = i;
				l = j;
				max = zeros[i][j];
			}
		}
	}
}

void reCalZeros(int** zeros, int k, int l)
{
	if (k == 0)
	{
		for (int i = 0; i < n; i++)
		{
			if (jobWorkMat[l][i] == 0)
			{
				zeros[0][l]--;
				zeros[1][i]--;
			}
		}
	}
	else
	{
		for (int i = 0; i < n; i++)
		{
			if (jobWorkMat[i][l] == 0)
			{
				zeros[1][l]--;
				zeros[0][i]--;
			}
		}
	}
}

vector<int*> coverRowCol()
{
	vector<int*> covers;

	int** zeros = new int* [2];
	for (int i = 0; i < 2; i++)
		zeros[i] = new int[n];

	countZeros(zeros);

	int k = 0, l = 0, max = 0;
	findMax(zeros, k, l, max);

	while (max != 0)
	{
		int* cov = new int[2];
		cov[0] = k, cov[1] = l;
		covers.push_back(cov);
		reCalZeros(zeros, k, l);
		findMax(zeros, k, l, max);
	}

	return covers;
}

void extraZeros(vector<int*> covers)
{
	int* row = new int[n];
	int* col = new int[n];
	int min = 100;

	for (int i = 0; i < n; i++)
		row[i] = 0, col[i] = 0;

	for (int i = 0; i < covers.size(); i++)
	{
		if (covers[i][0] == 0)
		{
			row[covers[i][1]]++;
		}
		else
		{
			col[covers[i][1]]++;
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (row[i] == 0 && col[j] == 0 && jobWorkMat[i][j] < min)
			{
				min = jobWorkMat[i][j];
			}
		}
	}

	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (row[i] == 0 && col[j] == 0)
			{
				jobWorkMat[i][j] -= min;
			}
		}
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			if (row[i] != 0 && col[j] != 0 && jobWorkMat[i][j] != 0)
			{
				jobWorkMat[i][j] += min;
			}
		}
	}
}

bool isAloneRow(int k)
{
	int counter = 0;
	for (int i = 0; i < n; i++)
		if (jobWorkMat[k][i] == 0)
			counter++;
	return counter == 1 ? true : false;
}
//bool isAloneCol(int l)
//{
//	int counter = 0;
//	for (int i = 0; i < n; i++)
//		if (jobWorkMat[i][l] == 0)
//			counter++;
//	return counter == 1 ? true : false;
//}

void clearZero(vector<int*> covers, int k)
{
	for (int i = 0; i < n; i++)
		if (jobWorkMat[k][i] == 0)
		{
			cout << "Worker " << k << " works on " << i << endl;
			jobWorkMat[k][i] = 1;
			total += temp[k][i];
			
			for (int j = 0; j < n; j++)
			{
				if (jobWorkMat[j][i] == 0)
				{
					jobWorkMat[j][i] = 1;
				}
			}
		}
			
}
bool findOptimums(vector<int*> covers)
{
	int count = 1;
	int okay = n;
	while (okay)
	{
		for (int i = 0; i < n; i++)
		{
			if (covers[i][1] != -1 && isAloneRow(covers[i][1]))
			{
				okay--;
				clearZero(covers, covers[i][1]);
				covers[i][1] = -1;
			}
		}
		count++;
		if (count > 100)
			return true;
	}
	return false;
}

void generateRandom()
{


	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < n; j++)
		{
			jobWorkMat[i][j] = (rand() % 99) + 1;
			temp[i][j] = jobWorkMat[i][j];
		}
	}
}

int main()
{
	srand(time(NULL));

	string lol = " ";
	char lel = 'c';
	lol[0] = lel;

	cout << endl << lol << endl << endl;

	cout << "Please enter size of the matrix : ";
	cin >> n;
	jobWorkMat = new int* [n];
	for (int i = 0; i < n; i++)
	{
		jobWorkMat[i] = new int[n];
	}
	temp = new int* [n];
	for (int i = 0; i < n; i++)
	{
		temp[i] = new int[n];
	}

	bool again = true;
	while (again)
	{
		generateRandom();

		cout << endl << "Default matrix : ";
		printMatrix();

		findAndSubRow(); //step 1

		cout << endl << "After Step 1 : ";
		printMatrix();

		findAndSubCol(); //step 2

		cout << endl << "After Step 2 : ";
		printMatrix();

		vector<int*> covers = coverRowCol();

		while (covers.size() != n)
		{
			extraZeros(covers); //step 4

			cout << endl << "Needs extra zeros..." << endl << "After step 4 : ";
			printMatrix();
			covers = coverRowCol();
		}

		cout << endl;

		cout << endl << "Results of algorithm : " << endl;
		again = findOptimums(covers);
		cout << endl << "Total Assigment : " << total << endl << endl;
		if (again)
		{
		//	system("cls");
		}
			
	}


	return 0;
}