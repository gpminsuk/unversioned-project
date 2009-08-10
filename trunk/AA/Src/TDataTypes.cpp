#include "stdafx.h"
#include "TDataTypes.h"
#include "RResource.h"

TPrimitive::TPrimitive()
: pSubMesh(0), pMaterial(0), pBone(0), pAnimationSequence(0)
{
}

TPrimitive::~TPrimitive()
{
	delete pMaterial; pMaterial = 0;
}