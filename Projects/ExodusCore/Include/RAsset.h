#pragma once

// Shader
// IndexBuffer
// VertexBuffer
// Texture
#include "TDataTypes.h"
#include "AObject.h"

class RAsset: public AObject
{
public:
	TString Path;
	
	friend AAccessor& operator<<(AAccessor& Ac, RAsset& A)
	{
		Ac << A.Path;
		return Ac;
	}

	virtual TArray<TString> GetCreatablePrimitiveClassNames() = 0;
};

template<class T>
class RAssetPtr {
	RAsset* Ptr;

public:
	void Set(T* V) {
		Ptr = (RAsset*)(V);
	}

	T* Get() {
		return (T*)(Ptr);
	}

	friend AAccessor& operator<<(AAccessor& Ac, RAssetPtr& R)
	{
		if(Ac.IsLoading()) {
			TString Path;
			Ac << Path;
			R.Ptr = LoadAsset<RAsset>(Path);
		}
		if(Ac.IsSaving()) {
			Ac << R.Ptr->Path;
		}
		return Ac;
	}
};