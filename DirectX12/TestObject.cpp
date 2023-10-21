#include "TestObject.h"
#include "Framework/Resources/Model.h"
#include "Framework/Resources/MaterialManager.h"

void TestObject::Config()
{
	m_Model.reset(new Model("Asset/Model/Sphere.obj"));
	m_Model->LoadModel();

	m_Material = MaterialManager::GetInstance()->CreateMaterial("test", "Framework/Shader/TestVertex.cso", "Framework/Shader/TestPixel.cso", MaterialManager::OPACITY_RENDER_QUEUE);
}

void TestObject::Init()
{
	m_Position = XMFLOAT3(0, 0, 0);
	m_Scale = XMFLOAT3(1, 1, 1);
	m_Rotation = XMQuaternionIdentity();
}

void TestObject::Uninit()
{
}

void TestObject::Update()
{
}
