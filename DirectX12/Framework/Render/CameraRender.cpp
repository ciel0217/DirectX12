#include "CameraRender.h"
#include "DefferedRender.h"
#include "../Resources/CGameObject.h"
#include "CRender.h"
#include "../Resources/Material.h"

void CameraRender::SetUpRender()
{
	m_CurrentRender = new DefferedRender();
	m_CurrentRender->SetUpRender();
}

void CameraRender::UninitRender()
{
	delete m_CurrentRender;
}

void CameraRender::Draw(std::list<CGameObject*> gameObjects[])
{
	std::list<CRender*> opacityList;//�s����
	std::list<CRender*> transparentList;//������

	
	//3D��2D��Layer�����Ă邽��
	for (int i = 0; i < 2; i++)
	{
		for (auto gameObject : gameObjects[i])
		{
			CRender* render = dynamic_cast<CRender*>(gameObject);
			if (!render)continue;

			//TODO::2D�Ή����ĂȂ���
			if (render->GetMaterial()->GetRenderQueue() < OPACITY_RENDER_QUEUE)
				opacityList.push_back(render);
			else
				transparentList.push_back(render);
		}
	}

	opacityList.sort();
	transparentList.sort();

	for (auto gameObject : opacityList)
	{
		gameObject->Draw();
	}

}
