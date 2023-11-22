#pragma once
#include <list>
#include <typeinfo>
#include <type_traits>
#include "Camera.h"
#include <memory>


enum Layer
{
	e2D = 0,
	e3D,
	eMaxLayer
};
							
class CScene
{

protected:
	std::shared_ptr<Camera> m_CurrentMainCamera;
	std::list<CGameObject *> m_GameObjects[eMaxLayer];				//ゲームオブジェクトのすべて

public:

	CScene() {}
	virtual ~CScene()
	{
		for (int i = 0; i < eMaxLayer; i++)
		{
			
		}
	}

	virtual void SetObject() = 0;
	virtual void Config();
	virtual void Init();
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();

	template <typename T>
	T* AddGameObject(int layer) {
		T* obj = new T();

		Camera* camera = dynamic_cast<Camera*>(obj);
		if (camera) {
			m_CurrentMainCamera.reset(camera);
			return obj;
		}

		/*if (typeid(*obj) == typeid(SkyBox)) {
			for (auto camera : m_SceneCameras) {
				camera->SetSkyBox((SkyBox*)obj);
			}
			return obj;
		}*/

		m_GameObjects[layer].push_back(obj);

		return obj;
	}

	template <typename T>
	T* GetGameObject(int Layer)
	{
		for (auto gameobject : m_GameObjects[Layer]) {
			if (typeid(*gameobject) == typeid(T))//型を調べる
			{
				return (T*)gameobject;
			}
		}

		return NULL;
	}

	std::list<CGameObject *>* GetGameObjectAll(){ return m_GameObjects; }

	std::shared_ptr<Camera> GetCurrentMainCamera() { return m_CurrentMainCamera; }
};

