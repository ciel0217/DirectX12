#include "CLight.h"

class DirectionalLight : public CLight
{
public:
	DirectionalLight()
	{
		m_Light.isEnable = true;
		m_Light.lightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
		m_Light.lightType = LightType::eDirLight;
		m_Light.lightDir = XMFLOAT3(0.5f, 1.0f, -1.0f);
	}
	void Config()override;
	void Init()override;
	void Uninit()override;
	void Update()override;

};