#include "BK_Entry.h"

BK_Entry::BK_Entry(int (*df)(Entry *,
							 Entry *, unsigned int)) : BK(df) {}

Entry *BK_Entry::get(bud::string q)
{
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
			for (auto & newEdge : edge->_edges)
			{
				lookList.push_back(newEdge);
			}
		}
	}
	return NULL;
}
