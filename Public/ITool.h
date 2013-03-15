#ifndef ITOOL_H
# define ITOOL_H

abstract_class ITool
{
public:
	virtual std::string						getName() = 0;
	
	virtual bool									onMouseInput(const s_inputDatas& datas, IViewport* pViewport) = 0;
	virtual void									onKeyboardInput(IWEKeys::e_IWEKey nChar, bool keyDown, int repeatCount, IViewport* pViewport) = 0;
	virtual void									onMouseLeave() = 0;

	virtual void									onToolChange() = 0;
	virtual void									onToolActivate() = 0;

	virtual void									tick() = 0;
};

#endif // !ITOOL_H