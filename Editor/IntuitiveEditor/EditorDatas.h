#ifndef EDITOR_DATAS_H
# define EDITOR_DATAS_H

static_class CEditorDatas
{
public:
	static std::vector<ISelectableTarget*>    						m_vecPickedUnits;
	static long																						m_iTerrainSize;
	static float																					m_fTerrainWorldSize;
	//static bool																						m_bProjectLoaded;
};

#endif // !EDITOR_DATAS_H