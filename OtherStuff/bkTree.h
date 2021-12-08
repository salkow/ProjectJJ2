#ifndef BKTREE_HPP
#define BKTREE_HPP

#include "my_string.h"
#include "../include/core.h"
#include "util.h"
#include "vector.h"
#include <functional> //TODO: check if this is allowed

class bkNode
{
	bud::string* str;
	// MatchType type;
	std::function<unsigned long(bud::string, bud::string, unsigned int)> distance;
	bud::vector<bkNode*> childs = bud::vector<bkNode*>(2 * MAX_WORD_LENGTH, nullptr);

public:
	bkNode(bud::string* s, std::function<unsigned long(bud::string, bud::string, unsigned int)> d);
	~bkNode();
	void add(bud::string* s, unsigned int tol);
	bud::vector<bud::string*> find(const bud::string& s, unsigned int tol);
};


class bkTree{
	bkNode* root;
	MatchType type;

public:
	bkTree(MatchType t){
		type = t;
	};
	~bkTree();

	void add(Query* s, unsigned int tol);
	bud::vector<int> find(bud::string& word) const;

	bud::vector<int> find(bud::string& word, unsigned int tol) const;
};

#endif // !BKTREE_HPP
