
#include "ACTEngine.h"
/*
 *	创建w32主窗口
 */
HWND CreateWin32Window(
	HINSTANCE hInstance,
	LPCWSTR lpName,
	DWORD dwStyle,
	HWND hWndParent,
	WNDPROC wndProc,
	LPARAM dwInitParam) {
		
		//【1】窗口创建四步曲之一：开始设计一个完整的窗口类
		WNDCLASSEX wndClass = { 0 };							//用WINDCLASSEX定义了一个窗口类
		wndClass.cbSize = sizeof( WNDCLASSEX ) ;			//设置结构体的字节数大小
		wndClass.style = CS_HREDRAW | CS_VREDRAW;	//设置窗口的样式
		wndClass.lpfnWndProc = wndProc;					//设置指向窗口过程函数的指针
		wndClass.cbClsExtra		= 0;								//窗口类的附加内存，取0就可以了
		wndClass.cbWndExtra		= 0;							//窗口的附加内存，依然取0就行了
		wndClass.hInstance = hInstance;						//指定包含窗口过程的程序的实例句柄。
		wndClass.hIcon=NULL;//(HICON)::LoadImage(NULL,L"icon.ico",IMAGE_ICON,0,0,LR_DEFAULTSIZE|LR_LOADFROMFILE);  //本地加载自定义ico图标//LoadIcon(NULL, IDI_APPLICATION);
		wndClass.hCursor = LoadCursor( NULL, IDC_ARROW );    //指定窗口类的光标句柄。
		wndClass.hbrBackground=(HBRUSH)GetStockObject(WHITE_BRUSH);  //为hbrBackground成员指定一个白色画刷句柄	
		wndClass.lpszMenuName = NULL;						//用一个以空终止的字符串，指定菜单资源的名字。
		wndClass.lpszClassName = lpName;		//用一个以空终止的字符串，指定窗口类的名字。

		//【2】窗口创建四步曲之二：注册窗口类
		if( !RegisterClassEx( &wndClass ) )				//设计完窗口后，需要对窗口类进行注册，这样才能创建该类型的窗口
		{
			MessageBox(NULL, TEXT("This program requires Windows NT!"),
				lpName, MB_ICONERROR);
			return NULL;		
		}

		//【3】窗口创建四步曲之三：正式创建窗口
		HWND hwnd = CreateWindow( 
			lpName,
			NULL,				//窗口的标题，默认没有，可以后面自行设置
			dwStyle, 
			0,
			0,
			0,
			0, 
			hWndParent, 
			NULL, 
			hInstance, 
			NULL );

		//【4】窗口创建四步曲之四：窗口的移动、显示与更新
		//MoveWindow(hwnd,cxScreen >>2,cyScreen >>2,cxScreen >>1,	cyScreen >>1, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
		//ShowWindow( hwnd, true );    //调用ShowWindow函数来显示窗口
		//UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

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
			,&d[0] , &d[1] , &d[2] , &d[3],&d[4] , &d[5] , &d[6] , &d[7],&d[8] , &d[9] , &d[10]);//err代表转换成功数
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
		,&info[rowNum][8] , &info[rowNum][9] , &info[rowNum][10]);//err代表转换成功数
		rowNum ++;
	};

	//while(EOF != _ftscanf(fp , TEXT("%s %ld %lf %c\n"),a , &b , &c , &d));//_s需要指定长度sizeof(int)
	fclose(fp);
	return AE_TRUE;
}








/************************************************************************/
/*          CACTWindow                                                  */
/************************************************************************/
/*
 *	创建主窗口
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
		
		MoveWindow(hwnd,x,y,width,	height, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处
		ShowWindow( hwnd, true );    //调用ShowWindow函数来显示窗口
		UpdateWindow(hwnd);						//对窗口进行更新，就像我们买了新房子要装修一样

		m_instance = hInstance;
		m_name = lpName;

		return hwnd;
}
/*
 *	创建子窗口
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

		//【4】窗口创建四步曲之四：窗口的移动、显示与更新
		MoveWindow(hwnd,x,y,width,	height, true);		//调整窗口显示时的位置，使窗口左上角位于（250,80）处

		return hwnd;
}
/*
 *	销毁主窗口
 */
bool CACTWindow::DestoryMainWindow(
	HINSTANCE hInstance,
	LPCWSTR lpName) {
		//【6】窗口类的注销
		UnregisterClass(lpName, hInstance);  //程序准备结束，注销窗口类
		return 0;  
}


void CACTWindow::WindowRun(
	/*REFRESHFN refreshFn*/) {
	//【5】消息循环过程
	MSG msg = { 0 };				//定义并初始化msg
	//DWORD		tPre=0,tNow=0,tTim;
	while( msg.message != WM_QUIT )		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
	{
		if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
		{
			TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
			DispatchMessage( &msg );			//分发一个消息给窗口程序。
		}
		else
		{
			//tNow = GetTickCount();   //获取当前系统时间
			//tTim = tNow - tPre;
			//if(tTim >= 50)        //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
			//{
			//	//重绘
			//	tPre = GetTickCount();   //获取当前系统时间
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
		//【5】消息循环过程
		MSG msg = { 0 };				//定义并初始化msg
		DWORD		tPre=0,tNow=0,tTim;
		while( msg.message != WM_QUIT )		//使用while循环，如果消息不是WM_QUIT消息，就继续循环
		{
			if( PeekMessage( &msg, 0, 0, 0, PM_REMOVE ) )   //查看应用程序消息队列，有消息时将队列中的消息派发出去。
			{
				TranslateMessage( &msg );		//将虚拟键消息转换为字符消息
				DispatchMessage( &msg );			//分发一个消息给窗口程序。
			}
			else
			{
				tNow = GetTickCount();   //获取当前系统时间
				tTim = tNow - tPre;
				if(tTim >= 60)        //当此次循环运行与上次绘图时间相差0.05秒时再进行重绘操作
				{
					//重绘
					tPre = GetTickCount();   //获取当前系统时间
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
	//x方向的判断
	if (m_human->x >= (m_cache->rect.right >> 1) && fx > 0
		|| m_human->x <= (m_cache->rect.right >> 1) && fx < 0) {
		if (/*m_scene->rect.bottom + fy > m_scene->m_img->GetHeight()
			|| m_scene->rect.top + fy < 0
			||*/ m_scene->rect.left + fx < 0
			|| m_scene->rect.right + fx > m_scene->m_img->GetWidth()) {
			scenexMov = false;
			if (m_human->x + fx >= m_cache->rect.left && m_human->x + fx <= (m_cache->rect.right)-m_human->rect.right)//人物宽度)
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
	//y方向fy大小的判断
	int l_dir = GetHumanDir() , l_wth = m_human->rect.right;
	m_human->SetWhere(1);//设置在空中
	
	//判断人物矩形框下沿是否碰到黑色的
	if(m_human->vy>0)
	for (int i = m_human->x + l_dir *l_wth /2 + m_cache->rect.left
		, j = m_human->y + m_human->rect.bottom + m_scene->rect.top
		; (i != m_human->x+ m_cache->rect.left + l_wth* (1 - l_dir * 2))
		&& i >=0
		; i+=1 -l_dir*2) {
		int bpp = m_scene->m_bImg->GetBPP();
		if (m_scene->m_bImg->GetBPP()) //确认该图像包含Alpha通道
		{
			
			DWORD dw = m_scene->m_bImg->GetPixel(i, j);
					//	byte *pByte = (byte *)GetPixel(i, j);
					/*pByte[0] = pByte[0] * pByte[3] / 255;
					pByte[1] = pByte[1] * pByte[3] / 255;
					pByte[2] = pByte[2] * pByte[3] / 255;*/
			if (dw == 0) {
				m_human->SetWhere(0);//碰到了黑色，设置在地上
				m_human->vy = 0;
				break;
			}
		}
	}
	fy = m_human->vy;	//这里应该是重力加速度的计算公式
	//y方向的判断
	if (m_human->y >= (m_cache->rect.bottom >> 1) && fy > 0
		|| m_human->y <= (m_cache->rect.bottom >> 1) && fy < 0) {
		if (m_scene->rect.bottom + fy > m_scene->m_img->GetHeight()
			|| m_scene->rect.top + fy < 0
			) {
			sceneyMov = false;
			if (m_human->y + fy >= m_cache->rect.top && m_human->y + fy <= (m_cache->rect.bottom) - m_human->rect.bottom)//人物高度)
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