#include <iostream>

#include "editDistance.h"
#include <climits>

#include "../my_string.h"
#include "../util.h"
#include "../vector.h"

using bud::string;
using bud::min;

 unsigned long getEdit(string &f, string &s, unsigned int tolerance)
 { // this function uses 1 2d array with b+1 for the x and a+1 for y lengths.
 	unsigned int a = s.size(), b = f.size();
 	unsigned int d[MAX_WORD_LENGTH][MAX_WORD_LENGTH];

 	for (unsigned int i = 0; i < a + 1; i++)
 	{ // we need to initialize the first row and the first column from 0 to a+1 or b+1 respectively
		 d[0][i] = i;
 	}
 	for (unsigned int i = 1; i < b + 1; i++)
 	{
		d[i][0] = i;
		unsigned int col_min = INT_MAX;
 		for (unsigned int j = 1; j < a + 1; j++)
 		{ // for each cell
 			if (s[j - 1] == f[i - 1])
 			{							   // check if the current letters for each word are the same
 				d[i][j] = d[i - 1][j - 1]; // if yes get the replace diagonal cell since we dont
 										   // need to make any change
 			}
 			else
 			{ // if not get the minimum out of the replace, insert, delete cells and add 1
				d[i][j] = min(d[i-1][j-1]+1, d[i-1][j]+1, d[i][j-1]+1);
 			}
			if(col_min > d[i][j]){
				col_min = d[i][j];
			}
 		}
		if (col_min >= tolerance)
		{
			return col_min;
		}
 	}
 	return d[b][a];
 }