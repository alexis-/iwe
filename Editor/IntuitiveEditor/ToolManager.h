#ifndef TOOL_MANAGER_H
# define TOOL_MANAGER_H


class CToolManager : public IToolManager
{
public:
	static CToolManager*					getInstance();

public:
	ITool*												getTool(std::string sName);
  bool                          isToolEnabled(ITool* pTool);

public:

	virtual bool									onMouseInput(const s_inputDatas& datas, IViewport* pViewport);
	virtual void									onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport);
	virtual void									onMouseLeave();
  
	virtual void									registerTool(ITool*         pTool,
                                             IUIButton*     pButton,
                                             bool           bGlobal = false);

	virtual void									selectTool(const std::string& sTool);
	//virtual void									selectTool(const std::string& sTool, void* pDatas);

	virtual void									tick();

private:
	CToolManager();
	static CToolManager*					m_pInst;

	std::map<std::string, ITool*>	m_mapTools;
	std::map<std::string, ITool*>	m_mapGlobalTools;
	ITool*												pSelectedTool;
};

#endif // !TOOL_MANAGER_H