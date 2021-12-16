#include "BK_Entry.h"

#define __BK_ENTRY_MAX_SEARCH_DISTANCE__ 4

BK_Entry::BK_Entry(int (*df)(Entry *,
							 Entry *, unsigned int)) : BK(df) {}

Entry *BK_Entry::get(const bud::string &q)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	bud::vector<Node *> lookList;
	lookList.push_back(root);

	while (lookList.size() != 0)
	{
		Node *curr = lookList.back();
		lookList.pop_back();
		if (curr->content->first == q)
			return curr->content;
		else
		{
			for (auto &edge : curr->_edges)
			{
				if (!__BK_T_TRAVERSE_USE_SEARCH__ || abs(edge->parentDistance - 1) <= 1 ||
					1 <= abs(edge->parentDistance + 1))
				{
					lookList.push_back(edge);
				}
			}
		}
	}
	return nullptr;
}

bud::vector<bud::pair<Entry *, int>> BK_Entry::search(const bud::string &query, int maxDistance) const
{

	bud::vector<bud::pair<Entry *, int>> results;

	if (root == NULL)
		return results;

	Entry temp(query, bud::unordered_set<Query *>());

	bud::vector<Node *> lookList;
	lookList.push_back(root);

	Node *curr;
	do
	{
		curr = lookList.back();
		lookList.pop_back();

		// if the parent node fits our criteria, add to results and explore it's children
		int distance = distanceFunction(curr->content, &temp, __BK_ENTRY_MAX_SEARCH_DISTANCE__);
		if (distance <= maxDistance && !curr->deleted)
			results.push_back(bud::pair(curr->content, distance));

		// iterate over parent's children, explore only those with
		// |child->parentDistance - maxDistance| <= maxDistance <= |child->parentDistance + maxDistance|
		for (const auto &edge : curr->_edges)
		{
			// if the child's parentDistance fits our criteria, explore
			if (abs(edge->parentDistance - maxDistance) <= maxDistance ||
				maxDistance <= edge->parentDistance + maxDistance)
			{
				lookList.push_back(edge);
				// bud::vector<bud::pair<Entry *, int>> temp_results = recSearch(edge, query, maxDistance);
				// add it's results to ours
				// results.insert(temp_results.begin(), temp_results.end());
			}
		}

	} while (!lookList.empty());

	// handle null root
	return results;
}

bud::vector<bud::pair<Entry *, int>> BK_Entry::recSearch(Node *parent, const bud::string &query, int maxDistance) const
{
	bud::vector<bud::pair<Entry *, int>> results;

	Entry temp(query, bud::unordered_set<Query *>());

	// if the parent node fits our criteria, add to results and explore it's children
	int distance = distanceFunction(parent->content, &temp, __BK_ENTRY_MAX_SEARCH_DISTANCE__);
	if (distance <= maxDistance && !parent->deleted)
		results.push_back(bud::pair(parent->content, distance));

	// iterate over parent's children, explore only those with
	// |child->parentDistance - maxDistance| <= maxDistance <= |child->parentDistance + maxDistance|
	for (const auto &edge : parent->_edges)
	{
		// if the child's parentDistance fits our criteria, call recSearch
		if (abs(edge->parentDistance - maxDistance) <= maxDistance ||
			maxDistance <= abs(edge->parentDistance + maxDistance))
		{

			bud::vector<bud::pair<Entry *, int>> temp_results = recSearch(edge, query, maxDistance);
			// add it's results to ours
			results.insert(temp_results.begin(), temp_results.end());
		}
	}
	return results;
}
