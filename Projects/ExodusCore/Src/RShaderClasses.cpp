#include "stdafx.h"
#include "RShaderClasses.h"

#include "RDXResource.h"

#include "BLightComponent.h"
#include "BRenderingBatch.h"

void RBaseShaderPass::ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch) {
    for (unsigned int i = 0; i < InBatch->Lights.Size(); ++i) {
        BLightComponent* LightComponent = InBatch->Lights(i);
        LightComponent->ConfigureShader(InPixelShader);
    }
}

void RRenderTargetShaderPass::ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch) {
}