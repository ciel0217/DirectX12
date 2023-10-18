#pragma once
#include "CRender.h"

struct WorldMatrix
{
	XMMATRIX w;
};


class Model;

class MeshRender : public CRender
{
private:
	std::shared_ptr<Model> m_Model;
public:
	void SetUpRender()override;
	void Draw()override;
};
