
/*
 *	ACTEngine.h
 *	һ���ܿ��ƺ��ACT���������
 *	Ŀ�꣺1���߶�����Ծ�����������������������ж������ⲿ����ͼƬ����Ϣ���������Ʒ�ʽ����������hp
 *	      2���ؿ������������֡��ϰ�
 *        3������
 *        4�����ˣ�·������Ұ
 *        5������
 */
#ifndef _ACT_ENGINE_H_
#define _ACT_ENGINE_H_

#include <windows.h>
#include <atlimage.h>	//CImage�������ͷ�ļ�
#include <tchar.h>//ʹ��swprintf_s���������ͷ�ļ�
#include <MMSystem.h>	//����ý��(����)���������ͷ�ļ�
#include "ComDef.h"
#include <strsafe.h>
#include <assert.h>

#pragma comment(lib,"winmm.lib")			//����PlaySound����������ļ�
#pragma comment(lib,"Msimg32.lib")		//���ʹ��TransparentBlt��������Ŀ��ļ�


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
 *	�������࣬�����˻���������ͼ�������
 */
class CBeing {
public:
	CBeing():m_img(NULL),m_bImg(NULL),x(0),y(0), vx(0), vy(0), ax(0), ay(1) {}
	CImage *m_img, *m_bImg;
	int imgN;
	int x,y;//ͼ���ճ����ַ
	int vx, vy, ax, ay;//x�ٶȺ�y�ٶ�
	RECT rect;//ͼ����ʾ��С������CSCENE
	double zoom;
	inline void Mov(INT fx , int fy) {//�ƶ�
		x += fx;
		y += fy;
	}
	inline void SetPos(INT px , INT py) {//��ֱ�ƶ�
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
 *	������࣬����ĳ�ʼ��������Ķ�������ִ�к��������ﵱǰ����������Ϣ
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
		*	1.���AlphaFormat�ֶ�Ϊ0,Դλͼ�е���������ʹ��ͬ���ĳ���alphaֵ����SourceConstantAlpha�ֶ��е�ֵ����ֵʵ������0��255��������0��1������0��ʾ��ȫ͸����255��ʾ��ȫ��͸����Ŀ��������255-SourceConstantAlphaֵ��Ϊalphaֵ��

		2. ���AlphaFormat�ֶε�ֵ��AC_SRC_ALPHA��Դ�豸�����ÿ�����ر����и��Ե�alphaͨ��������������32-bpp�������豸�����ģ�����ѡ����32-bpp DDB��DIB�ε��ڴ��豸�����ġ���Щ����£�ÿ��Դ������4��8λͨ��:�졢�̡�����alpha��ÿ�����ص�alphaͨ����SourceConstantAlpha�ֶ�һ�����ڰ�Դ��Ŀ����������ʵ�����ڼ��������ʽ����:

		Tmp.Red = Src.Red * SourceConstantAlpha / 255;

		Tmp.Green = Src.Green * SourceConstantAlpha / 255;

		Tmp.Blue = Src.Blue * SourceConstantAlpha / 255;

		Tmp.Alpha = Src.Alpha * SourceConstantAlpha / 255;

		Beta = 255 �C Tmp.alpha;

		Dst.Red = Tmp.Red + Round((Beta * Dst.Red )/255);

		Dst.Green = Tmp.Green + Round((Beta * Dst.Green)/255);

		Dst.Blue = Tmp.Blue + Round((Beta * Dst.Blue )/255);

		Dst.Alpha = Tmp.Alpha + Round((Beta * Dst.Alpha)/255);
		 */
		if (m_img->GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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

		if (m_bImg->GetBPP() == 32) //ȷ�ϸ�ͼ�����Alphaͨ��
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
	//img�ǰ������ж���ͼƬ��png
	//cfg�ǰ���ͼƬ������Ϣ��dat
	//���磺������npc������ҿ���
	//����:�����ܶ�ͼ��λ�ã��������ң������ﶯ��ͼ��λ�ã���������ɱ��������������ȣ����ж����򣨹������������ܴ���
	//���磺�����ܶ�ͼ�Ĵ�����������ɱ���İ�������
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
	//��õ�ǰִ�ж���
	int GetActNo(void) {
		return actNo;
	}
	//��õ�ǰִ�ж���
	int GetActDir(void) {
		return actDir;
	}
	void SetWhere(int n) {
		where = n;
	}
	int GetWhere(void) {
		return where;
	}
	//�ı䵱ǰִ�ж���
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
	//���ݶ���//����ֵ��ʾFALSE��ʾ�ƶ���TRUE��ʾδ�ƶ�
	RET_AE ActNext(HDC hdcDest,INT fx , int fy){
		int dir , shiftX = 0;
		RET_AE ret = AE_TRUE;
		//SetStretchBltMode (hdcDest,HALFTONE);
		//SetBrushOrgEx(hdcDest, 0, 0, NULL);
		if ('J'== (char)info[act[actNo][1]][HUMAN_ACT_CHAR])//���act
		{
			ChangeActNo(info[act[actNo][1]][HUMAN_ACT_X] , actDir);
		} else if('M'== (char)info[act[actNo][1]][HUMAN_ACT_CHAR]) {
			Mov(fx , fy);
			ret = AE_FALSE;
		}
		if (actDir)//�������
		{
			dir = -1;//����ľ���Ĭ��Ϊ��
			shiftX = rect.right / 2;// info[act[0][0]][3] * zoom / 2;//SHIFT_LEFT_MOVE;//������������
		}
		else
		{
			dir = 1;//����
			shiftX = 0;
		}
		m_bImg->StretchBlt(hdcDest 
			,x + shiftX,y,info [act[actNo][1]][HUMAN_ACT_X + POS_W] * zoom * dir, info[act[actNo][1]][HUMAN_ACT_X + POS_H] * zoom
			,info [act [actNo][1]][HUMAN_ACT_X] ,info[act[actNo][1]][HUMAN_ACT_X + POS_Y] ,info[act[actNo][1]][HUMAN_ACT_X + POS_W] , info[act[actNo][1]][HUMAN_ACT_X + POS_H],SRCAND);//����������
		m_img->StretchBlt(hdcDest 
			, x + shiftX, y, info[act[actNo][1]][HUMAN_ACT_X + POS_W] * zoom * dir, info[act[actNo][1]][HUMAN_ACT_X + POS_H] * zoom
			, info[act[actNo][1]][HUMAN_ACT_X], info[act[actNo][1]][HUMAN_ACT_X + POS_Y], info[act[actNo][1]][HUMAN_ACT_X + POS_W], info[act[actNo][1]][HUMAN_ACT_X + POS_H], SRCPAINT);//������ǰ��
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
		,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4],AC_SRC_OVER);��֧�־�������drawҲ���ܾ���*/
		
		/*m_img->TransparentBlt(hdcDest 
		,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
		,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4],RGB(255,255,255));*/
		/*m_img->Draw(hdcDest ,x,y,info [act[actNo][1]][3] * zoom * dir , info[act[actNo][1]][4] * zoom
			,info [act [actNo][1]][1] ,info[act[actNo][1]][2] ,info[act[actNo][1]][3] , info[act[actNo][1]][4]);*/
		//�ж��Ƿ��е�����Ҫ����tmpact
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
	//���ݸ��Ӷ���
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
					tmpAct[i][0] = -1;//������tmpact
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
		1	ͼx	ͼy	ͼw	ͼh	��x	��y	��w	��h	��x	��y	��w	��h
		2
		3
		4
		5
		6
		7
	************************************************************************/
	int (*info)[HUMAN_INFO_LENGTH];
	int act[10][3];//ʮ�ֶ�����������ʼ���act[i][0]/�͵�ǰ���������act[i][1]������act[i][2]
	int tmpAct[20][5];//����������һ���Ƿ��е���֮�ࡢ��i���������ǵڼ���tmpAct[i][0]/ִ�е���һ֡tmpAct[i][1]/����x/����ytmpAct[i][2/3]����tmpAct[i][4]
	int actNum;//act�м��֣����ļ��õ�
	int actNo;//ִ��actNo
	int actDir;//ִ�з���0���䣬1����
	int where;//�����������������0������1��ˮ��2

};

/*
 *	����������
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
	//img�ǰ������ж���ͼƬ��png
	//cfg�ǰ���ͼƬ������Ϣ��dat
	//���磺������npc������ҿ���
	//����:�����ܶ�ͼ��λ�ã��������ң������ﶯ��ͼ��λ�ã���������ɱ��������������ȣ����ж����򣨹������������ܴ���
	//���磺�����ܶ�ͼ�Ĵ�����������ɱ���İ�������
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
		1	ͼx	ͼy	ͼw	ͼh	��x	��y	��w	��h	��x	��y	��w	��h
		2
		3
		4
		5
		6
		7
	************************************************************************/
	int (*info)[11];
	int act[10][3];//ʮ�ֶ�����������ʼ���act[i][0]/�͵�ǰ���������act[i][1]������act[i][2]
	int tmpAct[20][4];//����������һ���Ƿ��е���֮�ࡢ��i���������ǵڼ���tmpAct[i][0]/ִ�е���һ֡tmpAct[i][1]/����x/����ytmpAct[i][2/3]
	int actNum;//act�м��֣����ļ��õ�
	int actNo;//ִ��actNo
	
};
/*
 *	������
 */
class CScene : public CBeing{
public:
	CScene();
	/*img��ͼ��bImg�����ϰ���ͼ��rc��ʾ����*/
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
 *	˫������ͼ��
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
		bufHdc = CreateCompatibleDC(hdc);  //����һ����hdc���ݵ��ڴ�DC
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
 *	���ڵĴ��������ٺ��Ӵ��ڵĴ���
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
 *	�����࣬��Ϸ��������ˢ�¡��������������
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