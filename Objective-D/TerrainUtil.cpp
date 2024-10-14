#include "TerrainUtil.h"

// 터레인을 담당하는 유틸이다.
// 터레인 유틸은 한 번에 한 개의 매쉬만 가질 수 있다.

// 터레인으로 사용할 매쉬를 설정한다
void TerrainUtil::SetMesh(Mesh* MeshPtr) {
	TerrainMesh = MeshPtr;
	if (TerrainMesh)
		TerrainMesh->ReleaseUploadBuffers();
}

// 터레인을 렌더링할 쉐이더를 설정한다
void TerrainUtil::SetShader(Shader* ShaderPtr) {
	TerrainShader = ShaderPtr;
}

// 특정 오브젝트가 터레인의 바닥에 닿았는지 검사한다. 
// 닿았을 경우 true를 리턴한다.
bool TerrainUtil::CheckFloor(XMFLOAT3 Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			return true;
	}

	return false;
}

// 특정 오브젝트릐 위치를 터레인 바닥의 위치로 이동시킨다. 
// 이 함수를 업데이트 루프에서 사용할 경우 오브젝트가 터레인 바닥에서 떨어지지 않게 된다.
void TerrainUtil::ClampToFloor(XMFLOAT3& Position) {
	if(TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
}

// 특정 오브젝트의 위치가 터레인 바닥에 도달할 경우 터레인 바닥의 높이로 오브젝트를 이동시킨다
void TerrainUtil::CheckCollision(XMFLOAT3& Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
	}
}

// 터레인의 색상을 변경한다
void TerrainUtil::SetColor(XMFLOAT3 ColorValue) {
	TerrainColor.x = ColorValue.x;
	TerrainColor.y = ColorValue.y;
	TerrainColor.z = ColorValue.z;
}

// 터레인의 크기를 설정한다
void TerrainUtil::Scale(XMFLOAT3 SizeValue) {
	XMMATRIX ScaleMat = XMMatrixScaling(SizeValue.x, SizeValue.y, SizeValue.z);
	TranslateMatrix = Mat4::Multiply(ScaleMat, TranslateMatrix);
}

// 터레인을 렌더링한다
void TerrainUtil::Render(ID3D12GraphicsCommandList* CmdList) {
	if (TerrainShader)
		TerrainShader->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (TerrainMesh)
		TerrainMesh->Render(CmdList);
}

// 쉐이더로 터레인 변환 값을 전달한다
void TerrainUtil::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMLoadFloat4x4(&TranslateMatrix);
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &TerrainColor, 16);
}