#pragma once
#include "DirectX_3D.h"
#include <fstream>

enum class MeshType
{ Text, Binary };


class CVertex {
protected:
	XMFLOAT3 Position{};

public:
	CVertex() {
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	CVertex(XMFLOAT3 PositionValue) {
		Position = PositionValue;
	}

	~CVertex() { }
};

class CDiffusedVertex : public CVertex {
protected:
	XMFLOAT4 Diffuse{};

public:
	CDiffusedVertex() {
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f); Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}

	CDiffusedVertex(float x, float y, float z, XMFLOAT4 Diffuse) {
		Position = XMFLOAT3(x, y, z); Diffuse = Diffuse;
	}

	CDiffusedVertex(XMFLOAT3 Position, XMFLOAT4 Diffuse) {
		Position = Position; Diffuse = Diffuse;
	}

	~CDiffusedVertex() {}
};

class Mesh {
private:
	int Ref{};

public:
	void AddRef();
	void Release();

protected:
	UINT Vertices{};
	XMFLOAT3* Position{};
	ID3D12Resource* PositionBuffer{};
	ID3D12Resource* PositionUploadBuffer{};

	XMFLOAT3* Normal{};
	ID3D12Resource* NormalBuffer{};
	ID3D12Resource* NormalUploadBuffer{};

	XMFLOAT2* TextureCoords{};
	ID3D12Resource* TextureCoordBuffer{};
	ID3D12Resource* TextureCoordUploadBuffer{};

	UINT Indices{};
	UINT* PnIndices{};
	ID3D12Resource* IndexBuffer{};
	ID3D12Resource* IndexUploadBuffer{};

	UINT NumVertexBufferViews{};
	D3D12_VERTEX_BUFFER_VIEW* VertexBufferViews{};

	D3D12_INDEX_BUFFER_VIEW	IndexBufferView{};

	D3D12_PRIMITIVE_TOPOLOGY PromitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT Slot{};
	UINT Stride{};
	UINT Offset{};

	UINT StartIndex{};
	int	BaseVertex{};

public:
	BoundingOrientedBox	OOBB = BoundingOrientedBox();

	Mesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, MeshType Type);
	Mesh();
	~Mesh();
	void ReleaseUploadBuffers();
	void Render(ID3D12GraphicsCommandList* CmdList);
	BOOL RayIntersectionByTriangle(XMVECTOR& xmRayOrigin, XMVECTOR& xmRayDirection, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* pfNearHitDistance);
	int CheckRayIntersection(XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection, float* pfNearHitDistance);
	void CreateImagePanelMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
	void ImportMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode);
	float GetHeightAtPosition(Mesh* terrainMesh, float x, float z, const XMFLOAT4X4& worldMatrix);
	bool IsPointInTriangle(XMFLOAT2 pt, XMFLOAT2 v0, XMFLOAT2 v1, XMFLOAT2 v2);
	float ComputeHeightOnTriangle(XMFLOAT3 pt, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
};

// 이미지 출력용 매쉬
extern Mesh* ImagePannel;