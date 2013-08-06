#pragma once

#include "RResource.h"

class BPrimitive;

class RStandardVertexBuilder: public RShaderConfigure
{
public:
	RStandardVertexBuilder(class RShader* InShader) :
			RShaderConfigure(InShader) {
	}

	virtual void ConfigureLight(BRenderingBatch* InBatch);
};

class RBaseSceneVertexShader: public RShaderConfigure
{
public:
	RBaseSceneVertexShader(class RShader* InShader) :
			RShaderConfigure(InShader) {
	}

	virtual void ConfigureLight(BRenderingBatch* InBatch);
};

class RBaseScenePixelShader: public RShaderConfigure
{
public:
	RBaseScenePixelShader(class RShader* InShader) :
			RShaderConfigure(InShader) {
	}

	virtual void ConfigureLight(BRenderingBatch* InBatch);
};
