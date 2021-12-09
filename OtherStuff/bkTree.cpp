#include "bkTree.h"
#include "my_string.h"
#include "vector.h"
#include "unordered_map.h"
#include "pair.h"
#include "util.h"

#include "appMatching/editDistance.h"
#include "appMatching/hammingDistance.h"
#include <iostream>
#include <stdexcept>

using bud::pair;
using bud::string;
using bud::unordered_map;
using bud::vector;

bkNode::bkNode(string* s, std::function<unsigned long(bud::string, bud::string, unsigned int)> d):
distance(d)
{
	str = s;
}

void bkNode::add(string* s, unsigned int tol)
{
	unsigned long curr = distance(*str, *s, tol);
	if (childs[curr] == nullptr) //check if the branch is already created
	{
		childs[curr] = new bkNode(s, distance);//if not create it
	}
	else
	{
		childs[curr]->add(s, tol);//else repeat with the child
	}
}
bkNode::~bkNode()
{
	for (auto* x : childs)
	{
		delete x;
	}
}

bud::vector<bud::string*> bkNode::find(const bud::string& s, unsigned int tol)
{
	vector<string*> fin;
	if (*str != string(""))
	{
		unsigned int temp = distance(*str, s, tol);

		if (temp <= tol)//check if we can add the current to the array(by checking with the tol)
		{
			fin.emplace_back(str);
		}
		for (unsigned long i = temp - tol < 0 ? 0 : unsigned(temp - tol); i <= unsigned(temp + tol);i++)//for temp-tol (if negative we assume 0) till temp+tol repeat actions for each child
		{
			if (childs[i] != nullptr)
			{
				vector<string*> t = childs[i]->find(s, tol);
				fin.insert(t.begin(), t.end());//and update the final vector for each outcome 
			}
		}
	}
	return fin;
}

bkTree::~bkTree() { delete root; }

void bkTree::add(Query* s, unsigned int tol) {
	unsigned long i = 0;
	unsigned long len;
	if(!root){
		if(s->m_match_type == MT_EDIT_DIST){
			root = new bkNode(&(s->m_str[i]), getEdit);
		}else{
			// root = new bkNode(&(s->m_str[i]), s->m_match_type);
		}
		len = s->m_str[i].size();
		i++;
	}
	for(;i<s->m_str.size();i++){
		if(type == MatchType::MT_EDIT_DIST || (type == MatchType::MT_HAMMING_DIST && s->m_str[i].size() == len)){
			root->add(&s->m_str[i], tol);
		}
	}
}

vector<int> bkTree::find(bud::string& word, unsigned int tol) const
{
	// bud::vector<bud::string*> matching_words = root->find(word, tol);

	// bud::vector<int> matching_queries;

	// bud::unordered_map<int, bool, HashFunction> inserted_queries(m_hash_map->size());

	// for (auto* query_word : matching_words)
	// {
	// 	const auto* queries_with_that_word = m_hash_map->operator[](query_word);

	// 	for (auto query : *queries_with_that_word)
	// 	{
	// 		if (inserted_queries[query] == nullptr)
	// 		{
	// 			matching_queries.emplace_back(query);

	// 			inserted_queries.insert(pair<int const, bool>(query, true));
	// 		}
	// 	}
	// }

	// return matching_queries;
}

bud::vector<int> bkTree::find(string& word) const { return find(word, 1); }
