#include "TerrainUtil.h"
#include "CBVUtil.h"
#include "RootConstants.h"
#include "RootConstantUtil.h"

// 터레인 충돌처리 유틸이다.

// 터레인의 정보를 입력한다.
void TerrainUtil::InputData(XMFLOAT4X4& TMat, XMFLOAT4X4& RMat, XMFLOAT4X4& SMat, Mesh* MeshData) {
	XMMATRIX ResultMatrix = XMMatrixMultiply(XMLoadFloat4x4(&SMat), XMLoadFloat4x4(&RMat));
	ResultMatrix = XMMatrixMultiply(ResultMatrix, XMLoadFloat4x4(&TMat));
	XMStoreFloat4x4(&TerrainMatrix, XMMatrixTranspose(ResultMatrix));

	TerrainMesh = MeshData;

	// 높이 캐시를 비우고 새로운 값으로 높이 캐시를 설정한다.
	TerrainMesh->ClearHeightCache();
	TerrainMesh->SetHeightCache(TerrainMesh, TerrainMatrix);
}

// 특정 오브젝트가 터레인의 바닥에 닿았는지 검사한다.
// 닿았을 경우 true를 리턴한다.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position, float HeightOffset) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset)
			return true;
	}

	return false;
}

// 특정 오브젝트의 위치를 터레인 바닥의 위치로 이동시킨다.
// 이 함수를 업데이트 루프에서 사용할 경우 오브젝트가 터레인 바닥에서 떨어지지 않게 된다.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position, float HeightOffset) {
	if (TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset;
}

// 특정 위치에서의 터레인 높이를 리턴한다.
float TerrainUtil::GetFloorHeight(float x, float z, float HeightOffset) {
	if (TerrainMesh)
		return TerrainMesh->GetHeightAtPosition(TerrainMesh, x, z, TerrainMatrix) + HeightOffset;
	return 0.0;
}

// 특정 오브젝트의 위치가 터레인 바닥에 도달할 경우 터레인 바닥의 높이로 오브젝트를 이동시킨다
bool TerrainUtil::CheckCollision(XMFLOAT3& Position, float HeightOffset) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset) {
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TerrainMatrix) + HeightOffset;
			return true;
		}
	}

	return false;
}