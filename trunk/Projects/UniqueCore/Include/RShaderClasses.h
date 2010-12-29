#pragma once

#include "RResource.h"

class BPrimitive;

class RStandardVertexBuilder : public RShaderConfigure
{
public:
	RStandardVertexBuilder(class RShader* InShader) : RShaderConfigure(InShader) {}

	void ConfigureShader(BPrimitive* InPrimitive);
};

class RBaseSceneVertexShader : public RShaderConfigure
{
public:
	RBaseSceneVertexShader(class RShader* InShader) : RShaderConfigure(InShader) {}

	void ConfigureShader(BPrimitive* InPrimitive);
};

class RBaseScenePixelShader : public RShaderConfigure
{
public:
	RBaseScenePixelShader(class RShader* InShader) : RShaderConfigure(InShader) {}

	void ConfigureShader(BPrimitive* InPrimitive);	
};