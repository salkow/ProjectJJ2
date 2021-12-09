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
			for (bud::size_type i = 1; i <= edge->_edges.size(); i++)
			{
				lookList.push_back(edge->_edges.at(i));
			}
		}
	}
	return NULL;
}
