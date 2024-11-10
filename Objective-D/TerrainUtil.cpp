#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// �ͷ��� �浹ó�� ��ƿ�̴�.

// �ͷ����� ������ �Է��Ѵ�.
// �� �ͷ��� �Ž� �� �� ���� �����ϸ� �ȴ�. �����ڿ��� ������ ���� �����Ѵ�.
// �ͷ����� �ƴ� ��ü�� �ش� �Լ��� ������ �ʿ䰡 ����.
void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;

	// ���� ĳ�ø� ���� ���ο� ������ ���� ĳ�ø� �����Ѵ�.
	TerrainMesh->ClearHeightCache();
	TerrainMesh->SetHeightCache(TerrainMesh, TerrainMatrix);
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴ��� ��ġ�� �̵���Ų��.
// Ư�� ������Ʈ�� �ͷ����� �ٴڿ� ��Ҵ��� �˻��Ѵ�. ����� ��� true�� �����Ѵ�.
bool TerrainUtil::CheckFloor(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset) {
	float Height = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;

	if (Position.y < Height)
		return true;

	return false;
}

// �� �Լ��� ������Ʈ �������� ����� ��� ������Ʈ�� �ͷ��� �ٴڿ��� �������� �ʰ� �ȴ�.
void TerrainUtil::ClampToFloor(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset) {
	Position.y = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;
}

// Ư�� ��ġ������ �ͷ��� ���̸� �����Ѵ�.
float TerrainUtil::GetFloorHeight(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset) {
	float Height = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;
	return Height;
}

// Ư�� ������Ʈ�� ��ġ�� �ͷ��� �ٴڿ� ������ ��� �ͷ��� �ٴ��� ���̷� ������Ʈ�� �̵���Ų��
void TerrainUtil::CheckCollision(const TerrainUtil& Other, XMFLOAT3& Position, float HeightOffset) {
	float Height = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;

	if (Position.y < Height)
		Position.y = Height;
}