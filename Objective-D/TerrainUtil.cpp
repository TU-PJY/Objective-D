#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// 터레인 충돌처리 유틸이다.

// 터레인의 정보를 입력한다.
// 한 터레인 매쉬 당 한 번만 실행하면 된다. 생성자에서 실행할 것을 권장한다.
// 터레인이 아닌 객체는 해당 함수를 실행할 필요가 없다.
void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;

	// 높이 캐시를 비우고 새로운 값으로 높이 캐시를 설정한다.
	TerrainMesh->ClearHeightCache();
	TerrainMesh->SetHeightCache(TerrainMesh, TerrainMatrix);
}

// 현재 위치와 높이 오프셋을 입력한다
void TerrainUtil::InputPosition(XMFLOAT3& PositionValue, float HeightOffsetValue) {
	Position = PositionValue;
	HeightOffset = HeightOffsetValue;
}

// 터레인의 높이로 객체 높이 위치를 변경한다.
void TerrainUtil::SetHeightToTerrain(XMFLOAT3& PositionValue) {
	PositionValue.y = Position.y;
}

// 특정 오브젝트의 위치를 터레인 바닥의 위치로 이동시킨다.
// 특정 오브젝트가 터레인의 바닥에 닿았는지 검사한다. 닿았을 경우 true를 리턴한다.
bool TerrainUtil::CheckCollision(const TerrainUtil& Other) {
	float Height = Other.TerrainMesh->GetHeightAtPosition(Other.TerrainMesh, Position.x, Position.z, Other.TerrainMatrix) + HeightOffset;

	if (Position.y < Height) {
		Position.y = Height;
		return true;
	}

	return false;
}