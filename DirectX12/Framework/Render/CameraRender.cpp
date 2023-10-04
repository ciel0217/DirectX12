#include "CameraRender.h"
#include "DefferedRender.h"

void CameraRender::SetUpRender()
{
	m_CurrentRender = new DefferedRender();
	m_CurrentRender->SetUpRender();
}

void CameraRender::UninitRender()
{
	delete m_CurrentRender;
}

void CameraRender::Draw()
{
}
