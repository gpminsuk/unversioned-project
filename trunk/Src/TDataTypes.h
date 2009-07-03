#pragma once

#include <vector>

template <class T>
class TArray
{
public:
	std::vector<T> mArray;

	T operator[] (int idx)
	{
		return mArray[idx];
	}

	void AddItem(T Item)
	{
		mArray.push_back(Item);
	}

	void DeleteItem(unsigned int index)
	{
		std::vector<T>::iterator it = mArray.begin();
		advance(it,index);
		mArray.erase(it);
	}

	size_t Size()
	{
		return mArray.size();
	}
};

class TIndex16
{
public:
	TIndex16() { _1 = 0; _2 = 0; _3 = 0; }
	TIndex16(short __1, short __2, short __3) { _1 = __1; _2 = __2; _3 = __3; }
	short _1,_2,_3;
};

class TScalar
{
public:
	TScalar() : x(0) {}
	TScalar(float x1) : x(x1) {}
	TScalar(TScalar& s) : x(s.x) {}

	float x;
};

class TVector2
{ 
public:
	TVector2() : x(0), y(0) {}
	TVector2(float x1, float y1) : x(x1), y(y1) {}
	TVector2(TVector2& v) : x(v.x), y(v.y) {}

	float x;
	float y;
};

class TVector3
{
public:
	TVector3() : x(0), y(0), z(0) {}
	TVector3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
	TVector3(TVector3& v) : x(v.x), y(v.y), z(v.z) {}

	TVector3 operator/ (float f) { return TVector3(x/f,y/f,z/f); }
	TVector3 operator- () { return TVector3(-x,-y,-z); }

	float x;
	float y;
	float z;
};

class TVector4
{
public:
	TVector4() : x(0), y(0), z(0), w(0) {}
	TVector4(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
	TVector4(TVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}

	float x;
	float y;
	float z;
	float w;
};
typedef TVector4 TQuarternion;

class TMatrix
{
public:

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;
};

class TPrimitiveTemplateBase
{
public:
	TPrimitiveTemplateBase();
	~TPrimitiveTemplateBase();

	TQuarternion qTranslation;
	TQuarternion qRotation;
	TQuarternion qScale;

	class RMaterial *pMaterial;

	class RShaderBase *pShader;

	class RVertexBuffer *pVertexBuffer;
	class RIndexBuffer *pIndexBuffer;
	
	struct SBatchInfo
	{
		short BatchNum;
		short SlotNum;
	};
	SBatchInfo BatchInfo;
};

class TBatch
{
public:
	TBatch() : nVertices(0), nVertexStride(0) {}
	TArray<TPrimitiveTemplateBase*> m_pTemplates;

	int nVertices;
	int nVertexStride;
};

class TBox : public TPrimitiveTemplateBase
{
public:
	TBox() : Extent(10.f,10.f,10.f) { Create(); }
	TBox(TVector3 _Extent) : Extent(_Extent) { Create(); }

	void Resize(TVector3 _Extent) { Extent = _Extent; Create(); }

private:
	void Create();
	
	TVector3 Extent;
};

class TSphere : public TPrimitiveTemplateBase
{
public:
	TSphere() : Radius(10.f) {}
	TSphere(float _Radius) : Radius(_Radius) {}

	float Radius;
};

class TQuad : public TPrimitiveTemplateBase
{
public:
	TQuad() : Extent(0.f, 1.f, 0.f, 0.f) { Create(); }
	TQuad(TVector4 _Extent) : Extent(_Extent) { Create(); }

private:
	void Create();

	TVector4 Extent;
};

class TMesh : public TPrimitiveTemplateBase
{
};

/*
template <typename T>
class TArray
{
private: 
	enum
	{
		DEFAULT_SIZE = 16,
	};

	T* m_Data;
	int m_Size;
	int m_Num;

	void Init(int size);

public: 
	TArray();
	TArray(int size);
	~TArray();
	void Add(T item);
	void Resize(int size);
	int GetLength();
	T& operator [](int& index);
	void operator =(TArray<T>& R);
};*/