#include "StdAfx.h"
#include "CBoxComponent.h"
#include "CBoxPrimitive.h" 

CBoxComponent::CBoxComponent()
{
	CBoxPrimitive* BoxPrimitive = new CBoxPrimitive();
	Primitives.AddItem(BoxPrimitive);
}

CBoxComponent::~CBoxComponent()
{
}

void CBoxComponent::UpdateComponent()
{
}