//-----------------------------------��API˵����----------------------------------------------
//  API����:��D3DAPI
//	 2018��3�� Create by NaughtyBear
//  ���������������� && ��ʼ��Direct3D
//------------------------------------------------------------------------------------------------

#ifndef _D3D_TOOL_H
#define _D3D_TOOL_H

#include <string>
#include <d3d9.h>
using namespace std;


#define WINDOW_TITLE	L"������������Ϩ�����Ϸ�������롿"	//Ϊ���ڱ��ⶨ��ĺ�
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }   //����һ����ȫ�ͷź꣬���ں���COM�ӿ�ָ����ͷ�

class CWindow
{
public:
	CWindow();//Ĭ�Ϲ��캯��,Ĭ�ϴ��� 800*600�Ĵ���
	~CWindow();

	HRESULT windowBuilt(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) const; //	������Windows���ڵĴ�������
	void windowDestroy() const;//	������Windows���ڵ�ע��
	string getTitleName() const;


	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);//	���������ڹ��̺���WndProc,�Դ�����Ϣ���д����˴���ΪĬ�ϵĴ��ڹ��̺���


//��Ա����
private:
	
	string m_windowTitle;
	int m_windowWidth;
	int m_windowHeight;

public:
//ȫ�ֱ���
	static LPDIRECT3DDEVICE9 g_pd3dDevice;
	static WNDCLASSEX g_pwndClass;
	static int g_pwindowWidth;
	static int g_pwindowHeight;
	static float  fps; //������Ҫ�����FPSֵ
	static int     frameCount ;//֡��
	static float  currentTime ;//��ǰʱ��
	static float  lastTime ;//����ʱ��
};


//���麯��D3DTool�����ڱ��̳У���������д�ӿ�
class D3DTool
{
public:
	virtual ~D3DTool();

public:
	static void Direct3D_Render(HWND hwnd);//	������ʹ��Direct3D������Ⱦ
	static void Direct3D_CleanUp();//	��������Դ���������ڴ˺����н��г����˳�ǰ��Դ��������
	static HRESULT Objects_Init(HWND hwnd);//	��������Ⱦ��Դ��ʼ���������ڴ˺����н���Ҫ����Ⱦ���������Դ�ĳ�ʼ��
	static HRESULT Direct3D_Init(HWND hwnd);//	������Direct3D��ʼ������������Direct3D�ĳ�ʼ��
	static VOID Matrix_Set();//	��������װ��Direct3D�Ĵ�任�ĺ�����������任��ȡ���任��ͶӰ�任���ӿڱ任������

protected:
	static float Get_FPS();

//�ӿ�
public:
	virtual HRESULT ObjectsInit(HWND hwnd) = 0;
	virtual HRESULT Direct3DInit(HWND hwnd) = 0;
	virtual void Direct3DRender(HWND hwnd) = 0;
	virtual void Direct3DCleanup() = 0;
	virtual VOID MatrixSet() = 0;
};


#endif // !_D3D_TOOL_H



