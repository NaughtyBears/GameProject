//-----------------------------------【API说明】----------------------------------------------
//  API名称:：D3DAPI
//	 2018年3月 Create by NaughtyBear
//  描述：创建窗口类 && 初始化Direct3D
//------------------------------------------------------------------------------------------------

#ifndef _D3D_TOOL_H
#define _D3D_TOOL_H

#include <string>
#include <d3d9.h>
using namespace std;


#define WINDOW_TITLE	L"【致我们永不熄灭的游戏开发梦想】"	//为窗口标题定义的宏
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }   //定义一个安全释放宏，便于后面COM接口指针的释放

class CWindow
{
public:
	CWindow();//默认构造函数,默认创建 800*600的窗口
	~CWindow();

	HRESULT windowBuilt(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) const; //	描述：Windows窗口的创建函数
	void windowDestroy() const;//	描述：Windows窗口的注销
	string getTitleName() const;


	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//	描述：窗口过程函数WndProc,对窗口消息进行处理，此处仅为默认的窗口过程函数


//成员变量
private:
	
	string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;

public:
//全局变量
	static LPDIRECT3DDEVICE9 g_pd3dDevice;
	static WNDCLASSEX g_pwndClass;
	static int g_pwindowWidth;
	static int g_pwindowHeight;
	static float  fps; //我们需要计算的FPS值
	static int     frameCount ;//帧数
	static float  currentTime ;//当前时间
	static float  lastTime ;//持续时间
};


//纯虚函数D3DTool，用于被继承，被子类重写接口
class D3DTool
{
public:
	virtual ~D3DTool();

public:
	static void Direct3D_Render(HWND hwnd);//	描述：使用Direct3D进行渲染
	static void Direct3D_CleanUp();//	描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
	static HRESULT Objects_Init(HWND hwnd);//	描述：渲染资源初始化函数，在此函数中进行要被渲染的物体的资源的初始化
	static HRESULT Direct3D_Init(HWND hwnd);//	描述：Direct3D初始化函数，进行Direct3D的初始化
	static VOID Matrix_Set();//	描述：封装了Direct3D四大变换的函数，即世界变换，取景变换，投影变换，视口变换的设置

protected:
	static float Get_FPS();

//接口
public:
	virtual HRESULT ObjectsInit(HWND hwnd) = 0;
	virtual HRESULT Direct3DInit(HWND hwnd) = 0;
	virtual void Direct3DRender(HWND hwnd) = 0;
	virtual void Direct3DCleanup() = 0;
	virtual VOID MatrixSet() = 0;
};


#endif // !_D3D_TOOL_H



