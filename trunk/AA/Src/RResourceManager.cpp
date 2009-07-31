#include "StdAfx.h"
#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"

void LoadASEFile(char* fn)
{
	char line[1024];
	char string[1024];
	FILE *fp;
	fopen_s(&fp, fn, "rt");
	if(!fp)
		return;

	fgets(line, 1024, fp);
	sscanf_s(line,"%s",string, 1024);

	if(strcmp(string,"*3DSMAX_ASCIIEXPORT") != 0)
		return;

	while(!feof(fp))
	{
		fgets(line, 1024, fp);
		sscanf_s(line,"%s",string, 1024);

		if(!strcmp(string, "*COMMENT"))
			continue;

		if(!strcmp(string, "*SCENE"))
		{
			while(1)
			{
				fgets(line, 1024, fp);
				sscanf_s(line,"%s",string, 1024);
				if(!strcmp(string, "*SCENE_FIRSTFRAME"))
				{

				}

				if(!strcmp(string, "*SCENE_LASTFRAME"))
				{

				}

				if(!strcmp(string, "*SCENE_FRAMESPEED"))
				{

				}

				if(!strcmp(string, "*SCENE_TICKSPERFRAME"))
				{

				}

				if(!strcmp(string, "}"))
				{
					break;
				}
			}
		}

		if(!strcmp(string, "*MATERIAL_LIST"))
		{
			int nMatCount = 0;
			fgets(line, 1024, fp);
			sscanf_s(line,"%s%d", string, nMatCount);

			for(int i=0;i<nMatCount;++i)
			{
				while(1)
				{
					fgets(line, 1024, fp);
					sscanf_s(line,"%s",string, 1024);
					if(!strcmp(string, "*MATERIAL_NAME"))
					{

					}

					if(!strcmp(string, "*MATERIAL_CLASS"))
					{

					}

					if(!strcmp(string, "*MATERIAL_AMBIENT"))
					{

					}

					if(!strcmp(string, "*MATERIAL_DIFFUSE"))
					{

					}

					if(!strcmp(string, "*MATERIAL_SPECULAR"))
					{

					}

					if(!strcmp(string, "*MATERIAL_SHINE"))
					{

					}

					if(!strcmp(string, "*MATERIAL_SHINESTRENGTH"))
					{

					}

					if(!strcmp(string, "*MATERIAL_TRANSPARENCY"))
					{

					}

					if(!strcmp(string, "*MATERIAL_WIRESIZE"))
					{

					}

					if(!strcmp(string, "*NUMSUBMTLS"))
					{
						int nSubMatCount = 0;
						fgets(line, 1024, fp);
						sscanf_s(line,"%s%d", string, nSubMatCount);
						for(int j=0;j<nSubMatCount;++j)
						{
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);
								if(!strcmp(string, "*MATERIAL_NAME"))
								{

								}

								if(!strcmp(string, "*MATERIAL_CLASS"))
								{

								}

								if(!strcmp(string, "*MATERIAL_AMBIENT"))
								{

								}

								if(!strcmp(string, "*MATERIAL_DIFFUSE"))
								{

								}

								if(!strcmp(string, "*MATERIAL_SPECULAR"))
								{

								}

								if(!strcmp(string, "*MATERIAL_SHINE"))
								{

								}

								if(!strcmp(string, "*MATERIAL_SHINESTRENGTH"))
								{

								}

								if(!strcmp(string, "*MATERIAL_TRANSPARENCY"))
								{

								}

								if(!strcmp(string, "*MATERIAL_WIRESIZE"))
								{

								}

								if(!strcmp(string, "*MATERIAL_SHADING"))
								{

								}

								if(!strcmp(string, "*MATERIAL_XP_FALLOFF"))
								{

								}

								if(!strcmp(string, "*MATERIAL_SELFILLUM"))
								{

								}

								if(!strcmp(string, "*MATERIAL_FALLOFF"))
								{

								}

								if(!strcmp(string, "*MATERIAL_XP_TYPE"))
								{

								}

								if(!strcmp(string, "*MAP_DIFFUSE"))
								{
									while(1)
									{
										fgets(line, 1024, fp);
										sscanf_s(line,"%s",string, 1024);

										if(!strcmp(string, "*MAP_NAME"))
										{

										}

										if(!strcmp(string, "*MAP_CLASS"))
										{

										}

										if(!strcmp(string, "*MAP_SUBNO"))
										{

										}

										if(!strcmp(string, "*MAP_AMOUNT"))
										{

										}

										if(!strcmp(string, "*BITMAP"))
										{

										}

										if(!strcmp(string, "*MAP_TYPE"))
										{

										}

										if(!strcmp(string, "*UVW_U_OFFSET"))
										{

										}

										if(!strcmp(string, "*UVW_V_OFFSET"))
										{

										}

										if(!strcmp(string, "*UVW_U_TILING"))
										{

										}

										if(!strcmp(string, "*UVW_V_TILING"))
										{

										}

										if(!strcmp(string, "*UVW_ANGLE"))
										{

										}

										if(!strcmp(string, "*UVW_BLUR"))
										{

										}

										if(!strcmp(string, "*UVW_BLUR_OFFSET"))
										{

										}

										if(!strcmp(string, "*UVW_NOUSE_AMT"))
										{

										}

										if(!strcmp(string, "*UVW_NOISE_SIZE"))
										{

										}

										if(!strcmp(string, "*UVW_NOISE_LEVEL"))
										{

										}

										if(!strcmp(string, "*UVW_NOISE_PHASE"))
										{

										}

										if(!strcmp(string, "*BITMAP_FILTER "))
										{

										}

										if(!strcmp(string, "}"))
										{
											break;
										}
									}
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
						}
					}

					if(!strcmp(string, "}"))
					{
						break;
					}
				}
			}
		}

		if(!strcmp(string, "*GEOMOBJECT"))
		{
			while(1)
			{
				fgets(line, 1024, fp);
				sscanf_s(line,"%s",string, 1024);

				if(!strcmp(string, "*NODE_NAME"))
				{

				}

				if(!strcmp(string, "*NODE_TM"))
				{
					while(1)
					{
						fgets(line, 1024, fp);
						sscanf_s(line,"%s",string, 1024);

						if(!strcmp(string, "*MESH_FACE"))
						{

						}

						if(!strcmp(string, "}"))
						{
							break;
						}
					}
					continue;
				}

				if(!strcmp(string, "*MESH"))
				{
					int nTextureVerts = 0;
					int nTextureIndices = 0;
					int nASEVerts = 0;
					int nASEIndices = 0;
					TVector2 *pTextureVerts = 0;
					TIndex16 *pTextureIndices = 0;
					TVector3 *pASEVertices = 0;
					TIndex16 *pASEIndices = 0;
					while(1)
					{
						fgets(line, 1024, fp);
						sscanf_s(line,"%s",string, 1024);

						if(!strcmp(string, "*TIMEVALUE"))
						{

						}

						if(!strcmp(string, "*MESH_NUMVERTEX"))
						{
							sscanf_s(line,"%s%d", string, 1024, &nASEVerts);
							pASEVertices = new TVector3[nASEVerts];
						}

						if(!strcmp(string, "*MESH_NUMFACES"))
						{
							sscanf_s(line,"%s%d", string, 1024, &nASEIndices);
							pASEIndices = new TIndex16[nASEIndices];
						}

						if(!strcmp(string, "*MESH_VERTEX_LIST"))
						{
							int index;
							TVector3 Position;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*MESH_VERTEX"))
								{
									sscanf_s(line,"%s%d%f%f%f", string, 1024, &index, &Position.x, &Position.y, &Position.z);
									pASEVertices[index] = Position;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}	
						
						if(!strcmp(string, "*MESH_FACE_LIST"))
						{
							int index;
							TIndex16 IndexBuf;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*MESH_FACE"))
								{
									sscanf_s(line,"%s%d: A: %hd B: %hd C: %hd", string, 1024, &index, &IndexBuf._1, &IndexBuf._2, &IndexBuf._3);
									pASEIndices[index] = IndexBuf;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}

						if(!strcmp(string, "*MESH_NUMTVERTEX"))
						{
							sscanf_s(line,"%s%d", string, 1024, &nTextureVerts);
							pTextureVerts = new TVector2[nTextureVerts];
						}

						if(!strcmp(string, "*MESH_TVERTLIST"))
						{
							int index;
							TVector2 UV;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*MESH_TVERT"))
								{
									sscanf_s(line,"%s%d%f%f", string, 1024, &index, &UV.x, &UV.y);
									UV.y = 1.0f - UV.y;
									pTextureVerts[index] = UV;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}

						if(!strcmp(string, "*MESH_NUMTVFACES"))
						{
							sscanf_s(line,"%s%d", string, 1024, &nTextureIndices);
							pTextureIndices = new TIndex16[nTextureIndices];
						}

						if(!strcmp(string, "*MESH_TFACELIST"))
						{
							int index;
							TIndex16 TVertexIndex;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*MESH_TFACE"))
								{
									sscanf_s(line,"%s%d: A: %hd B: %hd C: %hd", string, 1024, &index, &TVertexIndex._1, &TVertexIndex._2, &TVertexIndex._3);
									pTextureIndices[index] = TVertexIndex;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}

						if(!strcmp(string, "}"))
						{
							RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
							RSystemMemoryVertexBufferTable::pVertexBuffer.AddItem(pVB);

							RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
							RSystemMemoryIndexBufferTable::pIndexBuffer.AddItem(pIB);

							pVB->Declaration = new VertexDeclaration[2];
							pVB->Declaration[0].Offset = 0;
							pVB->Declaration[0].Type = DECLTYPE_FLOAT3;	// Position
							pVB->Declaration[1].Offset = 12;
							pVB->Declaration[1].Type = DECLTYPE_FLOAT2;	// UV

							struct VD
							{
								TVector3 Pos;
								TVector2 UV;
							};

							pVB->nVertexStride = sizeof(VD);
							pVB->nVertices = nASEVerts;
							pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

							VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);
							for(int i=0;i<nASEVerts;++i)
							{
								Vertex[i].Pos = pASEVertices[i];
								Vertex[i].UV = TVector2(0.0f,0.0f);
							}
							/*
							pVB->nVertices = nASEIndices*3;
							pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

							VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);
							for(int i=0;i<nASEIndices;++i)
							{
								Vertex[i*3 + 0].Pos = pASEVertices[pASEIndices[i]._1];
								Vertex[i*3 + 0].UV = pTextureVerts[pTextureIndices[i]._1];
								Vertex[i*3 + 1].Pos = pASEVertices[pASEIndices[i]._2];
								Vertex[i*3 + 1].UV = pTextureVerts[pTextureIndices[i]._2];
								Vertex[i*3 + 2].Pos = pASEVertices[pASEIndices[i]._3];
								Vertex[i*3 + 2].UV = pTextureVerts[pTextureIndices[i]._3];
							}*/
							
							pIB->nIndices = nASEIndices;
							pIB->pIndices = new TIndex16[pIB->nIndices];
							for(int i=0;i<nASEIndices;++i)
							{
								pIB->pIndices[i] = pASEIndices[i];
							}
							break;
						}
					}
					delete[] pASEIndices;
					delete[] pASEVertices;
					delete[] pTextureIndices;
					delete[] pTextureVerts;
				}

				if(!strcmp(string, "*PROP_MOTIONBLUR"))
				{

				}

				if(!strcmp(string, "*PROP_CASTSHADOW"))
				{

				}

				if(!strcmp(string, "*PROP_RECVSHADOW"))
				{

				}

				if(!strcmp(string, "*MATERIAL_REF"))
				{

				}

				if(!strcmp(string, "}"))
				{
					break;
				}
			}
		}
	}
	
	fclose(fp);
}

RResourceManager::RResourceManager(void)
{
}

RResourceManager::~RResourceManager(void)
{
}

bool RResourceManager::LoadResources()
{
	//////////////////////////////////// Texture Loading
	RTextureBuffer *pTexBuffer = GDriver->CreateTextureBuffer();
	RTextureBufferTable::pTextureBuffer.AddItem(pTexBuffer);

	//////////////////////////////////// Shader Loading
	RDirectXShader *pShader = new RDirectXShader();
	sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	RShaderTable::pShaders.AddItem(pShader);

	pShader = new RDirectXShader();
	sprintf_s(pShader->m_FileName, 256, "RTShader.fx");
	RShaderTable::pShaders.AddItem(pShader);

	for(unsigned int i=0;i<RShaderTable::pShaders.Size();++i)
	{
		GDriver->CompileShaderFromFile(RShaderTable::pShaders(i));
	}

	//////////////////////////////////// Geometry Loading
	/////////////////////////////////////////////////////// Vertex Buffer Loading
	/////////////////////////////////////////////////////// Index Buffer Loading

	LoadASEFile("../Resources/beast.ase");
	return true;
}

bool RResourceManager::ReleaseAllResources()
{
	//////////////////////////////////// Geometry Releasing
	/////////////////////////////////////////////////////// Vertex Buffer Releasing
	for(unsigned int i=0;i<RSystemMemoryVertexBufferTable::pVertexBuffer.Size();++i)
	{
		delete RSystemMemoryVertexBufferTable::pVertexBuffer(i);
	}
	RSystemMemoryVertexBufferTable::pVertexBuffer.Clear(true);

	/////////////////////////////////////////////////////// Index Buffer Releasing
	for(unsigned int i=0;i<RSystemMemoryIndexBufferTable::pIndexBuffer.Size();++i)
	{
		delete RSystemMemoryIndexBufferTable::pIndexBuffer(i);
	}
	RSystemMemoryIndexBufferTable::pIndexBuffer.Clear(true);

	//////////////////////////////////// Shader Releasing
	for(unsigned int i=0;i<RShaderTable::pShaders.Size();++i)
	{
		delete RShaderTable::pShaders(i);
	}
	RShaderTable::pShaders.Clear(true);

	//////////////////////////////////// Texture Releasing
	for(unsigned int i=0;i<RTextureBufferTable::pTextureBuffer.Size();++i)
	{
		delete RTextureBufferTable::pTextureBuffer(i);		
	}
	RTextureBufferTable::pTextureBuffer.Clear(true);

	return true;
}