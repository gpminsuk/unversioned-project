#include "TDataTypes.h"

#include "xmlparse.h"

#pragma comment(lib,"xmlparse.lib")

struct TXMLAttribute
{
	TString AttributeName;
	TString AttributeValue;

	bool IsAttribute(const char* Name, const char* Value)
	{
		return AttributeName == Name && AttributeValue == Value;
	}

	bool GetAttribute(const char* Name, TString& Value)
	{
		if(AttributeName == Name)
		{
			Value = AttributeValue;
			return true;
		}
		return false;
	}
};

struct TXMLElement
{
	TXMLElement* Parent;
	TString ElementName;
	TArray<TXMLAttribute> Attributes;
	TArray<TXMLElement> ChildElements;

	bool IsElementName(const char* Name)
	{
		return ElementName == Name;
	}

	bool GetChildElement(const char* Name, TXMLElement& Ret)
	{
		for(unsigned int i=0;i<ChildElements.Size();++i)
		{
			TXMLElement& Element = ChildElements(i);
			if(Element.IsElementName(Name))
			{
				Ret = Element;
				return true;
			}
		}
		return false;
	}

	bool GetAttributeValue(const char* Name, TString& Ret)
	{
		for(unsigned int i=0;i<Attributes.Size();++i)
		{
			TXMLAttribute& Attribute = Attributes(i);
			if(Attribute.GetAttribute(Name, Ret))
			{
				return true;
			}
		}
		return false;
	}

	bool HasAttribute(const char* Name, const char* Value)
	{
		for(unsigned int i=0;i<Attributes.Size();++i)
		{
			TXMLAttribute& Attribute = Attributes(i);
			if(Attribute.IsAttribute(Name, Value))
			{
				return true;
			}
		}
		return false;
	}	
};

class UXMLParser
{
public:
	UXMLParser();
	~UXMLParser();

	static void EndElement(void *UserData, const char *Name);
	static void StartElement(void *UserData, const char *Name, const char **Atts);

	bool ReadXMLFile(char* FileName);

	TArray<TXMLElement> Elements;
	TXMLElement* ParentElement;

	XML_Parser Parser;

	virtual void Parse() = 0;
};

class UXMLApplicationParser : public UXMLParser
{
public:
	UXMLApplicationParser();
	~UXMLApplicationParser();

	virtual void Parse();
};