#include "stdafx.h"
#include "SSceneInitParameters.h"

void SSceneInitParameters::print() const {
	printf("\nSSceneInitParameter hash: 0x%I64X\n", std::hash<SSceneInitParameters>()(*this));
	printf("\t%s\n", m_SceneResource.to_string().c_str());
	for (int i = 0; i < m_aAdditionalBrickResources.size(); ++i)
		printf("\t\t%s\n", m_aAdditionalBrickResources[i].to_string().c_str());
}
