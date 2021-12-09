#ifndef _ENTRY_H_
#define _ENTRY_H_

#include "my_string.h"
#include "pair.h"
#include "unordered_set.h"
#include "query.h"

using Entry = bud::pair<bud::string, bud::unordered_set<Query *>>;

#endif
