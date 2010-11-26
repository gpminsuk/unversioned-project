#pragma once

#include "UXMLParser.h"

class MyXMLParser : public UXMLParser
{
public:
	MyXMLParser();
	~MyXMLParser();

	virtual void Parse();
};