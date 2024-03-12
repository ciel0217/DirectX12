#include "TestScene.h"
#include "TestObject.h"
#include "Framework/Resources/Camera.h"
#include "Framework/Resources/DirectionalLight.h"

void TestScene::SetObject()
{
	AddGameObject<Camera>(e3D);
	AddGameObject<TestObject>(e3D);
	AddGameObject<DirectionalLight>(e2D);
}
