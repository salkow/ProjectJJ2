#include <iostream>

#include "editDistance.h"

#include "../my_string.h"

using bud::string;

unsigned long getEdit(string f, string s, unsigned int tolerance){//this function uses 1 2d array with b+1 for the x and a+1 for y lengths.
    unsigned long a=f.size(), b=s.size();
    // std::cout << a << std::endl << b << std::endl;
    // unsigned long d[b+1][a+1];
    unsigned long** d = new unsigned long*[b+1];
    for(unsigned long i=0;i<b+1;i++){
        d[i] = new unsigned long[a+1];
    }


    for(unsigned long i=0;i<a+1;i++){//we need to initialize the first row and the first column from 0 to a+1 or b+1 respectively
        d[0][i] = i;
    }
    for(unsigned long i=0;i<b+1;i++){
        d[i][0] = i;
    }


    for(unsigned long i=1;i<b+1;i++){
        for(unsigned long j=1;j<a+1;j++){//for each cell
            if(f[j-1] == s[i-1]){//check if the current letters for each word are the same
                d[i][j] = d[i-1][j-1];//if yes get the replace diagonal cell since we dont need to make any changes
            }else{//if not get the minimum out of the replace, insert, delete cells and add 1
                if(d[i-1][j-1] <= d[i-1][j]){
                    if(d[i-1][j-1] <= d[i][j-1]){
                        d[i][j] = d[i-1][j-1]+1;//replace case
                    }else{
                        d[i][j] = d[i][j-1]+1;//insert case
                    }
                }else{
                    if(d[i-1][j] <= d[i][j-1]){
                        d[i][j] = d[i-1][j]+1;//delete case
                    }else{
                        d[i][j] = d[i][j-1]+1;//insert case
                    }
                }
            }
            if(d[i][j] >= tolerance){
                unsigned long fin = d[i][j];
                for(unsigned long k=0;k<b+1;k++){
                    delete[] d[k];
                }
                delete[] d;
                return fin;
            }
        }
    }
    unsigned long fin = d[b][a];//get the final, clear the memory and return it
    for(unsigned long i=0;i<b+1;i++){
        delete[] d[i];
    }
    delete[] d;
    return fin;
}
