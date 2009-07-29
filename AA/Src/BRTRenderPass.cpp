#include "StdAfx.h"

#include "RResource.h"

#include "BRTRenderPass.h"
#include "BDriver.h"

BRTRenderPass::BRTRenderPass(RRenderTarget* RT)
{
	m_RenderTarget = RT;
}

BRTRenderPass::~BRTRenderPass()
{	
}

void BRTRenderPass::BeginPass(BViewport* Viewport)
{
	RShaderBase* pShader = RShaderTable::pShaders;
	GDriver->SetRenderTarget(0, GDriver->GetBackBuffer());
	pShader->BeginShader();
	pShader->SetParameter(Viewport);
}

void BRTRenderPass::EndPass()
{
	RShaderBase* pShader = RShaderTable::pShaders;
	pShader->EndShader();
}

#include "CDirectXDriver.h"

void BRTRenderPass::DrawPrimitive()
{
	TBatch Batch;
	TQuad Tmp;
	Batch.m_pTemplates.AddItem(&Tmp);
	Batch.nVertexStride = Tmp.pVertexBuffer->nVertexStride;
	Batch.nVertices += Tmp.pVertexBuffer->nVertices;
	TBatch* pBatch = &Batch;
	//GDriver->DrawPrimitive();
	CDirectXDriver* Dr = ((CDirectXDriver*)GDriver);

	CDirectXVertexBuffer* VB = new CDirectXVertexBuffer();
	HRESULT hr;
	hr = Dr->GetDevice()->CreateVertexBuffer(
		pBatch->nVertices * pBatch->nVertexStride,
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		0,
		D3DPOOL_DEFAULT,
		&VB->VB,
		NULL);
	if(hr != D3D_OK)
	{
	}
	void *pData;
	hr = VB->VB->Lock(0, pBatch->nVertices * pBatch->nVertexStride
		, (void**)&pData, D3DLOCK_DISCARD);
	if(hr != D3D_OK)
	{
		switch(hr)
		{
		case D3DERR_INVALIDCALL:
			break;
		}
	}
	else
	{
		char* pcData = static_cast<char*>(pData);
		for(int i=0;i<(int)pBatch->m_pTemplates.Size();++i)
		{			
			memcpy(pcData, pBatch->m_pTemplates[i]->pVertexBuffer->pVertices, 
				pBatch->m_pTemplates[i]->pVertexBuffer->nVertices * pBatch->m_pTemplates[i]->pVertexBuffer->nVertexStride);
			pcData += pBatch->m_pTemplates[i]->pVertexBuffer->nVertices * pBatch->m_pTemplates[i]->pVertexBuffer->nVertexStride;
		}
		VB->VB->Unlock();
	}

	Dr->GetDevice()->SetStreamSource(0, VB->VB, 0, pBatch->nVertexStride);

	hr = Dr->GetDevice()->CreateIndexBuffer(
		14*sizeof(TIndex16),
		D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_DEFAULT,
		&VB->IB,
		NULL);

	hr = VB->IB->Lock(0, 14*sizeof(TIndex16), (void**)&pData, D3DLOCK_DISCARD);
	if(hr != D3D_OK)
	{
	}
	else
	{
		TIndex16* pcData = static_cast<TIndex16*>(pData);
		short BaseIdx = 0;
		for(int i=0;i<(int)pBatch->m_pTemplates.Size();++i)
		{
			for(int k=0;k<pBatch->m_pTemplates[i]->pIndexBuffer->nIndices;++k)
			{
				TIndex16 tmpIndex;
				tmpIndex._1 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._1 + BaseIdx;
				tmpIndex._2 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._2 + BaseIdx;
				tmpIndex._3 = pBatch->m_pTemplates[i]->pIndexBuffer->pIndices[k]._3 + BaseIdx;
				pcData[k] = tmpIndex;
			}
			BaseIdx += pBatch->m_pTemplates[i]->pVertexBuffer->nVertices;
			pcData += pBatch->m_pTemplates[i]->pIndexBuffer->nIndices;
		}
		VB->IB->Unlock();
	}

	Dr->GetDevice()->SetIndices(VB->IB);

	Dr->GetDevice()->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 12, 0, 14);
}