#include "stdafx.h"
#include "RShaderCompiler.h"

void RShaderCompiler::CompileShader() {
    FILE* CacheFile;
    fopen_s(&CacheFile, "ShaderCache.cache", "r+");
    if (CacheFile) {
        fclose(CacheFile);
    }
}
