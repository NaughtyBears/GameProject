#include "d3dTool.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//����CWindow�࣬��ʹ��Ĭ�Ϲ��캯������800*600�Ĵ���
	CWindow cw;

	//���ڵĴ���
	if (FAILED(cw.windowBuilt(hInstance, hPrevInstance, lpCmdLine, nShowCmd)))
	{
		return E_FAIL;
	}

	//��Ϣѭ������
	MSG msg = { 0 }; 
	while (msg.message != WM_QUIT)	
	{
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage(&msg);		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage(&msg);		//�ú����ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			D3DTool::Direct3D_Render(NULL);  
		}
	}

	//�������ע��
	cw.windowDestroy();

	return 0;
}