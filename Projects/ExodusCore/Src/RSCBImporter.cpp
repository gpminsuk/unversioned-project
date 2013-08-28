#include "stdafx.h"
#include "RSCBImporter.h"

#include "RBoneHierarchy.h"
#include "RMesh.h"

bool RSCBImporter::Import(TString& Filename, RAnimationSequence*& AnimationSequence, RBoneHierarchy*& BoneHierarchy, RMesh*& Model) {
    char* fn = Filename.Str;
    TString NodeName;
    TString ParentNodeName;
    FILE *fp;
    fopen_s(&fp, fn, "rb");
    if (!fp)
        return false;

    char magic[8];
    fread(magic, sizeof(char) * 8, 1, fp);
    int majorVersion;
    fread(&majorVersion, sizeof(short), 1, fp);
	int minorVersion;
	fread(&minorVersion, sizeof(short), 1, fp);
	char objName[128];
	fread(objName, sizeof(char) * 128, 1, fp);

	int numVertices;
	fread(&numVertices, sizeof(int), 1, fp);
	int numFaces;
	fread(&numFaces, sizeof(int), 1, fp);
	int unknown;
	fread(&unknown, sizeof(int), 1, fp);

	float f;
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);

	Model = new RMesh();

	RSubMesh* subMesh = new RSubMesh();
	subMesh->pVB = new RSystemMemoryVertexBuffer();
	subMesh->pIB = new RSystemMemoryIndexBuffer();

	Model->SubMeshes.AddItem(subMesh);

	subMesh->pVB->nVertices = numVertices;
	// SkeletalMesh_GPU_Skin_VD 임시로 이 Protocol 사용
	RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pVertices = new RVertexDeclaration::SkeletalMesh_GPU_Skin_VD[subMesh->pVB->nVertices];
	subMesh->pVB->pVertices = (char*)pVertices;

	for(int i=0;i<numVertices;++i) {
		fread(&pVertices[i].Position, sizeof(TVector3), 1, fp);
	}
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);
	fread(&f, sizeof(float), 1, fp);

	subMesh->pIB->nIndices = numFaces;
	subMesh->pIB->pIndices = new TIndex16[subMesh->pIB->nIndices];
	for(int i=0;i<numFaces;++i) {
		int in;
		fread(&in, sizeof(int), 1, fp);
		subMesh->pIB->pIndices[i]._1 = in;
		fread(&in, sizeof(int), 1, fp);
		subMesh->pIB->pIndices[i]._2 = in;
		fread(&in, sizeof(int), 1, fp);
		subMesh->pIB->pIndices[i]._3 = in;
			
		char matName[64];
		fread(matName, sizeof(char) * 64, 1, fp);
		fread(&f, sizeof(float), 1, fp);
		fread(&f, sizeof(float), 1, fp);
		fread(&f, sizeof(float), 1, fp);
		fread(&f, sizeof(float), 1, fp);
		fread(&f, sizeof(float), 1, fp);
		fread(&f, sizeof(float), 1, fp);
	}

    fclose(fp);
	return true;
}
