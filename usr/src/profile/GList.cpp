#include "pch.h"
// --------------------------------------------------------------------------
//					www.UnitedBusinessTechnologies.com
//			  Copyright (c) 1998 - 2002  All Rights Reserved.
//
// Source in this file is released to the public under the following license:
// --------------------------------------------------------------------------
// This toolkit may be used free of charge for any purpose including corporate
// and academic use.  For profit, and Non-Profit uses are permitted.
//
// This source code and any work derived from this source code must retain 
// this copyright at the top of each source file.
// 
// UBT welcomes any suggestions, improvements or new platform ports.
// email to: XMLFoundation@UnitedBusinessTechnologies.com
// --------------------------------------------------------------------------

#include "GList.h"

void GList::AppendList(const GList *pListToCopy)
{
	if (pListToCopy)
	{
		GListIterator it( pListToCopy );
		while (it())
		{
			void *p = (void *)it++;
			AddLast(p);
		}
	}
}

GList::GList(const GList &cpy)
{
	AppendList(&cpy);
}

void GList::operator=(const GList &cpy)
{
	AppendList(&cpy);
}

#include <stdio.h>
GList::GList()
{
	// constructs an initially empty list
	FirstNode = LastNode = CurrentNode = 0;
	iSize = 0;
}

GList::~GList()
{
//	inline RemoveAll();
	while (FirstNode)
	{
		// inline RemoveFirst()
		if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) LastNode = 0;
		Node *Save = FirstNode->NextNode;
		delete FirstNode;
		FirstNode = Save;
		iSize--;
	}

}

void * GList::First() const
{
	if (!FirstNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = FirstNode;
	return FirstNode->Data;
}

void * GList::Last() const
{
	if (!LastNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = LastNode;
	return LastNode->Data;
}

void * GList::Current() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	return CurrentNode->Data;
}

void * GList::Next() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = CurrentNode->NextNode;
	return Current();
}

void * GList::Previous() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	GList *pThis = (GList *)this;
	pThis->CurrentNode = CurrentNode->PrevNode;
	return Current();
}

void * GList::Tail() const
{
	if (!CurrentNode) 
	{
		return 0;
	}
	return CurrentNode->NextNode->Data;
}

void GList::AddBeforeCurrent( void * Data )
{
// Add maintains list integrity by adding the new node before the current
// node, if the current node is the first node, the new node becomes the
// first node.
	Node *pN;
	pN = new Node(CurrentNode, 0);
	if (FirstNode == 0)
		FirstNode = LastNode = CurrentNode = pN;
	if (FirstNode->PrevNode)
		FirstNode = FirstNode->PrevNode;
	iSize++;
	pN->Data = Data;
}

void GList::AddAfterCurrent( void * Data )
{
// AddLast maintains list integrity by adding the new node after the current
// node, if the current node is the last node, the new node becomes the last
// node.
	Node *pN;
	pN = new Node(CurrentNode);
	if (FirstNode == 0)
		FirstNode = LastNode = CurrentNode = pN;
	if (LastNode->NextNode)
		LastNode = LastNode->NextNode;
	iSize++;
	pN->Data = Data;
}

void GList::AddHead( void * Data)
{
// AddHead maintains list integrity by making the new node the first node.
	Node *pN;
	pN = new Node(FirstNode, 0);
	if (LastNode == 0)
		LastNode = CurrentNode = pN;
	iSize++;
	pN->Data = Data;
	FirstNode = pN;
}

void GList::AddLast( void * Data)
{
// AddLast maintains list integrity by making the new node the last node.
//	Node *pN = m_nodePool.getNode(LastNode);
	Node *pN = new Node(LastNode);
	if (FirstNode == 0)
		FirstNode = CurrentNode = pN;
	iSize++;
	pN->Data = Data;
	LastNode = pN;
}

void GList::Remove(void *Data)
{
	void *p = First();
	while(p)
	{
		if (p == Data)
		{
			RemoveCurrent();
			break;
		}
		p = Next();
	}
}

void GList::RemoveAll()
{
	while (FirstNode)
	{
		// inline RemoveFirst()
		if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) LastNode = 0;
		Node *Save = FirstNode->NextNode;
		delete FirstNode;
		FirstNode = Save;
		iSize--;
	}
}

void GList::RemoveFirst()
{
// RemoveFirst maintains list integrity by making the first node the node
// after the first node.  If the first node is the only node in the list,
// removing it produces an empty list.
	if (FirstNode) 
	{
		if (CurrentNode == FirstNode) CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) LastNode = 0;
		Node *Save = FirstNode->NextNode;
		delete FirstNode;
		FirstNode = Save;
		iSize--;
	}
}

void * GList::RemoveLast()
{
	void *ret = 0;

	if (LastNode) 
	{
		ret = LastNode->Data;

		if (CurrentNode == LastNode) 
			CurrentNode = LastNode->PrevNode;
		if (FirstNode == LastNode) 
			FirstNode = 0;
		Node *Save = LastNode->PrevNode;

//		m_nodePool.releaseNode(LastNode);
		delete LastNode;

		LastNode = Save;
		iSize--;
	}

	return ret;
}

void GList::RemoveCurrent()
// RemoveCurrent maintains list integrity by making the current node
// the next node if it exists, if there is not a next node, current node
// becomes the previous node if it exists, otherwise, current = NULL.
// If the current node is also the first node, then the first node becomes
// the next node by default.  If the current node is also the last node,
// then the last node becomes the previous node by default.  If current ==
// last == first, then removing the current node produces an empty list
// and current = first = last = NULL.
{
	if (CurrentNode) 
	{
		Node *Save;
		if (!CurrentNode->PrevNode) FirstNode = CurrentNode->NextNode;
		if (CurrentNode->NextNode) Save = CurrentNode->NextNode;
		else if (CurrentNode->PrevNode) Save = LastNode = CurrentNode->PrevNode;
		else Save = FirstNode = LastNode = 0;
		delete CurrentNode;
		CurrentNode = Save;
		iSize--;
	}
}

void GList::RemoveNext()
{
// RemoveNext maintains list integrity by checking if the next node is the
// last node, if this is the case, then the last node becomes the current
// node.
	if (CurrentNode) 
	{
		if (CurrentNode->NextNode) 
		{
			if (!CurrentNode->NextNode->NextNode) LastNode = CurrentNode;
			Node * DeleteNode = CurrentNode->NextNode;
			delete DeleteNode;
			iSize--;
		}
	}
}

void GList::RemovePrevious()
{
// RemovePrevious maintains list integrity by checking if the next node is
// the first node, if this is the case, then the first node becomes the
// current node.
	if (CurrentNode) 
	{
		if (CurrentNode->PrevNode) 
		{
			if (!CurrentNode->PrevNode->PrevNode) FirstNode = CurrentNode;
			Node * DeleteNode = CurrentNode->PrevNode;
			delete DeleteNode;
			iSize--;
		}
	}
}

void GList::SetCurrent(Node *p)
{
	CurrentNode = p;
}

GListIterator::GListIterator(const GList *iList, int IterateFirstToLast)
{
	pTList = (GList *)iList;
	iDataNode = 0;
	if (pTList)
	{
		if (IterateFirstToLast) 
			iDataNode = (GList::Node *)((GList *)iList)->FirstNode;
		else 
			iDataNode = ((GList *)iList)->LastNode;
	}
}

void GListIterator::reset(int IterateFirstToLast /* = 1 */)
{
	if (IterateFirstToLast) 
		iDataNode = (GList::Node *)((GList *)pTList)->FirstNode;
	else 
		iDataNode = ((GList *)pTList)->LastNode;
}

void * GListIterator::operator ++ (int)
{
	void *pRet = iDataNode->Data;
	iCurrentNode = iDataNode;
	iDataNode = iDataNode->NextNode;
	return pRet;
}

void * GListIterator::operator -- (int)
{
	void *pRet = iDataNode->Data;
	iCurrentNode = iDataNode;
	iDataNode = iDataNode->PrevNode;
	return pRet;
}

int GListIterator::operator ()  (void) const
{
	return iDataNode != 0;
}
