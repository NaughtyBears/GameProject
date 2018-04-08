#include "d3dTool.h"
#include <d3dx9.h>
#include <tchar.h>

//-----------------------------------�����ļ��������֡�---------------------------------------
//	���������������������Ŀ��ļ�
//------------------------------------------------------------------------------------------------
#pragma comment(lib,"winmm.lib")  //����PlaySound����������ļ�
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

//-----------------------------------��ȫ�ֱ������岿�֡�-------------------------------------
//	������ȫ�ֱ����Ķ���
//------------------------------------------------------------------------------------------------
int CWindow::g_pwindowWidth = 0;
int CWindow::g_pwindowHeight = 0;
WNDCLASSEX  CWindow::g_pwndClass = { 0 };
LPDIRECT3DDEVICE9 CWindow::g_pd3dDevice = NULL;
//�����ĸ���̬����
float  CWindow::fps = 0; //������Ҫ�����FPSֵ
int     CWindow::frameCount = 0;//֡��
float  CWindow::currentTime = 0.0f;//��ǰʱ��
float  CWindow::lastTime = 0.0f;//����ʱ��

/////////////////////////////////////////////////////////////////////////////////��D3DTool Class ʵ�֡�///////////////////////////////////////////////////////////////////////
//-----------------------------------��Matrix_Set( )������--------------------------------------
//	��������װ��Direct3D�Ĵ�任�ĺ�����������任��ȡ���任��ͶӰ�任���ӿڱ任������
//--------------------------------------------------------------------------------------------------
VOID D3DTool::Matrix_Set()
{
	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮һ��������任���������
	//--------------------------------------------------------------------------------------


	//--------------------------------------------------------------------------------------
	//���Ĵ�任֮������ȡ���任���������
	//--------------------------------------------------------------------------------------
	D3DXMATRIX matView; //����һ������
	D3DXVECTOR3 vEye(0.0f, 0.0f, -15.0f);  //�������λ��
	D3DXVECTOR3 vAt(0.0f, 0.0f, 0.0f); //�۲���λ��
	D3DXVECTOR3 vUp(0.0f, 1.0f, 0.0f);//���ϵ�����
	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp); //�����ȡ���任����
	CWindow::g_pd3dDevice->SetTransform(D3DTS_VIEW, &matView); //Ӧ��ȡ���任����

													  //--------------------------------------------------------------------------------------
													  //���Ĵ�任֮������ͶӰ�任���������
													  //--------------------------------------------------------------------------------------
	D3DXMATRIX matProj; //����һ������
	D3DXMatrixPerspectiveFovLH(&matProj, D3DX_PI / 4.0f, 1.0f, 1.0f, 1000.0f); //����ͶӰ�任����
	CWindow::g_pd3dDevice->SetTransform(D3DTS_PROJECTION, &matProj);  //����ͶӰ�任����

															 //--------------------------------------------------------------------------------------
															 //���Ĵ�任֮�ġ����ӿڱ任������
															 //--------------------------------------------------------------------------------------
	D3DVIEWPORT9 vp; //ʵ����һ��D3DVIEWPORT9�ṹ�壬Ȼ��������������������ֵ�Ϳ�����
	vp.X = 0;		//��ʾ�ӿ�����ڴ��ڵ�X����
	vp.Y = 0;		//�ӿ���ԶԴ��ڵ�Y����
	vp.Width = CWindow::g_pwindowWidth;	//�ӿڵĿ��
	vp.Height = CWindow::g_pwindowHeight; //�ӿڵĸ߶�
	vp.MinZ = 0.0f; //�ӿ�����Ȼ����е���С���ֵ
	vp.MaxZ = 1.0f;	//�ӿ�����Ȼ����е�������ֵ
	CWindow::g_pd3dDevice->SetViewport(&vp); //�ӿڵ�����

}



//-----------------------------------��ȫ�ֺ���Direct3D_Init( )��--------------------------------------
//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
//------------------------------------------------------------------------------------------------
HRESULT D3DTool::Direct3D_Init(HWND hwnd)
{
	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮һ�����ӿڡ�������Direct3D�ӿڶ���, �Ա��ø�Direct3D���󴴽�Direct3D�豸����
	//--------------------------------------------------------------------------------------
	LPDIRECT3D9  pD3D = NULL; //Direct3D�ӿڶ���Ĵ���
	if (NULL == (pD3D = Direct3DCreate9(D3D_SDK_VERSION))) //��ʼ��Direct3D�ӿڶ��󣬲�����DirectX�汾Э��
		return E_FAIL;

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮��,ȡ��Ϣ������ȡӲ���豸��Ϣ
	//--------------------------------------------------------------------------------------
	D3DCAPS9 caps; int vp = 0;
	if (FAILED(pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps)))
	{
		return E_FAIL;
	}
	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;   //֧��Ӳ���������㣬���ǾͲ���Ӳ���������㣬���׵�
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING; //��֧��Ӳ���������㣬����ֻ�ò��������������

	//--------------------------------------------------------------------------------------
	// ��Direct3D��ʼ���Ĳ���֮���������ݡ������D3DPRESENT_PARAMETERS�ṹ��
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
	// ��Direct3D��ʼ���Ĳ���֮�ģ����豸��������Direct3D�豸�ӿ�
	//--------------------------------------------------------------------------------------
	//HRESULT		typedef long HRESULT
	if (FAILED(pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		hwnd, vp, &d3dpp, &CWindow::g_pd3dDevice)))
		return E_FAIL;

	SAFE_RELEASE(pD3D) //LPDIRECT3D9�ӿڶ����ʹ����ɣ����ǽ����ͷŵ�

		if (!(S_OK == Objects_Init(hwnd))) return E_FAIL;     //����һ��Objects_Init��������Ⱦ��Դ�ĳ�ʼ��
	return S_OK;
}

//-----------------------------------��ȫ�ֺ���Object_Init( )��--------------------------------------
//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
//--------------------------------------------------------------------------------------------------
HRESULT D3DTool::Objects_Init(HWND hwnd)
{


	return S_OK;
}

//-----------------------------------��ȫ�ֺ���Direct3D_CleanUp( )��--------------------------------
//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
//---------------------------------------------------------------------------------------------------
void D3DTool::Direct3D_CleanUp()
{



}

//-----------------------------------��ȫ�ֺ���Direct3D_Render( )��--------------------------------------
//	������ʹ��Direct3D������Ⱦ
//--------------------------------------------------------------------------------------------------
void D3DTool::Direct3D_Render(HWND hwnd)
{



}

//-----------------------------------��Get_FPS( )������------------------------------------------
//	���������ڼ���ÿ��֡���ʵ�һ������
//--------------------------------------------------------------------------------------------------
float D3DTool::Get_FPS()
{

	CWindow::frameCount++;//ÿ����һ��Get_FPS()������֡������1
	CWindow::currentTime = timeGetTime()*0.001f;//��ȡϵͳʱ�䣬����timeGetTime�������ص����Ժ���Ϊ��λ��ϵͳʱ�䣬������Ҫ����0.001���õ���λΪ���ʱ��

 //�����ǰʱ���ȥ����ʱ�������1���ӣ��ͽ���һ��FPS�ļ���ͳ���ʱ��ĸ��£�����֡��ֵ����
	if (CWindow::currentTime - CWindow::lastTime > 1.0f) //��ʱ�������1����
	{
		CWindow::fps = (float)CWindow::frameCount / (CWindow::currentTime - CWindow::lastTime);//������1���ӵ�FPSֵ
		CWindow::lastTime = CWindow::currentTime; //����ǰʱ��currentTime��������ʱ��lastTime����Ϊ��һ��Ļ�׼ʱ��
		CWindow::frameCount = 0;//������֡��frameCountֵ����
	}

	return CWindow::fps;
}

//-----------------------------------������&&������--------------------------------------
//	������D3DTool��Ĺ���&&����������ʵ��
//------------------------------------------------------------------------------------------------
D3DTool::~D3DTool()
{

}


/////////////////////////////////////////////////////////////////////////////////��CWindow Class ʵ�֡�///////////////////////////////////////////////////////////////////////


//-----------------------------------��ȫ�ֺ��� WndProc( )��--------------------------------------
//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д���
//------------------------------------------------------------------------------------------------
LRESULT CALLBACK CWindow::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)						//switch��俪ʼ
	{
	case WM_PAINT:						// ���ǿͻ����ػ���Ϣ
		D3DTool::Direct3D_Render(hwnd);                 //����Direct3D��Ⱦ����
		ValidateRect(hwnd, NULL);		// ���¿ͻ�������ʾ
		break;									//������switch���

	case WM_KEYDOWN:					// ���Ǽ��̰�����Ϣ
		if (wParam == VK_ESCAPE)    // ��������µļ���ESC
			DestroyWindow(hwnd);		// ���ٴ���, ������һ��WM_DESTROY��Ϣ
		break;									//������switch���

	case WM_DESTROY:					//���Ǵ���������Ϣ
		D3DTool::Direct3D_CleanUp();			//�����Զ������Դ������Game_CleanUp���������˳�ǰ����Դ����
		PostQuitMessage(0);			//��ϵͳ�����и��߳�����ֹ����������ӦWM_DESTROY��Ϣ
		break;									//������switch���

	default:										//������case�����������ϣ���ִ�и�default���
		return DefWindowProc(hwnd, message, wParam, lParam);		//����ȱʡ�Ĵ��ڹ���
	}

	return 0;									//�����˳�
}

//-----------------------------------������&&������--------------------------------------
//	������CWindow��Ĺ���&&����������ʵ��
//------------------------------------------------------------------------------------------------
//Ĭ�Ϲ��캯��,Ĭ�ϴ��� 800*600�Ĵ���
CWindow::CWindow()
{
	m_windowWidth = 800;
	m_windowHeight = 600;
	m_windowTitle = "��ʼ������";

	CWindow::g_pwindowWidth = m_windowWidth;
	CWindow::g_pwindowHeight = m_windowHeight;
	
}
//��������
CWindow::~CWindow()
{

}


//-----------------------------------��windowBuilt( )������--------------------------------------
//	������Windows���ڵĴ�������
//------------------------------------------------------------------------------------------------
HRESULT CWindow::windowBuilt(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) const
{
	CWindow::g_pwndClass.cbSize = sizeof(WNDCLASSEX);
	CWindow::g_pwndClass.style = CS_HREDRAW | CS_VREDRAW;
	CWindow::g_pwndClass.lpfnWndProc = WndProc;
	CWindow::g_pwndClass.cbClsExtra = 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
	CWindow::g_pwndClass.cbWndExtra = 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
	CWindow::g_pwndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
	CWindow::g_pwndClass.hIcon = (HICON)::LoadImage(NULL, "icon.ico", IMAGE_ICON, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��
	CWindow::g_pwndClass.hCursor = LoadCursor(NULL, IDC_ARROW);    //ָ��������Ĺ������
	CWindow::g_pwndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
	CWindow::g_pwndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
	CWindow::g_pwndClass.lpszClassName = "��ʼ������";		//��һ���Կ���ֹ���ַ�����ָ������������֡�

	//��2�����ڴ����Ĳ���֮����ע�ᴰ����
	if (!RegisterClassEx(&CWindow::g_pwndClass))				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		return -1;

	//��3�����ڴ����Ĳ���֮������ʽ��������
	HWND hwnd = CreateWindowW(L"��ʼ������", WINDOW_TITLE,				//ϲ���ּ��Ĵ������ں���CreateWindow
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, m_windowWidth,
		m_windowHeight, NULL, NULL, hInstance, NULL);

	//Direct3D��Դ�ĳ�ʼ�����ɹ�����ʧ�ܶ���messagebox������ʾ
	if (S_OK == D3DTool::Direct3D_Init(hwnd))
	{
		MessageBox(hwnd, "Direct3D��ʼ�����~��", "NaughtyBear����Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ����  
	}
	else
	{
		MessageBox(hwnd, "Direct3D��ʼ��ʧ��~��", "NaughtyBear����Ϣ����", 0); //ʹ��MessageBox����������һ����Ϣ���� 
	}

	//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
	MoveWindow(hwnd, 250, 80, m_windowWidth, m_windowHeight, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
	ShowWindow(hwnd, nShowCmd);    //����ShowWindow��������ʾ����
	UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��


	return S_OK;
}

//-----------------------------------��windowDestroy( )������--------------------------------------
//	������Windows���ڵ�ע��
//------------------------------------------------------------------------------------------------
void CWindow::windowDestroy() const
{
	//�������ע��
	UnregisterClass("��ʼ������", g_pwndClass.hInstance);  //����׼��������ע��������
}

//��ȡֵ
string CWindow::getTitleName() const
{

	return m_windowTitle;
}