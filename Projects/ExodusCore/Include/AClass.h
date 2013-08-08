#pragma once

#include "TDataTypes.h"

enum EClassType {
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
 

#define DECLARE_CLASS_OPERATOR(InClassName)												\
	friend AAccessor &operator<<( AAccessor& Ac, InClassName*& A )						\
	{																					\
		if(Ac.IsLoading())																\
		{																				\
			TString ClassNameStr;														\
			Ac << ClassNameStr;															\
			A = ConstructClass<##InClassName>(ClassNameStr);							\
		}																				\
		else if(Ac.IsSaving())															\
		{																				\
			Ac << ##InClassName::Class()->ClassName;									\
		}																				\
		return Ac << *(AObject**)&A;													\
	}																					\
 
#define DECLARE_CLASS(InClassName, ClassType)											\
	DECLARE_CLASS_BASE(InClassName, ClassType)											\
	DECLARE_CLASS_OPERATOR(InClassName)													\
 
#define DECLARE_CLASS_BASE(InClassName, ClassType)										\
public:																					\
	DECLARE_CONSTRUCTOR_##ClassType(InClassName)										\
class UClass_##InClassName : public AClass												\
	{																					\
	public:																				\
	UClass_##InClassName()																\
	: AClass(TString(#InClassName))														\
		{																				\
		Constructor = new UClassConstructor_##InClassName();							\
		}																				\
	};																					\
	static UClass_##InClassName Class__##InClassName;									\
	static AClass* Class()																\
	{																					\
		return (AClass*)&Class__##InClassName;											\
	}																					\
 
#define IMPLEMENT_CLASS(InClassName)													\
	InClassName::UClass_##InClassName InClassName::Class__##InClassName;				\
 
class AObject;

class AClassConstructor {
public:
    virtual AObject* Construct() = 0;
};

class AClass {
public:
    AClass(TString ClassName);
    virtual ~AClass();

    AClassConstructor* Constructor;
    TString ClassName;
};

class AClassManager {
public:
    TArray<AClass*> Classes;

    void AddClass(AClass* Class);
};

extern AClassManager* GClassManager;

template<typename T>
inline AClass* GetClass(TString& ClassName) {
    for (unsigned int i = 0; i < GClassManager->Classes.Size(); ++i) {
        AClass* Class = GClassManager->Classes(i);
        if (Class->ClassName == ClassName) {
            return Class;
        }
    }
    return 0;
}

template<typename T>
inline T* ConstructClass(TString& ClassName) {
    AClass* Class = GetClass<T>(ClassName);
    if (Class) {
        return (T*) Class->Constructor->Construct();
    }
    return 0;
}
