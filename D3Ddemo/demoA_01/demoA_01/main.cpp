#include "d3dTool.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//创建CWindow类，并使用默认构造函数创建800*600的窗口
	CWindow cw;

	//窗口的创建
	if (FAILED(cw.windowBuilt(hInstance, hPrevInstance, lpCmdLine, nShowCmd)))
	{
		return E_FAIL;
	}

	//消息循环过程
	MSG msg = { 0 }; 
	while (msg.message != WM_QUIT)	
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage(&msg);		//将虚拟键消息转换为字符消息
			DispatchMessage(&msg);		//该函数分发一个消息给窗口程序。
		}
		else
		{
			D3DTool::Direct3D_Render(NULL);  
		}
	}

	//窗口类的注销
	cw.windowDestroy();

	return 0;
}