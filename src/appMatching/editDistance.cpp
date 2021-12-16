#include <iostream>

#include "editDistance.h"
#include <climits>

#include "../my_string.h"

using bud::string;

unsigned long getEdit(string &f, string &s, unsigned int max)
{

	int matrix[2][s.size() + 1];
	int i;
	int j;

	/*
      Initialize the 0 row of "matrix".

        0
        1
        2
        3

     */

	for (j = 0; j <= s.size(); j++)
	{
		matrix[0][j] = j;
	}

	/* Loop over column. */
	for (i = 1; i <= f.size(); i++)
	{
		char c1;
		/* The first value to consider of the ith column. */
		int min_j;
		/* The last value to consider of the ith column. */
		int max_j;
		/* The smallest value of the matrix in the ith column. */
		int col_min;
		/* The next column of the matrix to fill in. */
		int next;
		/* The previously-filled-in column of the matrix. */
		int prev;

		c1 = f[i - 1];
		min_j = 1;
		if (i > max)
		{
			min_j = i - max;
		}
		max_j = s.size();
		if (s.size() > max + i)
		{
			max_j = max + i;
		}
		col_min = INT_MAX;
		next = i % 2;
		if (next == 1)
		{
			prev = 0;
		}
		else
		{
			prev = 1;
		}
		matrix[next][0] = i;
		/* Loop over rows. */
		for (j = 1; j <= s.size(); j++)
		{
			if (j < min_j || j > max_j)
			{
				/* Put a large value in there. */
				matrix[next][j] = max + 1;
			}
			else
			{
				char c2;

				c2 = s[j - 1];
				if (c1 == c2)
				{
					/* The character at position i in f is the same as
                       the character at position j in s. */
					matrix[next][j] = matrix[prev][j - 1];
				}
				else
				{
					/* The character at position i in f is not the
                       same as the character at position j in s, so
                       work out what the minimum cost for getting to cell
                       i, j is. */
					int del;
					int insert;
					int substitute;
					int minimum;

					del = matrix[prev][j] + 1;
					insert = matrix[next][j - 1] + 1;
					substitute = matrix[prev][j - 1] + 1;
					minimum = del;
					if (insert < minimum)
					{
						minimum = insert;
					}
					if (substitute < minimum)
					{
						minimum = substitute;
					}
					matrix[next][j] = minimum;
				}
			}
			/* Find the minimum value in the ith column. */
			if (matrix[next][j] < col_min)
			{
				col_min = matrix[next][j];
			}
		}
		if (col_min > max)
		{
			/* All the elements of the ith column are greater than the
               maximum, so no match less than or equal to max can be
               found by looking at succeeding columns. */
			return max + 1;
		}
	}
	return matrix[f.size() % 2][s.size()];
}

// unsigned long getEdit(string &f, string &s, unsigned int tolerance)
// { // this function uses 1 2d array with b+1 for the x and a+1 for y lengths.
// 	unsigned long a = f.size(), b = s.size();
// 	// std::cout << a << std::endl << b << std::endl;
// 	// unsigned long d[b+1][a+1];
// 	unsigned long **d = new unsigned long *[b + 1];
// 	for (unsigned long i = 0; i < b + 1; i++)
// 	{
// 		d[i] = new unsigned long[a + 1];
// 	}

// 	for (unsigned long i = 0; i < a + 1; i++)
// 	{ // we need to initialize the first row and the first column from 0 to a+1 or b+1 respectively
// 		d[0][i] = i;
// 	}
// 	for (unsigned long i = 0; i < b + 1; i++)
// 	{
// 		d[i][0] = i;
// 	}

// 	for (unsigned long i = 1; i < b + 1; i++)
// 	{
// 		for (unsigned long j = 1; j < a + 1; j++)
// 		{ // for each cell
// 			if (f[j - 1] == s[i - 1])
// 			{							   // check if the current letters for each word are the same
// 				d[i][j] = d[i - 1][j - 1]; // if yes get the replace diagonal cell since we dont
// 										   // need to make any changes
// 			}
// 			else
// 			{ // if not get the minimum out of the replace, insert, delete cells and add 1
// 				if (d[i - 1][j - 1] <= d[i - 1][j])
// 				{
// 					if (d[i - 1][j - 1] <= d[i][j - 1])
// 					{
// 						d[i][j] = d[i - 1][j - 1] + 1; // replace case
// 					}
// 					else
// 					{
// 						d[i][j] = d[i][j - 1] + 1; // insert case
// 					}
// 				}
// 				else
// 				{
// 					if (d[i - 1][j] <= d[i][j - 1])
// 					{
// 						d[i][j] = d[i - 1][j] + 1; // delete case
// 					}
// 					else
// 					{
// 						d[i][j] = d[i][j - 1] + 1; // insert case
// 					}
// 				}
// 			}
// 			if (tolerance != 0 && d[i][j] >= tolerance)
// 			{
// 				unsigned long fin = d[i][j];
// 				for (unsigned long k = 0; k < b + 1; k++)
// 				{
// 					delete[] d[k];
// 				}
// 				delete[] d;
// 				return fin;
// 			}
// 		}
// 	}
// 	unsigned long fin = d[b][a]; // get the final, clear the memory and return it
// 	for (unsigned long i = 0; i < b + 1; i++)
// 	{
// 		delete[] d[i];
// 	}
// 	delete[] d;
// 	return fin;
// }
