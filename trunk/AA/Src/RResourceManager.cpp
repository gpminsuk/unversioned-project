#include "StdAfx.h"

#include "CBoxPrimitive.h"
#include "CCylinderPrimitive.h"

#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"

void LoadASEFile(char* fn)
{
	char line[1024];
	char string[1024];
	TString NodeName;
	TString ParentNodeName;
	FILE *fp;
	fopen_s(&fp, fn, "rt");
	if(!fp)
		return;

	fgets(line, 1024, fp);
	sscanf_s(line,"%s",string, 1024);

	if(strcmp(string,"*3DSMAX_ASCIIEXPORT") != 0)
		return;

	RAnimationSequence *AnimationSequence = new RAnimationSequence();
	RBoneHierarchy* BoneHierarchy = new RBoneHierarchy();
	RSkeletalMesh*	Model = new RSkeletalMesh();

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
					sscanf_s(line,"%s%d", string, 1024, &AnimationSequence->StartFrame);
					continue;
				}

				if(!strcmp(string, "*SCENE_LASTFRAME"))
				{
					sscanf_s(line,"%s%d", string, 1024, &AnimationSequence->EndFrame);
					continue;
				}

				if(!strcmp(string, "*SCENE_FRAMESPEED"))
				{
					sscanf_s(line,"%s%d", string, 1024, &AnimationSequence->FrameSpeed);
					continue;
				}

				if(!strcmp(string, "*SCENE_TICKSPERFRAME"))
				{
					sscanf_s(line,"%s%d", string, 1024, &AnimationSequence->TickPerFrame);
					continue;
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
					char *Spos = strchr(line, '"');
					char *Epos = strchr(Spos+1, '"');
					Epos -= 1;
					memcpy_s(NodeName.Str, 1024, Spos+1, Epos-Spos);
					NodeName.Str[Epos-Spos] = '\0';
					continue;
				}

				if(!strcmp(string, "*NODE_PARENT"))
				{
					char *Spos = strchr(line, '"');
					char *Epos = strchr(Spos+1, '"');
					Epos -= 1;
					memcpy_s(ParentNodeName.Str, 1024, Spos+1, Epos-Spos);
					ParentNodeName.Str[Epos-Spos] = '\0';
					continue;
				}

				if(!strcmp(string, "*NODE_TM"))
				{
					RBoneHierarchy::RBone *Bone = new RBoneHierarchy::RBone();
					TVector3 Inherit_Pos, Inherit_Rot;
					while(1)
					{
						fgets(line, 1024, fp);
						sscanf_s(line,"%s",string, 1024);

						if(!strcmp(string, "*NODE_NAME"))
						{
							char *Spos = strchr(line, '"');
							char *Epos = strchr(Spos+1, '"');
							Epos -= 1;
							memcpy_s(Bone->BoneName.Str, 1024, Spos+1, Epos-Spos);
							Bone->BoneName.Str[Epos-Spos] = '\0';
							continue;
						}
						
						if(!strcmp(string, "*TM_ROW0"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Bone->BoneTM._11, &Bone->BoneTM._12, &Bone->BoneTM._13);
							continue;
						}

						if(!strcmp(string, "*TM_ROW1"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Bone->BoneTM._21, &Bone->BoneTM._22, &Bone->BoneTM._23);
							continue;
						}

						if(!strcmp(string, "*TM_ROW2"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Bone->BoneTM._31, &Bone->BoneTM._32, &Bone->BoneTM._33);
							continue;
						}

						if(!strcmp(string, "*TM_ROW3"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Bone->BoneTM._41, &Bone->BoneTM._42, &Bone->BoneTM._43);
							continue;
						}

						if(!strcmp(string, "*INHERIT_POS"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Inherit_Pos.x, &Inherit_Pos.y, &Inherit_Pos.z);
							continue;
						}

						if(!strcmp(string, "*INHERIT_ROT"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &Inherit_Rot.x, &Inherit_Rot.y, &Inherit_Rot.z);
							continue;
						}

						if(!strcmp(string, "}"))
						{
							break;
						}
					}
					if(!strcmp(ParentNodeName.Str, ""))
					{
						BoneHierarchy->RootBone = Bone;
					}
					else if(BoneHierarchy->RootBone)
					{
						BoneHierarchy->AddBone(Bone, ParentNodeName);
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
					TVector3 *pASENormals = 0;
					TIndex16 *pASEIndices = 0;
					RSkeletalSubMesh *Mesh = new RSkeletalSubMesh();
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
							pASENormals = new TVector3[nASEIndices];
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
									sscanf_s(line,"%s%d%hd%hd%hd", string, 1024, &index, &TVertexIndex._1, &TVertexIndex._2, &TVertexIndex._3);
									pTextureIndices[index] = TVertexIndex;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}

						if(!strcmp(string, "*MESH_NORMALS"))
						{
							int index;
							TVector3 TNormal;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);
							
								if(!strcmp(string, "*MESH_FACENORMAL"))
								{
									sscanf_s(line,"%s%d%f%f%f", string, 1024, &index, &TNormal.x, &TNormal.y, &TNormal.z);
									pASENormals[index] = TNormal;
									continue;
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
							RSystemMemoryVertexBufferTable::VertexBuffers.AddItem(pVB);

							RSystemMemoryIndexBuffer *pIB = new RSystemMemoryIndexBuffer();
							RSystemMemoryIndexBufferTable::IndexBuffers.AddItem(pIB);

							pVB->Declaration = new VertexDeclaration[3];
							pVB->Declaration[0].Offset = 0;
							pVB->Declaration[0].Type = DECLTYPE_FLOAT3;	// Position
							pVB->Declaration[1].Offset = 12;
							pVB->Declaration[1].Type = DECLTYPE_FLOAT2;	// UV

							pVB->nVertexStride = sizeof(RSkeletalSubMesh::VD);
							pVB->nVertices = nASEIndices*3;
							pVB->pVertices = new char[pVB->nVertexStride*pVB->nVertices];

							RSkeletalSubMesh::VD *Vertex = reinterpret_cast<RSkeletalSubMesh::VD*>(pVB->pVertices);
							for(int i=0;i<nASEIndices;++i)
							{
								Vertex[i*3 + 0].Pos = pASEVertices[pASEIndices[i]._1];
								Vertex[i*3 + 0].Normal = pASENormals[pASEIndices[i]._1];
								if(pTextureVerts && pTextureIndices) Vertex[i*3 + 0].UV = pTextureVerts[pTextureIndices[i]._1];
								else	Vertex[i*3 + 0].UV = TVector2(0.0f,0.0f);

								Vertex[i*3 + 1].Pos = pASEVertices[pASEIndices[i]._2];
								Vertex[i*3 + 1].Normal = pASENormals[pASEIndices[i]._2];
								if(pTextureVerts && pTextureIndices) Vertex[i*3 + 1].UV = pTextureVerts[pTextureIndices[i]._2];
								else	Vertex[i*3 + 1].UV = TVector2(0.0f,0.0f);

								Vertex[i*3 + 2].Pos = pASEVertices[pASEIndices[i]._3];
								Vertex[i*3 + 2].Normal = pASENormals[pASEIndices[i]._3];
								if(pTextureVerts && pTextureIndices) Vertex[i*3 + 2].UV = pTextureVerts[pTextureIndices[i]._3];
								else	Vertex[i*3 + 2].UV = TVector2(0.0f,0.0f);
							}
							
							pIB->nIndices = nASEIndices;
							pIB->pIndices = new TIndex16[pIB->nIndices];
							for(int i=0;i<nASEIndices;++i)
							{
								pIB->pIndices[i] = TIndex16(i*3 + 0, i*3 + 1, i*3 + 2);
							}
							Mesh->pIB = pIB;
							Mesh->pVB = pVB;
							strcpy_s(Mesh->BoneName.Str, 1024, NodeName.Str);
							break;
						}
					}
					delete[] pASEIndices;
					delete[] pASENormals;
					delete[] pASEVertices;
					delete[] pTextureIndices;
					delete[] pTextureVerts;
					Model->SkeletalSubMeshes.AddItem(Mesh);
					continue;
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

				if(!strcmp(string, "*TM_ANIMATION"))
				{
					RAnimationBoneSequence* Seq = new RAnimationBoneSequence();
					while(1)
					{
						fgets(line, 1024, fp);
						sscanf_s(line,"%s",string, 1024);

						if(!strcmp(string, "*NODE_NAME"))
						{
							char *Spos = strchr(line, '"');
							char *Epos = strchr(Spos+1, '"');
							Epos -= 1;
							memcpy_s(Seq->BoneName.Str, 1024, Spos+1, Epos-Spos);
							Seq->BoneName.Str[Epos-Spos] = '\0';
							continue;
						}

						if(!strcmp(string, "*CONTROL_POS_TRACK"))
						{
							float Time;
							TVector3 Pos;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*CONTROL_POS_SAMPLE"))
								{
									sscanf_s(line,"%s%f%f%f%f", string, 1024, &Time, &Pos.x, &Pos.y, &Pos.z);
									RAnimationBoneSequence::POSKEY Key;
									Key.Pos = Pos;
									Key.Time = Time;
									Seq->PosKeys.AddItem(Key);
									continue;
								}

								if(!strcmp(string, "}"))
								{
									break;
								}
							}
							continue;
						}

						if(!strcmp(string, "*CONTROL_ROT_TRACK"))
						{
							float Time;
							TQuaternion Rot;
							while(1)
							{
								fgets(line, 1024, fp);
								sscanf_s(line,"%s",string, 1024);

								if(!strcmp(string, "*CONTROL_ROT_SAMPLE"))
								{
									sscanf_s(line,"%s%f%f%f%f%f", string, 1024, &Time, &Rot.v.x, &Rot.v.y, &Rot.v.z, &Rot.w);
									RAnimationBoneSequence::ROTKEY Key;
									Rot.Normalize();
									Key.Rot = Rot;
									Key.Time = Time;
									Seq->RotKeys.AddItem(Key);
									continue;
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
							break;
						}
					}
					AnimationSequence->AnimationBoneSequences.AddItem(Seq);	
					continue;
				}

				if(!strcmp(string, "}"))
				{
					break;
				}
			}
		}
	}

	RAnimationSequenceTable::Sequences.AddItem(AnimationSequence);
	RBoneHierarchyTable::BoneHierarchies.AddItem(BoneHierarchy);
	RSkeletalMeshTable::SkeletalMeshes.AddItem(Model);

	fclose(fp);
}

RResourceManager::RResourceManager(void)
{
}

RResourceManager::~RResourceManager(void)
{
}
#include <Windows.h>
bool RResourceManager::LoadResources()
{
	//////////////////////////////////// Texture Loading
	RTextureBuffer *pTexBuffer = GDriver->CreateTextureBuffer();
	RTextureBufferTable::TextureBuffers.AddItem(pTexBuffer);
	
	pTexBuffer = GDriver->CreateFontBuffer(128,32);
	RTextureBufferTable::TextureBuffers.AddItem(pTexBuffer);

	//////////////////////////////////// Shader Loading
	RDirectXShader *pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	wsprintf(pShader->m_FileName, TEXT("Shader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "RTShader.fx");
	wsprintf(pShader->m_FileName, TEXT("RTShader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "LineShader.fx");
	wsprintf(pShader->m_FileName, TEXT("LineShader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	wsprintf(pShader->m_FileName, TEXT("FontShader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	wsprintf(pShader->m_FileName, TEXT("ParticleShader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	for(unsigned int i=0;i<RShaderTable::Shaders.Size();++i)
	{
		GDriver->CompileShaderFromFile(RShaderTable::Shaders(i));
	}

	//////////////////////////////////// Geometry Loading
	/////////////////////////////////////////////////////// Basic Geometry Creating
	extern TBoxPrimitive* BoxPrimitive;
	BoxPrimitive = new TBoxPrimitive();
	extern TCylinderPrimitive* GCylinderPrimitive;
	GCylinderPrimitive = new TCylinderPrimitive(RenderType_Opaque);
	extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
	GCylinderPrimitiveWireFrame= new TCylinderPrimitive(RenderType_Line);
	/////////////////////////////////////////////////////// Vertex Buffer Loading
	/////////////////////////////////////////////////////// Index Buffer Loading

	LoadASEFile("../Resources/woman_01_all.ase");
	return true;
}

bool RResourceManager::ReleaseAllResources()
{
	//////////////////////////////////// Geometry Releasing
	/////////////////////////////////////////////////////// Basic Geometry Releasing
	extern TBoxPrimitive* BoxPrimitive;
	delete BoxPrimitive;
	extern TCylinderPrimitive* GCylinderPrimitive;
	delete GCylinderPrimitive;
	extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
	delete GCylinderPrimitiveWireFrame;
	/////////////////////////////////////////////////////// Vertex Buffer Releasing
	for(unsigned int i=0;i<RSystemMemoryVertexBufferTable::VertexBuffers.Size();++i)
	{
		delete RSystemMemoryVertexBufferTable::VertexBuffers(i);
	}
	RSystemMemoryVertexBufferTable::VertexBuffers.Clear(true);

	/////////////////////////////////////////////////////// Index Buffer Releasing
	for(unsigned int i=0;i<RSystemMemoryIndexBufferTable::IndexBuffers.Size();++i)
	{
		delete RSystemMemoryIndexBufferTable::IndexBuffers(i);
	}
	RSystemMemoryIndexBufferTable::IndexBuffers.Clear(true);

	//////////////////////////////////// Shader Releasing
	for(unsigned int i=0;i<RShaderTable::Shaders.Size();++i)
	{
		delete RShaderTable::Shaders(i);
	}
	RShaderTable::Shaders.Clear(true);

	//////////////////////////////////// Texture Releasing
	for(unsigned int i=0;i<RTextureBufferTable::TextureBuffers.Size();++i)
	{
		delete RTextureBufferTable::TextureBuffers(i);		
	}
	RTextureBufferTable::TextureBuffers.Clear(true);

	//////////////////////////////////// Animation Releasing
	for(unsigned int i=0;i<RAnimationSequenceTable::Sequences.Size();++i)
	{
		delete RAnimationSequenceTable::Sequences(i);
	}
	RAnimationSequenceTable::Sequences.Clear(true);

	//////////////////////////////////// Model Releasing
	for(unsigned int i=0;i<RSkeletalMeshTable::SkeletalMeshes.Size();++i)
	{
		delete RSkeletalMeshTable::SkeletalMeshes(i);
	}
	RSkeletalMeshTable::SkeletalMeshes.Clear(true);

	//////////////////////////////////// Bone Releasing
	for(unsigned int i=0;i<RBoneHierarchyTable::BoneHierarchies.Size();++i)
	{
		delete RBoneHierarchyTable::BoneHierarchies(i);
	}
	RBoneHierarchyTable::BoneHierarchies.Clear(true);

	return true;
}