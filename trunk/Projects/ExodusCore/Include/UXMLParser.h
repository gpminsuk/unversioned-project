#include "TDataTypes.h"
#include "AObject.h"

#include "expat.h"

#pragma comment(lib,"libexpat.lib")

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
		if (AttributeName == Name)
				{
			Value = AttributeValue;
			return true;
		}
		return false;
	}

	TString& GetValue()
	{
		return AttributeValue;
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
		for (unsigned int i = 0; i < ChildElements.Size(); ++i)
				{
			TXMLElement& Element = ChildElements(i);
			if (Element.IsElementName(Name))
					{
				Ret = Element;
				return true;
			}
		}
		return false;
	}

	bool GetAttributeValue(const char* Name, TString& Ret)
			{
		for (unsigned int i = 0; i < Attributes.Size(); ++i)
				{
			TXMLAttribute& Attribute = Attributes(i);
			if (Attribute.GetAttribute(Name, Ret))
					{
				return true;
			}
		}
		return false;
	}

	bool HasAttribute(const char* Name, const char* Value)
			{
		for (unsigned int i = 0; i < Attributes.Size(); ++i)
				{
			TXMLAttribute& Attribute = Attributes(i);
			if (Attribute.IsAttribute(Name, Value))
					{
				return true;
			}
		}
		return false;
	}

	bool GetValue(char* Path, TString& Ret)
			{
		char* RestPath = strchr(Path, '.');
		if (RestPath)
		{
			char ElementName[1024];
			memcpy(ElementName, Path, (RestPath - Path));
			ElementName[(RestPath - Path)] = '\0';
			for (unsigned int i = 0; i < ChildElements.Size(); ++i)
					{
				TXMLElement& Element = ChildElements(i);
				if (Element.ElementName == ElementName)
						{
					Element.GetValue(RestPath + 1, Ret);
					return true;
				}
			}
		}
		else
		{
			for (unsigned int i = 0; i < Attributes.Size(); ++i)
					{
				TXMLAttribute& Attribute = Attributes(i);
				if (Attribute.AttributeName == Path)
						{
					Ret = Attribute.GetValue();
					return true;
				}
			}
		}
		return false;
	}
};

class BXMLParser: public AObject
{
public:

	BXMLParser();
	~BXMLParser();

	static void EndElement(void *UserData, const char *Name);
	static void StartElement(void *UserData, const char *Name, const char **Atts);

	bool ReadXMLFile(char* FileName);
	bool GetValue(char* Path, TString& Ret);

	TXMLElement Root;
	TXMLElement* ParentElement;

	XML_Parser Parser;

	virtual void Parse() = 0;
};

class CXMLApplicationParser: public BXMLParser
{
public:
	CXMLApplicationParser();
	~CXMLApplicationParser();

	virtual void Parse();
};
