#include "StdAfx.h"

#include "BThing.h"

#include "CMeshCollisionBody.h"

#include "RMesh.h"

CMeshCollisionBody::CMeshCollisionBody(BThing* InOwner)
    :
    BCollisionBody(InOwner) {
}

CMeshCollisionBody::~CMeshCollisionBody() {
    for (unsigned int i = 0; i < Primitives.Size(); ++i) {
        delete Primitives(i);
    }
    Primitives.Clear();
}

void CMeshCollisionBody::SetMesh(RMesh* InMesh) {
	Mesh = InMesh;
}

bool SameSide(TVector3 p1, TVector3 p2, TVector3 a, TVector3 b) {
	TVector3 cp1 = (b - a) ^ (p1 - a);
	TVector3 cp2 = (b - a) ^ (p2 - a);
	if((cp1 | cp2) >= 0) {
		return true;
	}
	return false;
}

bool PointInTriangle(TVector3 p, TVector3 a, TVector3 b, TVector3 c) {
	if(SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b)) {
		return true;
	}
	return false;
}
#define EPSILON 0.000001

int triangle_intersection( TVector3   V0,  // Triangle vertices
						  TVector3   V1,
						  TVector3   V2,
						  TVector3    O,  //Ray origin
						  TVector3    D,  //Ray direction
						  float* out )
{
	TVector3 e1, e2;  //Edge1, Edge2
	TVector3 P, Q, T;
	float det, inv_det, u, v;
	float t;

	//Find vectors for two edges sharing V0
	e1 = V1 - V0;
	e2 = V2 - V0;
	//Begin calculating determinant - also used to calculate u parameter
	P = D ^ e2;
	//if determinant is near zero, ray lies in plane of triangle
	det = e1 | P;
	//NOT CULLING
	if(det > -EPSILON && det < EPSILON) return 0;
	inv_det = 1.f / det;

	//calculate distance from V0 to ray origin
	T = O - V0;

	//Calculate u parameter and test bound
	u = (T | P) * inv_det;
	//The intersection lies outside of the triangle
	if(u < 0.f || u > 1.f) return 0;

	//Prepare to test v parameter
	Q = T ^ e1;

	//Calculate V parameter and test bound
	v = (D | Q) * inv_det;
	//The intersection lies outside of the triangle
	if(v < 0.f || u + v  > 1.f) return 0;

	t = (e2 | Q) * inv_det;

	if(t > EPSILON) { //ray intersection
		*out = t;
		return 1;
	}

	// No hit, no win
	return 0;
}

TVector3 CMeshCollisionBody::LineCheck(TVector3& Start, TVector3& End, TVector3& Extent) {    
	for(unsigned int i=0;i<Mesh->SubMeshes.Size();++i) {
		RSubMesh* SubMesh = Mesh->SubMeshes(i);
		RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pVertices = (RVertexDeclaration::SkeletalMesh_GPU_Skin_VD*)SubMesh->pVB->pVertices;
		for(unsigned int j=0;j<SubMesh->pIB->nIndices;++j) {
			TVector3 p = TVector3(Start.x, 0, Start.z);
			TVector3 a = pVertices[SubMesh->pIB->pIndices[j]._1].Position;
			TVector3 b = pVertices[SubMesh->pIB->pIndices[j]._2].Position;
			TVector3 c = pVertices[SubMesh->pIB->pIndices[j]._3].Position;
			float out;
			if(triangle_intersection(a, b, c, Start, (End - Start), &out)) {
				return Start + (End - Start) * out;
				
			}
		}
	}
    return TVector3(0.0f, 0.0f, 0.0f);
}
