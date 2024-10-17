#include "MeshUtil.h"
#include <vector>
#include <string>
#include <sstream>
#include <iostream>

// 매쉬를 담당하는 유틸이다.

// ResourList에서 해당 함수를 사용하여 매쉬를 로드하도록 한다
Mesh::Mesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList, char* Directory, MeshType Type) {
	if (Directory) {
		if (Type == MeshType::Text)
			ImportMesh(Device, CmdList, Directory, true);
		else if (Type == MeshType::Binary)
			ImportMesh(Device, CmdList, Directory, false);
	}
}

Mesh::Mesh() {}

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

// 스카이박스 매쉬를 생성한다. 프로그램 실행 시 최초 1회 실행되고 그 이후에는 굳이 실행할 필요 없다.
void Mesh::CreateSkyboxMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	float SkyboxVertices[][8] = {
		// x, y, z, nx, ny, nz, u, v
	   // Front face (중앙)
	   { -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.2509f, 0.3339f },  // Top-left
	   {  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.4991f, 0.3339f },  // Top-right
	   {  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.4991f, 0.6661f },  // Bottom-right
	   { -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.2509f, 0.6661f },  // Bottom-left

	   // Back face (오른쪽)
	   {  1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.7491f, 0.3339f },  // Top-left
	   { -1.0f,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.9991f, 0.3339f },  // Top-right
	   { -1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.9991f, 0.6661f },  // Bottom-right
	   {  1.0f, -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.7491f, 0.6661f },  // Bottom-left

	   // Left face (왼쪽)
	   { -1.0f,  1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0009f, 0.3339f },   // Top-left
	   { -1.0f,  1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.2491f, 0.3339f },   // Top-right
	   { -1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.2491f, 0.6661f },   // Bottom-right
	   { -1.0f, -1.0f,  1.0f, -1.0f, 0.0f, 0.0f, 0.0009f, 0.6661f },   // Bottom-left

	   // Right face (오른쪽 중앙)
	   {  1.0f,  1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.5009f, 0.3339f },    // Top-left
	   {  1.0f,  1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.7491f, 0.3339f },    // Top-right
	   {  1.0f, -1.0f,  1.0f, 1.0f, 0.0f, 0.0f, 0.7491f, 0.6661f },    // Bottom-right
	   {  1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.5009f, 0.6661f },    // Bottom-left

	   // Top face (중앙 아래)
	   { -1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.2509f, 0.0009f },    // Top-left
	   {  1.0f,  1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.4991f, 0.0009f },    // Top-right
	   {  1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.4991f, 0.3339f },    // Bottom-right
	   { -1.0f,  1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.2509f, 0.3339f },    // Bottom-left

	   // Bottom face (중앙 위)
	   { -1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.2509f, 0.6661f },   // Top-left
	   {  1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.4991f, 0.6661f },   // Top-right
	   {  1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.4991f, 0.9991f },   // Bottom-right
	   { -1.0f, -1.0f,  1.0f, 0.0f, -1.0f, 0.0f, 0.2509f, 0.9991f }    // Bottom-left
	};

	// 인덱스 배열 (각 면을 두 개의 삼각형으로 구성)
	UINT indices[] = {
		// Front face (시계 방향)
		0, 2, 1, 0, 3, 2,
		// Back face (시계 방향)
		4, 6, 5, 4, 7, 6,
		// Left face (시계 방향)
		8, 10, 9, 8, 11, 10,
		// Right face (시계 방향)
		12, 14, 13, 12, 15, 14,
		// Top face (시계 방향)
		16, 18, 17, 16, 19, 18,
		// Bottom face (시계 방향)
		20, 22, 21, 20, 23, 22
	};

	// 정점 개수와 인덱스 개수 설정
	Vertices = 24;
	Indices = 36;

	// 정점 및 관련 데이터 할당
	Position = new XMFLOAT3[Vertices];
	Normal = new XMFLOAT3[Vertices];
	TextureCoords = new XMFLOAT2[Vertices];
	PnIndices = new UINT[Indices];

	// SkyboxVertices 데이터를 이용하여 각 정점의 포지션, 노말, 텍스처 좌표를 설정
	for (UINT i = 0; i < Vertices; i++) {
		Position[i] = XMFLOAT3(SkyboxVertices[i][0], SkyboxVertices[i][1], SkyboxVertices[i][2]);
		Normal[i] = XMFLOAT3(SkyboxVertices[i][3], SkyboxVertices[i][4], SkyboxVertices[i][5]);
		TextureCoords[i] = XMFLOAT2(SkyboxVertices[i][6], SkyboxVertices[i][7]);
	}

	// 인덱스 데이터 복사
	for (UINT i = 0; i < Indices; i++) {
		PnIndices[i] = indices[i];
	}

	// DirectX 버퍼 생성 및 뷰 설정 (원래 코드와 동일)
	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	// 버퍼 뷰 설정
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

	// 인덱스 버퍼 생성
	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	// 인덱스 버퍼 뷰 설정
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
}

// 이미지 출력을 위한 두께가 없는 정사각형 매쉬를 생성한다. 프로그램 실행 시 최초 1회 실행되고 그 이후에는 굳이 실행할 필요 없다.
void Mesh::CreateImagePannelMesh(ID3D12Device* Device, ID3D12GraphicsCommandList* CmdList) {
	float ImagePannel[][8] = {
		// x, y, z, nx, ny, nz, u, v
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f },
		{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f },
		{  0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f }
	};

	// 인덱스 배열
	UINT indices[] = { 0, 2, 1, 0, 3, 2 };

	// 정점 개수와 인덱스 개수
	Vertices = 4;
	Indices = 6;

	// 정점 및 관련 데이터 할당
	Position = new XMFLOAT3[Vertices];
	Normal = new XMFLOAT3[Vertices];
	TextureCoords = new XMFLOAT2[Vertices];
	PnIndices = new UINT[Indices];

	// ImagePannel 데이터를 이용하여 각 정점의 포지션, 노말, 텍스처 좌표를 설정
	for (UINT i = 0; i < Vertices; i++) {
		Position[i] = XMFLOAT3(ImagePannel[i][0], ImagePannel[i][1], ImagePannel[i][2]);
		Normal[i] = XMFLOAT3(ImagePannel[i][3], ImagePannel[i][4], ImagePannel[i][5]);
		TextureCoords[i] = XMFLOAT2(ImagePannel[i][6], ImagePannel[i][7]);
	}

	// 인덱스 데이터 복사
	for (UINT i = 0; i < Indices; i++) {
		PnIndices[i] = indices[i];
	}

	// DirectX 버퍼 생성 (위 코드의 ImportMesh 함수와 유사)
	PositionBuffer = ::CreateBufferResource(Device, CmdList, Position, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &PositionUploadBuffer);
	NormalBuffer = ::CreateBufferResource(Device, CmdList, Normal, sizeof(XMFLOAT3) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &NormalUploadBuffer);
	TextureCoordBuffer = ::CreateBufferResource(Device, CmdList, TextureCoords, sizeof(XMFLOAT2) * Vertices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, &TextureCoordUploadBuffer);

	// 버퍼 뷰 설정
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

	// 인덱스 버퍼 생성
	IndexBuffer = ::CreateBufferResource(Device, CmdList, PnIndices, sizeof(UINT) * Indices, D3D12_HEAP_TYPE_DEFAULT, D3D12_RESOURCE_STATE_INDEX_BUFFER, &IndexUploadBuffer);

	// 인덱스 버퍼 뷰 설정
	IndexBufferView.BufferLocation = IndexBuffer->GetGPUVirtualAddress();
	IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	IndexBufferView.SizeInBytes = sizeof(UINT) * Indices;
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
	// XMFLOAT4X4를 XMMATRIX로 변환
	XMMATRIX gmtxWorld = XMLoadFloat4x4(&worldMatrix);

	// 월드 좌표로 변환된 삼각형의 각 점 좌표를 계산
	for (UINT i = 0; i < terrainMesh->Indices; i += 3) {
		XMFLOAT3 v0 = terrainMesh->Position[terrainMesh->PnIndices[i]];
		XMFLOAT3 v1 = terrainMesh->Position[terrainMesh->PnIndices[i + 1]];
		XMFLOAT3 v2 = terrainMesh->Position[terrainMesh->PnIndices[i + 2]];

		// 로컬 좌표를 월드 좌표로 변환
		XMVECTOR v0World = XMVector3Transform(XMLoadFloat3(&v0), gmtxWorld);
		XMVECTOR v1World = XMVector3Transform(XMLoadFloat3(&v1), gmtxWorld);
		XMVECTOR v2World = XMVector3Transform(XMLoadFloat3(&v2), gmtxWorld);

		// 월드 좌표로 변환된 점을 다시 저장
		XMStoreFloat3(&v0, v0World);
		XMStoreFloat3(&v1, v1World);
		XMStoreFloat3(&v2, v2World);

		// 삼각형 내부인지 확인
		if (IsPointInTriangle(XMFLOAT2(x, z), XMFLOAT2(v0.x, v0.z), XMFLOAT2(v1.x, v1.z), XMFLOAT2(v2.x, v2.z))) {
			// 삼각형 평면에서 Y 값을 계산
			float height = ComputeHeightOnTriangle(XMFLOAT3(x, 0, z), v0, v1, v2);
			return height;
		}
	}
	// 삼각형을 찾지 못한 경우 기본 값 반환 (예: 0)
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