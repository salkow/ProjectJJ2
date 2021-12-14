#include "BK_Entry.h"

BK_Entry::BK_Entry(int (*df)(Entry *,
							 Entry *, unsigned int)) : BK(df) {}

Entry *BK_Entry::get(bud::string q)
{
	if (root == nullptr)
	{
		return nullptr;
	}
	bud::vector<Node *> lookList;
	lookList.push_back(root);

	while (lookList.size() != 0)
	{
		Node *edge = lookList.back();
		if (edge->content->first == q)
			return edge->content;
		else
		{
			lookList.pop_back();
			for (auto &newEdge : edge->_edges)
			{
				lookList.push_back(newEdge);
			}
		}
	}
	return NULL;
}

bud::vector<bud::pair<Entry *, int>> BK_Entry::search(bud::string query, int maxDistance) const
{
	// handle null root
	if (root == NULL)
		return bud::vector<bud::pair<Entry *, int>>();
	else
		return recSearch(root, query, maxDistance);
}

bud::vector<bud::pair<Entry *, int>> BK_Entry::recSearch(Node *parent, bud::string query, int maxDistance) const
{
	bud::vector<bud::pair<Entry *, int>> results;

	Entry temp(query, bud::unordered_set<Query *>());

	// if the parent node fits our criteria, add to results and explore it's children
	int distance = distanceFunction(parent->content, &temp, 0);
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
			for (const auto &result : temp_results)
			{
				results.push_back(result);
			}
		}
	}
	return results;
}