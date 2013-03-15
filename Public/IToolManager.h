#ifndef ITOOL_MANAGER_H
# define ITOOL_MANAGER_H

interface_class IToolManager
{
public:
	virtual void									registerTool(ITool*         pTool,
                                             IUIButton*     pButton,
                                             bool           bGlobal = false) = 0;

	virtual bool									onMouseInput(const s_inputDatas& datas, IViewport* pViewport) = 0;
	virtual void									onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport) = 0;
	virtual void									onMouseLeave() = 0;

	//virtual void									selectTool(const std::string& sTool) = 0;
	//virtual void									selectTool(const std::string& sTool, void* pDatas) = 0;

	virtual void									tick() = 0;
};

#endif // !ITOOL_MANAGER_H