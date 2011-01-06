#include "stdafx.h"
#include "RShaderClasses.h"

#include "BLightComponent.h"
#include "BRenderingBatch.h"

void RBaseScenePixelShader::ConfigureLight(BRenderingBatch* InBatch)
{
	for(unsigned int i=0;i<InBatch->Lights.Size();++i)
	{
		BLightComponent* LightComponent = InBatch->Lights(i);
		LightComponent->ConfigureShader(Shader);
	}	
}