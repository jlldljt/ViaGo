
/*
 *	ACTEngine.h
 *	一个能控制横版ACT人物的引擎
 *	目标：1、走动、跳跃、攻击，基本动作，各自判定区域，外部传入图片及信息及按键控制方式，攻击力、hp
 *	      2、关卡、背景、音乐、障碍
 *        3、主角
 *        4、敌人，路径、视野
 *        5、声音
 */
#ifndef _ACT_ENGINE_H_
#define _ACT_ENGINE_H_

#include <windows.h>
#include <atlimage.h>	//CImage类包含的头文件
#include <tchar.h>//使用swprintf_s函数所需的头文件
#include <MMSystem.h>	//播放媒体(音乐)所需包含的头文件
#include "ComDef.h"
#include <strsafe.h>
#include <assert.h>

#pragma comment(lib,"winmm.lib")			//调用PlaySound函数所需库文件
#pragma comment(lib,"Msimg32.lib")		//添加使用TransparentBlt函数所需的库文件


#define  SHIFT_LEFT_MOVE   50
enum RET_AE {
	AE_TRUE = 0,
	AE_FALSE,
};

enum MOVE_AE {
	AE_UP = 0,
	AE_DOWN,
	AE_LEFT,
	AE_RIGHT,
};

typedef INT (* REFRESHFN)(DWORD tick);
/*
 *	活动物体的类，描述了基本活动物体的图像和配置
 */
class CBeing {
public:
	CBeing():m_img(NULL),m_bImg(NULL),x(0),y(0), vx(0), vy(0), ax(0), ay(1) {}
	CImage *m_img, *m_bImg;
	int imgN;
	int x,y;//图像的粘贴地址
	int vx, vy, ax, ay;//x速度和y速度
	RECT rect;//图像显示大小，比如CSCENE
	double zoom;
	inline void Mov(INT fx , int fy) {//移动
		x += fx;
		y += fy;
	}
	inline void SetPos(INT px , INT py) {//垂直移动
		x = px;
		y = py;
	}
	inline void SetZoom(double z) {
		zoom = z;
	}
protected:
private:
	
};
/*
 *	人物的类，人物的初始化，人物的动作序列执行函数，人物当前动作特征信息
 */
#define HUMAN_INFO_LENGTH  11
#define POS_Y              1
#define POS_W              2
#define POS_H              3
#define HUMAN_ACT_CHAR     0
#define HUMAN_ACT_X        1
#define HUMAN_SKILL_X      5
#define HUMAN_SKILL_MOVE_X 9
class CHuman :public CBeing {
public:
	CHuman();
	CHuman(LPCTSTR img ,LPCTSTR cfg) :info(NULL) {
		m_img = new CImage();
		m_bImg = new CImage();
		imgN = 1;
		m_img->Load(img);
		m_bImg->Load(img);
		if(m_img->IsNull() || m_bImg->IsNull())
		{
			exit(0);
		}
		/*
		*	1.如果AlphaFormat字段为0,源位图中的所有像素使用同样的常量alpha值，即SourceConstantAlpha字段中的值，该值实际上是0和255，而不是0和1。这里0表示完全透明，255表示完全不透明。目标像素以255-SourceConstantAlpha值作为alpha值。

		2. 如果AlphaFormat字段的值是AC_SRC_ALPHA，源设备表面的每个像素必须有各自的alpha通道。即，必须是32-bpp的物理设备上下文，或是选中了32-bpp DDB和DIB段的内存设备上下文。这些情况下，每个源像素有4个8位通道:红、绿、蓝和alpha。每个像素的alpha通道和SourceConstantAlpha字段一起用于把源和目标混合起来。实际用于计算的运算式如下:

		Tmp.Red = Src.Red * SourceConstantAlpha / 255;

		Tmp.Green = Src.Green * SourceConstantAlpha / 255;

		Tmp.Blue = Src.Blue * SourceConstantAlpha / 255;

		Tmp.Alpha = Src.Alpha * SourceConstantAlpha / 255;

		Beta = 255 – Tmp.alpha;

		Dst.Red = Tmp.Red + Round((Beta * Dst.Red )/255);

		Dst.Green = Tmp.Green + Round((Beta * Dst.Green)/255);

		Dst.Blue = Tmp.Blue + Round((Beta * Dst.Blue )/255);

		Dst.Alpha = Tmp.Alpha + Round((Beta * Dst.Alpha)/255);
		 */
		if (m_img->GetBPP() == 32) //确认该图像包含Alpha通道
		{ 
			int i;
			int j;
			for (i=0; i<m_img->GetWidth(); i++)
			{
				for (j=0; j<m_img->GetHeight(); j++)
				{
					byte *pByte = (byte *)m_img->GetPixelAddress(i, j);
					pByte[0] = pByte[0] * pByte[3] / 255;
					pByte[1] = pByte[1] * pByte[3] / 255;
					pByte[2] = pByte[2] * pByte[3] / 255;
				}
			}
		}

		if (m_bImg->GetBPP() == 32) //确认该图像包含Alpha通道
		{ 
			int i;
			int j;
			for (i=0; i<m_bImg->GetWidth(); i++)
			{
				for (j=0; j<m_bImg->GetHeight(); j++)
				{
					byte *pByte = (byte *)m_bImg->GetPixelAddress(i, j);
					pByte[0] = 255 - pByte[3];
					pByte[1] = 255 - pByte[3];
					pByte[2] = 255 - pByte[3];
				}
			}
		}

		InitCfgFromFile(cfg , 0);
		for (int i = 0; i < 20 ; i++) {
			tmpAct[i][0] = -1;
		}
		ChangeActNo(0 , 0);
		rect.right = info[act[0][0]][HUMAN_ACT_X + POS_W];
		rect.bottom = info[act[0][0]][HUMAN_ACT_X + POS_H];

	}
	~CHuman() {
		if (NULL != m_img)
		{
			delete m_img;
			m_img = NULL;
		}
		if (NULL != m_bImg)
		{
			delete m_bImg;
			m_bImg = NULL;
		}
		if (NULL != info)
		{
			free((void *)info);
			info = NULL;
		}
	}
	//img是包含所有动作图片的png
	//cfg是包含图片所有信息的dat
	//例如：人物是npc还是玩家控制
	//例如:人物跑动图的位置（上下左右），人物动作图的位置（攻击、必杀技、防御、复活等），判定区域（攻击、防御、受创）
	//例如：人物跑动图的触发按键，必杀技的按键序列
	RET_AE InitFromFile(LPCTSTR img, LPCTSTR cfg ,CHAR param);
	RET_AE InitCfgFromFile(LPCTSTR cfg ,CHAR param);
	RET_AE CfgParse();
	/*RET_AE draw(
	HDC hdcDest) {
	SetStretchBltMode (hdcDest,HALFTONE);
	SetBrushOrgEx(hdcDest, 0, 0, NULL);
	m_img->StretchBlt(hdcDest ,x,y,w*zoom,h*zoom,x , y , w , h,SRCCOPY);
	return AE_TRUE;
	}*/
	void ToZero(void) {
		for (int i = 0; i< actNum ;i++)
		{
			act[i][1] = act[i][0];
		}
	}
	//RET_AE MovNext(HDC hdcDest ){
	//	SetStretchBltMode (hdcDest,HALFTONE);
	//	SetBrushOrgEx(hdcDest, 0, 0, NULL);
	//	m_img->StretchBlt(hdcDest 
	//		,x,y,info[mov[1]][2]*zoom,info[mov[1]][3]*zoom
	//		,info[mov[1]][0] ,info[mov[1]][1] ,info[mov[1]][2] , info[mov[1]][3],SRCCOPY);
	//	if (mov[1] >= mov[0]) {
	//		mov[1] = mov[0];
	//	} else {
	//		mov[1] ++;
	//	}
	//	return AE_TRUE;
	//}
	//获得当前执行动作
	int GetActNo(void) {
		return actNo;
	}
	//获得当前执行动作
	int GetActDir(void) {
		return actDir;
	}
	void SetWhere(int n) {
		where = n;
	}
	int GetWhere(void) {
		return where;
	}
	//改变当前执行动作
	RET_AE ChangeActNo(int no , int dir) {
		
		if (-1 != dir)
		{
			actDir = dir;
		}

		if (actNo == no) {
			return AE_FALSE;
		}

		actNo = no;
		
		
		ToZero();
		return AE_TRUE;
	}
	//表演动作//返回值表示FALSE表示移动，TRUE表示未移动
	RET_AE ActNext(HDC hdcDest,INT fx , int fy){
		int dir , shiftX = 0;
		RET_AE ret = AE_TRUE;
		//SetStretchBltMode (hdcDest,HALFTONE);
		//SetBrushOrgEx(hdcDest, 0, 0, NULL);
		if ('J'== (char)info[act[actNo][1]][HUMAN_ACT_CHAR])//变更act
		{
			ChangeActNo(info[act[actNo][1]][HUMAN_ACT_X] , actDir);
		} else if('M'== (char)info[act[actNo][1]][HUMAN_ACT_CHAR]) {
			Mov(fx , fy);
			ret = AE_FALSE;
		}
		if (actDir)//如果变向
		{
			dir = -1;//向左的镜像，默认为右
			shiftX = rect.right / 2;// info[act[0][0]][3] * zoom / 2;//SHIFT_LEFT_MOVE;//反向坐标修正
		}
		else
		{
			dir = 1;//正常
			shiftX = 0;
		}
		m_bImg->StretchBlt(hdcDest 
			,x + shiftX,y,info [act[actNo][1]][HUMAN_ACT_X + POS_W] * zoom * dir, info[act[actNo][1]][HUMAN_ACT_X + POS_H] * zoom
			,info [act [actNo][1]][HUMAN_ACT_X] ,info[act[actNo][1]][HUMAN_ACT_X + POS_Y] ,info[act[actNo][1]][HUMAN_ACT_X + POS_W] , info[act[actNo][1]][HUMAN_ACT_X + POS_H],SRCAND);//贴动作背景
		m_img->StretchBlt(hdcDest 
			, x + shiftX, y, info[act[actNo][1]][HUMAN_ACT_X + POS_W] * zoom * dir, info[act[actNo][1]][HUMAN_ACT_X + POS_H] * zoom
			, info[act[actNo][1]][HUMAN_ACT_X], info[act[actNo][1]][HUMAN_ACT_X + POS_Y], info[act[actNo][1]][HUMAN_ACT_X + POS_W], info[act[actNo][1]][HUMAN_ACT_X + POS_H], SRCPAINT);//贴动作前景
		HBRUSH newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdcDest , (HBRUSH)newBrush);
		
		Rectangle(hdcDest , x + shiftX, y, x + shiftX + info [act[actNo][1]][HUMAN_ACT_X + POS_W] * zoom * dir , y+info[act[actNo][1]][HUMAN_ACT_X + POS_H] * zoom);
		SelectObject(hdcDest ,(HBRUSH)oldBrush);
		DeleteObject((HBRUSH)newBrush);
		/*m_img->StretchBlt(hdcDest 
		,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
		,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4],MERGEPAINT);*/

		/*m_img->AlphaBlend(hdcDest 
		,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
		,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4],AC_SRC_OVER);不支持镜像，所以draw也不能镜像*/
		
		/*m_img->TransparentBlt(hdcDest 
		,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
		,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4],RGB(255,255,255));*/
		/*m_img->Draw(hdcDest ,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
			,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4]);*/
		//判断是否有道具需要加入tmpact
		if ('F'== (char)info[act[actNo][1]][HUMAN_ACT_CHAR]/*0 != info[act[actNo][1]][5] || 0 != info[act[actNo][1]][6]*/)
		{
			for (int i = 0; i < 20 ; i++) {
				if(-1 == tmpAct[i][0]) {
					tmpAct[i][0] = actNo;
					tmpAct[i][1] = act[actNo][1];
					tmpAct[i][2] = x + shiftX;
					tmpAct[i][3] = y;
					tmpAct[i][4] = dir;
					break;
				}
			}
		}

		if (act[actNo][1] == act[actNo][2]/*'E' == (char)info[act[actNo][1]][0]*/) {
			act[actNo][1] = act[actNo][0];
		} else {
			act[actNo][1] ++;
		}

		return ret;
	}
	//表演附加动作
	RET_AE TmpActNext(HDC hdcDest){
		SetStretchBltMode (hdcDest,HALFTONE);
		SetBrushOrgEx(hdcDest, 0, 0, NULL);
		
		for (int i = 0; i < 20 ; i++) {
			if(-1 < tmpAct[i][0]) {
				tmpAct[i][2]+=info[tmpAct[i][1]][HUMAN_SKILL_MOVE_X]*zoom*tmpAct[i][4];
				tmpAct[i][3]+=info[tmpAct[i][1]][HUMAN_SKILL_MOVE_X + POS_Y]*zoom;

				m_bImg->StretchBlt(hdcDest 
					,tmpAct[i][2],tmpAct[i][3],info[tmpAct[i][1]][HUMAN_SKILL_X + POS_W]*zoom*tmpAct[i][4],info[tmpAct[i][1]][HUMAN_SKILL_X + POS_H]*zoom
					,info[tmpAct[i][1]][HUMAN_SKILL_X] ,info[tmpAct[i][1]][HUMAN_SKILL_X + POS_Y] ,info[tmpAct[i][1]][HUMAN_SKILL_X + POS_W] , info[tmpAct[i][1]][HUMAN_SKILL_X + POS_H],SRCAND);

				m_img->StretchBlt(hdcDest 
					, tmpAct[i][2], tmpAct[i][3], info[tmpAct[i][1]][HUMAN_SKILL_X + POS_W] * zoom*tmpAct[i][4], info[tmpAct[i][1]][HUMAN_SKILL_X + POS_H] * zoom
					, info[tmpAct[i][1]][HUMAN_SKILL_X], info[tmpAct[i][1]][HUMAN_SKILL_X + POS_Y], info[tmpAct[i][1]][HUMAN_SKILL_X + POS_W], info[tmpAct[i][1]][HUMAN_SKILL_X + POS_H], SRCPAINT);
				HBRUSH newBrush = (HBRUSH)GetStockObject(NULL_BRUSH);
				HBRUSH oldBrush = (HBRUSH)SelectObject(hdcDest , (HBRUSH)newBrush);
				Rectangle(hdcDest ,tmpAct[i][2],tmpAct[i][3],tmpAct[i][2] + info[tmpAct[i][1]][HUMAN_SKILL_X + POS_W]*zoom*tmpAct[i][4],tmpAct[i][3] + info[tmpAct[i][1]][HUMAN_SKILL_X + POS_H]*zoom);
				SelectObject(hdcDest ,(HBRUSH)oldBrush);
				DeleteObject((HBRUSH)newBrush);
				/*m_img->TransparentBlt(hdcDest 
					,tmpAct[i][2],tmpAct[i][3],info[tmpAct[i][1]][7]*zoom*tmpAct[i][4],info[tmpAct[i][1]][8]*zoom
					,info[tmpAct[i][1]][5] ,info[tmpAct[i][1]][6] ,info[tmpAct[i][1]][7] , info[tmpAct[i][1]][8],RGB(255,255,255));
			*/
			
				if (tmpAct[i][1] == act[tmpAct[i][0]][2]/*'E'== (char)info[tmpAct[i][1]][0]*/) {
					tmpAct[i][0] = -1;//结束该tmpact
				} else {
					tmpAct[i][1] ++;
				}
			}
		}
		
		return AE_TRUE;
	}
	/*RET_AE PlayAct(int no){
	SetStretchBltMode (hdcDest,HALFTONE);
	SetBrushOrgEx(hdcDest, 0, 0, NULL);
	m_img->TransparentBlt(hdcDest 
	,x,y,info[act[no][1]][2]*zoom,info[act[no][1]][3]*zoom
	,info[act[no][1]][0] ,info[act[no][1]][1] ,info[act[no][1]][2] , info[act[no][1]][3],RGB(255,255,255));
	if (act[no][1] >= act[no][2]) {
	act[no][1] = act[no][0];
	} else {
	act[no][1] ++;
	}
	return AE_TRUE;
	}*/
protected:
private:
	/************************************************************************
	        1   2   3   4   5   6   7   8
		1	图x	图y	图w	图h	功x	功y	功w	功h	判x	判y	判w	判h
		2
		3
		4
		5
		6
		7
	************************************************************************/
	int (*info)[HUMAN_INFO_LENGTH];
	int act[10][3];//十种动作的数组起始序号act[i][0]/和当前动作的序号act[i][1]最大序号act[i][2]
	int tmpAct[20][5];//遗留动作，一般是飞行道具之类、第i遗留动作是第几种tmpAct[i][0]/执行到哪一帧tmpAct[i][1]/动作x/动作ytmpAct[i][2/3]方向tmpAct[i][4]
	int actNum;//act有几种，读文件得到
	int actNo;//执行actNo
	int actDir;//执行方向；0不变，1变向
	int where;//所处环境，比如地面0、空中1、水中2

};

/*
 *	武器道具类
 */
class CWeapon :public CBeing {
public:
	CWeapon();
	CWeapon(LPCTSTR img ,LPCTSTR cfg) :info(NULL){
		m_img = new CImage();
		imgN = 1;
		m_img->Load(img);
		InitCfgFromFile(cfg , 0);
	}
	~CWeapon() {
		if (NULL != m_img)
		{
			delete m_img;
			m_img = NULL;
		}
		
		if (NULL != info)
		{
			free((void *)info);
			info = NULL;
		}
	}
	//img是包含所有动作图片的png
	//cfg是包含图片所有信息的dat
	//例如：人物是npc还是玩家控制
	//例如:人物跑动图的位置（上下左右），人物动作图的位置（攻击、必杀技、防御、复活等），判定区域（攻击、防御、受创）
	//例如：人物跑动图的触发按键，必杀技的按键序列
	RET_AE InitFromFile(LPCTSTR img, LPCTSTR cfg ,CHAR param);
	RET_AE InitCfgFromFile(LPCTSTR cfg ,CHAR param);
	RET_AE CfgParse();
	void ToZero(void) {
		for (int i = 0; i< 10 ;i++)
		{
			act[i][1] = 0;
		}
	}

	RET_AE ActNext(HDC hdcDest, int no){
		SetStretchBltMode (hdcDest,HALFTONE);
		SetBrushOrgEx(hdcDest, 0, 0, NULL);
		m_img->TransparentBlt(hdcDest 
			,x,y,info[act[no][1]][2]*zoom,info[act[no][1]][3]*zoom
			,info[act[no][1]][0] ,info[act[no][1]][1] ,info[act[no][1]][2] , info[act[no][1]][3],RGB(255,255,255));
		if (act[no][1] >= act[no][2]) {
			act[no][1] = act[no][0];
		} else {
			act[no][1] ++;
		}
		return AE_TRUE;
	}
protected:
private:
	/************************************************************************
	        1   2   3   4   5   6   7   8
		1	图x	图y	图w	图h	功x	功y	功w	功h	判x	判y	判w	判h
		2
		3
		4
		5
		6
		7
	************************************************************************/
	int (*info)[11];
	int act[10][3];//十种动作的数组起始序号act[i][0]/和当前动作的序号act[i][1]最大序号act[i][2]
	int tmpAct[20][4];//遗留动作，一般是飞行道具之类、第i遗留动作是第几种tmpAct[i][0]/执行到哪一帧tmpAct[i][1]/动作x/动作ytmpAct[i][2/3]
	int actNum;//act有几种，读文件得到
	int actNo;//执行actNo
	
};
/*
 *	场景类
 */
class CScene : public CBeing{
public:
	CScene();
	/*img整图像bImg背景障碍物图像rc显示区域*/
	CScene(LPCTSTR img , LPCTSTR bImg , RECT rc) {
		m_img = new CImage();
		m_bImg = new CImage();
		imgN = 1;
		m_img->Load(img);
		m_bImg->Load(bImg);
		if (m_img->IsNull() || m_bImg->IsNull())
		{
			exit(0);
		}
		rect = rc;
	}
	~CScene() {
		if (NULL != m_img)
		{
			delete m_img;
			m_img = NULL;
		}

		if (NULL != m_bImg)
		{
			delete m_bImg;
			m_bImg = NULL;
		}
		
	}
	//RET_AE InitHumanFromFile(LPCTSTR img, LPCTSTR cfg ,CHAR param);
	RET_AE CfgParse();
	RET_AE draw(
		HDC hdcDest ,
		int xDest,
		int yDest,
		int wDest,
		int hDest,
		int xSrc,
		int ySrc,
		int wSrc,
		int hSrc) {
			m_img->TransparentBlt(hdcDest ,xDest,yDest,wDest,hDest,xSrc , ySrc , wSrc , hSrc,RGB(255,255,255));
	}
	RET_AE draw(
		HDC hdcDest ,
		RECT rect,
		int offset) {
			/*RECT rect;  
			HWND hWnd=WindowFromDC(hdcDest);  
			GetClientRect(hWnd, &rect);*/
			SetStretchBltMode (hdcDest,HALFTONE);
			SetBrushOrgEx(hdcDest, 0, 0, NULL);
			m_img->StretchBlt(hdcDest ,rect.left + offset,rect.top,rect.right - rect.left,rect.bottom - rect.top,0 , 0 , m_img->GetWidth() , m_img->GetHeight(),SRCCOPY);
			m_img->StretchBlt(hdcDest ,rect.left - rect.right + rect.left + offset,rect.top,rect.right - rect.left,rect.bottom - rect.top,0 , 0 , m_img->GetWidth() , m_img->GetHeight(),SRCCOPY);

			//ReleaseDC(hWnd,hdcDest);

			return AE_TRUE;
	}

	RET_AE draw(
		HDC hdcDest,
		RECT dstRect,
		RECT srcRect) {
		/*RECT rect;
		HWND hWnd=WindowFromDC(hdcDest);
		GetClientRect(hWnd, &rect);*/
		SetStretchBltMode(hdcDest, HALFTONE);
		SetBrushOrgEx(hdcDest, 0, 0, NULL);
		//m_img->StretchBlt(hdcDest, dstRect, srcRect, SRCCOPY);
		m_bImg->StretchBlt(hdcDest, dstRect, srcRect, SRCCOPY);
		//ReleaseDC(hWnd,hdcDest);

		return AE_TRUE;
	}

};
/*
 *	双缓冲贴图类
 */
class CDrawingCache {
public:
	HDC hdc;
	HDC bufHdc;
	HBITMAP hBmp;
	HWND hwnd;
	RECT rect;
	CDrawingCache(HWND hWnd) {
		assert(NULL != hWnd);		
		GetClientRect(hWnd,&rect);
		hdc = GetDC(hWnd);
		bufHdc = CreateCompatibleDC(hdc);  //创建一个和hdc兼容的内存DC
		hBmp = CreateCompatibleBitmap(hdc,rect.right - rect.left , rect.bottom - rect.top);
		SelectObject(bufHdc,hBmp);
		hwnd = hWnd;
	}
	~CDrawingCache() {
		DeleteObject(hBmp);
		DeleteDC(bufHdc);
		ReleaseDC(hwnd,hdc);
	}
	void Refresh() {
		BitBlt(hdc,rect.left,rect.top,rect.right - rect.left, rect.bottom - rect.top,bufHdc,0,0,SRCCOPY);
	}
};
/*
 *	窗口的创建、销毁和子窗口的创建
 */
class CACTWindow {
public:
	CACTWindow(void)
	{
		m_instance=NULL;
		m_name=NULL;
	}
	~CACTWindow() {
		DestoryMainWindow(m_instance , m_name);
	}
	HWND CreateMainWindow(
		HINSTANCE hInstance,
		LPCWSTR lpName,
		int x,
		int y,
		int width,
		int height,
		WNDPROC wndProc,
		LPARAM dwInitParam);
	HWND CreateChildWindow(
		HWND hWndParent,
		LPCWSTR lpName,
		int x,
		int y,
		int width,
		int height,	
		WNDPROC wndProc,
		LPARAM dwInitParam);
	bool DestoryMainWindow(
		HINSTANCE hInstance,
		LPCWSTR lpName);
	void WindowRun(
		/*REFRESHFN refreshFn*/);
private:
	HINSTANCE m_instance;
	LPCWSTR m_name;
};
/*
 *	引擎类，游戏的启动、刷新、场景、物体添加
 */
class CACTEngine{
public:
	CACTEngine() {
		m_actWnd = NULL;m_window=NULL;m_human=NULL;m_scene=NULL;m_cache=NULL;
	}
	CACTEngine(
		HINSTANCE hInstance,
		LPCWSTR lpName,
		int x,
		int y,
		int width,
		int height,	
		HWND hWndParent,
		WNDPROC wndProc,
		LPARAM dwInitParam) {
			dirL = 0; dirR = 0;dirU = 0;dirD = 0;refresh = true;
			m_actWnd = NULL;m_window=NULL;m_human=NULL;m_scene=NULL;m_cache=NULL;
			m_window = new CACTWindow();
			m_actWnd = m_window->CreateMainWindow(
				hInstance,
				lpName,
				x,
				y,
				width,
				height,
				wndProc,
				dwInitParam);
			m_cache = new CDrawingCache(m_actWnd);
	}
	~CACTEngine() {
		delete m_window;
		if (NULL != m_scene) {
			delete m_scene;
			m_scene = NULL;
		}
		if (NULL != m_human) {
			delete m_human;
			m_human = NULL;
		}
		delete m_cache;
	};
	void SetLeft(UINT num) {
		dirL = num;
	}
	void ResetL() {
		dirL = 0;
	}
	void SetRight(UINT num) {
		dirR = num;
	}
	void ResetR() {
		dirR = 0;
	}
	void SetUp(UINT num) {
		dirU = num;
	}
	void ResetU() {
		dirU = 0;
	}
	void SetDown(UINT num) {
		dirD = num;
	}
	void ResetD() {
		dirD = 0;
	}
	int GetHumanAct(void) {
		return m_human->GetActNo();
	}
	int GetHumanDir(void) {
		return m_human->GetActDir();
	}
	void SetScreenRefresh(bool on) {
		refresh = on;
	}

	RET_AE ChangeHumanAct(int no ,int dir) {
			return m_human->ChangeActNo(no , dir);
	}
	/*
	 *          up
	        1   2   3
	  left  4   5   6  right
		    7   8   9
			    down
	 */
	int CheckMove(void) {
		int fx = dirR != dirL ? (dirR - dirL)/abs(dirR - dirL) : 0;
		int fy = dirD != dirU ? (dirD - dirU)/abs(dirD - dirU) : 0;
		int dir = 5 + fx + fy*3;
		return dir;
	}
	////human
	//void MovLeft() {
	//	dirD = 0;
	//}
	void ActRun();
	RET_AE AddScene(LPCTSTR img , LPCTSTR bImg) {
		m_scene = new CScene(img, bImg, m_cache->rect);
		return AE_TRUE;
	}
	RET_AE AddHuman(LPCTSTR img , LPCTSTR cfg) {
		m_human = new CHuman(img , cfg);
		return AE_TRUE;
	}
	void SetHumanZoom(double z) {
		m_human->SetZoom(z);
	}
	HWND m_actWnd;
	CACTWindow *m_window;
protected:
private:
	INT GameRefresh(DWORD tick);
	bool refresh;
	CHuman *m_human;
	CScene *m_scene;
	CDrawingCache *m_cache;

	int dirL,dirR,dirU,dirD;
	
};


#endif