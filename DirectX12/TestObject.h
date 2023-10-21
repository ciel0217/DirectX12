#pragma once
#include "Framework/Resources/CGameObject.h"
#include "Framework/Render/MeshRender.h"

class TestObject : public CGameObject, public MeshRender
{
public:
	TestObject() : MeshRender(this) {}
	~TestObject(){}
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;
};