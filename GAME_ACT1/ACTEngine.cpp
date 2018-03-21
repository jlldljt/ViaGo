
#include "ACTEngine.h"
/*
 *	����w32������
 */
HWND CreateWin32Window(
	HINSTANCE hInstance,
	LPCWSTR lpName,
	DWORD dwStyle,
	HWND hWndParent,
	WNDPROC wndProc,
	LPARAM dwInitParam) {
		
		//��1�����ڴ����Ĳ���֮һ����ʼ���һ�������Ĵ�����
		WNDCLASSEX wndClass = { 0 };							//��WINDCLASSEX������һ��������
		wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//���ýṹ����ֽ�����С
		wndClass.style = CS_HREDRAW | CS_VREDRAW;	//���ô��ڵ���ʽ
		wndClass.lpfnWndProc = wndProc;					//����ָ�򴰿ڹ��̺�����ָ��
		wndClass.cbClsExtra		= 0;								//������ĸ����ڴ棬ȡ0�Ϳ�����
		wndClass.cbWndExtra		= 0;							//���ڵĸ����ڴ棬��Ȼȡ0������
		wndClass.hInstance = hInstance;						//ָ���������ڹ��̵ĳ����ʵ�������
		wndClass.hIcon=NULL;//(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //���ؼ����Զ���icoͼ��//LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //ָ��������Ĺ������
		wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);  //ΪhbrBackground��Աָ��һ����ɫ��ˢ���	
		wndClass.lpszMenuName = NULL;						//��һ���Կ���ֹ���ַ�����ָ���˵���Դ�����֡�
		wndClass.lpszClassName = lpName;		//��һ���Կ���ֹ���ַ�����ָ������������֡�

		//��2�����ڴ����Ĳ���֮����ע�ᴰ����
		if( !RegisterClassEx( &wndClass ) )				//����괰�ں���Ҫ�Դ��������ע�ᣬ�������ܴ��������͵Ĵ���
		{
			MessageBox(NULL, TEXT("This program requires Windows NT!"),
				lpName, MB_ICONERROR);
			return NULL;		
		}

		//��3�����ڴ����Ĳ���֮������ʽ��������
		HWND hwnd = CreateWindow( 
			lpName,
			NULL,				//���ڵı��⣬Ĭ��û�У����Ժ�����������
			dwStyle, 
			0,
			0,
			0,
			0, 
			hWndParent, 
			NULL, 
			hInstance, 
			NULL );

		//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
		//MoveWindow(hwnd,cxScreen >>2,cyScreen >>2,cxScreen >>1,	cyScreen >>1, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
		//ShowWindow( hwnd, true );    //����ShowWindow��������ʾ����
		//UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

		return hwnd;
}
/************************************************************************/
/*         CHuman                                                       */
/************************************************************************/
RET_AE CHuman::InitCfgFromFile(LPCTSTR cfg ,CHAR param) {
		
	FILE *fp = NULL;	
	int err;
	
	TCHAR aa[200],a[20];
	int d[HUMAN_INFO_LENGTH];
	int rowNum = 0;
	actNum = 0;
	_tfopen_s(&fp,cfg,TEXT("r"));
	//_ftprintf_s( fp, TEXT("%s %ld %f%c"), TEXT("a-string"),	65000, 3.14159, 'x' );
	fseek(fp,0,SEEK_SET);
	while(!feof(fp)) {
		_fgetts(aa,198,fp);
		err = _stscanf(aa, TEXT("%c\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d")
			,&d[0] , &d[1] , &d[2] , &d[3],&d[4] , &d[5] , &d[6] , &d[7],&d[8] , &d[9] , &d[10]);//err����ת���ɹ���
		if ((char)d[0] == 'T'/* && d[1] == 0*/)
		{
			act[actNum][0] = d[3];
			act[actNum][1] = 0;
			act[actNum][2] = d[4];
			actNum++;
		}
		rowNum ++;
	};
	//info = (int **)malloc(sizeof(int*) * rowNum);
	if (NULL != info) {
		free(info);
		info = NULL;
	}
	info = (int (*)[HUMAN_INFO_LENGTH])malloc(sizeof(int) * rowNum * HUMAN_INFO_LENGTH);
	fseek(fp,0,SEEK_SET);
	rowNum = 0;
	while(!feof(fp)) {
		_fgetts(aa,198,fp);
		err = _stscanf(aa, TEXT("%c\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d")
			,&info[rowNum][0] , &info[rowNum][1] , &info[rowNum][2] , &info[rowNum][3]
		,&info[rowNum][4] , &info[rowNum][5] , &info[rowNum][6] , &info[rowNum][7]
		,&info[rowNum][8] , &info[rowNum][9] , &info[rowNum][10]);//err����ת���ɹ���
		rowNum ++;
	};

	//while(EOF != _ftscanf(fp , TEXT("%s %ld %lf %c\n"),a , &b , &c , &d));//_s��Ҫָ������sizeof(int)
	fclose(fp);
	return AE_TRUE;
}








/************************************************************************/
/*          CACTWindow                                                  */
/************************************************************************/
/*
 *	����������
 */
HWND CACTWindow::CreateMainWindow(
	HINSTANCE hInstance,
	LPCWSTR lpName,
	int x,
	int y,
	int width,
	int height,
	WNDPROC wndProc,
	LPARAM dwInitParam) {
		if (NULL == wndProc || NULL == lpName ||NULL == hInstance) {
			return NULL;
		}
		HWND hwnd = CreateWin32Window(
			hInstance,
			lpName,
			WS_OVERLAPPEDWINDOW, 
			NULL, 
			wndProc,
			NULL );
		
		MoveWindow(hwnd,x,y,width,	height, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����
		ShowWindow( hwnd, true );    //����ShowWindow��������ʾ����
		UpdateWindow(hwnd);						//�Դ��ڽ��и��£��������������·���Ҫװ��һ��

		m_instance = hInstance;
		m_name = lpName;

		return hwnd;
}
/*
 *	�����Ӵ���
 */
HWND CACTWindow::CreateChildWindow(
	HWND hWndParent,
	LPCWSTR lpName,
	int x,
	int y,
	int width,
	int height,	
	WNDPROC wndProc,
	LPARAM dwInitParam) {
		if (NULL == wndProc || NULL == hWndParent || NULL == lpName ||NULL == m_instance) {
			return NULL;
		}

		
		HWND hwnd = CreateWin32Window(
			m_instance,
			lpName,
			WS_CHILD | WS_VISIBLE | WS_BORDER, 
			hWndParent, 
			wndProc,
			NULL );

		//��4�����ڴ����Ĳ���֮�ģ����ڵ��ƶ�����ʾ�����
		MoveWindow(hwnd,x,y,width,	height, true);		//����������ʾʱ��λ�ã�ʹ�������Ͻ�λ�ڣ�250,80����

		return hwnd;
}
/*
 *	����������
 */
bool CACTWindow::DestoryMainWindow(
	HINSTANCE hInstance,
	LPCWSTR lpName) {
		//��6���������ע��
		UnregisterClass(lpName, hInstance);  //����׼��������ע��������
		return 0;  
}


void CACTWindow::WindowRun(
	/*REFRESHFN refreshFn*/) {
	//��5����Ϣѭ������
	MSG msg = { 0 };				//���岢��ʼ��msg
	//DWORD		tPre=0,tNow=0,tTim;
	while( msg.message != WM_QUIT )		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
		{
			TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
			DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
		}
		else
		{
			//tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
			//tTim = tNow - tPre;
			//if(tTim >= 50)        //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
			//{
			//	//�ػ�
			//	tPre = GetTickCount();   //��ȡ��ǰϵͳʱ��
			//	GameRefresh(tTim);
			//}
		}

	}
}
/************************************************************************/
/*                CACTEngine                                            */
/************************************************************************/
void CACTEngine::ActRun(
	/*REFRESHFN refreshFn*/) {
		//��5����Ϣѭ������
		MSG msg = { 0 };				//���岢��ʼ��msg
		DWORD		tPre=0,tNow=0,tTim;
		while( msg.message != WM_QUIT )		//ʹ��whileѭ���������Ϣ����WM_QUIT��Ϣ���ͼ���ѭ��
		{
			if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //�鿴Ӧ�ó�����Ϣ���У�����Ϣʱ�������е���Ϣ�ɷ���ȥ��
			{
				TranslateMessage( &msg );		//���������Ϣת��Ϊ�ַ���Ϣ
				DispatchMessage( &msg );			//�ַ�һ����Ϣ�����ڳ���
			}
			else
			{
				tNow = GetTickCount();   //��ȡ��ǰϵͳʱ��
				tTim = tNow - tPre;
				if(tTim >= 60)        //���˴�ѭ���������ϴλ�ͼʱ�����0.05��ʱ�ٽ����ػ����
				{
					//�ػ�
					tPre = GetTickCount();   //��ȡ��ǰϵͳʱ��
					GameRefresh(tTim);
				}
			}

		}
}
int maxV = 10;
INT CACTEngine::GameRefresh(DWORD tick) {
	m_cache->Refresh();
	int fx = dirR - dirL ;
	int fy = dirD - dirU;
	if (fy < 0)
		m_human->vy = -maxV;
	else
		m_human->vy = m_human->vy  + m_human->ay < maxV ? m_human->vy + m_human->ay: maxV;
	bool humanxMov = false, scenexMov = false;
	bool humanyMov = false, sceneyMov = false;
	//x������ж�
	if (m_human->x >= (m_cache->rect.right >> 1) && fx > 0
		|| m_human->x <= (m_cache->rect.right >> 1) && fx < 0) {
		if (/*m_scene->rect.bottom + fy > m_scene->m_img->GetHeight()
			|| m_scene->rect.top + fy < 0
			||*/ m_scene->rect.left + fx < 0
			|| m_scene->rect.right + fx > m_scene->m_img->GetWidth()) {
			scenexMov = false;
			if (m_human->x + fx >= m_cache->rect.left && m_human->x + fx <= (m_cache->rect.right)-m_human->rect.right)//������)
			{
				humanxMov = true;
		
			} 
			else
			{
				humanxMov = false;
		
			}
		}
		else {
			scenexMov = true;
		}
	}
	else {
		humanxMov = true;
	}
	//y����fy��С���ж�
	int l_dir = GetHumanDir() , l_wth = m_human->rect.right;
	m_human->SetWhere(1);//�����ڿ���
	
	//�ж�������ο������Ƿ�������ɫ��
	if(m_human->vy>0)
	for (int i = m_human->x + l_dir *l_wth /2 + m_cache->rect.left
		, j = m_human->y + m_human->rect.bottom + m_scene->rect.top
		; (i != m_human->x+ m_cache->rect.left + l_wth* (1 - l_dir * 2))
		&& i >=0
		; i+=1 -l_dir*2) {
		int bpp = m_scene->m_bImg->GetBPP();
		if (m_scene->m_bImg->GetBPP()) //ȷ�ϸ�ͼ�����Alphaͨ��
		{
			
			DWORD dw = m_scene->m_bImg->GetPixel(i, j);
					//	byte *pByte = (byte *)GetPixel(i, j);
					/*pByte[0] = pByte[0] * pByte[3] / 255;
					pByte[1] = pByte[1] * pByte[3] / 255;
					pByte[2] = pByte[2] * pByte[3] / 255;*/
			if (dw == 0) {
				m_human->SetWhere(0);//�����˺�ɫ�������ڵ���
				m_human->vy = 0;
				break;
			}
		}
	}
	fy = m_human->vy;	//����Ӧ�����������ٶȵļ��㹫ʽ
	//y������ж�
	if (m_human->y >= (m_cache->rect.bottom >> 1) && fy > 0
		|| m_human->y <= (m_cache->rect.bottom >> 1) && fy < 0) {
		if (m_scene->rect.bottom + fy > m_scene->m_img->GetHeight()
			|| m_scene->rect.top + fy < 0
			) {
			sceneyMov = false;
			if (m_human->y + fy >= m_cache->rect.top && m_human->y + fy <= (m_cache->rect.bottom) - m_human->rect.bottom)//����߶�)
			{
				humanyMov = true;

			}
			else
			{
				humanyMov = false;

			}
		}
		else {
			sceneyMov = true;
		}
	}
	else {
		humanyMov = true;
	}
	
	if (NULL != m_scene)
	{

		if(refresh) {
			if (scenexMov == true)
			{
				//m_scene->x -= fx;
				m_scene->rect.left += fx;
				m_scene->rect.right += fx;
			}
			if (sceneyMov == true)
			{
				//m_scene->x -= fx;
				m_scene->rect.top += fy;
				m_scene->rect.bottom += fy;
			}
			//m_scene->x -= fx;
			//m_scene->y -= fy;
			/*if (m_cache->rect.right - m_cache->rect.left < m_scene->x) {
				m_scene->x = m_scene->x - (m_cache->rect.right - m_cache->rect.left);
			} else if(0 > m_scene->x) {
				m_scene->x = m_cache->rect.right - m_cache->rect.left + m_scene->x;
			}*/
		}
		m_scene->draw(m_cache->bufHdc ,m_cache->rect, m_scene->rect);
	}
	if (NULL != m_human)
	{
		if (humanxMov == false)
		{
			fx = 0;
		}
		if (humanyMov == false)
		{
			fy = 0;
		}
		if(AE_FALSE == m_human->ActNext(m_cache->bufHdc , fx , fy)) {
			SetScreenRefresh(true);
		} else {
			SetScreenRefresh(false);
		}
		m_human->TmpActNext(m_cache->bufHdc);
	}
	
	return 1;
}