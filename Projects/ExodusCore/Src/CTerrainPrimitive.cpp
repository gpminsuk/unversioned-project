#include "StdAfx.h"

#include "RResource.h"

#include "CTerrainPrimitive.h"
#include "BRenderingBatch.h"

TTerrainPrimitive::TTerrainPrimitive()
    :
    pLODIndices(0),
    nLODIndices(0),
    QuadTree(0),
    CellStepSize(1),
    bShouldTessellate(false) {

}

TTerrainPrimitive::~TTerrainPrimitive() {
    if (QuadTree) {
        for (unsigned int i = 0; i < QuadTreeSizeX; ++i) {
            for (unsigned int j = 0; j < QuadTreeSizeY; ++j) {
                QuadTree[i][j].Root->DestroyNode();
                delete QuadTree[i][j].Root;
            }
            delete[] QuadTree[i];
        }
        delete[] QuadTree;
    }
    delete pLODIndices;
}

unsigned int CTerrainPrimitive::GetNumIndices() {
    unsigned int Ret = 0;
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        TTerrainPrimitive* Prim =
            dynamic_cast<TTerrainPrimitive*>(Draws(i));
        if (Prim) {
            Ret += Prim->nLODIndices;
        }
    }
    return Ret;
}

unsigned int CTerrainPrimitive::FillDynamicVertexBuffer(char** pData) {
    unsigned int Ret = 0;
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        TTerrainPrimitive* Prim =
            dynamic_cast<TTerrainPrimitive*>(Draws(i));
        if (Prim) {
            memcpy((*pData), Prim->pBuffer->m_pVB->pVertices,
                   Prim->pBuffer->m_pVB->nVertices
                   * Prim->pBuffer->m_pVB->Protocol->Decl->GetStride());
            for (unsigned int k = 0; k < Prim->pBuffer->m_pVB->nVertices; ++k) {
                *((TVector3*) &((*pData)[k * Prim->pBuffer->m_pVB->Protocol->Decl->GetStride()])) =
                    TM.TransformVector3(
                        *((TVector3*) &((*pData)[k
                                                 * Prim->pBuffer->m_pVB->Protocol->Decl->GetStride()])));
            }
            *pData += Prim->pBuffer->m_pVB->nVertices
                      * Prim->pBuffer->m_pVB->Protocol->Decl->GetStride();
            Ret += Prim->pBuffer->m_pVB->nVertices;
        }
    }
    return Ret;
}

void CTerrainPrimitive::IndexTessellate() {
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        TTerrainPrimitive* Prim =
            dynamic_cast<TTerrainPrimitive*>(Draws(i));
        if (Prim) {
            if (Prim->bShouldTessellate) {
                Tessellate(LODOrigin);
                Prim->bShouldTessellate = false;
            }
        }
    }
}

unsigned int CTerrainPrimitive::FillDynamicIndexBuffer(TIndex16** pData, unsigned short* BaseIndex) {
    unsigned int Ret = 0;
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        TTerrainPrimitive* Prim =
            dynamic_cast<TTerrainPrimitive*>(Draws(i));
        if (Prim) {
            if (!Prim->pLODIndices)
                continue;
            for (unsigned int k = 0; k < GetNumIndices(); ++k) {
                TIndex16 tmpIndex;
                tmpIndex._1 = Prim->pLODIndices[k]._1 + *BaseIndex;
                tmpIndex._2 = Prim->pLODIndices[k]._2 + *BaseIndex;
                tmpIndex._3 = Prim->pLODIndices[k]._3 + *BaseIndex;
                (*pData)[k] = tmpIndex;
            }
            *BaseIndex += Prim->pBuffer->m_pVB->nVertices;
            *pData += GetNumIndices();
            Ret += Prim->pBuffer->m_pVB->nVertices;
        }
    }
    return Ret;
}

bool TTerrainPrimitive::Tessellate(TVector3 Origin) {
    delete[] pLODIndices;
    nLODIndices = 0;
    for (unsigned int i = 0; i < QuadTreeSizeX; ++i)
        for (unsigned int j = 0; j < QuadTreeSizeY; ++j)
            nLODIndices += QuadTree[i][j].CheckLODLeafNode(Origin);
    pLODIndices = new TIndex16[nLODIndices];
    TIndex16* TempLODIndices = pLODIndices;
    for (unsigned int i = 0; i < QuadTreeSizeX; ++i)
        for (unsigned int j = 0; j < QuadTreeSizeY; ++j)
            QuadTree[i][j].FillLODIndexBuffer(&TempLODIndices);
    return true;
}

void TTerrainPrimitive::CreateTerrainQuadTree(float** HeightValue, unsigned int CellSizeX, unsigned int CellSizeY, unsigned int MaxTessellationLevel) {
    QuadTreeSizeX = CellSizeX / MaxTessellationLevel;
    QuadTreeSizeY = CellSizeY / MaxTessellationLevel;

    QuadTree = new TTerrainQuadTree*[QuadTreeSizeX];
    for (unsigned int i = 0; i < QuadTreeSizeX; ++i) {
        QuadTree[i] = new TTerrainQuadTree[QuadTreeSizeY];
        for (unsigned int j = 0; j < QuadTreeSizeY; ++j) {
            QuadTree[i][j].Root = new TTerrainQuadTreeNode();
            QuadTree[i][j].Primitive = this;
            QuadTree[i][j].TreeCoord.x = i;
            QuadTree[i][j].TreeCoord.y = j;
            QuadTree[i][j].Root->Indices[0] = MaxTessellationLevel * i * 4
                                              + MaxTessellationLevel * j * CellSizeX * 4;
            QuadTree[i][j].Root->Indices[1] = MaxTessellationLevel * i * 4
                                              + MaxTessellationLevel * j * CellSizeX * 4
                                              + MaxTessellationLevel * 4 - 3;
            QuadTree[i][j].Root->Indices[2] = MaxTessellationLevel * i * 4
                                              + MaxTessellationLevel * (j + 1) * CellSizeX * 4
                                              - CellSizeX * 4 + 2;
            QuadTree[i][j].Root->Indices[3] = MaxTessellationLevel * i * 4
                                              + MaxTessellationLevel * (j + 1) * CellSizeX * 4
                                              - CellSizeX * 4 + MaxTessellationLevel * 4 - 1;

            QuadTree[i][j].Root->Origin.x =
                (float) (MaxTessellationLevel * CellStepSize * i
                         + MaxTessellationLevel * CellStepSize / 2);
            QuadTree[i][j].Root->Origin.z =
                (float) (MaxTessellationLevel * CellStepSize * j
                         + MaxTessellationLevel * CellStepSize / 2);
            float HeightSum = 0;
            for (unsigned int hi = 0; hi < MaxTessellationLevel; ++hi) {
                for (unsigned int hj = 0; hj < MaxTessellationLevel; ++hj) {
                    HeightSum +=
                        HeightValue[MaxTessellationLevel * i + hi][MaxTessellationLevel
                                * j + hj];
                }
            }
            HeightSum /= (MaxTessellationLevel * MaxTessellationLevel);
            QuadTree[i][j].Root->Origin.y = HeightSum;

            QuadTree[i][j].Root->CreateTerrainQuadTreeLeaves(HeightValue,
                    TIntPoint(i * 16, j * 16), NULL, &QuadTree[i][j], 4,
                    CellSizeX);
        }
    }
}

CTerrainPrimitive::CTerrainPrimitive(void)
    :
    NumCellX(10),
    NumCellY(10),
    NumPatchX(1),
    NumPatchY(1),
    MaxTessellationLevel(16) {
    RenderType = RenderType_Opaque;
}

CTerrainPrimitive::~CTerrainPrimitive(void) {
    DestroyTerrainPrimitive();
}

void CTerrainPrimitive::CreateDraws() {

}

RMaterial* CTerrainPrimitive::GetMaterial() {
	return RMaterialTable::Materials(0);
}

bool CTerrainPrimitive::CreateTerrainPrimitive(unsigned int NumCellX, unsigned int NumCellY, unsigned int NumPatchX, unsigned int NumPatchY) {
    DestroyTerrainPrimitive();
    for (unsigned int nPatchX = 0; nPatchX < NumPatchX; ++nPatchX) {
        for (unsigned int nPatchY = 0; nPatchY < NumPatchY; ++nPatchY) {
            TTerrainPrimitive* Primitive = new TTerrainPrimitive();
            Draws.AddItem(Primitive);
            Primitive->pBuffer = new RStaticPrimitiveBuffer();

            RSystemMemoryVertexBuffer *pVB = new RSystemMemoryVertexBuffer();
            Primitive->pBuffer->m_pVB = pVB;

            struct VD {
                TVector3 Pos;
                TVector3 Normal;
                TVector2 UV;
            };

            unsigned int SizeX = (NumCellX / NumPatchX);
            unsigned int SizeY = (NumCellY / NumPatchY);

            Primitive->CellHeight = SizeY;
            Primitive->CellWidth = SizeX;

            pVB->nVertices = SizeX * SizeY * 4;
            pVB->pVertices = new char[pVB->Protocol->Decl->GetStride() * pVB->nVertices];

            VD *Vertex = reinterpret_cast<VD*>(pVB->pVertices);
            float **HeightValue;
            HeightValue = new float*[SizeX];
            for (unsigned int i = 0; i < SizeX; ++i) {
                HeightValue[i] = new float[SizeY];
                for (unsigned int j = 0; j < SizeY; ++j) {
                    HeightValue[i][j] = 0.0f;
                }
            }
            for (unsigned int i = 0; i < SizeY; ++i) {
                for (unsigned int j = 0; j < SizeX; ++j) {
                    Vertex[(i * SizeX + j) * 4 + 0].Pos = TVector3((float) j,
                                                          0.0f, (float) i);
                    Vertex[(i * SizeX + j) * 4 + 0].UV = TVector2(0.0f, 0.0f);
                    Vertex[(i * SizeX + j) * 4 + 0].Normal = TVector3(0.0f,
                            1.0f, 0.0f);

                    Vertex[(i * SizeX + j) * 4 + 1].Pos = TVector3(
                            (float) j + 1, 0.0f, (float) i);
                    Vertex[(i * SizeX + j) * 4 + 1].UV = TVector2(0.0f, 0.0f);
                    Vertex[(i * SizeX + j) * 4 + 1].Normal = TVector3(0.0f,
                            1.0f, 0.0f);

                    Vertex[(i * SizeX + j) * 4 + 2].Pos = TVector3((float) j,
                                                          0.0f, (float) i + 1);
                    Vertex[(i * SizeX + j) * 4 + 2].UV = TVector2(0.0f, 0.0f);
                    Vertex[(i * SizeX + j) * 4 + 2].Normal = TVector3(0.0f,
                            1.0f, 0.0f);

                    Vertex[(i * SizeX + j) * 4 + 3].Pos = TVector3(
                            (float) j + 1, 0.0f, (float) i + 1);
                    Vertex[(i * SizeX + j) * 4 + 3].UV = TVector2(0.0f, 0.0f);
                    Vertex[(i * SizeX + j) * 4 + 3].Normal = TVector3(0.0f,
                            1.0f, 0.0f);
                }
            }

            Primitive->nLODIndices = SizeX * SizeY * 2;
            Primitive->pLODIndices = new TIndex16[Primitive->nLODIndices];

            for (unsigned int i = 0; i < SizeX; ++i) {
                for (unsigned int j = 0; j < SizeY; ++j) {
                    Primitive->pLODIndices[(i * SizeX + j) * 2 + 0] = TIndex16(
                                (i * SizeX + j) * 4 + 0, (i * SizeX + j) * 4 + 2,
                                (i * SizeX + j) * 4 + 3);
                    Primitive->pLODIndices[(i * SizeX + j) * 2 + 1] = TIndex16(
                                (i * SizeX + j) * 4 + 0, (i * SizeX + j) * 4 + 3,
                                (i * SizeX + j) * 4 + 1);
                }
            }

            Primitive->CreateTerrainQuadTree(HeightValue, SizeX, SizeY,
                                             MaxTessellationLevel);

            for (unsigned int i = 0; i < SizeX; ++i) {
                delete[] HeightValue[i];
            }
            delete[] HeightValue;
        }
    }
    return true;
}

bool CTerrainPrimitive::DestroyTerrainPrimitive() {
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        delete Draws(i);
    }
    Draws.Clear();

    return true;
}

void CTerrainPrimitive::UpdateTerrainPrimitive(TVector3 _LODOrigin) {
    if (LODOrigin != _LODOrigin) {
        LODOrigin = _LODOrigin;
        for (unsigned int i = 0; i < Draws.Size(); ++i) {
            TTerrainPrimitive *Prim =
                dynamic_cast<TTerrainPrimitive*>(Draws(i));
            if (Prim) {
                Prim->bShouldTessellate = true;
                Prim->LODOrigin = _LODOrigin;
            }
        }
    }
}

///////////////////////////////////////////////////// Render Thread Function //////////////////////////////////////////////////////

bool CTerrainPrimitive::Tessellate(TVector3 Origin) {
    for (unsigned int i = 0; i < Draws.Size(); ++i) {
        TTerrainPrimitive *Prim =
            dynamic_cast<TTerrainPrimitive*>(Draws(i));
        if (Prim)
            Prim->Tessellate(Origin);
    }
    return true;
}

//////////////////////////////////////////////////// Terrain QuadTree /////////////////////////////////////////////////////////////

TTerrainQuadTreeNode::TTerrainQuadTreeNode()
    :
    Node1(0),
    Node2(0),
    Node3(0),
    Node4(0),
    ParentNode(0),
    NeighborType(0) {

}

void TTerrainQuadTreeNode::DestroyNode() {
    if (Node1) {
        Node1->DestroyNode();
        delete Node1;
    }
    if (Node2) {
        Node2->DestroyNode();
        delete Node2;
    }
    if (Node3) {
        Node3->DestroyNode();
        delete Node3;
    }
    if (Node4) {
        Node4->DestroyNode();
        delete Node4;
    }
}

void TTerrainQuadTreeNode::CreateTerrainQuadTreeLeaves(float** HeightValue, TIntPoint Coordinate, TTerrainQuadTreeNode* Parent, TTerrainQuadTree* Tree, unsigned int Level, unsigned int NumCellX, E_QuadTreeNodeType NodeType) {
    unsigned int TessellationSize = (1 << (Level));
    Depth = Level;
    ParentNode = Parent;
    ThisTree = Tree;
    Coord = Coordinate;
    switch (NodeType) {
    case Node_LeftBottom: {
        Indices[0] = Parent->Indices[0];
        Indices[1] = Indices[0]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 - 3;
        Indices[2] = Indices[0]
                     + (((Parent->Indices[2] - 2) + NumCellX * 4)
                        - Parent->Indices[0]) / 2 - NumCellX * 4 + 2;
        Indices[3] = Indices[2]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 - 3;

        Origin.x = Parent->Origin.x - TessellationSize / 2;
        Origin.z = Parent->Origin.z - TessellationSize / 2;
        float HeightSum = 0;
        for (unsigned int hi = 0; hi < TessellationSize; ++hi)
            for (unsigned int hj = 0; hj < TessellationSize; ++hj)
                HeightSum += HeightValue[Coordinate.x + hi][Coordinate.y + hj];
        HeightSum /= (TessellationSize * TessellationSize);
        Origin.y = HeightSum;
    }
    break;
    case Node_RightBottom: {
        Indices[0] = Parent->Indices[0]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2;
        Indices[1] = Parent->Indices[1];
        Indices[2] = Indices[0]
                     + (((Parent->Indices[2] - 2) + NumCellX * 4)
                        - Parent->Indices[0]) / 2 - NumCellX * 4 + 2;
        Indices[3] = Indices[2]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 - 3;

        Origin.x = Parent->Origin.x + TessellationSize / 2;
        Origin.z = Parent->Origin.z - TessellationSize / 2;
        float HeightSum = 0;
        for (unsigned int hi = 0; hi < TessellationSize; ++hi)
            for (unsigned int hj = 0; hj < TessellationSize; ++hj)
                HeightSum += HeightValue[Coordinate.x + hi][Coordinate.y + hj];
        HeightSum /= (TessellationSize * TessellationSize);
        Origin.y = HeightSum;
    }
    break;
    case Node_LeftTop: {
        Indices[0] = Parent->Indices[0]
                     + (((Parent->Indices[2] - 2) + NumCellX * 4)
                        - Parent->Indices[0]) / 2;
        Indices[1] = Indices[0]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 - 3;
        Indices[2] = Parent->Indices[2];
        Indices[3] = Indices[2]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 - 3;

        Origin.x = Parent->Origin.x - TessellationSize / 2;
        Origin.z = Parent->Origin.z + TessellationSize / 2;
        float HeightSum = 0;
        for (unsigned int hi = 0; hi < TessellationSize; ++hi)
            for (unsigned int hj = 0; hj < TessellationSize; ++hj)
                HeightSum += HeightValue[Coordinate.x + hi][Coordinate.y + hj];
        HeightSum /= (TessellationSize * TessellationSize);
        Origin.y = HeightSum;
    }
    break;
    case Node_RightTop: {
        Indices[1] = Parent->Indices[0]
                     + (((Parent->Indices[2] - 2) + NumCellX * 4)
                        - Parent->Indices[0]) / 2
                     + (Parent->Indices[1] - Parent->Indices[0]);
        Indices[0] = Indices[1]
                     - ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2 + 3;
        Indices[2] = Parent->Indices[2]
                     + ((Parent->Indices[1] + 3) - Parent->Indices[0]) / 2;
        Indices[3] = Parent->Indices[3];
        Origin.x = Parent->Origin.x + TessellationSize / 2;
        Origin.z = Parent->Origin.z + TessellationSize / 2;
        float HeightSum = 0;
        for (unsigned int hi = 0; hi < TessellationSize; ++hi)
            for (unsigned int hj = 0; hj < TessellationSize; ++hj)
                HeightSum += HeightValue[Coordinate.x + hi][Coordinate.y + hj];
        HeightSum /= (TessellationSize * TessellationSize);
        Origin.y = HeightSum;
    }
    break;
    }

    if (Level) {
        TessellationSize >>= 1;
        Node1 = new TTerrainQuadTreeNode();
        Node2 = new TTerrainQuadTreeNode();
        Node3 = new TTerrainQuadTreeNode();
        Node4 = new TTerrainQuadTreeNode();
        Node1->CreateTerrainQuadTreeLeaves(HeightValue,
                                           TIntPoint(Coordinate.x, Coordinate.y), this, Tree, Level - 1,
                                           NumCellX, Node_LeftBottom);
        Node2->CreateTerrainQuadTreeLeaves(HeightValue,
                                           TIntPoint(Coordinate.x + TessellationSize, Coordinate.y), this,
                                           Tree, Level - 1, NumCellX, Node_RightBottom);
        Node3->CreateTerrainQuadTreeLeaves(HeightValue,
                                           TIntPoint(Coordinate.x, Coordinate.y + TessellationSize), this,
                                           Tree, Level - 1, NumCellX, Node_LeftTop);
        Node4->CreateTerrainQuadTreeLeaves(HeightValue,
                                           TIntPoint(Coordinate.x + TessellationSize,
                                                   Coordinate.y + TessellationSize), this, Tree, Level - 1,
                                           NumCellX, Node_RightTop);
    }
}

void TTerrainQuadTreeNode::FillLODIndexBuffer(TIndex16** pData) {
    if (bIsLODLeaf) {
        int Num = 0;
        if (NeighborType == 0) {
            (*pData)[0]._1 = Indices[0];
            (*pData)[0]._2 = Indices[2];
            (*pData)[0]._3 = Indices[3];
            (*pData)[1]._1 = Indices[0];
            (*pData)[1]._2 = Indices[3];
            (*pData)[1]._3 = Indices[1];
            *pData += 2;
        } else {
            if (NeighborType & 1) {
                (*pData)[Num]._1 = Indices[1];
                (*pData)[Num]._2 = Node2->Indices[2];
                (*pData)[Num++]._3 = Node2->Indices[3];
                (*pData)[Num]._1 = Node4->Indices[0];
                (*pData)[Num]._2 = Node4->Indices[3];
                (*pData)[Num++]._3 = Node4->Indices[1];
            } else {
                (*pData)[Num]._1 = Indices[1];
                (*pData)[Num]._2 = Node2->Indices[2];
                (*pData)[Num++]._3 = Indices[3];
            }

            if (NeighborType & 2) {
                (*pData)[Num]._1 = Indices[0];
                (*pData)[Num]._2 = Node1->Indices[2];
                (*pData)[Num++]._3 = Node1->Indices[3];
                (*pData)[Num]._1 = Node3->Indices[1];
                (*pData)[Num]._2 = Node3->Indices[0];
                (*pData)[Num++]._3 = Node3->Indices[2];
            } else {
                (*pData)[Num]._1 = Indices[0];
                (*pData)[Num]._2 = Indices[2];
                (*pData)[Num++]._3 = Node1->Indices[3];
            }

            if (NeighborType & 4) {
                (*pData)[Num]._1 = Indices[2];
                (*pData)[Num]._2 = Node3->Indices[3];
                (*pData)[Num++]._3 = Node3->Indices[1];
                (*pData)[Num]._1 = Node4->Indices[2];
                (*pData)[Num]._2 = Node4->Indices[3];
                (*pData)[Num++]._3 = Node4->Indices[0];
            } else {
                (*pData)[Num]._1 = Indices[2];
                (*pData)[Num]._2 = Indices[3];
                (*pData)[Num++]._3 = Node3->Indices[1];
            }

            if (NeighborType & 8) {
                (*pData)[Num]._1 = Indices[0];
                (*pData)[Num]._2 = Node1->Indices[3];
                (*pData)[Num++]._3 = Node1->Indices[1];
                (*pData)[Num]._1 = Node2->Indices[0];
                (*pData)[Num]._2 = Node2->Indices[2];
                (*pData)[Num++]._3 = Node2->Indices[1];
            } else {
                (*pData)[Num]._1 = Indices[0];
                (*pData)[Num]._2 = Node1->Indices[3];
                (*pData)[Num++]._3 = Indices[1];
            }

            *pData += Num;
        }
        //(*pData)[3]._1 = Indices[0];		(*pData)[3]._2 = Indices[2];		(*pData)[3]._3 = Indices[3];

    }

    else if (Node1) {
        Node1->FillLODIndexBuffer(pData);
        Node2->FillLODIndexBuffer(pData);
        Node3->FillLODIndexBuffer(pData);
        Node4->FillLODIndexBuffer(pData);
    }
}

void TTerrainQuadTreeNode::CheckLODLeafNode(TVector3 LODOrigin) {
    if (GetLODWeight(LODOrigin, Origin) > (1 << (Depth - 1)) * 1000 || !Node1) {
        bIsLODLeaf = true;
    } else if (Node1) {
        bIsLODLeaf = false;
        Node1->CheckLODLeafNode(LODOrigin);
        Node2->CheckLODLeafNode(LODOrigin);
        Node3->CheckLODLeafNode(LODOrigin);
        Node4->CheckLODLeafNode(LODOrigin);
    }
}

bool TTerrainQuadTreeNode::IsNeighborCracked(E_NeighborType Type) {
    int X, Z;
    TTerrainQuadTreeNode* Node = ThisTree->Root;
    switch (Type) {
    case NeighborType_Right: {
        X = Coord.x + (1 << Depth);
        Z = Coord.y;
    }
    break;
    case NeighborType_Left: {
        X = Coord.x - (1 << Depth);
        Z = Coord.y;
    }
    break;
    case NeighborType_Upper: {
        X = Coord.x;
        Z = Coord.y + (1 << Depth);
    }
    break;
    case NeighborType_Lower: {
        X = Coord.x;
        Z = Coord.y - (1 << Depth);
    }
    break;
    }
    TIntPoint TreeCoord = ThisTree->TreeCoord;
    while (Node->Coord.x > X) {
        if (ThisTree->TreeCoord.x - 1 < 0)
            return false;
        TreeCoord.x -= 1;
        Node = ThisTree->Primitive->QuadTree[TreeCoord.x][TreeCoord.y].Root;
    }
    while (Node->Coord.x + (1 << Node->Depth) <= X) {
        if (ThisTree->TreeCoord.x + 1
                >= (int) (ThisTree->Primitive->QuadTreeSizeX))
            return false;
        TreeCoord.x += 1;
        Node = ThisTree->Primitive->QuadTree[TreeCoord.x][TreeCoord.y].Root;
    }
    while (Node->Coord.y > Z) {
        if (ThisTree->TreeCoord.y - 1 < 0)
            return false;
        TreeCoord.y -= 1;
        Node = ThisTree->Primitive->QuadTree[TreeCoord.x][TreeCoord.y].Root;
    }
    while (Node->Coord.y + (1 << Node->Depth) <= Z) {
        if (ThisTree->TreeCoord.y + 1
                >= (int) (ThisTree->Primitive->QuadTreeSizeY))
            return false;
        TreeCoord.y += 1;
        Node = ThisTree->Primitive->QuadTree[TreeCoord.x][TreeCoord.y].Root;
    }

    while (!Node->bIsLODLeaf) {
        if (Node->Origin.x > X && Node->Origin.z > Z)
            Node = Node->Node1;
        else if (Node->Origin.x <= X && Node->Origin.z > Z)
            Node = Node->Node2;
        else if (Node->Origin.x > X && Node->Origin.z <= Z)
            Node = Node->Node3;
        else if (Node->Origin.x <= X && Node->Origin.z <= Z)
            Node = Node->Node4;
    }
    if (Node->Depth < Depth)
        return true;
    return false;
}

unsigned int TTerrainQuadTreeNode::CheckLODSolderNeighbor() {
    unsigned int Ret = 0;
    if (bIsLODLeaf) {
        NeighborType = 0;
        if (IsNeighborCracked(NeighborType_Right))
            NeighborType |= 1;
        if (IsNeighborCracked(NeighborType_Left))
            NeighborType |= 2;
        if (IsNeighborCracked(NeighborType_Upper))
            NeighborType |= 4;
        if (IsNeighborCracked(NeighborType_Lower))
            NeighborType |= 8;
        if (NeighborType == 0)
            Ret = 2;
        else if (NeighborType == 1 || NeighborType == 2 || NeighborType == 4
                 || NeighborType == 8)
            Ret = 5;
        else if (NeighborType == 6 || NeighborType == 9 || NeighborType == 5
                 || NeighborType == 10 || NeighborType == 12
                 || NeighborType == 3)
            Ret = 6;
        else if (NeighborType == 7 || NeighborType == 11 || NeighborType == 13
                 || NeighborType == 14)
            Ret = 7;
        else if (NeighborType == 15)
            Ret = 8;
    } else if (Node1) {
        Ret += Node1->CheckLODSolderNeighbor();
        Ret += Node2->CheckLODSolderNeighbor();
        Ret += Node3->CheckLODSolderNeighbor();
        Ret += Node4->CheckLODSolderNeighbor();
    }
    return Ret;
}

float TTerrainQuadTreeNode::GetLODWeight(TVector3 LODOrigin, TVector3 Origin) {
    return (Origin - LODOrigin).SizeSquared();
}

///////////////////////////////////////////////////////////// TTerainQuadTree //////////////////////////////////////////////////////////////

TTerrainQuadTree::TTerrainQuadTree()
    :
    Root(0) {

}

void TTerrainQuadTree::FillLODIndexBuffer(TIndex16** pData) {
    if (Root)
        Root->FillLODIndexBuffer(pData);
}

unsigned int TTerrainQuadTree::CheckLODLeafNode(TVector3 LODOrigin) {
    unsigned int Ret;
    if (Root) {
        Root->CheckLODLeafNode(LODOrigin);
        Ret = Root->CheckLODSolderNeighbor();
    }
    return Ret;
}
