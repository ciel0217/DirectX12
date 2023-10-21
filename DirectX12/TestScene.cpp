#include "TestScene.h"
#include "TestObject.h"
#include "Framework/Resources/Camera.h"

void TestScene::SetObject()
{
	AddGameObject<Camera>(e3D);
	AddGameObject<TestObject>(e3D);
}
