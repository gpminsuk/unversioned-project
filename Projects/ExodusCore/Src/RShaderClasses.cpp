#include "stdafx.h"
#include "RShaderClasses.h"

#include "RDXResource.h"
#include "BLight.h"
#include "BRenderingBatch.h"

void RBaseShaderPass::ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch) {
    for (unsigned int i = 0; i < InBatch->Lights.Size(); ++i) {
        BLight* Light = InBatch->Lights(i);
        Light->ConfigureShader(InPixelShader);
    }
}

void RRenderTargetShaderPass::ConfigureLight(RShader* InPixelShader, RShader* InVertexShader, BRenderingBatch* InBatch) {
}