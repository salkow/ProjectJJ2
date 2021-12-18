#include "editDistance.h"
#include <climits>

using bud::string;
using bud::min;

 unsigned long getEdit(string &f, string &s, unsigned int tolerance)
 { // this function uses 1 2d array with b+1 for the x and a+1 for y lengths.
	bool has_swapped = false;
	if(f.size() > s.size()){//used so always f has the bigger word
		has_swapped = true;
		f.swap(s);
	}

	unsigned int a = f.size();
	unsigned int b = s.size();

 	unsigned int d[MAX_WORD_LENGTH][MAX_WORD_LENGTH];

	 //array initialization is needed
 	for (unsigned int i = 0; i < a + 1; i++)
 	{
		 d[0][i] = i;
 	}
 	for (unsigned int i = 1; i < b + 1; i++)
 	{
		d[i][0] = i;
		unsigned int currMin = INT_MAX; //calculate the row's minimum, if its > than the tolerance no more
										// calculation needed
 		for (unsigned int j = 1; j < a + 1; j++)
 		{
 			if (f[j - 1] == s[i - 1])		// check if the current letters for each word are the same
 			{
 				d[i][j] = d[i - 1][j - 1]; // if yes get the replace diagonal cell since we dont
 										   // need to make any change
 			}
 			else
 			{ // if not get the minimum out of the replace, insert, delete cells and add 1
				d[i][j] = min(d[i-1][j-1]+1, d[i-1][j]+1, d[i][j-1]+1);
 			}
			if(currMin > d[i][j]){
				currMin = d[i][j];
			}
 		}
		if (currMin >= tolerance)
		{
			if(has_swapped){
				s.swap(f);
			}
			return currMin;
		}
 	}
	 if(has_swapped){
		 s.swap(f);
	 }
 	return d[b][a];
 }