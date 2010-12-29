#include "StdAfx.h"

#include "CBoxPrimitive.h"
#include "CCylinderPrimitive.h"

#include "RResourceManager.h"

#include "RDXResource.h"

#include "BDriver.h"
#include "BSoundDriver.h"

void LoadASEFile(const char* fn)
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
								
								if(!strcmp(string, "*MAP_DIFFUSE"))
								{
									while(1)
									{
										fgets(line, 1024, fp);
										sscanf_s(line,"%s",string, 1024);

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
			RBoneHierarchy::RBone *Bone = NULL;
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
					Bone = new RBoneHierarchy::RBone();
					TMatrix BoneTM;
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
							sscanf_s(line, "%s%f%f%f",string, 1024, &BoneTM._11, &BoneTM._12, &BoneTM._13);
							continue;
						}

						if(!strcmp(string, "*TM_ROW1"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &BoneTM._21, &BoneTM._22, &BoneTM._23);
							continue;
						}

						if(!strcmp(string, "*TM_ROW2"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &BoneTM._31, &BoneTM._32, &BoneTM._33);
							continue;
						}

						if(!strcmp(string, "*TM_ROW3"))
						{
							sscanf_s(line, "%s%f%f%f",string, 1024, &BoneTM._41, &BoneTM._42, &BoneTM._43);
							continue;
						}
						
						if(!strcmp(string, "}"))
						{
							break;
						}
					}
					if(!strcmp(ParentNodeName.Str, ""))
					{
						BoneHierarchy->RootBone.AddItem(Bone);
					}
					else if(BoneHierarchy->RootBone.Size())
					{
						BoneHierarchy->AddBone(Bone, ParentNodeName);
					}
					RBoneHierarchy::RBone* ItBone = Bone->Parent;
					TArray<RBoneHierarchy::RBone*> BoneStack;
					while(ItBone)
					{
						BoneStack.AddItem(ItBone);
						ItBone = ItBone->Parent;
					}
					for(int i=(int)BoneStack.Size()-1;i>=0;--i)
					{
						BoneTM = BoneTM * BoneStack(i)->InvTM;
					}
					Bone->TM = BoneTM;
					Bone->InvTM = BoneTM.Inverse();
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
							TMatrix InvWorld = BoneHierarchy->GetBoneMatrix(Bone);
							InvWorld.Inverse();
							for(int i=0;i<nASEIndices;++i)
							{
								Vertex[i*3 + 0].Pos = InvWorld.TransformVector3(pASEVertices[pASEIndices[i]._1]);
								//Vertex[i*3 + 0].Pos = pASEVertices[pASEIndices[i]._1];
								Vertex[i*3 + 0].Normal = pASENormals[pASEIndices[i]._1];
								if(pTextureVerts && pTextureIndices) Vertex[i*3 + 0].UV = pTextureVerts[pTextureIndices[i]._1];
								else	Vertex[i*3 + 0].UV = TVector2(0.0f,0.0f);

								Vertex[i*3 + 1].Pos = InvWorld.TransformVector3(pASEVertices[pASEIndices[i]._2]);
								//Vertex[i*3 + 1].Pos = pASEVertices[pASEIndices[i]._2];
								Vertex[i*3 + 1].Normal = pASENormals[pASEIndices[i]._2];
								if(pTextureVerts && pTextureIndices) Vertex[i*3 + 1].UV = pTextureVerts[pTextureIndices[i]._2];
								else	Vertex[i*3 + 1].UV = TVector2(0.0f,0.0f);

								Vertex[i*3 + 2].Pos = InvWorld.TransformVector3(pASEVertices[pASEIndices[i]._3]);
								//Vertex[i*3 + 2 ].Pos = pASEVertices[pASEIndices[i]._3];
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

				if(!strcmp(string, "*TM_ANIMATION"))
				{
					RAnimationBoneSequence* Seq = new RAnimationBoneSequence(AnimationSequence);
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
									TVector3 Axis;
									float Angle;
									sscanf_s(line,"%s%f%f%f%f%f", string, 1024, &Time, &Axis.x, &Axis.y, &Axis.z, &Angle);
									RAnimationBoneSequence::ROTKEY Key;
									Key.Rot = TQuaternion(Axis, Angle);
									Key.Time = Time;
									if(Seq->RotKeys.Size() == 0)
									{
										Key.Rot = TQuaternion();
										Seq->RotKeys.AddItem(Key);
									}
									else
									{
										//Key.Rot = Seq->RotKeys((int)(Seq->RotKeys.Size()-1)).Rot * Key.Rot;
										Seq->RotKeys.AddItem(Key);
									}
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
	TString Filename;
	//////////////////////////////////// Sound Loading

	//////////////////////////////////// Texture Loading
	
	RTextureBuffer *pTexBuffer = GDriver->CreateFontBuffer(128,32);
	RTextureBufferTable::TextureBuffers.AddItem(pTexBuffer);

	//////////////////////////////////// Shader Loading
	RDirectXShader *pShader = new RDirectXShader();
	//sprintf_s(pShader->m_FileName, 256, "Shader.fx");
	wsprintf(pShader->m_FileName, TEXT("Shader.fx"));
	RShaderTable::Shaders.AddItem(pShader);

	for(unsigned int i=0;i<RShaderTable::Shaders.Size();i+=2)
	{
		GDriver->CompileShaderFromFile(RShaderTable::Shaders(i));
	}

	//////////////////////////////////// Geometry Loading
	/////////////////////////////////////////////////////// Basic Geometry Creating
	extern TBoxPrimitive* GBoxPrimitiveOutside;
	GBoxPrimitiveOutside = new TBoxPrimitive(RenderType_Opaque, SideType_Outside);
	extern TBoxPrimitive* GBoxPrimitiveInside;
	GBoxPrimitiveInside = new TBoxPrimitive(RenderType_Opaque, SideType_Inside);
	extern TCylinderPrimitive* GCylinderPrimitive;
	GCylinderPrimitive = new TCylinderPrimitive(RenderType_Opaque);
	extern TCylinderPrimitive* GCylinderPrimitiveWireFrame;
	GCylinderPrimitiveWireFrame= new TCylinderPrimitive(RenderType_Line);
	/////////////////////////////////////////////////////// Vertex Buffer Loading
	/////////////////////////////////////////////////////// Index Buffer Loading
	return true;
}

bool RResourceManager::ReleaseAllResources()
{
	//////////////////////////////////// Geometry Releasing
	/////////////////////////////////////////////////////// Basic Geometry Releasing
	extern TBoxPrimitive* GBoxPrimitiveOutside;
	delete GBoxPrimitiveOutside;
	extern TBoxPrimitive* GBoxPrimitiveInside;
	delete GBoxPrimitiveInside;
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