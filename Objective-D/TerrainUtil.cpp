#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// �ͷ��� �浹ó�� ��ƿ�̴�.

// �ͷ����� ������ �Է��Ѵ�.
void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;

	// ���� ĳ�ø� ���� ���ο� ������ ���� ĳ�ø� �����Ѵ�.
	TerrainMesh->ClearHeightCache();
	TerrainMesh->SetHeightCache(TerrainMesh, TerrainMatrix);
}

// Ư�� ������Ʈ�� �ͷ����� �ٴڿ� ��Ҵ��� �˻��Ѵ�.
// ����� ��� true�� �����Ѵ�.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position, float HeightOffset) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset)
			return true;
	}

	return false;
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴ��� ��ġ�� �̵���Ų��.
// �� �Լ��� ������Ʈ �������� ����� ��� ������Ʈ�� �ͷ��� �ٴڿ��� �������� �ʰ� �ȴ�.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position, float HeightOffset) {
	if (TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset;
}

// Ư�� ��ġ������ �ͷ��� ���̸� �����Ѵ�.
float TerrainUtil::GetFloorHeight(float x, float z, float HeightOffset) {
	if (TerrainMesh)
		return TerrainMesh->GetHeightAtPosition(TerrainMesh, x, z, TerrainMatrix) + HeightOffset;
	return 0.0;
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴڿ� ������ ��� �ͷ��� �ٴ��� ���̷� ������Ʈ�� �̵���Ų��
bool TerrainUtil::CheckCollision(XMFLOAT3& Position, float HeightOffset) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset) {
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset;
			return true;
		}
	}

	return false;
}