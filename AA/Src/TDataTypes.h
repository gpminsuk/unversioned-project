#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
// TODO ∏ «Œ¿ª ¿ß«ÿ
#define SINE(x)		 sin(x)
#define COSINE(x)	 cos(x)
#define SQRT(x)		 sqrt(x)
#define ARCSINE(x)	 asin(x)
#define ARCCOSINE(x) acos(x)
#define MATH_PI			3.141592f

// Bitfield type.
typedef unsigned long       BITFIELD;	// For bitfields.

template <class T>
class TArray
{
public:
	std::vector<T> mArray;

	T& operator[] (int idx)
	{
		return mArray[idx];
	}

	T& operator() (int idx)
	{
		return mArray[idx];
	}

	void Clear(bool bFreeMemory = false)
	{
		mArray.clear();
		if(bFreeMemory)
		{
			mArray.vector<T>::~vector();
			mArray.vector<T>::vector();
		}
	}

	void AddItem(T Item)
	{
		mArray.push_back(Item);
	}

	void AddItems(TArray<T> Items)
	{
		for(unsigned int i=0;i<Items.Size();++i)
			mArray.push_back(Items(i));
	}

	void DeleteItem(unsigned int index)
	{
		std::vector<T>::iterator it = mArray.begin();
		advance(it,index);
		mArray.erase(it);
	}

	void DeleteItemByVal(T Item)
	{
		mArray.erase(find(mArray.begin(), mArray.end(), Item));
		
	}

	size_t Size()
	{
		return mArray.size();
	}
};

class TString
{
public:
	char Str[1024];

	bool operator ==(TString& _Str)
	{
		return !strcmp(Str,_Str.Str);
	}

	void operator =(TString& _Str)
	{
		memcpy(Str, _Str.Str, 1024);
	}
};

class TIndex16
{
public:
	TIndex16() { _1 = 0; _2 = 0; _3 = 0; }
	TIndex16(short __1, short __2, short __3) { _1 = __1; _2 = __2; _3 = __3; }
	short _1,_2,_3;
};

class TIntPoint
{
public:
	TIntPoint() : x(0), y(0) {}
	TIntPoint(int x1,int y1) : x(x1), y(y1) {}

	int x;
	int y;
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
	TVector2(const TVector2& v) : x(v.x), y(v.y) {}

	TVector2& operator-= (TVector2& v) { x -= v.x; y -= v.y; return *this; }
	TVector2& operator*= (TVector2& v) { x *= v.x; y *= v.y; return *this; }

	TVector2& operator-= (float f) { x -= f; y -= f; return *this; }
	TVector2& operator*= (float f) { x *= f; y *= f; return *this; }

	float x;
	float y;
};

class TVector3
{
public:
	TVector3() : x(0), y(0), z(0) {}
	TVector3(float x1, float y1, float z1) : x(x1), y(y1), z(z1) {}
	TVector3(const TVector3& v) : x(v.x), y(v.y), z(v.z) {}

	TVector3 operator* (float f) { return TVector3(x*f,y*f,z*f); }
	TVector3 operator/ (float f) { return TVector3(x/f,y/f,z/f); }
	TVector3 operator- () { return TVector3(-x,-y,-z); }
	TVector3 operator^ (TVector3& v) { return TVector3(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	TVector3 operator+ (TVector3& v) { return TVector3(x+v.x,y+v.y,z+v.z); }
	TVector3 operator- (TVector3& v) { return TVector3(x-v.x,y-v.y,z-v.z); }
	TVector3& operator= (const TVector3& v) { x=v.x; y=v.y; z=v.z; return *this; }
	float operator| (TVector3& v) { return x*v.x+y*v.y+z*v.z; }

	TVector3& operator+= (TVector3& v) { x += v.x; y += v.y; z += v.z; return *this; }
	TVector3& operator-= (TVector3& v) { x -= v.x; y -= v.y; z -= v.z; return *this; }
	TVector3& operator*= (TVector3& v) { x *= v.x; y *= v.y; z *= v.z; return *this; }

	bool operator!= (TVector3& v) { return (x != v.x || y != v.y || z != v.z); }

	float SizeSquared()
	{
		return x*x+y*y+z*z;
	}

	float Size()
	{
		return SQRT(SizeSquared());
	}

	TVector3& Normalize()
	{
		float Squared = SQRT(SizeSquared());
		x/=Squared; y/=Squared; z/=Squared;
		return *this;
	}

	float x;
	float y;
	float z;
};

class TVector4
{
public:
	TVector4() : x(0), y(0), z(0), w(0) {}
	TVector4(float x1, float y1, float z1, float w1) : x(x1), y(y1), z(z1), w(w1) {}
	TVector4(const TVector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {}
	TVector4(const TVector3& v, float w1) : x(v.x), y(v.y), z(v.z), w(w1) {}

	float x;
	float y;
	float z;
	float w;
};

class TQuaternion
{
public:
	TQuaternion() : v(), w(1) {}
	TQuaternion(float x1, float y1, float z1, float w1) : v(x1, y1, z1), w(w1) {}
	TQuaternion(const TQuaternion& _v) : v(_v.v), w(_v.w) {}
	TQuaternion(TVector3 Axis, float theta) : v(Axis.x*SINE(theta/2.0f),(Axis.y*SINE(theta/2.0f)),(Axis.z*SINE(theta/2.0f))), w(COSINE(theta/2.0f)) {}

	void Rotate(TVector3 Axis, float theta)
	{
		TQuaternion Temp(Axis, theta);
		*this *= Temp;
		//Normalize();
	}

	TVector3 TransformVector3D(TVector3& _v)
	{
		return (v^(v^_v))*2.0f + ((v^_v)*w*2.0f + _v);
	}

	TQuaternion& Normalize()
	{
		float Squared = SQRT(SizeSquared());
		v=v/Squared;
		w=w/Squared;
		return *this;
	}

	float SizeSquared()
	{
		return v.SizeSquared() + w*w;
	}

	void Initialize()
	{
		v.x=0.0f; v.y=0.0f; v.z=0.0f; w=1.0f;
	}

	TQuaternion& operator*=(TQuaternion& q)
	{
		float tmpw = w*q.w - (v|q.v);
		v = (v*q.w) + (q.v*w) + (v^q.v);
		w = tmpw;
		return *this;
	}

	TQuaternion operator*(TQuaternion& q)
	{
		float tmpw = w*q.w - (v|q.v);
		v = (v*q.w) + (q.v*w) + (v^q.v);
		w = tmpw;
		return TQuaternion(v,w);
	}

	TQuaternion operator*(const float& f)
	{
		TQuaternion Ret;
		Ret.v.x *= f;
		Ret.v.y *= f;
		Ret.v.z *= f;
		Ret.w *= f;
		return Ret;
	}

	TQuaternion operator/(const float& f)
	{
		TQuaternion Ret;
		Ret.v.x /= f;
		Ret.v.y /= f;
		Ret.v.z /= f;
		Ret.w /= f;
		return Ret;
	}

	TQuaternion operator+(TQuaternion& q)
	{
		TQuaternion Ret;
		Ret.v.x += q.v.x;
		Ret.v.y += q.v.y;
		Ret.v.z += q.v.z;
		Ret.w += q.w;
		return Ret;
	}
	
	static TQuaternion Slerp(TQuaternion a, TQuaternion b, float t)
	{
		float Pi = ARCCOSINE(a.v.x*b.v.x + a.v.y*b.v.y + a.v.z*b.v.z + a.w*b.w);
		TQuaternion res = (a*(SINE(Pi*(1-t))) + b*(SINE(Pi*t)))/SINE(Pi);
		return res;
	}

	TVector3 v;
	float w;
};

class TMatrix
{
public:

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	TMatrix()
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
	}

	TVector3 TransformVector3(TVector3 In)
	{
		TVector3 ret;
		ret.x = _11 * In.x + _21 * In.y + _31 * In.z + _41;
		ret.y = _12 * In.x + _22 * In.y + _32 * In.z + _42;
		ret.z = _13 * In.x + _23 * In.y + _33 * In.z + _43;
		return ret;
	}

	TMatrix& SetIdentity()
	{
		_11 = 1.0f; _12 = 0.0f; _13 = 0.0f; _14 = 0.0f;
		_21 = 0.0f; _22 = 1.0f; _23 = 0.0f; _24 = 0.0f;
		_31 = 0.0f; _32 = 0.0f; _33 = 1.0f; _34 = 0.0f;
		_41 = 0.0f; _42 = 0.0f; _43 = 0.0f; _44 = 1.0f;
		return *this;
	}

	TMatrix& operator*=(TMatrix& M)
	{
		TMatrix Res;
		Res._11 = _11 * M._11 + _12 * M._21 + _13 * M._31 + _14 * M._41;
		Res._12 = _11 * M._12 + _12 * M._22 + _13 * M._32 + _14 * M._42;
		Res._13 = _11 * M._13 + _12 * M._23 + _13 * M._33 + _14 * M._43;
		Res._13 = _11 * M._14 + _12 * M._24 + _13 * M._34 + _14 * M._44;

		Res._21 = _21 * M._11 + _22 * M._21 + _23 * M._31 + _24 * M._41;
		Res._22 = _21 * M._12 + _22 * M._22 + _23 * M._32 + _24 * M._42;
		Res._23 = _21 * M._13 + _22 * M._23 + _23 * M._33 + _24 * M._43;
		Res._24 = _21 * M._14 + _22 * M._24 + _23 * M._34 + _24 * M._44;

		Res._31 = _31 * M._11 + _32 * M._21 + _33 * M._31 + _34 * M._41;
		Res._32 = _31 * M._12 + _32 * M._22 + _33 * M._32 + _34 * M._42;
		Res._33 = _31 * M._13 + _32 * M._23 + _33 * M._33 + _34 * M._43;
		Res._34 = _31 * M._14 + _32 * M._24 + _33 * M._34 + _34 * M._44;

		Res._41 = _41 * M._11 + _42 * M._21 + _43 * M._31 + _44 * M._41;
		Res._42 = _41 * M._12 + _42 * M._22 + _43 * M._32 + _44 * M._42;
		Res._43 = _41 * M._13 + _42 * M._23 + _43 * M._33 + _44 * M._43;
		Res._44 = _41 * M._14 + _42 * M._24 + _43 * M._34 + _44 * M._44;
		return (*this = Res);
	}

	TMatrix& operator=(TMatrix& M)
	{
		_11 = M._11;	_12 = M._12;	_13 = M._13;	_14 = M._14;
		_21 = M._21;	_22 = M._22;	_23 = M._23;	_24 = M._24;
		_31 = M._31;	_32 = M._32;	_33 = M._33;	_34 = M._34;
		_41 = M._41;	_42 = M._42;	_43 = M._43;	_44 = M._44;
		return *this;
	}

	TMatrix& Translate(TVector3 Transform)
	{
		_41 = Transform.x;
		_42 = Transform.y;
		_43 = Transform.z;
		return *this;
	}

	TMatrix& Rotate(TQuaternion Rotation)
	{
		TMatrix OutMatrix;
		const float x2 = Rotation.v.x + Rotation.v.x;	
		const float y2 = Rotation.v.y + Rotation.v.y;  
		const float z2 = Rotation.v.z + Rotation.v.z;
		{
			const float xx2 = Rotation.v.x * x2;
			const float yy2 = Rotation.v.y * y2;			
			const float zz2 = Rotation.v.z * z2;

			OutMatrix._11 = (1.0f - (yy2 + zz2));	
			OutMatrix._22 = (1.0f - (xx2 + zz2));
			OutMatrix._33 = (1.0f - (xx2 + yy2));
		}
		{
			const float yz2 = Rotation.v.y * z2;   
			const float wx2 = Rotation.w * x2;	

			OutMatrix._32 = (yz2 - wx2);
			OutMatrix._23 = (yz2 + wx2);
		}
		{
			const float xy2 = Rotation.v.x * y2;
			const float wz2 = Rotation.w * z2;

			OutMatrix._21 = (xy2 - wz2);
			OutMatrix._12 = (xy2 + wz2);
		}
		{
			const float xz2 = Rotation.v.x * z2;
			const float wy2 = Rotation.w * y2;   

			OutMatrix._31 = (xz2 + wy2);
			OutMatrix._13 = (xz2 - wy2);
		}
		return *this;
	}
};

class TBoundingBox
{
public:
	TVector3 Extent;
};

class TBoundingSphere
{
public:
	float Radius;
};

class TBounds
{
public:
	TVector3 Position;

	TBoundingBox Box;
	TBoundingSphere Sphere;
};

class TBatch
{
public:
	TBatch() : nVertices(0), nVertexStride(0) {}
	TArray<class BPrimitive*> m_pTemplates;

	int nVertices;
	int nVertexStride;

	enum EPrimitiveType RenderType;

	int GetNumIndices();
	void IndexTessellate();
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