#include "stdafx.h"

#include "RSKNImporter.h"

#include "RMesh.h"
#include "CDirectXDriver.h"
#include "RShaderClasses.h"

bool RSKNImporter::Import(TString& Filename, RAnimationSequence*& AnimationSequence, RBoneHierarchy*& BoneHierarchy, RMesh*& Model) {
	char* fn = Filename.Str;
	TString NodeName;
	TString ParentNodeName;
	FILE *fp;
	fopen_s(&fp, fn, "rb");
	if (!fp)
		return false;

	Model = new RMesh();

	int magic;
	fread(&magic, sizeof(int), 1, fp);
	short version;
	fread(&version, sizeof(short), 1, fp);
	short numObject;
	fread(&numObject, sizeof(short), 1, fp);

	for (int n = 0; n < numObject; ++n) {
		RSubMesh* subMesh = new RSubMesh();

		subMesh->pVB = new RSystemMemoryVertexBuffer();
		subMesh->pIB = new RSystemMemoryIndexBuffer();

		if (version == 1 || version == 2) {
			int numMat;
			fread(&numMat, sizeof(int), 1, fp);
			for (int i = 0; i < numMat; ++i) {
				char name[64];
				fread(name, sizeof(char) * 64, 1, fp);
				int startVertex;
				fread(&startVertex, sizeof(int), 1, fp);
				int numVertices;
				fread(&numVertices, sizeof(int), 1, fp);
				int startIndex;
				fread(&startIndex, sizeof(int), 1, fp);
				int numIndices;
				fread(&numIndices, sizeof(int), 1, fp);
			}
		}

		fread(&subMesh->pIB->nIndices, sizeof(int), 1, fp);
		subMesh->pIB->nIndices /= 3;
		fread(&subMesh->pVB->nVertices, sizeof(int), 1, fp);

		subMesh->pIB->pIndices = new TIndex16[subMesh->pIB->nIndices];
		fread(subMesh->pIB->pIndices, sizeof(TIndex16), subMesh->pIB->nIndices,
			fp);

		RVertexDeclaration::SkeletalMesh_GPU_Skin_VD* pVertices = new RVertexDeclaration::SkeletalMesh_GPU_Skin_VD[subMesh->pVB->nVertices];
		subMesh->pVB->pVertices = (char*)pVertices;

		for(unsigned int i=0;i<subMesh->pVB->nVertices;++i) {
			fread(&pVertices[i].Position, sizeof(TVector3), 1, fp);
			fread(pVertices[i].BoneIndices, sizeof(char)*4, 1, fp);
			fread(pVertices[i].BoneWeights, sizeof(float)*4, 1, fp);
			fread(&pVertices[i].Normal, sizeof(TVector3), 1, fp);
			fread(&pVertices[i].TexCoord, sizeof(TVector2), 1, fp);
		}

		subMesh->pVB->Protocol = RVertexProtocol::Protocols(0);

		Model->SkeletalSubMeshes.AddItem(subMesh);
	}
	fclose(fp);
	return true;
}
