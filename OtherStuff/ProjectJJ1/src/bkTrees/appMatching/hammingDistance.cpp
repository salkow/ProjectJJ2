#include <iostream>

#include "hammingDistance.h"

#include "../../my_string.h"

using bud::string;

unsigned long getHamming(string f, string s){//simply for each letter (since we can assume that there are the same length)
    unsigned fin =0;
    for(unsigned i=0;i<f.size();i++){
        if(f[i] != s[i]){//check if they are the same or not.
            fin++;
        }
    }
    return fin;
}
