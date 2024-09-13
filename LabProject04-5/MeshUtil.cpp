#include "MeshUtil.h"

// �Ž��� ����ϴ� ��ƿ�̴�.

// ResourList���� �ش� �Լ��� ����Ͽ� �Ž��� �ε��ϵ��� �Ѵ�
Mesh::Mesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, MeshType Type) {
	if (Directory) {
		if(Type == MeshType::Text)
			ImportMesh(Device, CmdList, Directory, true);
		else if(Type == MeshType::Binary)
			ImportMesh(Device, CmdList, Directory, false);
	}

	ReleaseUploadBuffers();
}

Mesh::~Mesh() {
	if (Position) delete[] Position;
	if (Normal) delete[] Normal;
	if (TextureCoords) delete[] TextureCoords;

	if (PnIndices) delete[] PnIndices;

	if (VertexBufferViews) delete[] VertexBufferViews;

	if (PositionBuffer) PositionBuffer->Release();
	if (NormalBuffer) NormalBuffer->Release();
	if (TextureCoordBuffer) TextureCoordBuffer->Release();
	if (IndexBuffer) IndexBuffer->Release();
}

void Mesh::AddRef() {
	Ref++;
}

void Mesh::Release() {
	if (--Ref <= 0)
		delete this;
}

void Mesh::ReleaseUploadBuffers() {
	if (PositionUploadBuffer) PositionUploadBuffer->Release();
	if (NormalUploadBuffer) NormalUploadBuffer->Release();
	if (TextureCoordUploadBuffer) TextureCoordUploadBuffer->Release();
	if (IndexUploadBuffer) IndexUploadBuffer->Release();

	PositionUploadBuffer = NULL;
	NormalUploadBuffer = NULL;
	TextureCoordUploadBuffer = NULL;
	IndexUploadBuffer = NULL;
};

void Mesh::Render(ID3D12GraphicsCommandList* CmdList) {
	CmdList->IASetPrimitiveTopology(PromitiveTopology);
	CmdList->IASetVertexBuffers(Slot, NumVertexBufferViews, VertexBufferViews);
	if (IndexBuffer) {
		CmdList->IASetIndexBuffer(&IndexBufferView);
		CmdList->DrawIndexedInstanced(Indices, 1, 0, 0, 0);
	}

	else
		CmdList->DrawInstanced(Vertices, 1, Offset, 0);
}

BOOL Mesh::RayIntersectionByTriangle(XMVECTOR& xmRayOrigin, XMVECTOR& xmRayDirection, XMVECTOR v0, XMVECTOR v1, XMVECTOR v2, float* pfNearHitDistance) {
	float fHitDistance;
	BOOL bIntersected = TriangleTests::Intersects(xmRayOrigin, xmRayDirection, v0, v1, v2, fHitDistance);
	if (bIntersected && (fHitDistance < *pfNearHitDistance)) *pfNearHitDistance = fHitDistance;

	return(bIntersected);
}

int Mesh::CheckRayIntersection(XMVECTOR& xmvPickRayOrigin, XMVECTOR& xmvPickRayDirection, float* pfNearHitDistance) {
	int nIntersections = 0;
	bool bIntersected = OOBB.Intersects(xmvPickRayOrigin, xmvPickRayDirection, *pfNearHitDistance);
	if (bIntersected) {
		for (int i = 0; i < Indices; i += 3) {
			XMVECTOR v0 = XMLoadFloat3(&Position[PnIndices[i]]);
			XMVECTOR v1 = XMLoadFloat3(&Position[PnIndices[i + 1]]);
			XMVECTOR v2 = XMLoadFloat3(&Position[PnIndices[i + 2]]);
			BOOL bIntersected = RayIntersectionByTriangle(xmvPickRayOrigin, xmvPickRayDirection, v0, v1, v2, pfNearHitDistance);
			if (bIntersected) nIntersections++;
		}
	}

	return(nIntersections);
}

void Mesh::ImportMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, bool TextMode) {
	char Token[64] = { '\0' };

	if (TextMode) {
		std::ifstream In(Directory);

		while (true) {
			In >> Token;
			if (!In) break;

			if (!strcmp(Token, "<Vertices>:")) {
				In >> Vertices;
				Position = new XMFLOAT3[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> Position[i].x >> Position[i].y >> Position[i].z;
			}

			else if (!strcmp(Token, "<Normals>:")) {
				In >> Token;
				Normal = new XMFLOAT3[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> Normal[i].x >> Normal[i].y >> Normal[i].z;
			}

			else if (!strcmp(Token, "<TextureCoords>:")) {
				In >> Token;
				TextureCoords = new XMFLOAT2[Vertices];
				for (UINT i = 0; i < Vertices; i++) In >> TextureCoords[i].x >> TextureCoords[i].y;
			}

			else if (!strcmp(Token, "<Indices>:")) {
				In >> Indices;
				PnIndices = new UINT[Indices];
				for (UINT i = 0; i < Indices; i++) In >> PnIndices[i];
			}
		}
	}

	else {
		FILE* File = NULL;
		::fopen_s(&File, Directory, "rb");
		::rewind(File);

		char Token[64] = { '\0' };

		BYTE StrLength = 0;
		UINT Reads = 0;

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 14, File); //"<BoundingBox>:"
		Reads = (UINT)::fread(&OOBB.Center, sizeof(float), 3, File);
		Reads = (UINT)::fread(&OOBB.Extents, sizeof(float), 3, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 11, File); //"<Vertices>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		Position = new XMFLOAT3[Vertices];
		Reads = (UINT)::fread(Position, sizeof(float), 3 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 10, File); //"<Normals>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		Normal = new XMFLOAT3[Vertices];
		Reads = (UINT)::fread(Normal, sizeof(float), 3 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 16, File); //"<TextureCoords>:"
		Reads = (UINT)::fread(&Vertices, sizeof(int), 1, File);
		TextureCoords = new XMFLOAT2[Vertices];
		Reads = (UINT)::fread(TextureCoords, sizeof(float), 2 * Vertices, File);

		Reads = (UINT)::fread(&StrLength, sizeof(BYTE), 1, File);
		Reads = (UINT)::fread(Token, sizeof(char), 10, File); //"<Indices>:"
		Reads = (UINT)::fread(&Indices, sizeof(int), 1, File);
		PnIndices = new UINT[Indices];
		Reads = (UINT)::fread(PnIndices, sizeof(UINT), Indices, File);

		::fclose(File);
	}

	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	NumVertexBufferViews = 3;
	VertexBufferViews = new D3D12_VERTEX_BUFFER_VIEW[NumVertexBufferViews];

	VertexBufferViews[0].BufferLocation = PositionBuffer->GetGPUVirtualAddress();
	VertexBufferViews[0].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[0].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[1].BufferLocation = NormalBuffer->GetGPUVirtualAddress();
	VertexBufferViews[1].StrideInBytes = sizeof(XMFLOAT3);
	VertexBufferViews[1].SizeInBytes = sizeof(XMFLOAT3) * Vertices;

	VertexBufferViews[2].BufferLocation = TextureCoordBuffer->GetGPUVirtualAddress();
	VertexBufferViews[2].StrideInBytes = sizeof(XMFLOAT2);
	VertexBufferViews[2].SizeInBytes = sizeof(XMFLOAT2) * Vertices;

	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}

float Mesh::GetHeightAtPosition(Mesh* terrainMesh, float x, float z, const XMFLOAT4X4& worldMatrix) {
	// XMFLOAT4X4�� XMMATRIX�� ��ȯ
	XMMATRIX gmtxWorld = XMLoadFloat4x4(&worldMatrix);

	// ���� ��ǥ�� ��ȯ�� �ﰢ���� �� �� ��ǥ�� ���
	for (UINT i = 0; i < terrainMesh->Indices; i += 3) {
		XMFLOAT3 v0 = terrainMesh->Position[terrainMesh->PnIndices[i]];
		XMFLOAT3 v1 = terrainMesh->Position[terrainMesh->PnIndices[i + 1]];
		XMFLOAT3 v2 = terrainMesh->Position[terrainMesh->PnIndices[i + 2]];

		// ���� ��ǥ�� ���� ��ǥ�� ��ȯ
		XMVECTOR v0World = XMVector3Transform(XMLoadFloat3(&v0), gmtxWorld);
		XMVECTOR v1World = XMVector3Transform(XMLoadFloat3(&v1), gmtxWorld);
		XMVECTOR v2World = XMVector3Transform(XMLoadFloat3(&v2), gmtxWorld);

		// ���� ��ǥ�� ��ȯ�� ���� �ٽ� ����
		XMStoreFloat3(&v0, v0World);
		XMStoreFloat3(&v1, v1World);
		XMStoreFloat3(&v2, v2World);

		// �ﰢ�� �������� Ȯ��
		if (IsPointInTriangle(XMFLOAT2(x, z), XMFLOAT2(v0.x, v0.z), XMFLOAT2(v1.x, v1.z), XMFLOAT2(v2.x, v2.z))) {
			// �ﰢ�� ��鿡�� Y ���� ���
			float height = ComputeHeightOnTriangle(XMFLOAT3(x, 0, z), v0, v1, v2);
			return height;
		}
	}
	// �ﰢ���� ã�� ���� ��� �⺻ �� ��ȯ (��: 0)
	return 0.0f;
}

bool Mesh::IsPointInTriangle(XMFLOAT2 pt, XMFLOAT2 v0, XMFLOAT2 v1, XMFLOAT2 v2) {
	float d00 = (v1.x - v0.x) * (v1.x - v0.x) + (v1.y - v0.y) * (v1.y - v0.y);
	float d01 = (v1.x - v0.x) * (v2.x - v0.x) + (v1.y - v0.y) * (v2.y - v0.y);
	float d11 = (v2.x - v0.x) * (v2.x - v0.x) + (v2.y - v0.y) * (v2.y - v0.y);
	float d20 = (pt.x - v0.x) * (v1.x - v0.x) + (pt.y - v0.y) * (v1.y - v0.y);
	float d21 = (pt.x - v0.x) * (v2.x - v0.x) + (pt.y - v0.y) * (v2.y - v0.y);

	float denom = d00 * d11 - d01 * d01;
	float v = (d11 * d20 - d01 * d21) / denom;
	float w = (d00 * d21 - d01 * d20) / denom;
	float u = 1.0f - v - w;
	return (u >= 0) && (v >= 0) && (w >= 0);
}

float Mesh::ComputeHeightOnTriangle(XMFLOAT3 pt, XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2) {
	XMVECTOR p = XMLoadFloat3(&pt);
	XMVECTOR a = XMLoadFloat3(&v0);
	XMVECTOR b = XMLoadFloat3(&v1);
	XMVECTOR c = XMLoadFloat3(&v2);

	XMVECTOR n = XMVector3Cross(b - a, c - a);
	n = XMVector3Normalize(n);

	float d = -XMVectorGetX(XMVector3Dot(n, a));
	float height = -(XMVectorGetX(XMVector3Dot(n, XMVectorSet(1.0f, 0.0f, 1.0f, 0.0f) * p)) + d) / XMVectorGetY(n);

	return height;
}