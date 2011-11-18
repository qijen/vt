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

#include "pch.h"
#include "GStringList.h"
#include "GString.h"
#include <stdio.h>
#include <string.h> // for: strlen(), strstr()
#include "GProfile.h"
const char *GStringList::PeekLast()
{
	m_strSerializeDest = "";
	GString *pG = (GString *)GList::Last();
	if (pG)
		m_strSerializeDest = (const char *)*pG;
	return m_strSerializeDest;
}


const char *GStringList::PeekFirst()
{
	m_strSerializeDest = "";
	GString *pG = (GString *)GList::First();
	if (pG)
		m_strSerializeDest = (const char *)*pG;
	return m_strSerializeDest;
}

const char *GStringList::RemoveFirst()
{
	m_strSerializeDest = "";
	GString *pG = (GString *)GList::First();
	if (pG)
	{
		GList::RemoveFirst();
		m_strSerializeDest = (const char *)*pG;
		delete pG;
	}
	return m_strSerializeDest;
}

const char *GStringList::RemoveLast()
{
	m_strSerializeDest = "";
	GString *pG = (GString *)GList::RemoveLast();
	if (pG)
	{
		m_strSerializeDest = (const char *)*pG;
		delete pG;
	}
	return m_strSerializeDest;
}

GString *GStringList::AddFirst(const char *szString)
{
	GString *pstrString = new GString(szString);
	GList::AddHead(pstrString);
	return pstrString;
}


GString *GStringList::AddLast(const char *szString)
{
	GString *pstrString = new GString(szString);
	GList::AddLast(pstrString);
	return pstrString;
}

GStringList::GStringList(const GStringList &src)
{
	GStringIterator it(&src);
	while (it())
		AddLast(it++);
}

GStringList::GStringList()
{
}

void GStringList::operator+=(const GStringList &src)
{
	GStringIterator it(&src);
	while (it())
		AddLast(it++);
}

void GStringList::operator+=(const char *szSrc)
{
	AddLast(szSrc);
}

void GStringList::RemoveAll()
{
	int n = Size();
	for(int i=0; i<n;i++)
		delete (GString *)RemoveLast();
}

GStringList::~GStringList()
{
	while (FirstNode)
	{
		if (CurrentNode == FirstNode) 
			CurrentNode = FirstNode->NextNode;
		if (LastNode == FirstNode) 
			LastNode = 0;
		Node *Save = FirstNode->NextNode;
		delete (GString *)FirstNode->Data;
		delete FirstNode;
		FirstNode = Save;
		iSize--;
	}
}

void GStringIterator::reset()
{
	pDataNode = (GStringList::Node *)((GStringList *)pTList)->FirstNode;
}

GStringIterator::GStringIterator(const GStringList *pList)
{
	pTList = (GStringList *)pList;
	pDataNode = (GStringList::Node *)((GStringList *)pList)->FirstNode;
}

const char *GStringIterator::operator ++ (int)
{
	GString *pRet = (GString *)pDataNode->Data;
	pCurrentNode = pDataNode;
	pDataNode = pDataNode->NextNode;
	return pRet->StrVal();
}

int GStringIterator::operator ()  (void) const
{
	return pDataNode != 0;
}


const char *GStringList::Serialize(const char *pzDelimiter)
{
	m_strSerializeDest = "";
	GStringIterator it(this);
	int nCount = 0;

	while (it())
	{
		if (nCount)
			m_strSerializeDest += pzDelimiter;
		nCount++;
		m_strSerializeDest += it++;
	}
	return m_strSerializeDest;
}

GStringList::GStringList(const char *pzDelimiter, const char *pzSource)
{
	DeSerialize(pzDelimiter, pzSource);
}

void GStringList::DeSerialize(const char *pzDelimiter, const char *pzSource)
{
	if (!pzDelimiter || !pzSource || !pzDelimiter[0] || !pzSource[0])
		return;

	
	int nSourceLen = strlen(pzSource);

	int nDelimiterLen = strlen(pzDelimiter);
	
	if (!nSourceLen)
		return;	

	char *beg = (char *)pzSource; 
	char *del = strstr(beg,pzDelimiter);
	while(1)
	{
		if ( !del )
		{
			// there is only one entry in the list
			AddLast(beg);
			break;
		}

		// temporarily null on the delimiter
		char chOld = *del;
		*del = 0;

		// add this (now null terminated) entry
		AddLast(beg);

		// advance to the next string
		beg = del + nDelimiterLen;
		
		// unnull the previous
		*del = chOld;

		// advance to the next delimiter, break if none
		del = strstr(beg,pzDelimiter);
		if ( !del )
		{
			AddLast(beg);
			break;
		}
	}
	
}

