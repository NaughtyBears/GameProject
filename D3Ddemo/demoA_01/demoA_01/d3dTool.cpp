#include "d3dTool.h"
#include <d3dx9.h>
#include <tchar.h>

//-----------------------------------【库文件包含部分】---------------------------------------
//	描述：包含程序所依赖的库文件
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //调用PlaySound函数所需库文件
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-----------------------------------【全局变量定义部分】-------------------------------------
//	描述：全局变量的定义
//------------------------------------------------------------------------------------------------
int CWindow::g_pwindowWidth = 0;
int CWindow::g_pwindowHeight = 0;
WNDCLASSEX  CWindow::g_pwndClass = { 0 };
LPDIRECT3DDEVICE9 CWindow::g_pd3dDevice = NULL;
//定义四个静态变量
float  CWindow::fps = 0; //我们需要计算的FPS值
int     CWindow::frameCount = 0;//帧数
float  CWindow::currentTime = 0.0f;//当前时间
float  CWindow::lastTime = 0.0f;//持续时间

/////////////////////////////////////////////////////////////////////////////////【D3DTool Class 实现】///////////////////////////////////////////////////////////////////////
//-----------------------------------【Matrix_Set( )函数】--------------------------------------
//	描述：封装了Direct3D四大变换的函数，即世界变换，取景变换，投影变换，视口变换的设置
//--------------------------------------------------------------------------------------------------
VOID D3DTool::Matrix_Set()
{
	//--------------------------------------------------------------------------------------
	//【四大变换之一】：世界变换矩阵的设置
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//【四大变换之二】：取景变换矩阵的设置
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matView; //定义一个矩阵
	D3DXVECTOR3 vEye(0.0f, 0.0f, -15.0f);  //摄像机的位置
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); //观察点的位置
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);//向上的向量
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); //计算出取景变换矩阵
	CWindow::g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); //应用取景变换矩阵

													  //--------------------------------------------------------------------------------------
													  //【四大变换之三】：投影变换矩阵的设置
													  //--------------------------------------------------------------------------------------
	D3DXMATRIX matProj; //定义一个矩阵
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f); //计算投影变换矩阵
	CWindow::g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);  //设置投影变换矩阵

															 //--------------------------------------------------------------------------------------
															 //【四大变换之四】：视口变换的设置
															 //--------------------------------------------------------------------------------------
	D3DVIEWPORT9 vp; //实例化一个D3DVIEWPORT9结构体，然后做填空题给各个参数赋值就可以了
	vp.X = 0;		//表示视口相对于窗口的X坐标
	vp.Y = 0;		//视口相对对窗口的Y坐标
	vp.Width = CWindow::g_pwindowWidth;	//视口的宽度
	vp.Height = CWindow::g_pwindowHeight; //视口的高度
	vp.MinZ = 0.0f; //视口在深度缓存中的最小深度值
	vp.MaxZ = 1.0f;	//视口在深度缓存中的最大深度值
	CWindow::g_pd3dDevice->SetViewport(&vp); //视口的设置

}



//-----------------------------------【全局函数Direct3D_Init( )】--------------------------------------
//	描述：Direct3D初始化函数，进行Direct3D的初始化
//------------------------------------------------------------------------------------------------
HRESULT D3DTool::Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之一，创接口】：创建Direct3D接口对象, 以便用该Direct3D对象创建Direct3D设备对象
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D接口对象的创建
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //初始化Direct3D接口对象，并进行DirectX版本协商
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之二,取信息】：获取硬件设备信息
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //支持硬件顶点运算，我们就采用硬件顶点运算，妥妥的
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //不支持硬件顶点运算，无奈只好采用软件顶点运算

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之三，填内容】：填充D3DPRESENT_PARAMETERS结构体
	//--------------------------------------------------------------------------------------
	D3DPRESENT_PARAMETERS d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = CWindow::g_pwindowWidth;
	d3dpp.BackBufferHeight = CWindow::g_pwindowHeight;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = true;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = 0;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	//--------------------------------------------------------------------------------------
	// 【Direct3D初始化四步曲之四，创设备】：创建Direct3D设备接口
	//--------------------------------------------------------------------------------------
	//HRESULT		typedef long HRESULT
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &CWindow::g_pd3dDevice)))
		return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9接口对象的使命完成，我们将其释放掉

		if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //调用一次Objects_Init，进行渲染资源的初始化
	return S_OK;
}

//-----------------------------------【全局函数Object_Init( )】--------------------------------------
//	描述：渲染资源初始化函数，在此函数中进行要被渲染的物体的资源的初始化
//--------------------------------------------------------------------------------------------------
HRESULT D3DTool::Objects_Init(HWND hwnd)
{


	return S_OK;
}

//-----------------------------------【全局函数Direct3D_CleanUp( )】--------------------------------
//	描述：资源清理函数，在此函数中进行程序退出前资源的清理工作
//---------------------------------------------------------------------------------------------------
void D3DTool::Direct3D_CleanUp()
{



}

//-----------------------------------【全局函数Direct3D_Render( )】--------------------------------------
//	描述：使用Direct3D进行渲染
//--------------------------------------------------------------------------------------------------
void D3DTool::Direct3D_Render(HWND hwnd)
{



}

//-----------------------------------【Get_FPS( )函数】------------------------------------------
//	描述：用于计算每秒帧速率的一个函数
//--------------------------------------------------------------------------------------------------
float D3DTool::Get_FPS()
{

	CWindow::frameCount++;//每调用一次Get_FPS()函数，帧数自增1
	CWindow::currentTime = timeGetTime()*0.001f;//获取系统时间，其中timeGetTime函数返回的是以毫秒为单位的系统时间，所以需要乘以0.001，得到单位为秒的时间

 //如果当前时间减去持续时间大于了1秒钟，就进行一次FPS的计算和持续时间的更新，并将帧数值清零
	if (CWindow::currentTime - CWindow::lastTime > 1.0f) //将时间控制在1秒钟
	{
		CWindow::fps = (float)CWindow::frameCount / (CWindow::currentTime - CWindow::lastTime);//计算这1秒钟的FPS值
		CWindow::lastTime = CWindow::currentTime; //将当前时间currentTime赋给持续时间lastTime，作为下一秒的基准时间
		CWindow::frameCount = 0;//将本次帧数frameCount值清零
	}

	return CWindow::fps;
}

//-----------------------------------【构造&&析构】--------------------------------------
//	描述：D3DTool类的构造&&析构函数的实现
//------------------------------------------------------------------------------------------------
D3DTool::~D3DTool()
{

}


/////////////////////////////////////////////////////////////////////////////////【CWindow Class 实现】///////////////////////////////////////////////////////////////////////


//-----------------------------------【全局函数 WndProc( )】--------------------------------------
//	描述：窗口过程函数WndProc,对窗口消息进行处理
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK CWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch语句开始
	{
	case WM_PAINT:						// 若是客户区重绘消息
		D3DTool::Direct3D_Render(hwnd);                 //调用Direct3D渲染函数
		ValidateRect(hwnd, NULL);		// 更新客户区的显示
		break;									//跳出该switch语句

	case WM_KEYDOWN:					// 若是键盘按下消息
		if (wParam == VK_ESCAPE)    // 如果被按下的键是ESC
			DestroyWindow(hwnd);		// 销毁窗口, 并发送一条WM_DESTROY消息
		break;									//跳出该switch语句

	case WM_DESTROY:					//若是窗口销毁消息
		D3DTool::Direct3D_CleanUp();			//调用自定义的资源清理函数Game_CleanUp（）进行退出前的资源清理
		PostQuitMessage(0);			//向系统表明有个线程有终止请求。用来响应WM_DESTROY消息
		break;									//跳出该switch语句

	default:										//若上述case条件都不符合，则执行该default语句
		return DefWindowProc(hwnd, message, wParam, lParam);		//调用缺省的窗口过程
	}

	return 0;									//正常退出
}

//-----------------------------------【构造&&析构】--------------------------------------
//	描述：CWindow类的构造&&析构函数的实现
//------------------------------------------------------------------------------------------------
//默认构造函数,默认创建 800*600的窗口
CWindow::CWindow()
{
	m_windowWidth = 800;
	m_windowHeight = 600;
	m_windowTitle = "初始化窗口";

	CWindow::g_pwindowWidth = m_windowWidth;
	CWindow::g_pwindowHeight = m_windowHeight;
	
}
//析构函数
CWindow::~CWindow()
{

}


//-----------------------------------【windowBuilt( )函数】--------------------------------------
//	描述：Windows窗口的创建函数
//------------------------------------------------------------------------------------------------
HRESULT CWindow::windowBuilt(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) const
{
	CWindow::g_pwndClass.cbSize = sizeof(WNDCLASSEX);
	CWindow::g_pwndClass.style = CS_HREDRAW | CS_VREDRAW;
	CWindow::g_pwndClass.lpfnWndProc = WndProc;
	CWindow::g_pwndClass.cbClsExtra = 0;								//窗口类的附加内存，取0就可以了
	CWindow::g_pwndClass.cbWndExtra = 0;							//窗口的附加内存，依然取0就行了
	CWindow::g_pwndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
	CWindow::g_pwndClass.hIcon = (HICON)::LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //本地加载自定义ico图标
	CWindow::g_pwndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //指定窗口类的光标句柄。
	CWindow::g_pwndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
	CWindow::g_pwndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
	CWindow::g_pwndClass.lpszClassName = "初始化窗口";		//用一个以空终止的字符串，指定窗口类的名字。

	//【2】窗口创建四步曲之二：注册窗口类
	if (!RegisterClassEx(&CWindow::g_pwndClass))				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		return -1;

	//【3】窗口创建四步曲之三：正式创建窗口
	HWND hwnd = CreateWindowW(L"初始化窗口", WINDOW_TITLE,				//喜闻乐见的创建窗口函数CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_windowWidth,
		m_windowHeight, NULL, NULL, hInstance, NULL);

	//Direct3D资源的初始化，成功或者失败都用messagebox予以显示
	if (S_OK == D3DTool::Direct3D_Init(hwnd))
	{
		MessageBox(hwnd, "Direct3D初始化完成~！", "NaughtyBear的消息窗口", 0); //使用MessageBox函数，创建一个消息窗口  
	}
	else
	{
		MessageBox(hwnd, "Direct3D初始化失败~！", "NaughtyBear的消息窗口", 0); //使用MessageBox函数，创建一个消息窗口 
	}

	//【4】窗口创建四步曲之四：窗口的移动、显示与更新
	MoveWindow(hwnd, 250, 80, m_windowWidth, m_windowHeight, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
	ShowWindow(hwnd, nShowCmd);    //调用ShowWindow函数来显示窗口
	UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样


	return S_OK;
}

//-----------------------------------【windowDestroy( )函数】--------------------------------------
//	描述：Windows窗口的注销
//------------------------------------------------------------------------------------------------
void CWindow::windowDestroy() const
{
	//窗口类的注销
	UnregisterClass("初始化窗口", g_pwndClass.hInstance);  //程序准备结束，注销窗口类
}

//获取值
string CWindow::getTitleName() const
{

	return m_windowTitle;
}