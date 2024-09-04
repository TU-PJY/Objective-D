#include "TerrainUtil.h"

void TerrainUtil::SetMesh(std::string TerrainMeshName) {
	TerrainMesh = meshUtil.GetTerrain(TerrainMeshName);
	if (TerrainMesh)
		TerrainMesh->ReleaseUploadBuffers();
}

void TerrainUtil::SetShader(Shader* ShaderPtr) {
	TerrainShader = ShaderPtr;
}

bool TerrainUtil::CheckFloor(XMFLOAT3 Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			return true;
	}

	return false;
}

void TerrainUtil::ClampToFloor(XMFLOAT3& Position) {
	if(TerrainMesh)
		Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
}

void TerrainUtil::CheckCollision(XMFLOAT3& Position) {
	if (TerrainMesh) {
		if (Position.y < TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix))
			Position.y = TerrainMesh->GetHeightAtPosition(TerrainMesh, Position.x, Position.z, TranslateMatrix);
	}
}

void TerrainUtil::SetColor(XMFLOAT3 ColorValue) {
	TerrainColor.x = ColorValue.x;
	TerrainColor.y = ColorValue.y;
	TerrainColor.z = ColorValue.z;
}

void TerrainUtil::Scale(XMFLOAT3 SizeValue) {
	XMMATRIX ScaleMat = XMMatrixScaling(SizeValue.x, SizeValue.y, SizeValue.z);
	TranslateMatrix = Mat4::Multiply(ScaleMat, TranslateMatrix);
}

void TerrainUtil::Render(ID3D12GraphicsCommandList* CmdList) {
	if (TerrainShader)
		TerrainShader->Render(CmdList);

	UpdateShaderVariables(CmdList);

	if (TerrainMesh)
		TerrainMesh->Render(CmdList);
}

void TerrainUtil::UpdateShaderVariables(ID3D12GraphicsCommandList* CmdList) {
	XMMATRIX ResultMatrix = XMLoadFloat4x4(&TranslateMatrix);
	XMFLOAT4X4 xmf4x4World;
	XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(ResultMatrix));
	CmdList->SetGraphicsRoot32BitConstants(1, 16, &xmf4x4World, 0);
	CmdList->SetGraphicsRoot32BitConstants(1, 3, &TerrainColor, 16);
}