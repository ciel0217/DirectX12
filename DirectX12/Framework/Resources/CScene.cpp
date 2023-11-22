#include "CScene.h"

void CScene::Config()
{

	m_CurrentMainCamera->Config();
	
	for (int i = 0; i < eMaxLayer; i++)
	{
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Config();
		}
	}
}

void CScene::Init()
{
	m_CurrentMainCamera->Init();

	for (int i = 0; i < eMaxLayer; i++) 
	{
		for (auto gameobject : m_GameObjects[i]) {
			gameobject->Init();
		}
	}
}

void CScene::Uninit()
{
	m_CurrentMainCamera->Uninit();

	for (int i = 0; i < eMaxLayer; i++)
	{
		for (auto gameobject : m_GameObjects[i]) 
		{
			gameobject->Uninit();
		}

		//m_GameObjects[i].clear();
	}
}

void CScene::Update()
{
	m_CurrentMainCamera->Update();

	for (int i = 0; i < eMaxLayer; i++)
	{
		for (auto gameobject : m_GameObjects[i])
		{
			gameobject->Update();
		}
	}
}

void CScene::Draw()
{
	m_CurrentMainCamera->PreRender();
	m_CurrentMainCamera->Draw(m_GameObjects);
}
