#pragma once

#include <vector>
#include <math.h>
#include <algorithm>
// TODO 맵핑을 위해
#define SINE(x)		 sin(x)
#define COSINE(x)	 cos(x)
#define SQRT(x)		 sqrt(x)
#define ARCSINE(x)	 asin(x)
#define ARCCOSINE(x) acos(x)
#define MATH_PI			3.141592f

// Bitfield type.
typedef unsigned long BITFIELD;	// For bitfields.

#define AACCESSOR_OPERATOR(type)				\
	virtual AAccessor& operator<<( ##type& A )	\
{											\
	Access(&A, sizeof(A));					\
	return *this;							\
}											\

class AObject;

class AAccessor
{
public:
	AAccessor();
	~AAccessor();
	virtual void Access(void *A, int Size) = 0;
	virtual bool IsValid();

	AACCESSOR_OPERATOR(float)
		;
	AACCESSOR_OPERATOR(int)
		;
	AACCESSOR_OPERATOR(unsigned int)
		;
	AACCESSOR_OPERATOR(unsigned long)
		;
	AACCESSOR_OPERATOR(short)
		;
	AACCESSOR_OPERATOR(char)
		;
	
	virtual AAccessor& operator<<(class AObject*& A);

	bool IsLoading() const {
		return bIsLoading;
	}
	bool IsSaving() const {
		return bIsSaving;
	}
protected:
	FILE* FilePointer;

	bool bIsLoading;
	bool bIsSaving;
};

template<class T>
class TArray
{
public:
	std::vector<T> mArray;

	T& operator[](int idx)
			{
		return mArray[idx];
	}

	T& operator()(int idx)
			{
		return mArray[idx];
	}

	T& EndItem()
	{
		return mArray[Size() - 1];
	}

	void Clear(bool bFreeMemory = false)
			{
		mArray.clear();
		if (bFreeMemory)
		{
			mArray.vector < T > ::~vector();
			mArray.vector < T > ::vector();
		}
	}

	void AddItem(T Item)
			{
		mArray.push_back(Item);
	}

	void AddItems(TArray<T> Items)
			{
		for (unsigned int i = 0; i < Items.Size(); ++i)
			mArray.push_back(Items(i));
	}

	void DeleteItem(unsigned int index)
			{
		std::vector<T>::iterator it = mArray.begin();
		advance(it, index);
		mArray.erase(it);
	}

	void DeleteItemByVal(T Item)
			{
		std::vector<T>::iterator it = find(mArray.begin(), mArray.end(), Item);
		if (it == mArray.end())
			return;

		mArray.erase(it);
	}

	size_t Size()
	{
		return mArray.size();
	}

	friend AAccessor& operator<<(AAccessor& Ac, TArray& A)
			{
		if (Ac.IsLoading())
		{
			size_t Size;
			Ac << Size;
			// TODO 속도향상 Stack미리 잡아서
			for (unsigned int i = 0; i < Size; ++i)
					{
				T* NewItem = ::new T;
				Ac << *NewItem;
				A.AddItem(*NewItem);
			}
		}
		else if (Ac.IsSaving())
		{
			size_t Size = A.Size();
			Ac << Size;
			for (unsigned int i = 0; i < Size; ++i)
					{
				Ac << A[i];
			}
		}
		return Ac;
	}
};

class TString
{
public:
	char Str[1024];

	TString()
	{
		Str[0] = '\0';
	}
	;

	TString(const char* _Str)
			{
		*this = _Str;
	}
	;

	bool operator ==(TString& _Str)
			{
		return !strcmp(Str, _Str.Str);
	}

	bool operator ==(const char* _Str)
			{
		return !strcmp(Str, _Str);
	}

	void operator =(const TString _Str)
			{
		memcpy(Str, _Str.Str, 1024);
	}

	void operator =(const char* _Str)
			{
		size_t len = (strlen(_Str) < 1024) ? strlen(_Str) : 1024;
		memcpy(Str, _Str, len + 1);
	}

	TString& operator +(TString& _Str)
	{
		int len1 = strlen(Str);
		int len2 = strlen(_Str.Str);
		memcpy(Str + len1, _Str.Str, len2);
		Str[len1 + len2] = '\0';
		return *this;
	}

	TString& operator +(const char* _Str)
	{
		int len1 = strlen(Str);
		int len2 = strlen(_Str);
		memcpy(Str + len1, _Str, len2);
		Str[len1 + len2] = '\0';
		return *this;
	}

	char* GetBuffer()
	{
		return Str;
	}

	unsigned int GetLength()
	{
		return (unsigned int) strlen(Str);
	}

	int ToInt()
	{
		return atoi(Str);
	}

	static TString Format(const char* format, ...);
	friend AAccessor& operator<<(AAccessor& Ac, TString& A)
			{
		for (unsigned int i = 0; i < 1024; ++i)
				{
			Ac << A.Str[i];
		}
		return Ac;
	}
};

class TFilename: public TString
{
public:
};

class TIndex16
{
public:
	TIndex16() {
		_1 = 0;
		_2 = 0;
		_3 = 0;
	}
	TIndex16(short __1, short __2, short __3) {
		_1 = __1;
		_2 = __2;
		_3 = __3;
	}
	short _1, _2, _3;

	friend AAccessor& operator<<(AAccessor& Ac, TIndex16& A)
			{
		Ac << A._1 << A._2 << A._3;
		return Ac;
	}
};

class TIntPoint
{
public:
	TIntPoint() :
			x(0), y(0) {
	}
	TIntPoint(int x1, int y1) :
			x(x1), y(y1) {
	}

	int x;
	int y;
};

class TScalar
{
public:
	TScalar() :
			x(0) {
	}
	TScalar(float x1) :
			x(x1) {
	}
	TScalar(TScalar& s) :
			x(s.x) {
	}

	float x;
};

class TVector2
{
public:
	TVector2() :
			x(0), y(0) {
	}
	TVector2(float x1, float y1) :
			x(x1), y(y1) {
	}
	TVector2(const TVector2& v) :
			x(v.x), y(v.y) {
	}

	TVector2 operator-(TVector2& v) {
		return TVector2(x - v.x, y - v.y);
	}

	TVector2& operator-=(TVector2& v) {
		x -= v.x;
		y -= v.y;
		return *this;
	}
	TVector2& operator*=(TVector2& v) {
		x *= v.x;
		y *= v.y;
		return *this;
	}

	TVector2& operator-=(float f) {
		x -= f;
		y -= f;
		return *this;
	}
	TVector2& operator*=(float f) {
		x *= f;
		y *= f;
		return *this;
	}

	float SizeSquared()
	{
		return x * x + y * y;
	}

	float Size()
	{
		return SQRT(SizeSquared());
	}

	float x;
	float y;
};

class TVector3
{
public:
	TVector3() :
			x(0), y(0), z(0) {
	}
	TVector3(float x1, float y1, float z1) :
			x(x1), y(y1), z(z1) {
	}
	TVector3(const TVector3& v) :
			x(v.x), y(v.y), z(v.z) {
	}

	TVector3 operator*(float f) {
		return TVector3(x * f, y * f, z * f);
	}
	TVector3 operator/(float f) {
		return TVector3(x / f, y / f, z / f);
	}
	TVector3 operator-() {
		return TVector3(-x, -y, -z);
	}
	TVector3 operator^(TVector3& v) {
		return TVector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
	TVector3 operator+(TVector3& v) {
		return TVector3(x + v.x, y + v.y, z + v.z);
	}
	TVector3 operator-(TVector3& v) {
		return TVector3(x - v.x, y - v.y, z - v.z);
	}
	TVector3& operator=(const TVector3& v) {
		x = v.x;
		y = v.y;
		z = v.z;
		return *this;
	}
	float operator|(TVector3& v) {
		return x * v.x + y * v.y + z * v.z;
	}

	TVector3& operator+=(TVector3& v) {
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}
	TVector3& operator-=(TVector3& v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	TVector3& operator*=(TVector3& v) {
		x *= v.x;
		y *= v.y;
		z *= v.z;
		return *this;
	}

	bool operator!=(TVector3& v) {
		return (x != v.x || y != v.y || z != v.z);
	}

	static TVector3 Lerp(TVector3 Start, TVector3 End, float t)
	{
		return Start * (1.0f - t) + End * t;
	}

	static TVector3 Slerp(TVector3 Start, TVector3 End, float t);

	float SizeSquared()
	{
		return x * x + y * y + z * z;
	}

	float Size()
	{
		return SQRT(SizeSquared());
	}

	TVector3& Normalize()
	{
		float Squared = SQRT(SizeSquared());
		if(Squared == 0.0f) {
			x = 0.0f;
			y = 0.0f;
			z = 0.0f;
		}
		else {
			x /= Squared;
			y /= Squared;
			z /= Squared;
		}		
		return *this;
	}

	friend AAccessor& operator<<(AAccessor& Ac, TVector3& V)
	{
		Ac << V.x;
		Ac << V.y;
		Ac << V.z;
		return Ac;
	}

	float x;
	float y;
	float z;
};

class TVector4
{
public:
	TVector4() :
			x(0), y(0), z(0), w(0) {
	}
	TVector4(float x1, float y1, float z1, float w1) :
			x(x1), y(y1), z(z1), w(w1) {
	}
	TVector4(const TVector4& v) :
			x(v.x), y(v.y), z(v.z), w(v.w) {
	}
	TVector4(const TVector3& v, float w1) :
			x(v.x), y(v.y), z(v.z), w(w1) {
	}

	float x;
	float y;
	float z;
	float w;
};

class TQuaternion
{
public:
	TQuaternion() :
			x(0), y(0), z(0), w(1) {
	}
	TQuaternion(float x1, float y1, float z1, float w1) :
			x(x1), y(y1), z(z1), w(w1) {
	}
	TQuaternion(const TQuaternion& _v) :
			x(_v.x), y(_v.y), z(_v.z), w(_v.w) {
	}
	TQuaternion(TVector3 Axis, float theta) :
			x(Axis.x * SINE(theta/2.0f)), y(Axis.y * SINE(theta/2.0f)), z(
					Axis.z * SINE(theta/2.0f)), w(COSINE(theta/2.0f)) {
	}

	void Rotate(TVector3 Axis, float theta)
			{
		TQuaternion Temp(Axis, theta);
		*this *= Temp;
	}

	TQuaternion& Normalize()
	{
		float Squared = SQRT(SizeSquared());
		x /= Squared;
		y /= Squared;
		z /= Squared;
		w /= Squared;
		return *this;
	}

	float SizeSquared()
	{
		return x * x + y * y + z * z + w * w;
	}

	void Initialize()
	{
		x = 0.0f;
		y = 0.0f;
		z = 0.0f;
		w = 1.0f;
	}

	TQuaternion operator-() {
		return TQuaternion(-x, -y, -z, -w);
	}

	TQuaternion& operator*=(TQuaternion& q)
			{
		float Dx = x * q.w + y * q.z - z * q.y + w * q.x;
		float Dy = -x * q.z + y * q.w + z * q.x + w * q.y;
		float Dz = x * q.y - y * q.x + z * q.w + w * q.z;
		float Dw = -x * q.x - y * q.y - z * q.z + w * q.w;

		x = Dx;
		y = Dy;
		z = Dz;
		w = Dw;
		return *this;
	}

	TQuaternion operator*(TQuaternion& q)
			{
		TQuaternion Ret;
		float Dx = x * q.w + y * q.z - z * q.y + w * q.x;
		float Dy = -x * q.z + y * q.w + z * q.x + w * q.y;
		float Dz = x * q.y - y * q.x + z * q.w + w * q.z;
		float Dw = -x * q.x - y * q.y - z * q.z + w * q.w;

		Ret.x = Dx;
		Ret.y = Dy;
		Ret.z = Dz;
		Ret.w = Dw;
		return Ret;
	}

	TQuaternion operator*(const float& f)
			{
		TQuaternion Ret;
		Ret.x = x * f;
		Ret.y = y * f;
		Ret.z = z * f;
		Ret.w = w * f;
		return Ret;
	}

	TQuaternion operator/(const float& f)
			{
		TQuaternion Ret;
		Ret.x = x / f;
		Ret.y = y / f;
		Ret.z = z / f;
		Ret.w = w / f;
		return Ret;
	}

	TQuaternion operator+(TQuaternion& q)
			{
		TQuaternion Ret;
		Ret.x = x + q.x;
		Ret.y = y + q.y;
		Ret.z = z + q.z;
		Ret.w = w + q.w;
		return Ret;
	}

	bool operator!=(TQuaternion& q) {
		return (x != q.x || y != q.y || z != q.z || w != q.w);
	}

	static TQuaternion Lerp(TQuaternion a, TQuaternion b, float t)
			{
		return (a * (1.0f - t) + b * t);
	}

	static TQuaternion Slerp(TQuaternion a, TQuaternion b, float t)
			{
		float CosinePi = a.x * b.x + a.y * b.y + a.z * b.z + a.w * b.w;
		if (CosinePi < 0.0f)
				{
			CosinePi = -CosinePi;
			b = -b;
		}
		float Pi = ARCCOSINE(CosinePi);
		TQuaternion res;
		if (Pi <= 1.0f)
				{
			if (Pi > 0) {
				res = a * (SINE(Pi*(1.0f-t)) / SINE(Pi))
						+ b * (SINE(Pi*t) / SINE(Pi));
				res.Normalize();
			}
			else {
				res = a;
			}
		}
		else
		{
			res = Lerp(a, b, t);
			res.Normalize();
		}
		return res;
	}

	float x;
	float y;
	float z;
	float w;
};

class TMatrix
{
public:

	static TMatrix Identity;

	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	TMatrix()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_24 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
		_34 = 0.0f;
		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
	}

	TMatrix(int zero)
			{
		_11 = 0.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;
		_21 = 0.0f;
		_22 = 0.0f;
		_23 = 0.0f;
		_24 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 0.0f;
		_34 = 0.0f;
		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 0.0f;
	}

	TMatrix(TVector3 Translation, TQuaternion Rotation, TVector3 Scale)
			{
		SetIdentity();
		_11 *= Scale.x;
		_22 *= Scale.y;
		_33 *= Scale.z;
		Rotate(Rotation);
		_41 = Translation.x;
		_42 = Translation.y;
		_43 = Translation.z;
		_44 = 1.0f;
	}

	TMatrix(TVector3 Translation, TQuaternion Rotation, float Scale)
			{
		*this = TMatrix(Translation, Rotation, TVector3(Scale, Scale, Scale));
	}

	TMatrix(TVector3 Direction, TVector3 Up)
	{
		TVector3 Forward = Direction;
		Forward.x = -Forward.x;
		Forward.y = 0;
		Forward.Normalize();

		TVector3 Side =  Forward ^ Up;
		Side.Normalize();

		TVector3 PlaneUp = Side ^ Forward;
		PlaneUp.Normalize();
		_11 = Side.x;
		_12 = PlaneUp.x;
		_13 = -Forward.x;
		_14 = 0.0f;

		_21 = Side.y;
		_22 = PlaneUp.y;
		_23 = -Forward.y;
		_24 = 0.0f;

		_31 = Side.z;
		_32 = PlaneUp.z;
		_33 = -Forward.z;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
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
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;
		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_24 = 0.0f;
		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
		_34 = 0.0f;
		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
		return *this;
	}

	TMatrix& operator*=(TMatrix& M)
			{
		TMatrix Res;
		Res._11 = _11 * M._11 + _12 * M._21 + _13 * M._31 + _14 * M._41;
		Res._12 = _11 * M._12 + _12 * M._22 + _13 * M._32 + _14 * M._42;
		Res._13 = _11 * M._13 + _12 * M._23 + _13 * M._33 + _14 * M._43;
		Res._14 = _11 * M._14 + _12 * M._24 + _13 * M._34 + _14 * M._44;

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

	TMatrix operator*(const TMatrix& M)
			{
		TMatrix Res;
		Res._11 = _11 * M._11 + _12 * M._21 + _13 * M._31 + _14 * M._41;
		Res._12 = _11 * M._12 + _12 * M._22 + _13 * M._32 + _14 * M._42;
		Res._13 = _11 * M._13 + _12 * M._23 + _13 * M._33 + _14 * M._43;
		Res._14 = _11 * M._14 + _12 * M._24 + _13 * M._34 + _14 * M._44;

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
		return Res;
	}

	TMatrix& operator=(TMatrix& M)
			{
		_11 = M._11;
		_12 = M._12;
		_13 = M._13;
		_14 = M._14;
		_21 = M._21;
		_22 = M._22;
		_23 = M._23;
		_24 = M._24;
		_31 = M._31;
		_32 = M._32;
		_33 = M._33;
		_34 = M._34;
		_41 = M._41;
		_42 = M._42;
		_43 = M._43;
		_44 = M._44;
		return *this;
	}

	TMatrix& operator*=(const float& f)
			{
		TMatrix Res;
		Res._11 = f * _11;
		Res._12 = f * _12;
		Res._13 = f * _13;
		Res._14 = f * _14;

		Res._21 = f * _21;
		Res._22 = f * _22;
		Res._23 = f * _23;
		Res._24 = f * _24;

		Res._31 = f * _31;
		Res._32 = f * _32;
		Res._33 = f * _33;
		Res._34 = f * _34;

		Res._41 = f * _41;
		Res._42 = f * _42;
		Res._43 = f * _43;
		Res._44 = f * _44;
		return (*this = Res);
	}

	TMatrix operator*(const float& f)
			{
		TMatrix Res;
		Res._11 = f * _11;
		Res._12 = f * _12;
		Res._13 = f * _13;
		Res._14 = f * _14;

		Res._21 = f * _21;
		Res._22 = f * _22;
		Res._23 = f * _23;
		Res._24 = f * _24;

		Res._31 = f * _31;
		Res._32 = f * _32;
		Res._33 = f * _33;
		Res._34 = f * _34;

		Res._41 = f * _41;
		Res._42 = f * _42;
		Res._43 = f * _43;
		Res._44 = f * _44;
		return Res;
	}

	TMatrix& operator+=(const TMatrix& M)
			{
		TMatrix Res;
		Res._11 = M._11 + _11;
		Res._12 = M._12 + _12;
		Res._13 = M._13 + _13;
		Res._14 = M._14 + _14;

		Res._21 = M._21 + _21;
		Res._22 = M._22 + _22;
		Res._23 = M._23 + _23;
		Res._24 = M._24 + _24;

		Res._31 = M._31 + _31;
		Res._32 = M._32 + _32;
		Res._33 = M._33 + _33;
		Res._34 = M._34 + _34;

		Res._41 = M._41 + _41;
		Res._42 = M._42 + _42;
		Res._43 = M._43 + _43;
		Res._44 = M._44 + _44;
		return (*this = Res);
	}

	TMatrix operator+(const TMatrix& M)
			{
		TMatrix Res;
		Res._11 = M._11 + _11;
		Res._12 = M._12 + _12;
		Res._13 = M._13 + _13;
		Res._14 = M._14 + _14;

		Res._21 = M._21 + _21;
		Res._22 = M._22 + _22;
		Res._23 = M._23 + _23;
		Res._24 = M._24 + _24;

		Res._31 = M._31 + _31;
		Res._32 = M._32 + _32;
		Res._33 = M._33 + _33;
		Res._34 = M._34 + _34;

		Res._41 = M._41 + _41;
		Res._42 = M._42 + _42;
		Res._43 = M._43 + _43;
		Res._44 = M._44 + _44;
		return Res;
	}

	inline float Determinant() const
	{
		return _11 * (
				_22 * (_33 * _44 - _34 * _43) -
						_32 * (_23 * _44 - _24 * _43) +
						_42 * (_23 * _34 - _24 * _33)
				) -
				_21 * (
						_12 * (_33 * _44 - _34 * _43) -
								_32 * (_13 * _44 - _14 * _43) +
								_42 * (_13 * _34 - _14 * _33)
						) +
				_31 * (
						_12 * (_23 * _44 - _24 * _43) -
								_22 * (_13 * _44 - _14 * _43) +
								_42 * (_13 * _24 - _14 * _23)
						) -
				_41 * (
						_12 * (_23 * _34 - _24 * _33) -
								_22 * (_13 * _34 - _14 * _33) +
								_32 * (_13 * _24 - _14 * _23)
						);
	}

	TMatrix& Inverse()
	{
		TMatrix Result;
		const float Det = Determinant();

		if (Det == 0.0f)
			return TMatrix::Identity;

		const float RDet = 1.0f / Det;

		Result._11 = RDet * (
				_22 * (_33 * _44 - _34 * _43) -
						_32 * (_23 * _44 - _24 * _43) +
						_42 * (_23 * _34 - _24 * _33)
				);
		Result._12 = -RDet * (
				_12 * (_33 * _44 - _34 * _43) -
						_32 * (_13 * _44 - _14 * _43) +
						_42 * (_13 * _34 - _14 * _33)
				);
		Result._13 = RDet * (
				_12 * (_23 * _44 - _24 * _43) -
						_22 * (_13 * _44 - _14 * _43) +
						_42 * (_13 * _24 - _14 * _23)
				);
		Result._14 = -RDet * (
				_12 * (_23 * _34 - _24 * _33) -
						_22 * (_13 * _34 - _14 * _33) +
						_32 * (_13 * _24 - _14 * _23)
				);

		Result._21 = -RDet * (
				_21 * (_33 * _44 - _34 * _43) -
						_31 * (_23 * _44 - _24 * _43) +
						_41 * (_23 * _34 - _24 * _33)
				);
		Result._22 = RDet * (
				_11 * (_33 * _44 - _34 * _43) -
						_31 * (_13 * _44 - _14 * _43) +
						_41 * (_13 * _34 - _14 * _33)
				);
		Result._23 = -RDet * (
				_11 * (_23 * _44 - _24 * _43) -
						_21 * (_13 * _44 - _14 * _43) +
						_41 * (_13 * _24 - _14 * _23)
				);
		Result._24 = RDet * (
				_11 * (_23 * _34 - _24 * _33) -
						_21 * (_13 * _34 - _14 * _33) +
						_31 * (_13 * _24 - _14 * _23)
				);

		Result._31 = RDet * (
				_21 * (_32 * _44 - _34 * _42) -
						_31 * (_22 * _44 - _24 * _42) +
						_41 * (_22 * _34 - _24 * _32)
				);
		Result._32 = -RDet * (
				_11 * (_32 * _44 - _34 * _42) -
						_31 * (_12 * _44 - _14 * _42) +
						_41 * (_12 * _34 - _14 * _32)
				);
		Result._33 = RDet * (
				_11 * (_22 * _44 - _24 * _42) -
						_21 * (_12 * _44 - _14 * _42) +
						_41 * (_12 * _24 - _14 * _22)
				);
		Result._34 = -RDet * (
				_11 * (_22 * _34 - _24 * _32) -
						_21 * (_12 * _34 - _14 * _32) +
						_31 * (_12 * _24 - _14 * _22)
				);

		Result._41 = -RDet * (
				_21 * (_32 * _43 - _33 * _42) -
						_31 * (_22 * _43 - _23 * _42) +
						_41 * (_22 * _33 - _23 * _32)
				);
		Result._42 = RDet * (
				_11 * (_32 * _43 - _33 * _42) -
						_31 * (_12 * _43 - _13 * _42) +
						_41 * (_12 * _33 - _13 * _32)
				);
		Result._43 = -RDet * (
				_11 * (_22 * _43 - _23 * _42) -
						_21 * (_12 * _43 - _13 * _42) +
						_41 * (_12 * _23 - _13 * _22)
				);
		Result._44 = RDet * (
				_11 * (_22 * _33 - _23 * _32) -
						_21 * (_12 * _33 - _13 * _32) +
						_31 * (_12 * _23 - _13 * _22)
				);

		return (*this = Result);
		/*TMatrix Result;
		 TMatrix Tmp;
		 Tmp._11	= _33 * _44 - _34 * _43;
		 Tmp._12	= _23 * _44 - _24 * _43;
		 Tmp._13	= _23 * _34 - _24 * _33;

		 Tmp._21	= _33 * _44 - _34 * _43;
		 Tmp._22	= _13 * _44 - _14 * _43;
		 Tmp._23	= _13 * _34 - _14 * _33;

		 Tmp._31	= _23 * _44 - _24 * _43;
		 Tmp._32	= _13 * _44 - _14 * _43;
		 Tmp._33	= _13 * _24 - _14 * _23;

		 Tmp._41	= _23 * _34 - _24 * _33;
		 Tmp._42	= _13 * _34 - _14 * _33;
		 Tmp._43	= _13 * _24 - _14 * _23;

		 float Det[4];

		 Det[0]		= _22*Tmp._11 - _32*Tmp._12 + _42*Tmp._13;
		 Det[1]		= _12*Tmp._21 - _32*Tmp._22 + _42*Tmp._23;
		 Det[2]		= _12*Tmp._31 - _22*Tmp._32 + _42*Tmp._33;
		 Det[3]		= _12*Tmp._41 - _22*Tmp._42 + _32*Tmp._43;

		 float Determinant = _11*Det[0] - _21*Det[1] + _31*Det[2] - _41*Det[3];

		 if(Determinant == 0.0f)
		 return TMatrix::Identity;

		 const float	RDet = 1.0f / Determinant;

		 Result._11 =  RDet * Det[0];
		 Result._12 = -RDet * Det[1];
		 Result._13 =  RDet * Det[2];
		 Result._14 = -RDet * Det[3];
		 Result._21 = -RDet * (_21*Tmp._11 - _31*Tmp._12 + _41*Tmp._13);
		 Result._22 =  RDet * (_11*Tmp._21 - _31*Tmp._22 + _41*Tmp._23);
		 Result._23 = -RDet * (_11*Tmp._31 - _21*Tmp._32 + _41*Tmp._33);
		 Result._24 =  RDet * (_11*Tmp._41 - _21*Tmp._42 + _31*Tmp._43);
		 Result._31 =  RDet * (
		 _21 * (_32 * _44 - _34 * _42) -
		 _31 * (_22 * _44 - _24 * _42) +
		 _41 * (_22 * _34 - _24 * _32)
		 );
		 Result._32 = -RDet * (
		 _11 * (_32 * _44 - _34 * _42) -
		 _31 * (_12 * _44 - _14 * _42) +
		 _41 * (_12 * _34 - _14 * _32)
		 );
		 Result._33 =  RDet * (
		 _11 * (_22 * _44 - _24 * _42) -
		 _21 * (_12 * _44 - _14 * _42) +
		 _41 * (_12 * _24 - _14 * _22)
		 );
		 Result._34 = -RDet * (
		 _11 * (_22 * _34 - _24 * _32) -
		 _21 * (_12 * _34 - _14 * _32) +
		 _31 * (_12 * _24 - _14 * _22)
		 );
		 Result._41 = -RDet * (
		 _21 * (_32 * _43 - _33 * _42) -
		 _31 * (_22 * _43 - _23 * _42) +
		 _41 * (_22 * _33 - _23 * _32)
		 );
		 Result._42 =  RDet * (
		 _11 * (_32 * _43 - _33 * _42) -
		 _31 * (_12 * _43 - _13 * _42) +
		 _41 * (_12 * _33 - _13 * _32)
		 );
		 Result._43 = -RDet * (
		 _11 * (_22 * _43 - _23 * _42) -
		 _21 * (_12 * _43 - _13 * _42) +
		 _41 * (_12 * _23 - _13 * _22)
		 );
		 Result._44 =  RDet * (
		 _11 * (_22 * _33 - _23 * _32) -
		 _21 * (_12 * _33 - _13 * _32) +
		 _31 * (_12 * _23 - _13 * _22)
		 );
		 *this = Result;
		 return *this;*/
	}

	TMatrix& Translate(TVector3 Transform)
			{
		_41 = Transform.x;
		_42 = Transform.y;
		_43 = Transform.z;
		return *this;
	}

	TVector3 GetTranslation()
	{		
		return TVector3(_41, _42, _43);
	}

	TQuaternion GetRotation()
	{
		float w = SQRT(1 + _11 + _22 + _33) / 2.0f;
		return TQuaternion(w, (_32 - _23)/(4 * w),(_13 - _31)/(4 * w),(_21 - _12)/(4 * w));
	}

	TMatrix& Scale(TVector3 InScale)
			{
		_11 *= InScale.x;
		_22 *= InScale.y;
		_33 *= InScale.z;
		return *this;
	}

	TMatrix& Rotate(TQuaternion Rotation)
			{
		TMatrix RotationMat;
		RotationMat._11 = 1.0f
				- (2.0f * Rotation.y * Rotation.y
						+ 2.0f * Rotation.z * Rotation.z);
		RotationMat._12 = (2.0f * Rotation.x * Rotation.y
				- 2.0f * Rotation.z * Rotation.w);
		RotationMat._13 = (2.0f * Rotation.x * Rotation.z
				+ 2.0f * Rotation.y * Rotation.w);

		RotationMat._21 = (2.0f * Rotation.x * Rotation.y
				+ 2.0f * Rotation.z * Rotation.w);
		RotationMat._22 = 1.0f
				- (2.0f * Rotation.x * Rotation.x
						+ 2.0f * Rotation.z * Rotation.z);
		RotationMat._23 = (2.0f * Rotation.y * Rotation.z
				- 2.0f * Rotation.x * Rotation.w);

		RotationMat._31 = (2.0f * Rotation.x * Rotation.z
				- 2.0f * Rotation.y * Rotation.w);
		RotationMat._32 = (2.0f * Rotation.y * Rotation.z
				+ 2.0f * Rotation.x * Rotation.w);
		RotationMat._33 = 1.0f
				- (2.0f * Rotation.x * Rotation.x
						+ 2.0f * Rotation.y * Rotation.y);

		*this *= RotationMat;
		/*const float x2 = Rotation.x + Rotation.x;	
		 const float y2 = Rotation.y + Rotation.y;  
		 const float z2 = Rotation.z + Rotation.z;
		 {
		 const float xx2 = Rotation.v.x * x2;
		 const float yy2 = Rotation.v.y * y2;			
		 const float zz2 = Rotation.v.z * z2;

		 _11 = (1.0f - (yy2 + zz2));	
		 _22 = (1.0f - (xx2 + zz2));
		 _33 = (1.0f - (xx2 + yy2));
		 }
		 {
		 const float yz2 = Rotation.v.y * z2;   
		 const float wx2 = Rotation.w * x2;	

		 _32 = (yz2 - wx2);
		 _23 = (yz2 + wx2);
		 }
		 {
		 const float xy2 = Rotation.v.x * y2;
		 const float wz2 = Rotation.w * z2;

		 _21 = (xy2 - wz2);
		 _12 = (xy2 + wz2);
		 }
		 {
		 const float xz2 = Rotation.v.x * z2;
		 const float wy2 = Rotation.w * y2;   

		 _31 = (xz2 + wy2);
		 _13 = (xz2 - wy2);
		 }*/
		return *this;
	}

	friend AAccessor& operator<<(AAccessor& Ac, TMatrix& A)
			{
		Ac << A._11 << A._12 << A._13 << A._14;
		Ac << A._21 << A._22 << A._23 << A._24;
		Ac << A._31 << A._32 << A._33 << A._34;
		Ac << A._41 << A._42 << A._43 << A._44;
		return Ac;
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

enum ECameraMode
{
	CameraMode_FirstPerson,
	CameraMode_ThridPerson,
	CameraMode_FreeMode,
	CameraMode_Editor,
	CameraMode_Right,
	CameraMode_Left,
	CameraMode_Top,
	CameraMode_Bottom,
	CameraMode_Front,
	CameraMode_Back,
};
