#include "bkTree.h"
#include "../my_string.h"
#include "../vector.h"
#include "../inverted_search_engine.h"
#include "../unordered_map.h"
#include "../pair.h"

#include "appMatching/editDistance.h"
#include "appMatching/hammingDistance.h"
#include <iostream>
#include <stdexcept>

using bud::pair;
using bud::string;
using bud::unordered_map;
using bud::vector;

bkNode::bkNode(string* s, match_type m)
{
	str = s;
	type = m;
}

void bkNode::add(string* s)
{
	unsigned long curr;
	if (type == match_type::EDIT_DISTANCE) //use the correct function for when editDistance is required or hamming
	{
		curr = getEdit(*str, *s);
	}
	else
	{
		curr = getHamming(*str, *s);
	}
	if (childs[curr] == nullptr) //check if the branch is already created
	{
		childs[curr] = new bkNode(s, type);//if not create it
	}
	else
	{
		childs[curr]->add(s);//else repeat with the child
	}
}
bkNode::~bkNode()
{
	for (auto* x : childs)
	{
		delete x;
	}
}

bud::vector<bud::string*> bkNode::find(const bud::string& s, int tol)
{
	vector<string*> fin;
	if (*str != string(""))
	{
		int temp;
		if (type == match_type::EDIT_DISTANCE)
		{
			temp = getEdit(*str, s);
		}
		else
		{
			temp = getHamming(*str, s);
		}

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

bkTree::bkTree(bud::vector<bud::vector<bud::string*>>& queries, match_type m)
	:
	inverted_search_engine(queries)
{
	type = m;
	if (m_words_from_all_queries.size() == 0)//if vector is empty there is no point so we throw exception
	{
		throw std::invalid_argument("Vector is empty!");
	}
	root = new bkNode(m_words_from_all_queries[0], type);//since its an constructor we can set the root like that.
	unsigned long len = m_words_from_all_queries[0]->size();
	for (unsigned i = 1; i < m_words_from_all_queries.size(); i++)//for each word in the array
	{
		if (m == match_type::EDIT_DISTANCE ||//if its edit distance we always need to add it to the tree
			(m == match_type::HAMMING_DISTANCE && m_words_from_all_queries[i]->size() == len))//if its hamming we have to check that the lengths are equal, if not just skip it
		{
			this->add(m_words_from_all_queries[i]);
		}
	}
}

bkTree::~bkTree() { delete root; }

void bkTree::add(string* s) { root->add(s); }

vector<int> bkTree::find(bud::string& word, int tol) const
{
	bud::vector<bud::string*> matching_words = root->find(word, tol);

	bud::vector<int> matching_queries;

	bud::unordered_map<int, bool, HashFunction> inserted_queries(m_hash_map->size());

	for (auto* query_word : matching_words)
	{
		const auto* queries_with_that_word = m_hash_map->operator[](query_word);

		for (auto query : *queries_with_that_word)
		{
			if (inserted_queries[query] == nullptr)
			{
				matching_queries.emplace_back(query);

				inserted_queries.insert(pair<int const, bool>(query, true));
			}
		}
	}

	return matching_queries;
}

bud::vector<int> bkTree::find(string& word) const { return find(word, 1); }
