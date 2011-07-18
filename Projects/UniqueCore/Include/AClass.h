#pragma once

#include "TDataTypes.h"

enum EClassType
{
	CLASS_Abstract,
};
																			
#define DECLARE_CONSTRUCTOR_CLASS_Abstract(ClassName)									\
class UClassConstructor_##ClassName : public AClassConstructor							\
{																						\
public:																					\
	AObject* Construct()																\
	{																					\
		return 0;																		\
	}																					\
};																						\

#define DECLARE_CONSTRUCTOR_(ClassName)													\
class UClassConstructor_##ClassName : public AClassConstructor							\
{																						\
public:																					\
	AObject* Construct()																\
	{																					\
		return new ClassName();															\
	}																					\
};																						\

#define DECLARE_CLASS(ClassName, ClassType) \
public:																					\
	DECLARE_CONSTRUCTOR_##ClassType(ClassName)											\
class UClass_##ClassName : public AClass												\
	{																					\
	public:																				\
	UClass_##ClassName()																\
	: AClass(TString(#ClassName))														\
		{																				\
		Constructor = new UClassConstructor_##ClassName();								\
		}																				\
	};																					\
	static UClass_##ClassName Class__##ClassName;										\
	static AClass* Class()																\
	{																					\
		return (AClass*)&Class__##ClassName;											\
	}																					\

#define IMPLEMENT_CLASS(ClassName)														\
	ClassName::UClass_##ClassName ClassName::Class__##ClassName;						\

class AObject;

class AClassConstructor
{
public:
	virtual AObject* Construct() = 0;
};

class AClass
{
public:
	AClass(TString ClassName);
	virtual ~AClass();

	AClassConstructor* Constructor;
	TString ClassName;
};

class AClassManager
{
public:
	TArray<AClass*> Classes;

	void AddClass(AClass* Class);
};

extern AClassManager* GClassManager;

template<typename T> 
inline AClass* GetClass(TString& ClassName)
{
	for(unsigned int i=0;i<GClassManager->Classes.Size();++i)
	{
		AClass* Class = GClassManager->Classes(i);
		if(Class->ClassName == ClassName)
		{
			return Class;
		}
	}
	return 0;
}

template<typename T> 
inline T* ConstructClass(TString& ClassName)
{
	AClass* Class = GetClass<T>(ClassName);
	if(Class)
	{
		return (T*)Class->Constructor->Construct();
	}
	return 0;
}