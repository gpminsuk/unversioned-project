#include "StdAfx.h"
#include "RResource.h"

#include "BDriver.h"

TArray<RMaterial*> RMaterialTable::Materials;

RVertexDeclaration* RVertexDeclaration::Position_Normal_TexCoord;
RVertexDeclaration* RVertexDeclaration::Position_Normal;
RVertexDeclaration* RVertexDeclaration::Position_TexCoord;
RVertexDeclaration* RVertexDeclaration::SkeletalMesh_GPU_Skin;

TArray<RVertexProtocol*> RVertexProtocol::Protocols;
TArray<RShaderPass*> RShaderPass::ShaderPasses;

RMaterial::RMaterial(TString Name) {
	for(unsigned int i=0;i<RShaderPass::ShaderPasses.Size();++i) {
		RShaderPass* ShaderPass = RShaderPass::ShaderPasses(i);
		for(unsigned int j=0;j<RVertexProtocol::Protocols.Size();++j) {
			RVertexProtocol* VertexProtocol = RVertexProtocol::Protocols(j);
			TString VertexShaderFullName = "";
			VertexShaderFullName = VertexShaderFullName + "..\\..\\Shaders\\Cache\\" + Name + VertexProtocol->GetName() + ShaderPass->GetName() + ".uvs";
			TString PixelShaderFullName = "";
			PixelShaderFullName = PixelShaderFullName + "..\\..\\Shaders\\Cache\\" + Name + VertexProtocol->GetName() + ShaderPass->GetName() + ".ups";
			TString PassRuleVertexShaderFile = "";
			PassRuleVertexShaderFile = PassRuleVertexShaderFile + "..\\..\\Shaders\\" + ShaderPass->GetName() + "VertexShader.uvs";
			TString PassRulePixelShaderFile = "";
			PassRulePixelShaderFile = PassRulePixelShaderFile + "..\\..\\Shaders\\" + ShaderPass->GetName() + "PixelShader.ups";
			FILE* fp = NULL;
			FILE* cachefp = NULL;
			fopen_s(&cachefp, VertexShaderFullName.GetBuffer(), "w+");
			if (cachefp) {
				fopen_s(&fp, PassRuleVertexShaderFile.GetBuffer(), "r");
				if (fp) {
					ReplaceInclude(fp, cachefp, VertexProtocol->GetName(), Name);
					fclose(fp);
				}
				fclose(cachefp);
			}
			fopen_s(&cachefp, PixelShaderFullName.GetBuffer(), "w+");
			if (cachefp) {
				fopen_s(&fp, PassRulePixelShaderFile.GetBuffer(), "r");
				if (fp) {
					ReplaceInclude(fp, cachefp, VertexProtocol->GetName(), Name);
					fclose(fp);
				}
				fclose(cachefp);
			}
			VertexShaderFileNames.AddItem(VertexShaderFullName);
			PixelShaderFileNames.AddItem(PixelShaderFullName);
		}
	}	
	GDriver->CompileMaterial(this);
}

void RMaterial::ReplaceInclude(FILE* fp, FILE* dstfp, TString VertexProtocolName, TString MaterialName) {
	char syntax[256];
	while (!feof(fp)) {
		fgets(syntax, 256, fp);
		char* syntaxTmp;
		if (syntaxTmp = strstr(syntax, "#include")) {
			if (syntaxTmp = strstr(syntax, "\"")) {
				syntaxTmp += 1;
				char rest[256];
				sprintf_s(rest, "%s", syntaxTmp);
				syntaxTmp[0] = '\0';
				sprintf_s(syntax, 256, "%s..\\%s", syntax, rest);
			}
		}

		if (syntaxTmp = strstr(syntax, "VertexProtocol.ush")) {
			char rest[256];
			char replace[256];
			sprintf_s(rest, "%s", syntaxTmp);
			syntaxTmp[0] = '\0';
			sprintf_s(replace, 256, "%s%s%s", syntax, VertexProtocolName.GetBuffer(), rest);
			fprintf_s(dstfp, "%s", replace);
		} else if (syntaxTmp = strstr(syntax, "Material.ush")) {
			char rest[256];
			char replace[256];
			sprintf_s(rest, "%s", syntaxTmp);
			syntaxTmp[0] = '\0';
			sprintf_s(replace, 256, "%s%s%s", syntax, MaterialName.GetBuffer(), rest);
			fprintf_s(dstfp, "%s", replace);
		} else {
			fprintf_s(dstfp, "%s", syntax);
		}
	}
}

RShaderBase* RMaterial::FindShader(RVertexProtocol* VertexProtocol, RShaderPass* ShaderPass) {
	for(unsigned int i=0;i<RShaderPass::ShaderPasses.Size();++i) {
		if(ShaderPass == RShaderPass::ShaderPasses(i)) {
			for(unsigned int j=0;j<RVertexProtocol::Protocols.Size();++j) {
				if(VertexProtocol == RVertexProtocol::Protocols(j)) {
					return Shaders(i*RShaderPass::ShaderPasses.Size() + j);
				}
			}
		}
	}
	return 0;
}