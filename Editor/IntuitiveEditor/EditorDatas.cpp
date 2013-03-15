#include "stdafx.h"

#include "EditorDatas.h"

std::vector<ISelectableTarget*>									CEditorDatas::m_vecPickedUnits = std::vector<ISelectableTarget*>();
float																						CEditorDatas::m_fTerrainWorldSize = 0.0f;
long																						CEditorDatas::m_iTerrainSize = 0;
//bool																						CEditorDatas::m_bProjectLoaded = false;
