#pragma once
#include "DirectX_3D.h"
#include "ResourceList.h"
#include <fstream>
#include <map>

class CVertex {
protected:
	XMFLOAT3 Position{};

public:
	CVertex() {
		Position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	}

	CVertex(XMFLOAT3 Position) {
		Position = Position;
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
	UINT Vertices = 0;
	XMFLOAT3* Position = NULL;
	ID3D12Resource* PositionBuffer = NULL;
	ID3D12Resource* PositionUploadBuffer = NULL;

	XMFLOAT3* Normal = NULL;
	ID3D12Resource* NormalBuffer = NULL;
	ID3D12Resource* NormalUploadBuffer = NULL;

	XMFLOAT2* TextureCoords = NULL;
	ID3D12Resource* TextureCoordBuffer = NULL;
	ID3D12Resource* TextureCoordUploadBuffer = NULL;

	UINT Indices = 0;
	UINT* PnIndices = NULL;
	ID3D12Resource* IndexBuffer = NULL;
	ID3D12Resource* IndexUploadBuffer = NULL;

	UINT NumVertexBufferViews = 0;
	D3D12_VERTEX_BUFFER_VIEW* VertexBufferViews = NULL;

	D3D12_INDEX_BUFFER_VIEW	IndexBufferView;

	D3D12_PRIMITIVE_TOPOLOGY PromitiveTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT Slot = 0;
	UINT Stride = 0;
	UINT Offset = 0;

	UINT StartIndex = 0;
	int	BaseVertex = 0;

public:
	BoundingOrientedBox	OOBB = BoundingOrientedBox();

	Mesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode = true);
	~Mesh();
	void ReleaseUploadBuffers();
	void Render(ID3D12GraphicsCommandList* CmdList);
	BOOL RayIntersectionByTriangle(XMVECTOR& xmRayOrigin, XMVECTOR& xmRayDirection, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* pfNearHitDistance);
	int CheckRayIntersection(XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection, float* pfNearHitDistance);
	void LoadMeshFromFile(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode);
	float GetHeightAtPosition(Mesh* terrainMesh, float x, float z, const XMFLOAT4X4& worldMatrix);
	bool IsPointInTriangle(XMFLOAT2 pt, XMFLOAT2 v0, XMFLOAT2 v1, XMFLOAT2 v2);
	float ComputeHeightOnTriangle(XMFLOAT3 pt, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2);
};

class MeshUtil {
private:
	std::map<std::string, Mesh*> LoadedMeshList;
	std::map<std::string, Mesh*> LoadedTerrainList;

public:
	void Init(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList);
	Mesh* GetMesh(std::string MeshName);
	Mesh* GetTerrain(std::string TerrainName);
};

extern MeshUtil meshUtil;