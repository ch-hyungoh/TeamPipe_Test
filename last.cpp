// last.cpp: 구현 파일//

#include "stdafx.h"
#include "MFCAkk2.h"
#include "last.h"
#include "afxdialogex.h"
#include "keyboard.h"
#include "XLEzAutomation.h"

//상대경로로 헤더파일을 지정합니다.
#include "./include/GL/glew.h"
#include "./include/GL/wglew.h"

//사용할 라이브러리를 지정해줍니다.
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "./lib/glew32.lib")

extern CString Zgap, ArcON, ArcOFF, Zturn, Boon, Pdelay, Speed, CH, PH, WC, ESpeed, RSpeed, LSpeed;
//ArcON, ArcOFF=플라즈마 아크 온오프,Zturn=GAP 포인트 리턴, boon 미소블럭 분해등분수
extern CString Dm, Db, Dmt, Dbt, e, Length, a, aa1, aa2, CL, CG, SL, W1, SN, Re, De, s, Dm2, Dmt2, RBJE, getsoo, guri, Lc2, Lc3, L, W; 
//Db=외경, Dm=모관 외경,e=편심, Lc=교접 길이, Dbt=두께,De=엘보지름, s=반경 편심, L=슬롯 길이, W=슬롯폭, CL=크라운길이, CG=크라운길이, RBJE=재질, getsoo=절단갯수, guri=피스간 갭
//Length=피스간 길이
extern CString NUM ,RBelimit, jejil1;
//RBeLimit=베벨링 한계각도
extern float ONIN;
extern CString A1, A2, OR;
//OR=오버컷 길이
extern CString B1, B2, B3, B4, B5, B6, B7, B8, B9, B10, B11, B12/*hrot*/, B13;
extern CString Bv1_1, Bv2_2, Bv3_3, Bv4_4;
extern CString Bv1_11, Bv2_22, Bv3_33, Bv4_44;
extern CString C1, C2, C3, C4, C5;
extern CString Bv1, Bv2, Bv3, Bv4, Bv5, Bv6, Bv7, Bv8, Bv9, Bv10, Bv11, Bv12;
extern int Am;
extern int AON;
extern int hole_print;

extern float thick;
extern int Me;
extern int Bvf;
extern int overcut;

// last 대화 상자

IMPLEMENT_DYNAMIC(last, CDialogEx)

last::last(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG18, pParent)
{

}

last::~last()
{
}

void last::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, m_picture);

	if (hole_print == 0)
	{
		CButton *R4;
		R4 = (CButton*)GetDlgItem(IDC_RADIO4);
		R4->SetCheck(TRUE);
	}
	else if (hole_print == 1)
	{
		CButton *R3;
		R3 = (CButton*)GetDlgItem(IDC_RADIO3);
		R3->SetCheck(TRUE);
	}


	if (!GetRenderingContext())
	{
		AfxMessageBox(CString("OpenGL 초기화중 에러가 발생하여 프로그램을 실행할 수 없습니다."));
	}

	SetTimer(1000, 30, NULL);
	DDX_Control(pDX, IDC_LIST1, m_LIST);
	DDX_Control(pDX, IDC_SLIDER_X, m_SL_X);
	DDX_Control(pDX, IDC_SLIDER_Y, m_SL_Y);
	DDX_Control(pDX, IDC_SLIDER_Z, m_SL_Z);

	SetDlgItemText(IDC_EDIT118, Boon);
}


BEGIN_MESSAGE_MAP(last, CDialogEx)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON1, &last::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &last::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &last::OnBnClickedButton3)
	ON_LBN_SELCHANGE(IDC_LIST1, &last::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON6, &last::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON4, &last::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON47, &last::OnBnClickedButton47)
	ON_BN_CLICKED(IDC_BUTTON5, &last::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON61, &last::OnBnClickedButton61)
	ON_BN_CLICKED(IDC_BUTTON31, &last::OnBnClickedButton31)
	ON_BN_CLICKED(IDC_BUTTON32, &last::OnBnClickedButton32)
	//ON_BN_CLICKED(IDC_BUTTON33, &last::OnBnClickedButton33) 6-11 수정
	ON_BN_CLICKED(IDC_BUTTON151, &last::OnBnClickedButton151)
	ON_BN_CLICKED(IDC_BUTTON34, &last::OnBnClickedButton34)
	ON_BN_CLICKED(IDC_BUTTON7, &last::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &last::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON64, &last::OnBnClickedButton64)
	ON_BN_CLICKED(IDC_BUTTON88, &last::OnBnClickedButton88)
	ON_BN_CLICKED(IDC_BUTTON153, &last::OnBnClickedButton153)
	ON_BN_CLICKED(IDC_BUTTON92, &last::OnBnClickedButton92)
	ON_BN_CLICKED(IDC_BUTTON149, &last::OnBnClickedButton149)
	ON_BN_CLICKED(IDC_RADIO3, &last::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &last::OnBnClickedRadio4)
END_MESSAGE_MAP()


// last 메시지 처리기


void last::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	

	if (FALSE == ::wglDeleteContext(m_hRC))
	{
		AfxMessageBox(CString("wglDeleteContext failed"));
	}
}

////////////////////////////////////////////////////////////Open GL로 모형 그리기 코드//////////////////////////////////////////////////
void last::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnTimer(nIDEvent);

	float PI = 3.141592654;
	float RbT = _tstof(Dbt);
	float RmT = _tstof(Dmt);
	/* 
	[2020년 7월 1일 역베벨링 수정으로 인한 절대값 표기 - 김대현]
	float Bevel_total = (_tstof(Bv1) + _tstof(Bv2) + _tstof(Bv3) + _tstof(Bv4)) / 4;
	*/
	float Bevel_total = (fabs(_tstof(Bv1)) + fabs(_tstof(Bv2)) + fabs(_tstof(Bv3)) + fabs(_tstof(Bv4))) / 4;
	if (Me < 100)
	{
		RbT = RbT / cos(Bevel_total *  (PI / 180.0));
		CString ToB;
		ToB.Format(L"%.3f", RbT);
		SetDlgItemText(IDC_EDIT117, ToB);//미소블럭 등분 수
	}
	else if (Me > 100)
	{
		RmT = RmT / cos(Bevel_total *  (PI / 180.0));
		CString ToB;
		ToB.Format(L"%.3f", RmT);
		SetDlgItemText(IDC_EDIT117, ToB);
	}

	CString ArcC;
	ArcC.Format(L"%d", Am);
	SetDlgItemText(IDC_EDIT69, ArcC);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);//선의 색깔
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	//z축을 중심으로 설정된 angle로 회전한다.
	glRotatef(angleX, 1.0f, 0.0f, 0.0f);
	glRotatef(angleZ, 0.0f, 1.0f, 0.0f);
	glRotatef(angleY, 0.0f, 0.0f, 1.0f);

	//점으로 그리기 위한 설정
	glBegin(GL_POINTS);


	float DB = _tstof(Db) / 2;//외경 지름을 반지름으로
	float DM = _tstof(Dm) / 2;
	float EE = _tstof(e);//편심
	float Cut = _tstof(Boon);//미소블럭 등분수
	//float PI = 3.141592654;
	float Lc = _tstof(Length);
	float X;


	////////////////////////// 지관 /////////////////////////
	if (Me == 1) // 수직교차
	{
		float alfa = 90;
		float log = Lc;


		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = (((-DB * cos(i * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180))) * (EE - DB * sin(i * (PI / 180))))));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (((-DB * cos(Q * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DM*DM) - (EE - DB * sin(Q * (PI / 180))) * (EE - DB * sin(Q * (PI / 180))))));
			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 2) // 예각교차
	{
		float alfa = _tstof(a);
		float log = Lc;
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180))) * (EE - DB * sin(i * (PI / 180)))));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = ((-DB * cos(Q * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DM*DM) - (EE - DB * sin(Q * (PI / 180))) * (EE - DB * sin(Q * (PI / 180)))));

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 3) // V셰입
	{
		float alfa = 90;
		EE = 0;
		float log = Lc;
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DB*DB) - (EE - DB * sin(i * (PI / 180))) * (EE - DB * sin(i * (PI / 180)))));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = ((-DB * cos(Q * (PI / 180))) / tan(alfa * (PI / 180))) - ((1 / sin(alfa * (PI / 180))) * sqrt((DB*DB) - (EE - DB * sin(Q * (PI / 180))) * (EE - DB * sin(Q * (PI / 180)))));

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 4) // 평판수직교차
	{
		float X = 0;
		float log = _tstof(Length);
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, 0 + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			for (float T = X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();

	}
	else if (Me == 5) // 평판예각교차
	{
		float alfa = _tstof(a);
		float log = _tstof(Length);
		float h = tan(alfa * (PI / 180)) * 2 * DB;

		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = (h / 2)*cos(i * (PI / 180)) + (h / 2);

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (h / 2)*cos(Q * (PI / 180)) + (h / 2);

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 6) // 더블마이트엔드컷
	{
		float alfa1 = _tstof(aa1);
		float alfa2 = _tstof(aa2);

		float phi = acos(EE / DB) * (180 / PI);
		float i = 0;
		float log = _tstof(Length);
		for (i; i < phi; i += (360 / Cut))
		{
			X = -((DB * cos(i * (PI / 180))) - EE) * tan(alfa1 * (PI / 180));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}
		for (i; i < 270 + (90 - phi); i += (360 / Cut))
		{
			X = -(EE - (DB * cos(i * (PI / 180)))) * tan(alfa2 * (PI / 180));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}
		for (i; i <= 360; i += (360 / Cut))
		{
			X = -((DB * cos(i * (PI / 180))) - EE) * tan(alfa1 * (PI / 180));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}

		float Q = 0;
		for (Q; Q < phi; Q += 45)
		{
			X = -((DB * cos(Q * (PI / 180))) - EE) * tan(alfa1 * (PI / 180));

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (Q; Q < 270 + (90 - phi); Q += 45)
		{
			X = -(EE - (DB * cos(Q * (PI / 180)))) * tan(alfa2 * (PI / 180));

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (Q; Q < 360; Q += 45)
		{
			X = -((DB * cos(Q * (PI / 180))) - EE) * tan(alfa1 * (PI / 180));

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 7) // 계판절단
	{
		float h = _tstof(SL);	//높이
		float o = _tstof(SN);	//개수
		float w = _tstof(W1);	//폭
		float log = _tstof(Length);

		float i = 0;
		float part = (360 / o);
		float Lp = (h / 2) * cos(0) + (h / 2);

		for (i = 0; i <= 360; i += (360 / Cut))
		{
			X = (h / 2)*cos(i * o * (PI / 180)) + (h / 2);

			if (Lp > X)
			{
				Lp = X;
			}
		}

		i = 0;

		for (int N = 0; N <= o - 1; N++)
		{

			for (i; i <= part; i += (360 / Cut))
			{
				X = (h / 2)*cos(i * o * (PI / 180)) + (h / 2);

				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, h * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면

				if (X <= 0.1)
				{
					for (float high = h; high >= 0; high--)
					{
						glVertex3f(DB * cos(i * (PI / 180)) * Zoom, high * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면
						glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
					}
					break;
				}

			}

			float idisk = i;
			float sita = (180 * w) / (PI * DB);

			for (float Q = 0; Q <= 360; Q += 45)
			{
				X = (h / 2)*cos(Q * o * (PI / 180)) + (h / 2);

				for (float T = 0; T <= log; T += 10)
				{
					glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}

			for (i; i <= idisk + sita; i += (360 / Cut))
			{
				X = (h / 2)*cos(i * o * (PI / 180)) + (h / 2);

				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, 0 * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);			//원기둥의 절단면
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
			}
			for (float high = 0; high <= h; high++)
			{
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, high * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 절단면
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
			}
			for (i; i <= part; i += (360 / Cut))
			{
				X = (h / 2)*cos(i * o * (PI / 180)) + (h / 2);

				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, h * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);			//원기둥의 절단면
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
			}
			part += (360 / o);

		}
		glEnd();
	}
	else if (Me == 8) // 크라운
	{
		float h = _tstof(CL);
		float o = _tstof(CG);
		float log = _tstof(Length);

		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = (h / 2)*cos(i*o* (PI / 180)) + (h / 2);

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (h / 2)*cos(Q * o * (PI / 180)) + (h / 2);

			for (float T = -X; T <= log; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 9) //애벌래
	{
		float X = 0;
		float G = _tstof(guri);
		float o = _tstof(getsoo);
		float log = _tstof(Length);
		float gan = log + G;

		for (float Q = 1; Q <= o; Q++)
		{
			for (float i = 0; i <= 360; i += (360 / Cut))
			{
				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

				glVertex3f(DB * cos(i * (PI / 180)) * Zoom, log * -Zoom + 0.5f, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
			}

			for (float Q = 0; Q <= 360; Q += 45)
			{
				for (float T = X; T <= log; T += 10)
				{
					glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, -T * Zoom + 0.5f, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}
			X = X + gan;
			log = log + gan;
		}
		glEnd();
	}
	else if (Me == 10) // 엘보
	{
		float PI = 3.141592654;
		float X;
		float Lc = _tstof(Length);
		float RE = _tstof(Re);
		float DE = _tstof(De);
		float Tb = _tstof(Dbt);
		float S = _tstof(s);
		float E = _tstof(e);

		float To = RE + (DE / 2);
		float R = DE / 2;
		float r = DB - Tb;

		float K = sqrt(powf((To + R) - (R - sqrt(powf(R, 2) - powf(r * sin(0 * (PI / 180)) - E, 2))), 2) - powf(To + S + (r * cos(0 * (PI / 180))), 2));


		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = -(sqrt(powf((To + R) - (R - sqrt(powf(R, 2) - powf(r * sin(i * (PI / 180)) - E, 2))), 2) - powf(To + S + (r * cos(i * (PI / 180))), 2)) - K);

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, X * Zoom, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 절단면

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, Lc * -Zoom, DB * sin(i * (PI / 180)) * Zoom);		//원기둥의 윗면
		}
		for (float Q = 0; Q < 360; Q += 45)
		{
			X = -(sqrt(powf((To + R) - (R - sqrt(powf(R, 2) - powf(r * sin(Q * (PI / 180)) - E, 2))), 2) - powf(To + S + (r * cos(Q * (PI / 180))), 2)) - K);

			for (float T = X; T >= -Lc; T -= 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, T * Zoom, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 11) //양쪽 평면절단
	{
		float alfa1 = 90.0 - _tstof(aa1);
		float alfa2 = 90.0 - _tstof(aa2);
		float h1, h2;
		float Lc = _tstof(Length);

		h1 = tan(alfa1 * (PI / 180)) * 2 * DB;
		h2 = tan(alfa2 * (PI / 180)) * 2 * DB;
		float EX1 = (Lc / 2) - ((h1 / 2) * cos(45 * (PI / 180)) + (h1 / 2));
		float EX2 = (Lc / 2) - ((h2 / 2) * cos(45 * (PI / 180)) + (h2 / 2));

		for (float i = 0; i <= 360; i = i + (360 / Cut))
		{
			X = (h1 / 2) * cos(i * (PI / 180)) + (h1 / 2);

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, (X + EX1) * Zoom, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 윗절단면
		}

		for (float i = 0; i <= 360; i = i + (360 / Cut))
		{
			X = (h2 / 2) * cos(i * (PI / 180)) + (h2 / 2);

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, (-X - EX2) * Zoom, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 아랫절단면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (h1 / 2)*cos(Q*(PI / 180)) + (h1 / 2);

			for (float T = X + EX1; T >= 0; T -= 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, T * Zoom, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}

			X = (h2 / 2)*cos(Q*(PI / 180)) + (h2 / 2);

			for (float T = -X - EX2; T <= 0; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, T * Zoom, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 12) // 양쪽사선절단 ( H )
	{
		float Lc = _tstof(Length);
		float Rm1 = _tstof(Dm) / 2.0;
		float Rm2 = _tstof(Dm2) / 2.0;
		float alfa1 = _tstof(aa1);//우측
		float alfa2 = _tstof(aa2);
		float E = _tstof(e);
		float S = _tstof(s);
		float AA = 0, BB = 0;

		float D1 = ((-DB * cos(0 * (PI / 180))) / tan(alfa1 * (PI / 180))) - ((1 / sin(alfa1 * (PI / 180))) * sqrt((Rm1*Rm1) - (E - DB * sin(0 * (PI / 180))) * (E - DB * sin(0 * (PI / 180)))));
		float D2 = ((-DB * cos(0 * (PI / 180))) / tan(alfa2 * (PI / 180))) - ((1 / sin(alfa2 * (PI / 180))) * sqrt((Rm2*Rm2) - (S - DB * sin(0 * (PI / 180))) * (S - DB * sin(0 * (PI / 180)))));
		for (float i = 0; i <= 360; i += (360 / Cut)) // 최소점 찾기
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa1 * (PI / 180))) - ((1 / sin(alfa1 * (PI / 180))) * sqrt((Rm1*Rm1) - (E - DB * sin(i * (PI / 180))) * (E - DB * sin(i * (PI / 180)))));

			if (D1 > X)
			{
				D1 = X;
				AA = i;
			}
		}
		for (float i = 0; i <= 360; i += (360 / Cut)) // 최소점 찾기
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa2 * (PI / 180))) - ((1 / sin(alfa2 * (PI / 180))) * sqrt((Rm2*Rm2) - (S - DB * sin(i * (PI / 180))) * (S - DB * sin(i * (PI / 180)))));

			if (D2 > X)
			{
				D2 = X;
				BB = i;
			}
		}

		float A1Dap = (Rm1 + DB * sin((90 - alfa1) * (PI / 180))) / sin(alfa1  * (PI / 180));
		float A2Dap = (Rm2 + DB * sin((90 - alfa2) * (PI / 180))) / sin(alfa2  * (PI / 180));

		float C = Lc - A1Dap - A2Dap;

		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa1 * (PI / 180))) - ((1 / sin(alfa1 * (PI / 180))) * sqrt((Rm1*Rm1) - (E - DB * sin(i * (PI / 180))) * (E - DB * sin(i * (PI / 180)))));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, (X - D1 + (C / 2)) * Zoom, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 윗면
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = ((-DB * cos(i * (PI / 180))) / tan(alfa2 * (PI / 180))) - ((1 / sin(alfa2 * (PI / 180))) * sqrt((Rm2*Rm2) - (S - DB * sin(i * (PI / 180))) * (S - DB * sin(i * (PI / 180)))));

			glVertex3f(DB * cos(i * (PI / 180)) * Zoom, -(X - D2 + (C / 2)) * Zoom, DB * sin(i * (PI / 180)) * Zoom);         //원기둥의 아랫면
		}

		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = ((-DB * cos(Q * (PI / 180))) / tan(alfa1 * (PI / 180))) - ((1 / sin(alfa1 * (PI / 180))) * sqrt((Rm1*Rm1) - (E - DB * sin(Q * (PI / 180))) * (E - DB * sin(Q * (PI / 180)))));

			for (float T = X - D1 + (C / 2); T >= 0; T -= 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, T * Zoom, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = ((-DB * cos(Q * (PI / 180))) / tan(alfa2 * (PI / 180))) - ((1 / sin(alfa2 * (PI / 180))) * sqrt((Rm2*Rm2) - (S - DB * sin(Q * (PI / 180))) * (S - DB * sin(Q * (PI / 180)))));

			for (float T = -(X - D2 + (C / 2)); T <= 0; T += 10)
			{
				glVertex3f(DB * cos(Q * (PI / 180)) * Zoom, T * Zoom, DB * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	////////////////////////// 모관 /////////////////////////
	else if (Me == 101) // 모관 수직
	{
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float t = 90;
		if (EE > DB)
		{
			for (float i = 0; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
		}
		else
		{
			float elimit = asin(EE / DB)*(180 / PI);
			for (float i = 0; i <= elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = elimit; i <= 180 - elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = 180 - elimit; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcR;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 우측면
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcL;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, -X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 좌측면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = LcR; T >= X; T -= 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = -LcL; T <= -X; T += 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}

	else if (Me == 102) // 모관 사선
	{
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float t = _tstof(a);
		if (EE > DB)
		{
			for (float i = 0; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
		}
		else
		{
			float elimit = asin(EE / DB)*(180 / PI);
			for (float i = 0; i <= elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = elimit; i <= 180 - elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = 180 - elimit; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcR;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 우측면
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcL;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, -X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 좌측면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = LcR; T >= X; T -= 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = -LcL; T <= -X; T += 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 103) // 모관 V셰입
	{
		float DB = _tstof(Dm) / 2;
		float DM = _tstof(Dm) / 2;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float t = 90;
		if (EE > DB)
		{
			for (float i = 0; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
		}
		else
		{
			float elimit = asin(EE / DB)*(180 / PI);
			for (float i = 0; i <= elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180)))*(EE - DB * sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = elimit; i <= 180 - elimit; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}
			for (float i = 180 - elimit; i <= 360; i += (360 / Cut))
			{
				X = (DB * cos(i * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(i * (PI / 180)))*(EE - (DB)* sin(i * (PI / 180))))));
				float Q2 = -acos((sqrt(1 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180)))))) * (180 / PI);
				float Y2 = DM * Q2 * (PI / 180);

				glVertex3f(DM * cos(Q2 * (PI / 180)) * Zoom, X * Zoom, DM * sin(Q2 * (PI / 180)) * Zoom);  //원기둥 절단면
			}

		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcR;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 우측면
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcL;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, -X * Zoom, DM * sin(i * (PI / 180)) * Zoom);  //원기둥 좌측면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = LcR; T >= X; T -= 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			X = (DB * cos(Q * (PI / 180)) / sin(t * (PI / 180))) + (1 / tan(t * (PI / 180))) * (sqrt((DM * DM) - ((EE - (DB)* sin(Q * (PI / 180)))*(EE - (DB)* sin(Q * (PI / 180))))));
			for (float T = -LcL; T <= -X; T += 10)
			{
				glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
			}
		}
		glEnd();
	}
	else if (Me == 104) // 모관 사각사각
	{
		float DM = _tstof(Dm) / 2;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);

		float Lline = _tstof(L);
		float Wline = _tstof(W);
		float Plimit = (180 * Wline) / (PI * DM);

		for (float i = 0; i < Lline / 2; i += 0.1)
		{
			glVertex3f(DM * cos(0 * (PI / 180)) * Zoom, i * Zoom, DM * sin(0 * (PI / 180)) * Zoom);	// 원기둥의 반높이
		}
		for (float i = 0; i < Plimit; i += 0.1)
		{
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, Lline / 2 * Zoom, DM * sin(i * (PI / 180)) * Zoom);	// 원기둥의 밑면
		}
		for (float i = Lline / 2; i > -Lline / 2; i -= 0.1)
		{
			glVertex3f(DM * cos(Plimit * (PI / 180)) * Zoom, i * Zoom, DM * sin(Plimit * (PI / 180)) * Zoom);	// 원기둥의 높이
		}
		for (float i = Plimit; i > 0; i -= 0.1)
		{
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, -Lline / 2 * Zoom, DM * sin(i * (PI / 180)) * Zoom);	// 원기둥의 밑면
		}
		for (float i = -Lline / 2; i < 0; i += 0.1)
		{
			glVertex3f(DM * cos(0 * (PI / 180)) * Zoom, i * Zoom, DM * sin(0 * (PI / 180)) * Zoom);	// 원기둥의 반높이
		}


		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcR;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, X * Zoom, DM * sin(i * (PI / 180)) * Zoom);	// 원기둥의 밑면
		}
		for (float i = 0; i <= 360; i += (360 / Cut))
		{
			X = LcL;
			glVertex3f(DM * cos(i * (PI / 180)) * Zoom, -X * Zoom, DM * sin(i * (PI / 180)) * Zoom);	// 원기둥의 밑면
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			if (Q < Plimit)
			{
				for (float T = LcR; T >= Lline / 2; T -= 10)
				{
					glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}
			else
			{
				for (float T = LcR; T >= 0; T -= 10)
				{
					glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}
		}
		for (float Q = 0; Q <= 360; Q += 45)
		{
			if (Q < Plimit)
			{
				for (float T = -LcR; T <= -Lline / 2; T += 10)
				{
					glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}
			else
			{
				for (float T = -LcL; T <= 0; T += 10)
				{
					glVertex3f(DM * cos(Q * (PI / 180)) * Zoom, T * Zoom, DM * sin(Q * (PI / 180)) * Zoom);	//원기둥의 옆면
				}
			}
		}
		glEnd();
	}
	// 형상 회전각
	m_SL_X.SetRange(0, 360);
	m_SL_Y.SetRange(0, 360);
	m_SL_Z.SetRange(0, 360);

	float SLx = m_SL_X.GetPos();
	angleX = SLx;
	float SLy = m_SL_Y.GetPos();
	angleY = SLy;
	float SLz = m_SL_Z.GetPos();
	angleZ = SLz;


	//화면 업데이트
	SwapBuffers(m_pDC->GetSafeHdc());
}


BOOL last::GetRenderingContext()
{
	//픽처 컨트롤에만 그리도록 DC 생성
	CWnd* pImage = GetDlgItem(IDC_PICTURE);
	CRect rc;
	pImage->GetWindowRect(rc);
	m_pDC = pImage->GetDC();


	if (NULL == m_pDC)
	{
		AfxMessageBox(CString("Unable to get a DC"));
		return FALSE;
	}


	if (!GetOldStyleRenderingContext())
	{
		return TRUE;
	}


	//Get access to modern OpenGL functionality from this old style context.
	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit())
	{
		AfxMessageBox(CString("GLEW could not be initialized!"));
		return FALSE;
	}


	GLint attribs[] =
	{
		//OpenGL 2.0 사용
		WGL_CONTEXT_MAJOR_VERSION_ARB, 2,
		WGL_CONTEXT_MINOR_VERSION_ARB, 0,
		// Uncomment this for forward compatibility mode
		//WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		// Uncomment this for Compatibility profile
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB,
		// We are using Core profile here
		//WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};


	HGLRC CompHRC = wglCreateContextAttribsARB(m_pDC->GetSafeHdc(), 0, attribs);
	if (CompHRC && wglMakeCurrent(m_pDC->GetSafeHdc(), CompHRC))
		m_hRC = CompHRC;

	return TRUE;
}

BOOL last::GetOldStyleRenderingContext()
{
	//A generic pixel format descriptor. This will be replaced with a more
	//specific and modern one later, so don't worry about it too much.
	static PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |            // support window
		PFD_SUPPORT_OPENGL |            // support OpenGL
		PFD_DOUBLEBUFFER,               // double buffered
		PFD_TYPE_RGBA,                  // RGBA type
		32,                             // 32-bit color depth
		0, 0, 0, 0, 0, 0,               // color bits ignored
		0,                              // no alpha buffer
		0,                              // shift bit ignored
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // accum bits ignored
		24,								// 24-bit z-buffer
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main layer
		0,                              // reserved
		0, 0, 0                         // layer masks ignored
	};

	// Get the id number for the best match supported by the hardware device context
	// to what is described in pfd
	int pixelFormat = ChoosePixelFormat(m_pDC->GetSafeHdc(), &pfd);

	//If there's no match, report an error
	if (0 == pixelFormat)
	{
		AfxMessageBox(CString("ChoosePixelFormat failed"));
		return FALSE;
	}

	//If there is an acceptable match, set it as the current 
	if (FALSE == SetPixelFormat(m_pDC->GetSafeHdc(), pixelFormat, &pfd))
	{
		AfxMessageBox(CString("SetPixelFormat failed"));
		return FALSE;
	}

	//Create a context with this pixel format
	if (0 == (m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		AfxMessageBox(CString("wglCreateContext failed"));
		return FALSE;
	}

	//Make it current. 
	if (FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		AfxMessageBox(CString("wglMakeCurrent failed"));
		return FALSE;
	}
	return TRUE;
}

int Onum = 1;
////////////////////////////////////////////////////////////////////////////////   G - Code   //////////////////////////////////////////////////////////////////////////////////////////
void last::OnBnClickedButton1()
{
	CString Va, Zr;
	GetDlgItemText(IDC_EDIT1, WC);
	GetDlgItemText(IDC_EDIT2, CH);
	GetDlgItemText(IDC_EDIT29, Speed);
	GetDlgItemText(IDC_EDIT3, PH);
	GetDlgItemText(IDC_EDIT45, Pdelay);
	GetDlgItemText(IDC_EDIT115, Va);
	GetDlgItemText(IDC_EDIT116, Zr);
	GetDlgItemText(IDC_EDIT37, ESpeed);
	GetDlgItemText(IDC_EDIT10, RSpeed);
	GetDlgItemText(IDC_EDIT37, LSpeed);

	GetDlgItemText(IDC_EDIT118, Boon);

	float RTb = _tstof(Dbt);
	float RTm = _tstof(Dmt);
	float Cut = _tstof(Boon);
	int Number = 10;
	float Wc = _tstof(WC) / 2;
	float hc = _tstof(CH);
	float pd = _tstof(Pdelay);
	float ph = _tstof(PH);
	float speed = _tstof(Speed);
	float Espeed = _tstof(ESpeed);
	float Rspeed = _tstof(RSpeed);
	float Lspeed = _tstof(LSpeed);
	float Sl = _tstof(A1);
	float vapp = _tstof(Va);
	float zrtn = _tstof(Zr);

	////////////////////////////////////////////////////////////////////////////////   NC code   //////////////////////////////////////////////////////////////////////////////////////////
	
	CString K = _T("%");
	CString temp;
	temp.Format(L"%s", K);
	m_LIST.AddString(temp);
	temp.Format(L"O%04d", Onum);
	m_LIST.AddString(temp);

	if (Me == 1) ////////////////////////////////////////////// 지관수직절단
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float DM = _tstof(Dm) / 2.0;
		float EE = _tstof(e);
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float alfa = 90.0;
		float F, X;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		// float YL = (DB * 2.0 * 3.141592654) / Cut; 변수교정[< >2020.09.08 - 김대현]
		float YL = (Rb * 2.0 * 3.141592654) / Cut;
		float topangle = 0;
		float DB = Rb; // 변수교정[< >2020.09.08 - 김대현]

		if (ONIN == 1)		//Set-ON
		{
			DB = DB - RTb; 
		}
		else if (ONIN == 2)	//Set-IN
		{
			DM = DM - RTm;
		/*
			[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현]
			Bev0_1 = -1 * _tstof(Bv1);
			Bev90_1 = -1 * _tstof(Bv2);
			Bev180_1 = -1 * _tstof(Bv3);
			Bev270_1 = -1 * _tstof(Bv4);
		*/
		}

		/////////////////////////////////////////////////
		////최대점 찾기
		/////////////////////////////////////////////////
		float top = -10000.0;//파이프의 반지름?의 최대값?
		for (float i = 0.0; i <= 360.0; i += (360.0 / Cut))
		{
			X = (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));
			if(X > top)
			{
				top = X;
				topangle = i;
			}
		}

		/////////////////////////////////////////////////
		//// 베벨링부분
		/////////////////////////////////////////////////
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= topangle; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		CString temp;

		F = top;
		if (ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		else if (ONIN == 2)	X = F - ((hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		X = X + _tstof(A1);
		Y = -1 * _tstof(A1);

		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/* 
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;
		
		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;
		
		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;
		
		ZZ = hc;
		float YY = 0.0;
		float Ying = 0.0;//Ying 각도
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			//Ying = (overR / DB) * (180 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180 / PI); 
		}
		for (float i = topangle; i <= 360.0 + topangle + Ying; i += (360.0 / Cut))
		{
			F =  (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));
			if(ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			else if(ONIN == 2)	X = F - ((hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}

		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 2) ////////////////////////////////////////////////////////////////// 지관사선절단
	{
		// float DB = _tstof(Db) / 2; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2;
		float DM = _tstof(Dm) / 2;
		float EE = _tstof(e);
		float alfa = _tstof(a);
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float X, F;
		float Y = 0;
		float ZZ = _tstof(Zgap);
		// float YL = (DB * 2 * 3.141592654) / Cut; 변수교정[< >2020.09.08 - 김대현]
		float YL = (Rb * 2 * 3.141592654) / Cut;
		float topangle;
		float DB = Rb; // 변수교정[< >2020.09.08 - 김대현]

		if (ONIN == 1) //Set-ON
		{
			DB = DB - RTb;
		}
		else if (ONIN == 2)	//Set-IN
		{
			DM = DM - RTm;

		/*
			[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현]
			Bev0_1 = _tstof(Bv1);
			Bev90_1 = -1 * _tstof(Bv2);
			Bev180_1 = -1 * _tstof(Bv3);
			Bev270_1 = -1 * _tstof(Bv4);
		*/
		}

		/////////////////////////////////////////////////
		////최대점 찾기
		/////////////////////////////////////////////////
		float top = -10000.0;
		for (float i = 0.0; i <= 360.0; i += (360.0 / Cut))
		{
			X = (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));//교접궤적
			if (X > top)
			{
				top = X;
				topangle = i;
			}
		}

		/////////////////////////////////////////////////
		//// 베벨링부분
		/////////////////////////////////////////////////
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= topangle; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		CString temp;

		
		F = top;
		if (ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		else if (ONIN == 2)	X = F - ((hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		Y = -1 * _tstof(A1);
		X = X + _tstof(A1);

		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180 / PI);
		}
		for (float i = topangle; i <= 360.0 + topangle + Ying; i += (360.0 / Cut))
		{
			F = (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));
			if (ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			else if (ONIN == 2)	X = F - ((hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}

		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 3) ///////////////////////////////////////////////////////// V 셰입
	{
		//float DB = _tstof(Db) / 2; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2;
		float DM = _tstof(Db) / 2;
		float EE = 0;
		float alfa = 90;
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float X, F;
		float Y = 0;
		float ZZ = _tstof(Zgap);
		//float YL = (DB * 2 * 3.141592654) / Cut; 변수교정[< >2020.09.08 - 김대현]
		float YL = (Rb * 2 * 3.141592654) / Cut;
		float topangle;
		float DB = Rb; // 변수교정[< >2020.09.08 - 김대현]

		if (ONIN == 1)
		{
			DB = DB - RTb;//Set-ON
		}
		else if (ONIN == 2)
		{
			DB = DB - RTb; //Set-in 
			DM = DM - RTb;
		}

		/////////////////////////////////////////////////
		////최대점 찾기
		/////////////////////////////////////////////////
		float top = -10000.0;
		for (float i = 0.0; i <= 360.0; i += (360.0 / Cut))
		{
			X = (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));
			if (X > top)
			{
				top = X;
				topangle = i;
			}
		}

		/////////////////////////////////////////////////
		//// 베벨링부분
		/////////////////////////////////////////////////
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= topangle; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		CString temp;

		Y = -1 * _tstof(A1);
		F = top;
		if (ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		else if (ONIN == 2)	X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		X = X + _tstof(A1);

		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			//Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		float i = 0.0;
		for (float i = topangle; i <= 360.0 + topangle + Ying; i += (360.0 / Cut))
		{
			F = (((-DB * cos(i * (PI / 180.0))) / tan(alfa * (PI / 180.0))) - ((1.0 / sin(alfa * (PI / 180.0))) * sqrt((DM*DM) - (EE - DB * sin(i * (PI / 180.0))) * (EE - DB * sin(i * (PI / 180.0))))));
			if (ONIN == 1)		X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			else if (ONIN == 2)	X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}

		Y = Y + _tstof(A2);
		X = X + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 4) /////////////////////////////////////////////////// 평판수직절단
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		// float YL = (DB * 2.0 * 3.141592654) / Cut; 변수교정[< >2020.09.08 - 김대현]
		float YL = (Rb * 2.0 * 3.141592654) / Cut;
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);
		
		// DB = DB - RTb; 변수교정[< >2020.09.08 - 김대현]
		
		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;
		float Ying = 0.0;
		float i = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		for (float k = 0.0; k <= 1; k++)
		{
			F = 0.0;
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2.0) * i * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);
			i += 360.0 + Ying;

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}
		i = 0.0;
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 5) /////////////////////////////////////////// 평판사선절단
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[<>2020.09.07 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float alfa = _tstof(a);
		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		//float YL = (Db * 2.0 * 3.141592654) / Cut; 변수교정[<>2020.09.07 - 김대현]
		float YL = (Rb * 2.0 * 3.141592654) / Cut;
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		float YY = 0.0;
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev180_1;
		float Ying = 0.0;
		float Blimit = _tstof(RBelimit);
		
		// DB = DB - RTb; 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON
	
		/////////////////////////////////////////////////
		//// 시작점 찾기
		/////////////////////////////////////////////////
		CString temp;

		if ((_tstof(Bv1) == -1 * _tstof(Bv3)) && _tstof(Bv2) == 0.0 && _tstof(Bv4) == 0.0)
		{
			//X = -DB * tan(alfa * (PI / 180.0)) * cos(180.0 * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
			X = -Rb * tan(alfa * (PI / 180.0)) * cos(180.0 * (PI / 180.0)) + Wc;
		}
		else
		{
			//F = - (DB - RTb) * tan(alfa * (PI / 180.0)) * cos(i * (PI / 180.0)); 오류수정[<>2020.08.05 - 김대현]
			F = -(DB * tan(alfa * (PI / 180.0)) * cos(180.0 * (PI / 180.0)));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		}

		X = X + _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;

		if (overcut == 2)
		{
			float overR = _tstof(OR);
			//Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		for (float i = 180.0; i <= 360.0 + 180.0 + Ying; i = i + (360.0 / Cut))
		{
			if ((_tstof(Bv1) == -1 * _tstof(Bv3)) && _tstof(Bv2) == 0.0 && _tstof(Bv4) == 0.0)
			{
				//X = -DB * tan(alfa * (PI / 180.0)) * cos((i + (360.0 / Cut)) * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
				X = -Rb * tan(alfa * (PI / 180.0)) * cos((i +(360.0/Cut)) * (PI / 180.0)) + Wc;
			}
			else
			{
				//[2020년 8월 05일 오류수정 - 김대현] 
				//F = - (DB - RTb) * tan(alfa * (PI / 180.0)) * cos(i * (PI / 180.0)); 오류수정[<>2020.08.05 - 김대현]
				F = - (DB  * tan(alfa * (PI / 180.0)) * cos(i * (PI / 180.0))); 
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			}
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if ((_tstof(Bv1) == -1 * _tstof(Bv3)) && _tstof(Bv2) == 0.0 && _tstof(Bv4) == 0.0 )
			{
				/* 	[2020년 8월 05일 한계검출 업데이트로 인한 삭제 - 김대현]
				float K = alfa;
				if (K > Blimit)
				{
					K = Blimit;
				}
				*/
				//[2020년 8월 05일 오류수정 - 김대현] 
				//Bevel = alfa * cos(i * (PI / 180.0));
				Bevel = alfa * cos((i+(360.0/Cut)) * (PI / 180.0));
			}
			else
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				else if (360 <= i && i < 450)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (450 <= i && i < 540)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (540 <= i && i < 630)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (630 <= i && i < 720)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);
				Bevel += change;
			}

			Number += 10;
			YY += (360.0 / Cut);
		}
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 6) ////////////////////////////////////////////// 더블마이트엔드컷
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[<>2020.09.07 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float DM = _tstof(Dm) / 2.0;
		float EE = _tstof(e);
		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float alfa1 = _tstof(aa1);
		float alfa2 = _tstof(aa2);
		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		//float YL = (Db * 2.0 * 3.141592654) / Cut; 변수교정[<>2020.09.07 - 김대현]
		float YL = (Rb * 2.0 * 3.141592654) / Cut;
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		// DB = DB - RTb; 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON

		/////////////////////////////////////////////////
		//// [2020년 8월 05일 추가 - 김대현] 시작점 찾기
		/////////////////////////////////////////////////
		CString temp;
		float phi = acos(EE / DB) * (180.0 / PI); //편심적용값
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= phi; i += (360.0 / Cut)) //첫 베벨링 찾기
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}

		float i = phi;

		if (Bev0_1 == alfa1 && Bev180_1 == alfa2) //첫 X좌표 찾기
		{
			//X = DB * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
			X = Rb * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc;
		}
		else
		{
			F = -((EE - (DB * cos(i * (PI / 180.0)))) * tan(alfa2 * (PI / 180.0)));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		}
		X = X + _tstof(A1);
		Y = -1 * _tstof(A1);
		
		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		/*@@ [↑2020.08.05 - 김대현]
		float phi = acos(EE / DB) * (180.0 / PI);
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= phi; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}
		*/
		float YY = 0.0;
		// float i = phi; [↑2020.08.05 - 김대현]

		//################################################################    90~270 SIDE    ###############################################
		for (i; i <= 270.0 + (90.0 - phi); i = i + (360.0 / Cut))
		{
			// Close junction trajectory [+2020.08.05 - 김대현]
			if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
			{
				//X = DB * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
				X = Rb * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc;
			}

			// Standard junction trajectory
			else
			{
				F = -((EE - (DB * cos(i * (PI / 180.0)))) * tan(alfa2 * (PI / 180.0)));
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			}
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);
		
			// Close Beveling [+2020.08.05 - 김대현]
			if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
			{
				Bevel = -alfa2 * cos((i + (360.0 / Cut)) * (PI / 180.0));
			}

			// Standard Beveling
			else
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);
				Bevel += change;
			}
				Number += 10;
				YY += (360.0 / Cut);
		}
		//###########################################################################################################################
		
		if (Bev0_1 == alfa1 && Bev180_1 == alfa2) // First Close Beveling [+2020.08.05 - 김대현]
		{
			Bevel = alfa1 * cos(i* (PI / 180.0));
		}
		//################################################################    270~450 SIDE    ###############################################
		for (i; i <= 360.0 + phi; i = i + (360.0 / Cut))
		{
			// Close junction trajectory [+2020.08.05 - 김대현]
			if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
			{
				//X = -DB * tan(alfa1 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
				X = -Rb * tan(alfa1 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc;
			}

			// Standard junction trajectory
			else
			{
				F = -(((DB * cos(i * (PI / 180.0))) - EE) * tan(alfa1 * (PI / 180.0)));
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			}
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			// Close Beveling [+2020.08.05 - 김대현]
			if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
			{
				Bevel = alfa1 * cos((i + (360.0/Cut)) * (PI / 180.0));
			}

			// Standard Beveling
			else
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				else if (360 <= i && i < 450)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (450 <= i && i < 540)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (540 <= i && i < 630)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (630 <= i && i < 720)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);
				Bevel += change;
			}
			Number += 10;
			YY += (360.0 / Cut);
		}
		//###########################################################################################################################

		//################################################################    OVER CUT    ###############################################
		// OverCut First Beveling [+2020.08.05 - 김대현]
		Bevel = Bev0_1;
		for (float i = 0.0; i <= phi; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}


		if (overcut == 2)
		{
			float overR = _tstof(OR);
			//float Ying = (overR / DB) * (180.0 / PI); 변수교정[<>2020.09.07 - 김대현]
			float Ying = (overR / Rb) * (180.0 / PI); 

			for (float i = phi; i <= phi + Ying; i += (360.0 / Cut))
			{
				/*[<>2020.08.05 - 김대현]
				F = - ((EE - (DB * cos(i * (PI / 180.0)))) * tan(alfa2 * (PI / 180.0)));
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
				Y = (_tstof(Db) / 2) * YY * (PI / 180.0);
				*/

				// Close junction trajectory [+2020.08.05 - 김대현]
				if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
				{
					//X = DB * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
					X = Rb * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 
				}

				// Standard junction trajectory
				else
				{
					F = -((EE - (DB * cos(i * (PI / 180.0)))) * tan(alfa2 * (PI / 180.0)));
					X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
				}
				Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
				m_LIST.AddString(temp);

				/*[<>2020.08.05 - 김대현]
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);

				Number += 10;
				YY += (360.0 / Cut);
				Bevel += change;
				*/

				// Close Beveling [+2020.08.05 - 김대현]
				if (Bev0_1 == alfa1 && Bev180_1 == alfa2)
				{
					Bevel = -alfa2 * cos((i + (360.0 / Cut)) * (PI / 180.0));
				}

				// Standard Beveling
				else
				{
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					change = (ToBev2 - ToBev1) / (Cut / 4.0);
					Bevel += change;
				}
				Number += 10;
				YY += (360.0 / Cut);
			}
		}
		//###########################################################################################################################
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);

		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 7) /////////////////////////////////////////////////////계판
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float Lc = _tstof(Length);
		float o = _tstof(SN);//갯수
		float w = _tstof(W1);//폭
		float h = _tstof(SL);//높이
		float X = _tstof(A1);
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		float PI = 3.141592654;

		// DB = DB - RTb;	 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON

		CString temp;

		X = _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f ;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		X = 0.0;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -X + Wc, Y + Wc, ZZ);
		m_LIST.AddString(temp);
		Number += 10;
		
		for (float i = 0.0; i <= 360.0 - (360.0 / o); i += (360.0 / o))
		{
			X = h + X;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -X + Wc, Y + Wc, ZZ);
			m_LIST.AddString(temp);
			Number += 10;
			
			Y = w + Y;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -X + Wc, Y - Wc, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			X = -h + X;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -X + Wc, Y - Wc, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			Y = (((PI * _tstof(Db)) / o) - w) + Y;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -X + Wc, Y + Wc, ZZ);
			m_LIST.AddString(temp);
			Number += 10;
		}
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X + Wc, Y + Wc, ZZ);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 8) ////////////////////////////////////////////////////크라운
	{
		// float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float h = _tstof(CL);
		float o = _tstof(CG);
		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		float Zero = 1.0;
		float Zeroangle = 0.0;
		float hrot = _tstof(B12);


		/////////////////////////////////////////////////
		//// 시작점 찾기
		/////////////////////////////////////////////////
		for (float i = 0.0; i <= 360.0; i = i + (360.0 / Cut))
		{
			X = (h / 2.0)*cos(i * o * (PI / 180.0)) + (h / 2.0);

			if (Zero >= X)
			{
				Zero = X;
				Zeroangle = i;
			}

		}
		
		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		float Bevel = 0.0;
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		for (float i = Zeroangle; i < 360.0 + Zeroangle + Ying; i = i + (360.0 / Cut))
		{
			F = - ((h / 2.0)*cos(i * o * (PI / 180.0)) + (h / 2.0));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			
			Number += 10;
			YY += (360.0 / Cut);
		}
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 9) ////////////////////////////////////////////////////////// 유니폼
	{
		float PI = 3.141592654;
		// float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float A, B;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		float X;
		float G = _tstof(guri);
		float o = _tstof(getsoo);
		float log = -1 * _tstof(Length);
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);
		float gan = log + G;

		// DB = DB - RTb;	 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON
		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;
		
		ZZ = hc;
		float YY = 0.0;
		X = 0.0;
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);

		}
		for (float Q = 1.0; Q <= o; Q++)
		{
			float Bevel = _tstof(Bv1);
			float Fx;
			Fx = X - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			if (Q != 1.0)
			{
				temp.Format(L"N%d  G01  X%.3f  A0.000  B0.000 ;", Number, Fx + _tstof(A1));
				m_LIST.AddString(temp);
				Number += 10;
			}
			for (int i = 0.0; i <= 1; i++)
			{
				Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, Fx, Y, ZZ, Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				YY += (360.0 + Ying);
			}
			YY = 0;
			float XLast1 = Fx + _tstof(A2);
			Y = Y + _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, XLast1, Y, ZZ, Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			Bevel = _tstof(Bv1_1);
			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;

			float XLast2 = X + log - _tstof(A1);
			temp.Format(L"N%d  G00  X%.3f ;", Number, XLast2);
			m_LIST.AddString(temp);
			Number += 10;

			/* 
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			Y = _tstof(A1);
			ZZ = zrtn;
			/* 
			[2020년 6월 11일 ZZ값 유지 - 김대현]
			 ZZ = _tstof(Zgap);
			*/

			temp.Format(L"N%d  G92  Y%.3f  Z%.3f;", Number, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;
			
			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%s ;", Number, Pdelay);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = hc;
			X = (X + log);
			Fx = X + (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc;
			for (int i = 0.0; i <= 1; i++)
			{
				Y = -(_tstof(Db) / 2) * YY * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, Fx, Y, ZZ, -Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				if(i == 0) YY += (360.0 + Ying);
			}

			float XLast3 = X - _tstof(A2);
			Y = Y - _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f  ;", Number, XLast3, Y, ZZ, -Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			if (Q != o)
			{
				Y = -1 * _tstof(A1);
				temp.Format(L"N%d  G92  Y%.3f  ;", Number, Y);
				m_LIST.AddString(temp);
				Number += 10;
			}

			X = X - G;
			YY = 0.0;
		}
		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = zrtn;
		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 10) //////////////////////////////////////////////////////// 엘보
	{
		float PI = 3.141592654;
		// float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float A, B;
		float X, F;
		float Y = 0;
		float ZZ = _tstof(Zgap);
		float Lc = _tstof(Length);
		float RE = _tstof(Re);
		float DE = _tstof(De);
		float Tb = _tstof(Dbt);
		float S = _tstof(s);
		float E = _tstof(e);
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);
		float To = RE + (DE / 2.0);
		float R = DE / 2.0;
		// float r = DB - Tb; 변수교정[< >2020.09.08 - 김대현]
		float r = Rb - Tb;

		//DB = DB - Tb;	변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - Tb;//Set-ON

		float ToBev1;
		float ToBev2;
		float change;

		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0;
		float Bevel = Bev0_1;

		float K = sqrt(powf((To + R) - (R - sqrt(powf(R, 2) - powf(r * sin(0 * (PI / 180.0)) - E, 2))), 2) - powf(To + S + (r * cos(0 * (PI / 180.0))), 2));
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		for (float i = 0.0; i <= 360.0 + Ying; i += (360.0 / Cut))
		{
			F = - (sqrt(powf((To + R) - (R - sqrt(powf(R, 2) - powf(r * sin(i * (PI / 180.0)) - E, 2))), 2) - powf(To + S + (r * cos(i * (PI / 180.0))), 2)) - K);
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * i * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}

			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 11) /////////////////////////////////////////////////////////////// 양쪽 평면절단
	{
		//float DB = _tstof(Db) / 2.0; 변수교정[<>2020.09.07 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float alfa1 = 90.0 - _tstof(aa1);
		float alfa2 = 90.0 - _tstof(aa2);
		float h1, h2;
		float Lc = _tstof(Length);
		float PI = 3.141592654;

		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);

		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		// DB = DB - RTb;	 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON

		/////////////////////////////////////////////////
		//// 최대점 찾기
		/////////////////////////////////////////////////
		//@@ h1 = tan(alfa1 * (PI / 180.0)) * 2.0 * DB; [<>2020.08.31 - 김대현]
		//@@ h2 = tan(alfa2 * (PI / 180.0)) * 2.0 * DB; [<>2020.08.31 - 김대현]
		h1 = tan(alfa1 * (PI / 180.0)) * DB;
		h2 = tan(alfa2 * (PI / 180.0)) * DB;
	
		//float Rich1 = ((h1 / 2.0)*cos(270.0 * (PI / 180.0)) + (h1 / 2.0)) - ((h1 / 2.0)*cos(180.0 * (PI / 180.0)) + (h1 / 2.0));
		//float Rich2 = ((h2 / 2.0)*cos(270.0 * (PI / 180.0)) + (h2 / 2.0)) - ((h2 / 2.0)*cos(180.0 * (PI / 180.0)) + (h2 / 2.0));
		//float totalLC = Rich1 + Rich2 + Lc;

		float ToBev1;
		float ToBev2;
		float change;

		CString temp;
		float Bevel = Bev0_1; //[2020년 8월 26일 추가 - 김대현]

		/////////////////////////////////////////////////
		//// [2020년 8월 26일 추가 - 김대현] 시작점 찾기
		/////////////////////////////////////////////////
		if (Bev0_1 == -alfa1 && Bev180_1 == alfa1) //첫 X좌표 찾기
		{
			//X = DB * tan(alfa1 * (PI / 180.0)) * cos(0 * (PI / 180.0)) + Wc;	변수교정[<>2020.09.07 - 김대현]
			X = Rb * tan(alfa1 * (PI / 180.0)) * cos(0 * (PI / 180.0)) + Wc; 
		}
		else
		{
			//[< >2020.08.31 - 김대현]
			//F = -((h1 / 2.0) * cos(0 * (PI / 180.0)) + (h1 / 2.0));
			//X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;

			F = h1 * cos(0 * (PI / 180.0));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
		}
		X = X + _tstof(A1);
		Y = Y * _tstof(A1);
		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		//@@ float Bevel = Bev180_1; [↑2020.08.05 - 김대현]

		// First Close Beveling [+2020.08.05 - 김대현]
		if (Bev0_1 == -alfa1 && Bev180_1 == alfa1)
		{
			Bevel = -alfa1 * cos(0 * (PI / 180.0));
		}

		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			//Ying = (overR / DB) * (180.0 / PI); 변수교정[<>2020.09.07 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		//################################################################    RIGHT SIDE    ###############################################
		//@@ for (float i = 180.0; i < 360.0 + 180.0 + Ying; i = i + (360.0 / Cut)) [<>2020.08.26 - 김대현] 
		for (float i = 0.0; i < 360.0 + Ying; i = i + (360.0 / Cut))
		{
			// Close junction trajectory [+2020.08.05 - 김대현]
			if (Bev0_1 == -alfa1 && Bev180_1 == alfa1)
			{
				//X = DB * tan(alfa1 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc; 변수교정[<>2020.09.07 - 김대현]
				X = Rb * tan(alfa1 * (PI / 180.0)) * cos(i * (PI / 180.0)) + Wc;
			}
			// Standard junction trajectory
			else
			{
				//F = (h1 / 2.0) * cos(i * (PI / 180.0)) + (h1 / 2.0); [<>2020.08.31 - 김대현]
				F = h1 * cos(i * (PI / 180.0));
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			}
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			
			m_LIST.AddString(temp);

			// Close Beveling [+2020.08.05 - 김대현] 
			if (Bev0_1 == -alfa1 && Bev180_1 == alfa1)
			{
				Bevel = -alfa1 * cos((i + (360.0/Cut)) * (PI / 180.0));
			}
			// Standard Beveling
			else
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				else if (360 <= i && i < 450)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (450 <= i && i < 540)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (540 <= i && i < 630)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (630 <= i && i < 720)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);
				Bevel += change;
			}
			Number += 10;
			YY += (360.0 / Cut);
		}
		//###########################################################################################################################
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		YY = 0.0;
		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = zrtn;
		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		Bev0_1 = _tstof(Bv1_1);
		Bev90_1 = _tstof(Bv2_2);
		Bev180_1 = _tstof(Bv3_3);
		Bev270_1 = _tstof(Bv4_4);

		Bevel = -1 * Bev0_1;

		//################################################# LEFT SIDE FOUND ENTRY LINE  #############################
		float Sline = _tstof(A1);
		if (Bev0_1 == -alfa2 && Bev180_1 == alfa2) // Close junction trajectory [+2020.08.05 - 김대현]
		{
			//X = -totalLC - (DB * tan(alfa2 * (PI / 180.0)) * cos(360 * (PI / 180.0))) - Wc; [<>2020.08.31 - 김대현]
			//X = -Lc - (DB * tan(alfa2 * (PI / 180.0)) * cos(360 * (PI / 180.0))) - Wc; 변수교정[<>2020.09.07 - 김대현]
			X = -Lc - (Rb * tan(alfa2 * (PI / 180.0)) * cos(360 * (PI / 180.0))) - Wc;
		}
		else // Standard junction trajectory
		{
			//F = -totalLC - ((h2 / 2.0) * cos(360 * (PI / 180.0)) + (h2 / 2.0)); [<>2020.08.31 - 김대현]
			F = -Lc - h2 * cos(360 * (PI / 180.0));
			X = F + (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc;
		}
		Y = Y - _tstof(OR);
		if (overcut == 2)
		{
			temp.Format(L"N%d  G00  X%.3f  Y%.3f ;", Number, X - Sline, Y);
			m_LIST.AddString(temp);
			Number += 10;
		}
		else
		{
			temp.Format(L"N%d  G00  X%.3f ;", Number, X - Sline);
			m_LIST.AddString(temp);
			Number += 10;
		}
		//#########################################################################################################

		/*  
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  M03 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		/* 
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		Y = _tstof(A1);
		temp.Format(L"N%d  G92  Y%.3f  Z%.3f ;", Number, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M01 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, Espeed);
		m_LIST.AddString(temp);
		Number += 10;

		
		ZZ = hc;

		//Bevel = -Bev180_1; [<>2020.08.26 - 김대현]
		Bevel = -Bev0_1;

		// First Close Beveling [+2020.08.05 - 김대현]
		if (Bev0_1 == -alfa2 && Bev180_1 == alfa2)
		{
			Bevel = alfa2 * cos(0 * (PI / 180.0));
		}
		

		//################################################################    LEFT SIDE    ###############################################
		//@@ for (float i = 360.0 + 180.0 + Ying; i > 180.0; i -= (360.0 / Cut)) [<>2020.08.26 - 김대현] 
		for (float i = 360.0-(360.0/Cut); i > 0.0-Ying; i -= (360.0 / Cut)) 
		{
			// Close junction trajectory [+2020.08.05 - 김대현]
			if (Bev0_1 == -alfa2 && Bev180_1 == alfa2)
			{
				//X = -totalLC -(DB * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0))) - Wc; [<>2020.08.31 - 김대현]
				//X = -Lc - (DB * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0))) - Wc; 변수교정[< >2020.09.07 - 김대현]
				X = -Lc - (Rb * tan(alfa2 * (PI / 180.0)) * cos(i * (PI / 180.0))) - Wc;
			}

			// Standard junction trajectory
			else
			{
				//F = -totalLC - ((h2 / 2.0) * cos(i * (PI / 180.0)) + (h2 / 2.0)); [<>2020.08.31 - 김대현]
				F = -Lc - h2 * cos(i * (PI / 180.0)); 
				X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc;
			}
			Y = -(_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			// Close Beveling [+2020.08.05 - 김대현] 
			if (Bev0_1 == -alfa2 && Bev180_1 == alfa2)
			{
				Bevel = alfa2 * cos((i - (360.0 / Cut)) * (PI / 180.0));
			}

			// Standard Beveling
			else
			{
				
				// -Deg[+2020.08.31 - 김대현]
				if (-180 <= i && i < -90)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				// -Deg[+2020.08.31 - 김대현]
				if (-90 <= i && i < 0)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				else if (360 <= i && i < 450)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (450 <= i && i < 540)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (540 <= i && i < 630)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (630 <= i && i < 720)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4.0);
				Bevel += change;
			}
			Number += 10;
			YY += (360.0 / Cut);
		}
		//###########################################################################################################################
		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = zrtn;
		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 12) ////////////////////////////////////////////////////// H타입 양쪽 절단
	{
		// float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float Rb = _tstof(Db) / 2.0;
		float Lc = _tstof(Length);
		float PI = 3.141592654;
		float X, F;
		float Y = 0.0;
		float ZZ = _tstof(Zgap);
		float Rm1 = _tstof(Dm) / 2.0;
		float Rm2 = _tstof(Dm2) / 2.0;
		float alfa1 = _tstof(aa1);
		float alfa2 = _tstof(aa2);
		float e1 = _tstof(e);
		float e2 = _tstof(s);

		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		// DB = DB - RTb;	 변수교정[< >2020.09.08 - 김대현]
		float DB = Rb - RTb; //Set-ON


		/////////////////////////////////////////////////
		////최대점 찾기
		/////////////////////////////////////////////////
		float top1 = -10000.0;
		float top2 = -10000.0;
		float topangle1 = 0.0;
		float topangle2 = 0.0;
		for (float i = 0.0; i <= 360.0; i += (360.0 / Cut))// 우측 최대점 찾기
		{
			X = (((-DB * cos(i * (PI / 180.0))) / tan(alfa1 * (PI / 180.0))) - ((1.0 / sin(alfa1 * (PI / 180.0))) * sqrt((Rm1*Rm1) - (e1 - DB * sin(i * (PI / 180.0))) * (e1 - DB * sin(i * (PI / 180.0))))));
			if (X > top1)
			{
				top1 = X;
				topangle1 = i;
			}
		}
		for (float i = 0.0; i <= 360.0; i += (360.0 / Cut))// 좌측 최대점 찾기
		{
			X = (((-DB * cos(i * (PI / 180.0))) / tan(alfa2 * (PI / 180.0))) - ((1.0 / sin(alfa2 * (PI / 180.0))) * sqrt((Rm2*Rm2) - (e2 - DB * sin(i * (PI / 180.0))) * (e2 - DB * sin(i * (PI / 180.0))))));
			if (X > top2)
			{
				top2 = X;
				topangle2 = i;
			}
		}

		/////////////////////////////////////////////////
		//// 베벨링부분
		/////////////////////////////////////////////////
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;

		for (float i = 0.0; i <= topangle1; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4);
			Bevel += change;
		}

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		
		CString temp;

		X = top1 - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc + _tstof(A1);
		Y = -1 * _tstof(A1);
		float repoint = X;

		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = 0;
		/*
		[2020년 6월 11일 Zgap 0로 변경 - 김대현]
		ZZ = _tstof(Zgap);
		*/

		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		float YY = 0.0;
		float Ying = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			// Ying = (overR / DB) * (180.0 / PI); 오버컷 오류 수정[< >2020.09.08 - 김대현]
			Ying = (overR / Rb) * (180.0 / PI);
		}
		for (float i = topangle1; i < 360.0 + topangle1 + Ying; i += (360.0 / Cut)) //////////////////////////// 우측부
		{
			F = (((-DB * cos(i * (PI / 180.0))) / tan(alfa1 * (PI / 180.0))) - ((1.0 / sin(alfa1 * (PI / 180.0))) * sqrt((Rm1*Rm1) - (e1 - DB * sin(i * (PI / 180.0))) * (e1 - DB * sin(i * (PI / 180.0))))));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) + Wc;
			Y = (_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}

			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}
		X = X + _tstof(A2);
		Y = Y + _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
		YY = 0.0;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;

		float i = topangle2;
		F = -Lc + (((DB * cos(i * (PI / 180.0))) / tan(alfa2 * (PI / 180.0))) + ((1.0 / sin(alfa2 * (PI / 180.0))) * sqrt((Rm2*Rm2) - (e2 - DB * sin(i * (PI / 180.0))) * (e2 - DB * sin(i * (PI / 180.0))))));
		X = F + (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc - _tstof(A1);
		Y = Y - _tstof(OR);
		if (overcut == 2)
		{
			temp.Format(L"N%d  G00  X%.3f  Y%.3f ;", Number, X, Y);
			m_LIST.AddString(temp);
			Number += 10;
		}
		else
		{
			temp.Format(L"N%d  G00  X%.3f ;", Number, X);
			m_LIST.AddString(temp);
			Number += 10;
		}

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		Y = _tstof(A1);
		ZZ = zrtn;
		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		 ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  Y%.3f  Z%.3f ;", Number, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M01 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, Espeed);
		m_LIST.AddString(temp);
		Number += 10;

		Bev0_1 = -1 * _tstof(Bv1_1);
		Bev90_1 = -1 * _tstof(Bv2_2);
		Bev180_1 = -1 * _tstof(Bv3_3);
		Bev270_1 = -1 * _tstof(Bv4_4);
		Bevel = Bev0_1;
		ZZ = hc;
		for (float i = 0.0; i <= topangle2; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4.0);
			Bevel += change;
		}

		for (float i = 360.0 + topangle2 + Ying; i > topangle2; i -= (360.0 / Cut)) //////////////////////좌측부
		{
			F = -Lc + (((DB * cos(i * (PI / 180.0))) / tan(alfa2 * (PI / 180.0))) + ((1.0 / sin(alfa2 * (PI / 180.0))) * sqrt((Rm2*Rm2) - (e2 - DB * sin(i * (PI / 180.0))) * (e2 - DB * sin(i * (PI / 180.0))))));
			X = F - (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc;
			Y = -(_tstof(Db) / 2) * YY * (PI / 180.0);

			temp.Format(L"N%05d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);

			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev1 - ToBev2) / (Cut / 4.0);
			Number += 10;
			YY += (360.0 / Cut);
			Bevel += change;
		}
		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		temp.Format(L"N%05d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = zrtn;
		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	else if (Me == 101)////////////////////////////////////////////////////////모관 수직/////////////////////////////////////////////////////////////////////////////////
	{
		float F, X, Y, ZZ;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float PI = 3.141592654;
		float EE = _tstof(e);
		float DB = _tstof(Db) / 2.0;
		float DM = _tstof(Dm) / 2.0;
		float t = 90.0;
		/*
		[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현,최준성,공병주]
		float Bev0_1 = -_tstof(Bv1);
		float Bev90_1 = -_tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		*/
		// [2020년 7월 14일 베벨링 입력값 추가 - 김대현]
		float Bev0_1 = -_tstof(Bv1);    //(-)
		float Bev90_1 = _tstof(Bv2);    //(+)
		float Bev180_1 = _tstof(Bv3);   //(+)
		float Bev270_1 = -_tstof(Bv4);  //(-)
		float hrot = _tstof(B12);
		float Ying = 0.0;

		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = 0;
			/*
			[2020년 6월 11일 Zgap 0로 변경 - 김대현]
			ZZ = _tstof(Zgap);
			*/
			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Rspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_1);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = 0.0;
				X = F - (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) + Wc;
				Y = (_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			X = X + _tstof(A2);
			Y = Y + _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;
		}
		if (ONIN == 1) // Set-ON
		{
			DB = DB - RTb - Wc;
		}
		else if (ONIN == 2) // Set-IN
		{
			DB = DB - Wc;
			DM = DM - RTm;
		}
		if (EE > DB)
		{
			float Bevel = Bev0_1;
			F = (DB * cos(0 * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(0 * (PI / 180.0)))*(EE - DB * sin(0 * (PI / 180.0))))));
			if (ONIN == 1) X = F;
			else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

			float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(0 * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(0 * (PI / 180.0)))))) * (180.0 / PI);
			Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
		}
		else
		{
			float elimit = asin(EE / DB) * (180.0 / PI);
			float ToBev1;
			float ToBev2;
			float change;
			float Bevel = Bev0_1;
			for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev2 - ToBev1) / (Cut / 4);
				Bevel += change;
			}
			F = (DB * cos(elimit * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(elimit * (PI / 180.0)))*(EE - DB * sin(elimit * (PI / 180.0))))));
			if (ONIN == 1) X = F;
			else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

			float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))))) * (180.0 / PI);
			Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
		}
		X = X - _tstof(A1);
		Y = Y + _tstof(A1);
		if (hole_print == 1)
		{
			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  X%.3f ;", Number, -LcR + X);
			m_LIST.AddString(temp);
			Number += 10;
		}
		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = zrtn;
		//[홀부만 출력 시 ZZ값 유지]
		if (hole_print == 0) 
		{
			ZZ = 0;
		}
		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;
		}

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;

		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;
		Ying = 0.0;
		if (EE > DB)
		{
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			for (float i = 360.0 + Ying; i <= 0.0; i -= (360.0 / Cut))
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = - acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
				m_LIST.AddString(temp);

				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}

				change = (ToBev1 - ToBev2) / (Cut / 4.0);
				Number += 10;
				Bevel += change;
			}
		}
		else
		{
			float elimit = asin(EE / DB) * (180.0 / PI);
			for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
			{
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev1 - ToBev2) / (Cut / 4);
				Bevel += change;
			}
			for (float i = 360.0 + elimit; i >= 180.0 - elimit; i -= (360.0 / Cut))
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
				else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
				m_LIST.AddString(temp);

				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}

				change = (ToBev1 - ToBev2) / (Cut / 4.0);
				Number += 10;
				Bevel += change;
			}
			for (float i = 180.0 - elimit; i >= elimit; i -= (360.0 / Cut))
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
				else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
				m_LIST.AddString(temp);

				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}

				change = (ToBev1 - ToBev2) / (Cut / 4.0);
				Number += 10;
				Bevel += change;
			}
			
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				float Ying = (overR / DB) * (180.0 / PI);
				
				for (float i = 360.0 + elimit; i >= 360.0 - elimit - Ying; i -= (360.0 / Cut))
				{
					F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
					if (ONIN == 1) X = F;
					else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

					float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
					Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

					if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
					else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
					m_LIST.AddString(temp);

					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}

					change = (ToBev1 - ToBev2) / (Cut / 4.0);
					Number += 10;
					Bevel += change;
				}
			}
		}
		
		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
		else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;
		
		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			X = -LcL - _tstof(A1);
			temp.Format(L"N%d  G00  X%.3f ;", Number, X);
			m_LIST.AddString(temp);
			Number += 10;

			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = zrtn;
			/*
			[2020년 6월 11일 ZZ값 유지 - 김대현]
			 ZZ = _tstof(Zgap);
			*/
			Y = _tstof(A1);
			temp.Format(L"N%d  G92  Y%.3f  Z%.3f ;", Number, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Lspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_11);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = -LcL;
				X = F + (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) - Wc;
				Y = -(_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			X = X - _tstof(A2);
			Y = Y - _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;
		}
		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}

	else if (Me == 102)//////////////////////////////////////////////////////////////////////모관 사선/////////////////////
	{
		float F, X, Y, ZZ;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float PI = 3.141592654;
		float EE = _tstof(e);
		float DB = _tstof(Db) / 2;
		float DM = _tstof(Dm) / 2;
		float t = _tstof(a);
	
		/*
		[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현,최준성,공병주]
		float Bev0_1 = -_tstof(Bv1);
		float Bev90_1 = -_tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		*/
		// [2020년 7월 14일 베벨링 입력값 추가 - 김대현]
		float Bev0_1 = -_tstof(Bv1);   //(-)
		float Bev90_1 = _tstof(Bv2);   //(+)
		float Bev180_1 = _tstof(Bv3);  //(+)
		float Bev270_1 = -_tstof(Bv4); //(-)
		float ABevel = 0;
		float BBevel = Bev0_1;
		float Bevel;
		float hrot = _tstof(B12);
		float Ying = 0.0;

		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = 0;
			/*
			[2020년 6월 11일 Zgap 0로 변경 - 김대현]
			ZZ = _tstof(Zgap);
			*/
			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Rspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_1);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = 0.0;
				X = F - (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) + Wc;
				Y = (_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			X = X + _tstof(A2);
			Y = Y + _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;
		}

		if (ONIN == 1) // Set-ON
		{
			DB = DB - RTb - Wc;
			/*
			[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현, 최준성, 공병주]
			Bev180_1 = -1* _tstof(Bv3);
			*/

			if (EE > DB)
			{
				X = (DB * cos(0 * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(0 * (PI / 180.0)))*(EE - DB * sin(0 * (PI / 180.0))))));
			}
			else
			{
				float elimit = asin(EE / DB) * (180.0 / PI);
				float ABev1;
				float ABev2;
				float BBev1;
				float BBev2;
				float changeA;
				float changeB;
				float Bev_A = 0;
				float Bev_B = Bev0_1;
				for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
				{
					if (0 <= i && i < 90)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (90 <= i && i < 180)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (270 <= i && i < 360)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					changeA = (ABev2 - ABev1) / (Cut / 4);
					changeB = (BBev2 - BBev1) / (Cut / 4);
					ABevel += changeA;
					BBevel += changeB;
				}
				X = (DB * cos(elimit * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(elimit * (PI / 180.0)))*(EE - DB * sin(elimit * (PI / 180.0))))));
				float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
			}
		}
		else if (ONIN == 2) // Set-IN
		{
			DB = DB - Wc;
			DM = DM - RTm;
			
			if (EE > DB)
			{
				float Bevel = Bev0_1;
				F = (DB * cos(0 * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(0 * (PI / 180.0)))*(EE - DB * sin(0 * (PI / 180.0))))));
				X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));
				float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(0 * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(0 * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
			}
			else
			{
				float elimit = asin(EE / DB) * (180.0 / PI);
				float ToBev1;
				float ToBev2;
				float change;
				Bevel = Bev0_1;
				for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
				{
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					change = (ToBev2 - ToBev1) / (Cut / 4);
					Bevel += change;
				}
				X = (DB * cos(elimit * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(elimit * (PI / 180.0)))*(EE - DB * sin(elimit * (PI / 180.0))))));
				float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
			}
		}
		X = X - _tstof(A1);
		Y = Y + _tstof(A1);
		if (hole_print == 1)
		{
			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  X%.3f ;", Number, -LcR + X);
			m_LIST.AddString(temp);
			Number += 10;
		}
		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		
		ZZ = zrtn;
		//[2020년 6월 26일 홀부만 출력 시 ZZ값 유지 - 김대현]
		if (hole_print == 0)
		{
			ZZ = 0;
		}
		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		 ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;
		}

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;

		float ToBev1;
		float ToBev2;
		float ABev1;
		float ABev2;
		float BBev1;
		float BBev2;
		float changeA;
		float changeB;
		float change;
		Bevel = Bev0_1;
		Ying = 0.0;
		if (EE > DB)
		{
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			for (float i = 360.0 + Ying; i >= 0.0; i -= (360.0 / Cut))
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				if (ONIN == 1)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B%.3f ;", Number, X, Y, ZZ, ABevel, BBevel);

					if (0 <= i && i < 90)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (90 <= i && i < 180)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (270 <= i && i < 360)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (450 <= i && i < 540)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (630 <= i && i < 720)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					changeA = (ABev1 - ABev2) / (Cut / 4);
					changeB = (BBev1 - BBev2) / (Cut / 4);
					ABevel += changeA;
					BBevel += changeB;
				}
				else if (ONIN == 2)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (450 <= i && i < 540)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (630 <= i && i < 720)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}

					change = (ToBev1 - ToBev2) / (Cut / 4.0);
					Bevel += change;
				}
				m_LIST.AddString(temp);
				Number += 10;
			}
		}
		else
		{
			float elimit = asin(EE / DB) * (180.0 / PI);
			for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
			{
				if (ONIN == 1)
				{
					if (0 <= i && i < 90)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (90 <= i && i < 180)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (270 <= i && i < 360)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (450 <= i && i < 540)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (630 <= i && i < 720)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					changeA = (ABev2 - ABev1) / (Cut / 4);
					changeB = (BBev2 - BBev1) / (Cut / 4);
					ABevel += changeA;
					BBevel += changeB;
				}
				else if (ONIN == 2)
				{
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (450 <= i && i < 540)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (630 <= i && i < 720)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					change = (ToBev2 - ToBev1) / (Cut / 4);
					Bevel += change;
				}
			}
			for (float i = 360.0 + elimit; i >= 180.0 - elimit; i -= (360.0 / Cut))//////////////////////2
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				if (ONIN == 1)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B%.3f ;", Number, X, Y, ZZ, ABevel, BBevel);
					if (0 <= i && i < 90)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (90 <= i && i < 180)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (270 <= i && i < 360)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (450 <= i && i < 540)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (630 <= i && i < 720)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (720 <= i && i < 810)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					changeA = (ABev1 - ABev2) / (Cut / 4);
					changeB = (BBev1 - BBev2) / (Cut / 4);
					ABevel += changeA;
					BBevel += changeB;
				}
				else if (ONIN == 2)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (450 <= i && i < 540)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (630 <= i && i < 720)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					else if (720 <= i && i < 810)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					change = (ToBev1 - ToBev2) / (Cut / 4.0);
					Bevel += change;
				}
				m_LIST.AddString(temp);
				Number += 10;
			}
			for (float i = 180.0 - elimit; i >= elimit; i -= (360.0 / Cut))///////////////1
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

				float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

				if (ONIN == 1)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B%.3f ;", Number, X, Y, ZZ, ABevel, BBevel);
					if (0 <= i && i < 90)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (90 <= i && i < 180)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (270 <= i && i < 360)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					else if (450 <= i && i < 540)
					{
						ABev1 = Bev90_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ABev1 = 0;
						ABev2 = Bev270_1;

						BBev1 = Bev180_1;
						BBev2 = 0;
					}
					else if (630 <= i && i < 720)
					{
						ABev1 = Bev270_1;
						ABev2 = 0;

						BBev1 = 0;
						BBev2 = Bev0_1;
					}
					else if (720 <= i && i < 810)
					{
						ABev1 = 0;
						ABev2 = Bev90_1;

						BBev1 = Bev0_1;
						BBev2 = 0;
					}
					changeA = (ABev1 - ABev2) / (Cut / 4);
					changeB = (BBev1 - BBev2) / (Cut / 4);
					ABevel += changeA;
					BBevel += changeB;
				}
				else if (ONIN == 2)
				{
					temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
					if (0 <= i && i < 90)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (90 <= i && i < 180)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (180 <= i && i < 270)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (270 <= i && i < 360)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					else if (360 <= i && i < 450)
					{
						ToBev1 = Bev0_1;
						ToBev2 = Bev90_1;
					}
					else if (450 <= i && i < 540)
					{
						ToBev1 = Bev90_1;
						ToBev2 = Bev180_1;
					}
					else if (540 <= i && i < 630)
					{
						ToBev1 = Bev180_1;
						ToBev2 = Bev270_1;
					}
					else if (630 <= i && i < 720)
					{
						ToBev1 = Bev270_1;
						ToBev2 = Bev0_1;
					}
					change = (ToBev1 - ToBev2) / (Cut / 4.0);
					Bevel += change;
				}
				m_LIST.AddString(temp);
				Number += 10;
			}
			
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				float Ying = (overR / DB) * (180.0 / PI);
				//float i = 360.0 + elimit; i >= 180.0 - elimit; i -= (360.0 / Cut))//////////////////////2
				for (float i = 360.0 + elimit; i >= 360.0 + elimit - Ying; i -= (360.0 / Cut))
				{
					F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
					if (ONIN == 1) X = F;
					else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

					float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
					Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);

					if (ONIN == 1)
					{
						temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B%.3f ;", Number, X, Y, ZZ, ABevel, BBevel);
						if (0 <= i && i < 90)
						{
							ABev1 = 0;
							ABev2 = Bev90_1;

							BBev1 = Bev0_1;
							BBev2 = 0;
						}
						else if (90 <= i && i < 180)
						{
							ABev1 = Bev90_1;
							ABev2 = 0;

							BBev1 = 0;
							BBev2 = Bev180_1;
						}
						else if (180 <= i && i < 270)
						{
							ABev1 = 0;
							ABev2 = Bev270_1;

							BBev1 = Bev180_1;
							BBev2 = 0;
						}
						else if (270 <= i && i < 360)
						{
							ABev1 = Bev270_1;
							ABev2 = 0;

							BBev1 = 0;
							BBev2 = Bev0_1;
						}
						else if (360 <= i && i < 450)
						{
							ABev1 = 0;
							ABev2 = Bev90_1;

							BBev1 = Bev0_1;
							BBev2 = 0;
						}
						else if (450 <= i && i < 540)
						{
							ABev1 = Bev90_1;
							ABev2 = 0;

							BBev1 = 0;
							BBev2 = Bev180_1;
						}
						else if (540 <= i && i < 630)
						{
							ABev1 = 0;
							ABev2 = Bev270_1;

							BBev1 = Bev180_1;
							BBev2 = 0;
						}
						else if (630 <= i && i < 720)
						{
							ABev1 = Bev270_1;
							ABev2 = 0;

							BBev1 = 0;
							BBev2 = Bev0_1;
						}
						changeA = (ABev1 - ABev2) / (Cut / 4);
						changeB = (BBev1 - BBev2) / (Cut / 4);
						ABevel += changeA;
						BBevel += changeB;
					}
					else if (ONIN == 2)
					{
						temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
						if (0 <= i && i < 90)
						{
							ToBev1 = Bev0_1;
							ToBev2 = Bev90_1;
						}
						else if (90 <= i && i < 180)
						{
							ToBev1 = Bev90_1;
							ToBev2 = Bev180_1;
						}
						else if (180 <= i && i < 270)
						{
							ToBev1 = Bev180_1;
							ToBev2 = Bev270_1;
						}
						else if (270 <= i && i < 360)
						{
							ToBev1 = Bev270_1;
							ToBev2 = Bev0_1;
						}
						else if (360 <= i && i < 450)
						{
							ToBev1 = Bev0_1;
							ToBev2 = Bev90_1;
						}
						else if (450 <= i && i < 540)
						{
							ToBev1 = Bev90_1;
							ToBev2 = Bev180_1;
						}
						else if (540 <= i && i < 630)
						{
							ToBev1 = Bev180_1;
							ToBev2 = Bev270_1;
						}
						else if (630 <= i && i < 720)
						{
							ToBev1 = Bev270_1;
							ToBev2 = Bev0_1;
						}
						change = (ToBev1 - ToBev2) / (Cut / 4.0);
						Bevel += change;
					}
					m_LIST.AddString(temp);
					Number += 10;
				}
			}
		}

		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B%.3f ;", Number, X, Y, ZZ, ABevel, BBevel);
		else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			X = -LcL - _tstof(A1);
			temp.Format(L"N%d  G00  X%.3f ;", Number, X);
			m_LIST.AddString(temp);
			Number += 10;

			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = zrtn;
			/*
			[2020년 6월 11일 ZZ값 유지 - 김대현]
			 ZZ = _tstof(Zgap);
			*/
			Y = _tstof(A1);
			temp.Format(L"N%d  G92  Y%.3f  Z%.3f ;", Number, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Lspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_11);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = -LcL;
				X = F + (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) - Wc;
				Y = -(_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			X = X - _tstof(A2);
			Y = Y - _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;
		}
		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}

	else if (Me == 103)//////////////////////////////////////////////모관 V셰입///////////////////////////////////////////////////////
	{
		float F, X, Y, ZZ;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float PI = 3.141592654;
		float EE = 0;
		float DB = _tstof(Dm) / 2;
		float DM = _tstof(Dm) / 2;
		float t = 90;
		/*
		[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현,최준성,공병주]
		float Bev0_1 = -_tstof(Bv1);
		float Bev90_1 = -_tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		*/
		// [2020년 7월 14일 베벨링 입력값 추가 - 김대현]
		float Bev0_1 = -_tstof(Bv1);   // (-)
		float Bev90_1 = _tstof(Bv2);   // (+)
		float Bev180_1 = _tstof(Bv3);  // (+)
		float Bev270_1 = -_tstof(Bv4); // (-)
		float hrot = _tstof(B12);
		float Ying = 0.0;

		CString temp;

		X = _tstof(A1);
		Y = -1 * _tstof(A1);

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = 0;
			/*
			[2020년 6월 11일 Zgap 0로 변경 - 김대현]
			ZZ = _tstof(Zgap);
			*/

			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Rspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_1);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k ++)
			{
				F = 0.0;
				X = F - (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) + Wc;
				Y = (_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;

			X = X + _tstof(A2);
			Y = Y + _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;
		}

		if (ONIN == 1) // Set-ON
		{
			DB = DB - RTb - Wc;
		}
		else if (ONIN == 2) // Set-IN
		{
			DB = DB - RTb - Wc;
			DM = DM - RTm;
		}
		
		float elimit = asin(EE / DB) * (180.0 / PI);
		float ToBev1;
		float ToBev2;
		float change;
		float Bevel = Bev0_1;
		for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4);
			Bevel += change;
		}
		F = (DB * cos(elimit * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(elimit * (PI / 180.0)))*(EE - DB * sin(elimit * (PI / 180.0))))));
		if (ONIN == 1) X = F;
		else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));

		float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(elimit * (PI / 180.0)))))) * (180.0 / PI);
		Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
		

		X = X - _tstof(A1);
		Y = Y + _tstof(A1);
		if (hole_print == 1)
		{
			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  X%.3f ;", Number, -LcR + X);
			m_LIST.AddString(temp);
			Number += 10;
		}
		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = zrtn;
		//[2020년 6월 26일 홀부만 출력 시 ZZ값 유지 - 김대현]
		if (hole_print == 0)
		{
			ZZ = 0;
		}
		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		 ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f ;", Number, X, Y,ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;
		
		if (hole_print == 1)
		{
			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;
		}

		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = hc;
		Bevel = Bev0_1;
		Ying = 0.0;

		elimit = asin(EE / DB) * (180.0 / PI);
		for (float i = 0.0; i <= elimit; i += (360.0 / Cut))
		{
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev2 - ToBev1) / (Cut / 4);
			Bevel += change;
		}
		for (float i = 360.0 + elimit; i>= 180.0 - elimit; i -= (360.0 / Cut))/////////////////////2
		{
			F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
			if (ONIN == 1) X = F;
			else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));
			float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
			Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
			if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
			else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev1 - ToBev2) / (Cut / 4.0);
			Number += 10;
			Bevel += change;
		}
		for (float i = 180.0 - elimit; i >= elimit; i -= (360.0 / Cut))///////////////////////////1
		{
			F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
			if (ONIN == 1) X = F;
			else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));
			float Q2 = acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
			Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
			if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
			else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
			m_LIST.AddString(temp);
			if (0 <= i && i < 90)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (90 <= i && i < 180)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (180 <= i && i < 270)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (270 <= i && i < 360)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			else if (360 <= i && i < 450)
			{
				ToBev1 = Bev0_1;
				ToBev2 = Bev90_1;
			}
			else if (450 <= i && i < 540)
			{
				ToBev1 = Bev90_1;
				ToBev2 = Bev180_1;
			}
			else if (540 <= i && i < 630)
			{
				ToBev1 = Bev180_1;
				ToBev2 = Bev270_1;
			}
			else if (630 <= i && i < 720)
			{
				ToBev1 = Bev270_1;
				ToBev2 = Bev0_1;
			}
			change = (ToBev1 - ToBev2) / (Cut / 4.0);
			Number += 10;
			Bevel += change;
		}
		
		if (overcut == 2)
		{
			float overR = _tstof(OR);
			Ying = (overR / DB) * (180.0 / PI);
			for (float i = 360.0 + elimit; i >= 360.0 + elimit - Ying; i -= (360.0 / Cut))
			{
				F = (DB * cos(i * (PI / 180.0)) / sin(t * (PI / 180.0))) + (1.0 / tan(t * (PI / 180.0))) * (sqrt((DM * DM) - ((EE - DB * sin(i * (PI / 180.0)))*(EE - DB * sin(i * (PI / 180.0))))));
				if (ONIN == 1) X = F;
				else if (ONIN == 2) X = F - (RTm - (hrot - hc)) * tan(Bevel * (PI / 180.0));
				float Q2 = -acos((sqrt(1.0 - ((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))*((EE / DM) - (DB / DM) * sin(i * (PI / 180.0)))))) * (180.0 / PI);
				Y = (_tstof(Dm) / 2) * Q2 * (PI / 180.0);
				if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
				else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
				m_LIST.AddString(temp);
				if (0 <= i && i < 90)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (90 <= i && i < 180)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (180 <= i && i < 270)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (270 <= i && i < 360)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				else if (360 <= i && i < 450)
				{
					ToBev1 = Bev0_1;
					ToBev2 = Bev90_1;
				}
				else if (450 <= i && i < 540)
				{
					ToBev1 = Bev90_1;
					ToBev2 = Bev180_1;
				}
				else if (540 <= i && i < 630)
				{
					ToBev1 = Bev180_1;
					ToBev2 = Bev270_1;
				}
				else if (630 <= i && i < 720)
				{
					ToBev1 = Bev270_1;
					ToBev2 = Bev0_1;
				}
				change = (ToBev1 - ToBev2) / (Cut / 4.0);
				Number += 10;
				Bevel += change;
			}
		}

		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		if (ONIN == 1) temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bevel);
		else if (ONIN == 2)	temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			X = -LcL - _tstof(A1);
			temp.Format(L"N%d  G00  X%.3f ;", Number, X);
			m_LIST.AddString(temp);
			Number += 10;

			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = zrtn;
			/*
			[2020년 6월 11일 ZZ값 유지 - 김대현]
			 ZZ = _tstof(Zgap);
			*/
			temp.Format(L"N%d  G92  Y0.000  Z%.3f ;", Number, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			Y = _tstof(A1);
			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  M01 ;", Number);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Lspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			float S_Bevel = _tstof(Bv1_11);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = -LcL;
				X = F + (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) - Wc;
				Y = -(_tstof(Dm) / 2.0) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			X = X - _tstof(A2);
			Y = Y - _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;
		}

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}
	else if (Me == 104)///////////////////////////////////////////////////////////모관 사각사각////////////////////////////////////////////////////////////////////////
	{
		float DM = _tstof(Dm) / 2.0;
		//float DB = _tstof(Db) / 2.0; 변수교정[< >2020.09.08 - 김대현]
		float DB = _tstof(Dm) / 2.0;
		float LcR = _tstof(Lc2);
		float LcL = _tstof(Lc3);
		float PI = 3.141592654;
	
		float Lline = _tstof(L);
		float Wline = _tstof(W);
		float Plimit = (180.0 * Wline) / (PI * DM);
	
		float F, X, Y, ZZ;
		/*
		[2020년 7월 4일 역베벨링 음수값입력으로 변경 - 김대현,최준성,공병주]
		float Bev0_1 = -_tstof(Bv1);
		float Bev90_1 = -_tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		*/
		// [2020년 7월 14일 베벨링 입력값 추가 - 김대현]
		float Bev0_1 = -_tstof(Bv1);   // (-)
		float Bev90_1 = _tstof(Bv2);   // (+)
		float Bev180_1 = _tstof(Bv3);  // (+)
		float Bev270_1 = -_tstof(Bv4); // (-)
		float hrot = _tstof(B12);
		float S_Bevel = _tstof(Bv1_1);
		float Ying = 0.0;
	
		CString temp;
	
		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		X = _tstof(A1);
		Y = -1 * _tstof(A1);
		temp.Format(L"N%d  G90 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = 0;
			/*
			[2020년 6월 11일 Zgap 0로 변경 - 김대현]
			ZZ = _tstof(Zgap);
			*/
			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Rspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = 0.0;
				X = F - (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) + Wc;
				Y = (_tstof(Dm)) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
		}

		if (ONIN == 1) // Set-ON
		{
			DB = DB - RTb - Wc;
		}
		
		if (hole_print == 1)
		{
			X = X + _tstof(A2);
			Y = Y + _tstof(A2);
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  X%.3f ;", Number, -LcR);
			m_LIST.AddString(temp);
			Number += 10;
		}

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		ZZ = zrtn;
		//[2020년 6월 26일 홀부만 출력 시 ZZ값 유지 - 김대현]
		if (hole_print == 0)
		{
			ZZ = 0;
		}
		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		 ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X0.000  Y0.000  Z%.3f ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;
	
		ZZ = ph;
		temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;
	
		float Sline = _tstof(A1);
		X = (Lline / 2.0) - Sline;
		Y = 0.0 + Sline;
	
		temp.Format(L"N%d  G00  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X, Y, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		if (hole_print == 1)
		{
			temp.Format(L"N%d  M01;", Number);
			m_LIST.AddString(temp);
			Number += 10;
		}
	
		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;
	
		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, speed);
		m_LIST.AddString(temp);
		Number += 10;
		ZZ = hc;

		////////////////////////사각절단//////////////////////////////
		X = (Lline / 2.0) - Wc; Y = 0.0;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev0_1);
		m_LIST.AddString(temp);
		Number += 10;	

		X = (Lline / 2.0) - Wc; Y = (-Wline / 2.0) + Wc;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev0_1);
		m_LIST.AddString(temp);
		Number += 10;
	
		X = (-Lline / 2.0) + Wc; Y = (-Wline / 2.0) + Wc;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev270_1);
		m_LIST.AddString(temp);
		Number += 10;

		X = (-Lline / 2.0) + Wc; Y = (Wline / 2.0) - Wc;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev180_1);
		m_LIST.AddString(temp);
		Number += 10;

		X = (Lline / 2.0) - Wc; Y = (Wline / 2.0) - Wc;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev90_1);
		m_LIST.AddString(temp);
		Number += 10;

		X = (Lline / 2.0) - Wc; Y = 0.0;
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev0_1);
		m_LIST.AddString(temp);
		Number += 10;
	

		if (overcut == 2)
		{
			float overR = _tstof(OR);
			X = (Lline / 2) - Wc; Y = overR;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X, Y, ZZ, Bev0_1);
			m_LIST.AddString(temp);
			Number += 10;
		}
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////

		float Mline = _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A%.3f  B0.000 ;", Number, X - Mline, Y - Mline, ZZ, Bev0_1);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;
	
		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;
		if (hole_print == 1)
		{
			X = -LcL;
			temp.Format(L"N%d  G00  X%.3f ;", Number, X);
			m_LIST.AddString(temp);
			Number += 10;

			/*
			[2020년 6월 11일 M03코드 제거 - 김대현]
			temp.Format(L"N%d  %s ;", Number, Zturn);
			m_LIST.AddString(temp);
			Number += 10;
			*/

			ZZ = zrtn;
			/*
			[2020년 6월 11일 ZZ값 유지 - 김대현]
			 ZZ = _tstof(Zgap);
			*/
			temp.Format(L"N%d  G92  Z%.3f ;", Number, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			ZZ = ph;
			temp.Format(L"N%d  G01  Z%.3f  F%.3f;", Number, ZZ, vapp);
			m_LIST.AddString(temp);
			Number += 10;

			Sline = _tstof(A1);
			Y = 0;
			temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, X - Sline, Y + Sline, ZZ);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  M01;", Number);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcON);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  F%.3f ;", Number, Lspeed);
			m_LIST.AddString(temp);
			Number += 10;
			ZZ = hc;

			S_Bevel = _tstof(Bv1_11);
			if (overcut == 2)
			{
				float overR = _tstof(OR);
				Ying = (overR / DB) * (180.0 / PI);
			}
			float i = 0.0;
			for (float k = 0.0; k <= 1; k++)
			{
				F = -LcL;
				X = F + (RTm - (hrot - hc)) * tan(S_Bevel * (PI / 180.0)) - Wc;
				Y = -(_tstof(Dm)) * i * (PI / 180.0);

				temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -S_Bevel);
				m_LIST.AddString(temp);
				Number += 10;
				i += 360.0 + Ying;
			}
			i = 0.0;
			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X - Mline, Y - Mline, ZZ, -S_Bevel);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  %s ;", Number, ArcOFF);
			m_LIST.AddString(temp);
			Number += 10;

			temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
			m_LIST.AddString(temp);
			Number += 10;
		}

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}

	if (Me != 11 && Me != 12 && Me != 9 && Me < 100)/////////////////////////////////////////지관 마무리 컷/////////////////////////////////////////
	{
		float DM = _tstof(Dm) / 2;
		float DB = _tstof(Db) / 2;
		float PI = 3.141592654;
		float Lc = _tstof(Length);
		float X, F;
		float Y = 0;
		float ZZ = _tstof(PH);
		float YL = (DB * 2 * 3.141592654) / Cut;
		float Bev0_1 = _tstof(Bv1);
		float Bev90_1 = _tstof(Bv2);
		float Bev180_1 = _tstof(Bv3);
		float Bev270_1 = _tstof(Bv4);
		float hrot = _tstof(B12);

		CString temp;

		/////////////////////////////////////////////////
		//// NC 출력부분
		/////////////////////////////////////////////////
		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = zrtn;
		temp.Format(L"N%d  G00  Z%.3f A0.000  B0.000 ;", Number, ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  X%.3f ;", Number, -Lc - _tstof(A1));
		m_LIST.AddString(temp);
		Number += 10;

		/*
		[2020년 6월 11일 M03코드 제거 - 김대현]
		temp.Format(L"N%d  %s ;", Number, Zturn);
		m_LIST.AddString(temp);
		Number += 10;
		*/

		/*
		[2020년 6월 11일 ZZ값 유지 - 김대현]
		 ZZ = _tstof(Zgap);
		*/
		temp.Format(L"N%d  G92  X%.3f  Y%.3f  Z%.3f  A0.000  B0.000 ;", Number, -Lc - _tstof(A1), _tstof(A1), ZZ);
		m_LIST.AddString(temp);
		Number += 10;

		ZZ = ph;
		temp.Format(L"N%d  G01 Z%.3f  F%.3f ;", Number, ZZ, vapp);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M01 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	
		temp.Format(L"N%d  %s ;", Number, ArcON);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G04  X%.3f ;", Number, pd);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  F%.3f ;", Number, Espeed);
		m_LIST.AddString(temp);
		Number += 10;
		
		ZZ = hc;
		float Bevel = _tstof(Bv1_1);
		float Ying = 0.0;
		float i = 0.0;
		if (overcut == 2)
		{
			float overR = _tstof(OR);//오버컷 길이 받는 과정
			Ying = (overR / DB) * (180.0 / PI);
		}
		for (float k = 0.0; k <= 1; k++)
		{
			F = -Lc;
			X = F + (RTb - (hrot - hc)) * tan(Bevel * (PI / 180.0)) - Wc;
			Y = -(_tstof(Db) / 2.0) * i * (PI / 180.0);

			temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -Bevel);
			m_LIST.AddString(temp);
			Number += 10;
			i += 360.0 + Ying;
		}
		i = 0.0;
		X = X - _tstof(A2);
		Y = Y - _tstof(A2);
		temp.Format(L"N%d  G01  X%.3f  Y%.3f  Z%.3f  A0.000  B%.3f ;", Number, X, Y, ZZ, -Bevel);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  %s ;", Number, ArcOFF);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  G00  Z%.3f  A0.000  B0.000 ;", Number, zrtn);
		m_LIST.AddString(temp);
		Number += 10;

		temp.Format(L"N%d  M30 ;", Number);
		m_LIST.AddString(temp);
		Number += 10;
	}

	temp.Format(L"%s",K);
	m_LIST.AddString(temp);
	Onum++;
}
///////////////////////////////////////////////////////////////////////////////////////////////// G - code 종료 ////////////////////////////////////////////////////////////////

void last::OnBnClickedButton2()  //////////////////파일 저장(RS232)
{
	LPCTSTR lpszFilter_RS232 = _T("RS232파일 (.NC)");
	char buf[1024];
	CString msg;
	CString filename;

	int count = m_LIST.GetCount();
	int i;

	CFileDialog RS232dlg(FALSE, _T("NC"), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter_RS232, nullptr);
	if (RS232dlg.DoModal() == IDOK)
	{
		filename = RS232dlg.GetPathName();

		CFile file(filename, CFile::modeWrite | CFile::modeCreate);
		for (i = 0; i < count; i++)
		{
			m_LIST.GetText(i, msg); msg += _T("\r\n");
			memset(buf, 0, sizeof(buf));

			WideCharToMultiByte(CP_ACP, 0, msg, -1, buf, sizeof(buf), NULL, NULL);
			file.Write(buf, strlen(buf));
		}
		file.Close();
	}
}

void last::OnBnClickedButton64()  //////////////////파일 저장(SNet)
{
	LPCTSTR lpszFilter_SNet = _T("SNet파일");
	char buf[1024];
	CString msg;
	CString filename;

	int count = m_LIST.GetCount();
	int i;

	CFileDialog SNetdlg(FALSE, _T(""), nullptr, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, lpszFilter_SNet, nullptr);
	if (SNetdlg.DoModal() == IDOK)
	{
		filename = SNetdlg.GetPathName();
		CFile file(filename, CFile::modeWrite | CFile::modeCreate);
		for (i = 1; i < count; i++)
		{
			m_LIST.GetText(i, msg); msg += _T("\r\n");
			memset(buf, 0, sizeof(buf));

			WideCharToMultiByte(CP_ACP, 0, msg, -1, buf, sizeof(buf), NULL, NULL);
			file.Write(buf, strlen(buf));
		}
		file.Close();
	}
}

void last::OnBnClickedButton3() ////////////////// RS232 연동
{
	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	CString strPath = path;
	int i = strPath.ReverseFind('\\');      // 실행파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strPath = strPath.Left(i);              // 뒤에 있는 현재 실행파일 이름을 지운다.

	CString File = strPath + L"\\RS232\\IDNC.exe"; // 주소 합치기

	SHELLEXECUTEINFO execinfo; // 실행을 위해 구조체 세트 
	ZeroMemory(&execinfo, sizeof(execinfo));
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = _T("open");
	execinfo.lpFile = File;
	execinfo.lpParameters = _T("\"c:\\octavescript.txt\"");
	execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;

	// 프로그램을 실행한다.
	int r = (int)ShellExecuteEx(&execinfo);

	//프로세스가 종료될 때까지 무한정 기다림
	WaitForSingleObject(execinfo.hProcess, INFINITE);
}



void last::OnLbnSelchangeList1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void last::OnBnClickedButton6()////////////////////////////////////////////////마지막 세팅///////////////////////////////////////////////////////////////
{
	CString Vapp, Zrtn,BOM;
	float PI = 3.141592654;
	float Wc, hc, pd, ph, speed, Espeed, Rspeed, Lspeed;
	float Thickness;
	float L_End_Thickness;
	float R_End_Thickness;
	float Bevel_0 = _tstof(Bv1);
	if (Bevel_0 < 0) Bevel_0 = -1 * Bevel_0;
	float Bevel_90 = _tstof(Bv2);
	if (Bevel_90 < 0) Bevel_90 = -1 * Bevel_90;
	float Bevel_180 = _tstof(Bv3);
	if (Bevel_180 < 0) Bevel_180 = -1 * Bevel_180;
	float Bevel_270 = _tstof(Bv4);
	if (Bevel_270 < 0) Bevel_270 = -1 * Bevel_270;
	float Bevel_total = (Bevel_0 + Bevel_90 + Bevel_180 + Bevel_270) / 4;
	float L_End_Bevel;
	float R_End_Bevel;
	if (Me > 100)
	{
		Thickness = _tstof(Dmt) / cos(Bevel_total *  (PI / 180.0));
		R_End_Bevel = _tstof(Bv1_1);
		L_End_Bevel = _tstof(Bv1_11);
		R_End_Thickness = _tstof(Dmt) / cos(R_End_Bevel *  (PI / 180.0));
		L_End_Thickness = _tstof(Dmt) / cos(L_End_Bevel *  (PI / 180.0));
	}
	else if (Me < 100)
	{
		Thickness = _tstof(Dbt) / cos(Bevel_total *  (PI / 180.0));
		if (Me == 11 || Me == 12)
		{
			float Bev_0 = _tstof(Bv1_1);
			if (Bev_0 < 0) Bev_0 = -1 * Bev_0;
			float Bev_90 = _tstof(Bv2_2);
			if (Bev_90 < 0) Bev_90 = -1 * Bev_90;
			float Bev_180 = _tstof(Bv3_3);
			if (Bev_180 < 0) Bev_180 = -1 * Bev_180;
			float Bev_270 = _tstof(Bv4_4);
			if (Bev_270 < 0) Bev_270 = -1 * Bev_270;
			L_End_Bevel = (Bev_0 + Bev_90 + Bev_180 + Bev_270) / 4;
		}
		else
		{
			L_End_Bevel = _tstof(Bv1_1);
		}
		L_End_Thickness = _tstof(Dbt) / cos(L_End_Bevel *  (PI / 180.0));
	}

	wchar_t chThisPath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, chThisPath);

	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	CString strPath = path;
	int i = strPath.ReverseFind('\\');      // 실행파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strPath = strPath.Left(i);              // 뒤에 있는 현재 실행파일 이름을 지운다.
	strPath = strPath + L"\\Excel";

	UpdateData(TRUE);

	// 엑셀파일이 있는 경로 지정
	CString strThisPath, m_city;
	if (jejil1 == _T("SUS") && Am == 50 && AON == 1)
	{
		m_city = L"SUS_AirAir_50A";
	}
	else if (jejil1 == _T("SUS") && Am == 130 && AON == 1)
	{
		m_city = L"SUS_AirAir_130A";
	}
	else if (jejil1 == _T("SUS") && Am == 130 && AON == 2)
	{
		m_city = L"SUS_N2N2_130A";
	}
	else if (jejil1 == _T("SUS") && Am == 200 && AON == 1)
	{
		m_city = L"SUS_AirAir_200A";
	}
	else if (jejil1 == _T("SUS") && Am == 200 && AON == 2)
	{
		m_city = L"SUS_N2N2_200A";
	}
	else if (jejil1 == _T("CARBON") && Am == 50 && AON == 1)
	{
		m_city = L"M_Steel_AirAir_50A";
	}
	else if (jejil1 == _T("CARBON") && Am == 50 && AON == 3)
	{
		m_city = L"M_Steel_O2Air_50A";
	}
	else if (jejil1 == _T("CARBON") && Am == 130 && AON == 1)
	{
		m_city = L"M_Steel_AirAir_130A";
	}
	else if (jejil1 == _T("CARBON") && Am == 130 && AON == 3)
	{
		m_city = L"M_Steel_O2Air_130A";
	}
	else if (jejil1 == _T("CARBON") && Am == 200 && AON == 1)
	{
		m_city = L"M_Steel_AirAir_200A";
	}
	else if (jejil1 == _T("CARBON") && Am == 200 && AON == 3)
	{
		m_city = L"M_Steel_O2Air_200A";
	}

	strThisPath.Format(L"%s\\%s.xlsx", strPath, m_city);
	//////////////////////////////////////
	GetModuleFileName(NULL, chThisPath, _MAX_PATH);

	CXLEzAutomation XL(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다
	XL.OpenExcelFile(strThisPath); // 액셀 파일 열기

	for (int i = 3; i < 1048576; i++)
	{
		CString EX1 = XL.GetCellValue(1, i); // WorkSheet의 값을 얻음
		CString EX2 = XL.GetCellValue(1, i + 1); // WorkSheet의 값을 얻음

		float Num1 = _tstof(EX1);
		float Num2 = _tstof(EX2);
		if (Num2 <= 0.0)
		{
			MessageBox(_T("값이 존재하지 않습니다."), _T("Error"), MB_OK | MB_ICONSTOP);
			break;
		}

		float Na;

		if (Num1 <= Thickness && Num2 > Thickness)
		{
			Na = (Num2 - Num1) / 100.0;
			float th = Num1;
			float Besoo = 0.0;
			for (int k = 0; k <= 100; k++)
			{
				if (Thickness <= th)
				{
					Besoo = k;
					break;
				}
				else
				{
					th = th + Na;
				}
			}

			CString D1;
			CString D2;
			float Dnum1;
			float Dnum2;

			D1 = XL.GetCellValue(2, i);
			D2 = XL.GetCellValue(2, i + 1);
			Dnum1 = _tstof(D1);
			Dnum2 = _tstof(D2);
			Na = (Dnum2 - Dnum1) / 100.0;
			hc = Dnum1 + (Na * Besoo);

			D1 = XL.GetCellValue(3, i);
			D2 = XL.GetCellValue(3, i + 1);
			Dnum1 = _tstof(D1);
			Dnum2 = _tstof(D2);
			Na = (Dnum1 - Dnum2) / 100.0;
			speed = Dnum1 - (Na * Besoo);

			D1 = XL.GetCellValue(4, i);
			D2 = XL.GetCellValue(4, i + 1);
			Dnum1 = _tstof(D1);
			Dnum2 = _tstof(D2);
			Na = (Dnum2 - Dnum1) / 100.0;
			ph = Dnum1 + (Na * Besoo);

			D1 = XL.GetCellValue(5, i);
			D2 = XL.GetCellValue(5, i + 1);
			Dnum1 = _tstof(D1);
			Dnum2 = _tstof(D2);
			Na = (Dnum2 - Dnum1) / 100.0;
			pd = Dnum1 + (Na * Besoo);

			break;
		}
	}
	if (Me < 100)
	{
		for (int i = 3; i < 1048576; i++)
		{
			CString EX1 = XL.GetCellValue(1, i); // WorkSheet의 값을 얻음
			CString EX2 = XL.GetCellValue(1, i + 1); // WorkSheet의 값을 얻음

			float Num1 = _tstof(EX1);
			float Num2 = _tstof(EX2);
			if (Num2 <= 0.0)
			{
				MessageBox(_T("값이 존재하지 않습니다."), _T("Error"), MB_OK | MB_ICONSTOP);
				break;
			}

			float Na;

			if (Num1 <= L_End_Thickness && Num2 > L_End_Thickness)
			{
				Na = (Num2 - Num1) / 100.0;
				float th = Num1;
				float Besoo = 0.0;
				for (int k = 0; k <= 100; k++)
				{
					if (L_End_Thickness <= th)
					{
						Besoo = k;
						break;
					}
					else
					{
						th = th + Na;
					}
				}

				CString D1;
				CString D2;
				float Dnum1;
				float Dnum2;

				D1 = XL.GetCellValue(3, i);
				D2 = XL.GetCellValue(3, i + 1);
				Dnum1 = _tstof(D1);
				Dnum2 = _tstof(D2);
				Na = (Dnum1 - Dnum2) / 100.0;
				Espeed = Dnum1 - (Na * Besoo);

				break;
			}
		}
	}
	else if (Me > 100)
	{
		for (int i = 3; i < 1048576; i++)
		{
			CString EX1 = XL.GetCellValue(1, i); // WorkSheet의 값을 얻음
			CString EX2 = XL.GetCellValue(1, i + 1); // WorkSheet의 값을 얻음

			float Num1 = _tstof(EX1);
			float Num2 = _tstof(EX2);
			if (Num2 <= 0.0)
			{
				MessageBox(_T("값이 존재하지 않습니다."), _T("Error"), MB_OK | MB_ICONSTOP);
				break;
			}

			float Na;

			if (Num1 <= R_End_Thickness && Num2 > R_End_Thickness)
			{
				Na = (Num2 - Num1) / 100.0;
				float th = Num1;
				float Besoo = 0.0;
				for (int k = 0; k <= 100; k++)
				{
					if (R_End_Thickness <= th)
					{
						Besoo = k;
						break;
					}
					else
					{
						th = th + Na;
					}
				}

				CString D1;
				CString D2;
				float Dnum1;
				float Dnum2;

				D1 = XL.GetCellValue(3, i);
				D2 = XL.GetCellValue(3, i + 1);
				Dnum1 = _tstof(D1);
				Dnum2 = _tstof(D2);
				Na = (Dnum1 - Dnum2) / 100.0;
				Rspeed = Dnum1 - (Na * Besoo);

				break;
			}
		}
		for (int i = 3; i < 1048576; i++)
		{
			CString EX1 = XL.GetCellValue(1, i); // WorkSheet의 값을 얻음
			CString EX2 = XL.GetCellValue(1, i + 1); // WorkSheet의 값을 얻음

			float Num1 = _tstof(EX1);
			float Num2 = _tstof(EX2);
			if (Num2 <= 0.0)
			{
				MessageBox(_T("값이 존재하지 않습니다."), _T("Error"), MB_OK | MB_ICONSTOP);
				break;
			}

			float Na;

			if (Num1 <= L_End_Thickness && Num2 > L_End_Thickness)
			{
				Na = (Num2 - Num1) / 100.0;
				float th = Num1;
				float Besoo = 0.0;
				for (int k = 0; k <= 100; k++)
				{
					if (L_End_Thickness <= th)
					{
						Besoo = k;
						break;
					}
					else
					{
						th = th + Na;
					}
				}

				CString D1;
				CString D2;
				float Dnum1;
				float Dnum2;

				D1 = XL.GetCellValue(3, i);
				D2 = XL.GetCellValue(3, i + 1);
				Dnum1 = _tstof(D1);
				Dnum2 = _tstof(D2);
				Na = (Dnum1 - Dnum2) / 100.0;
				Lspeed = Dnum1 - (Na * Besoo);

				break;
			}
		}
	}
	XL.ReleaseExcel(); // 엑셀 파일 해제

	 //////////////////////////////////////////////////////////////////////////////Wc 출력
	chThisPath[_MAX_PATH];
	GetCurrentDirectory(_MAX_PATH, chThisPath);

	path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	strPath = path;
	i = strPath.ReverseFind('\\');      // 실행파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strPath = strPath.Left(i);              // 뒤에 있는 현재 실행파일 이름을 지운다.
	strPath = strPath + L"\\Excel";

	int steelNum;
	if (jejil1 == _T("SUS"))
	{
		m_city = L"Kerf-Widths SUS";

		if (Am == 50 && AON == 1) steelNum = 3;
		else if (Am == 130 && AON == 1) steelNum = 4;
		else if (Am == 130 && AON == 2) steelNum = 5;
		else if (Am == 200 && AON == 1) steelNum = 6;
		else if (Am == 200 && AON == 2) steelNum = 7;
		else MessageBox(_T("System error"), _T("Error"), MB_OK | MB_ICONSTOP);
	}
	else if (jejil1 == _T("CARBON"))
	{
		m_city = L"Kerf-Widths M_Steel";

		if (Am == 50 && AON == 1) steelNum = 3;
		else if (Am == 50 && AON == 3) steelNum = 4;
		else if (Am == 130 && AON == 1) steelNum = 5;
		else if (Am == 130 && AON == 3) steelNum = 6;
		else if (Am == 200 && AON == 1) steelNum = 7;
		else if (Am == 200 && AON == 3) steelNum = 8;
		else MessageBox(_T("System error"), _T("Error"), MB_OK | MB_ICONSTOP);
	}

	strThisPath.Format(L"%s\\%s.xlsx", strPath, m_city);
	//////////////////////////////////////
	GetModuleFileName(NULL, chThisPath, _MAX_PATH);

	CXLEzAutomation EX(FALSE); // FALSE: 처리 과정을 화면에 보이지 않는다
	EX.OpenExcelFile(strThisPath); // 액셀 파일 열기

	for (int i = 2; i < 1048576; i++)
	{
		CString EX1 = EX.GetCellValue(i, 2); // WorkSheet의 값을 얻음
		CString EX2 = EX.GetCellValue(i + 1, 2); // WorkSheet의 값을 얻음

		float Num1 = _tstof(EX1);
		float Num2 = _tstof(EX2);

		float Na;

		if (Num1 <= Thickness && Num2 > Thickness)
		{
			Na = (Num2 - Num1) / 100.0;
			float th = Num1;
			float Besoo = 0.0;
			for (int k = 0; k <= 100; k++)
			{
				if (Thickness <= th)
				{
					Besoo = k;
					break;
				}
				else
				{
					th = th + Na;
				}
			}

			CString D1;
			CString D2;
			float Dnum1;
			float Dnum2;

			D1 = EX.GetCellValue(i, steelNum);
			D2 = EX.GetCellValue(i + 1, steelNum);
			Dnum1 = _tstof(D1);
			Dnum2 = _tstof(D2);
			Na = (Dnum2 - Dnum1) / 100.0;
			Wc = Dnum1 + (Na * Besoo);

			break;
		}
	}
	EX.ReleaseExcel(); // 엑셀 파일 해제

	WC.Format(L"%.3f", Wc);
	SetDlgItemText(IDC_EDIT1, WC);
	if (hc < 8.0) hc = 8.0;
	CH.Format(L"%.3f", hc);
	SetDlgItemText(IDC_EDIT2, CH);
	Speed.Format(L"%.3f", speed);
	SetDlgItemText(IDC_EDIT29, Speed);
	PH.Format(L"%.3f", ph);
	SetDlgItemText(IDC_EDIT3, PH);
	Pdelay.Format(L"%.3f", pd);
	SetDlgItemText(IDC_EDIT45, Pdelay);
	if (Me < 100)
	{
		ESpeed.Format(L"%.3f", Espeed);
		SetDlgItemText(IDC_EDIT37, ESpeed);
	}
	else if (Me > 100)
	{
		RSpeed.Format(L"%.3f", Rspeed);
		SetDlgItemText(IDC_EDIT10, RSpeed);
		LSpeed.Format(L"%.3f", Lspeed);
		SetDlgItemText(IDC_EDIT37, LSpeed);
	}
	Vapp.Format(L"300.000");
	SetDlgItemText(IDC_EDIT115, Vapp);

	
	Zrtn.Format(L"50.000");
	SetDlgItemText(IDC_EDIT116, Zrtn);
	
	if (Me >= 100 && Me <= 104 && ONIN == 1)
	{
		float cuth = _tstof(B12);
		CH.Format(L"%.3f", cuth);
		SetDlgItemText(IDC_EDIT2, CH);
	}
	
	//////////////////////////////////////////////////////////////////////////// [2020년 06월 11일 BOM(최장길이) 코딩 추가 - 공병주,김대현]

	SetDlgItemText(IDC_EDIT200, BOM);
	float BOMLl=0;
	float EE = _tstof(e);
	float SS = _tstof(s);
	float Lc = _tstof(Length);
	float Tb = _tstof(Dbt);
	float Tm = _tstof(Dmt);
	float Rb = _tstof(Db) / 2.0;
	float Rm = _tstof(Dm) / 2.0;
	float Rm2 = _tstof(Dm2) / 2.0;
	float G = _tstof(guri); // 유니폼 갭
	float o = _tstof(getsoo); // 유니폼 갯수
	float alfa1 = _tstof(aa1);//우측
	float alfa2 = _tstof(aa2);
	float alfa;
	float LcR = _tstof(Lc2); //모관 우측
	float LcL = _tstof(Lc3); //모관 좌측
	if (Me == 1)
	{
		alfa = atan(Rb / Rm) * (180.0 / PI);
	}
	else if (Me == 101)
	{
		alfa = 0.0;
	}
	else
	{
		alfa = _tstof(a);
	}

	if (Me == 1) // T-Joint Saddle Cuts
	{
		if (ONIN == 1) // Set-on
		{
			BOMLl = Lc - sqrt((Rm*Rm) - (EE - (Rb - Tb))*(EE - (Rb - Tb)));// Lc-sqrt(Rm^2-(e-(Rb-Tb))^2)

			BOM.Format(_T("%.3f"), BOMLl);
			SetDlgItemText(IDC_EDIT200, BOM);
		}
		else if(ONIN == 2) // Set-in
		{
			BOMLl = Lc - sqrt((Rm- Tm)*(Rm - Tm)-(EE - Rb)*(EE - Rb));// Lc-sqrt((Rm-Tm)^2-(e-Rb)^2)
			
			BOM.Format(_T("%.3f"), BOMLl);
			SetDlgItemText(IDC_EDIT200, BOM);
		}
	}

	else if (Me == 2) // Slant joint Saddle Cuts
	{
		if (ONIN == 1) // Set-on
		{
			BOMLl = Lc + ((Rb-Tb)/ tan(alfa * (PI / 180.0)))-((1/sin(alfa * (PI / 180.0)))*sqrt((Rm*Rm)-(EE*EE)));

			BOM.Format(_T("%.3f"), BOMLl);
			SetDlgItemText(IDC_EDIT200, BOM);
		}
		else if (ONIN == 2) // Set-in
		{
			BOMLl = Lc+ ((Rb) / tan(alfa * (PI / 180.0)))-1/sin(alfa * (PI / 180.0))*sqrt((Rm-Tm)*(Rm-Tm)-EE*EE);

			BOM.Format(_T("%.3f"), BOMLl);
			SetDlgItemText(IDC_EDIT200, BOM);
		}
	}

	else if (Me == 3) // V-shape Saddle Cut
	{
		BOMLl = Lc;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 4) // Strait End Cut
	{
		BOMLl = Lc;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 5) // Miter End Cut
	{
		BOMLl = Lc + (Rb - Tb)*tan(alfa*(PI / 180.0));
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 6) // Double Miter End Cut
	{
		if (alfa1 >= alfa2) {
			BOMLl = Lc - ((Rb - Tb)*tan(alfa2*(PI / 180.0)));
		}
		else {
			BOMLl = Lc - ((Rb - Tb)*tan(alfa1*(PI / 180.0)));
		}
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 7) // Slot End Cut
	{
		BOMLl = Lc;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 8) // Crown End Cut
	{
		BOMLl = Lc;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 9) // Multi-Uniform Strait End Cuts
	{
		BOMLl = (Lc *o) + (G *(o-1)) ;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 10) // Elbow End Cut
	{
		BOMLl = Lc;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 11) // Straight Miter End Cuts on Both Side
	{
		BOMLl = Lc + ((Rb - Tb)*tan((((PI / 2)*(180.0/PI)) - alfa1) * (PI / 180.0))) + ((Rb - Tb)*tan((((PI / 2)*(180.0/PI)) - alfa2) * (PI / 180.0)));
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 12) //  Straight Miter End Cuts on Both Side (H-Type Pattern) 
	{
		/* 
		[2020년 06월 17일 수식수정 - 김대현]
		BOMLl = Lc + (-1/sin(alfa1*(PI / 180.0))*sqrt(Rm*Rm-((EE - Rb + Tb)*(EE-Rb+Tb))))+(-1/sin(alfa2*(PI / 180.0))*sqrt(Rm2 * Rm2 - ((EE - Rb + Tb) * (EE - Rb + Tb))));
		*/
		float Xbr = ((1 / sin(alfa1 * (PI / 180))) * sqrt((Rm * Rm) - ((EE - (Rb - Tb)) * (EE - (Rb - Tb)))));
		float XbL = ((1 / sin(alfa2 * (PI / 180))) * sqrt((Rm2 * Rm2) - ((SS - (Rb - Tb)) * (SS - (Rb - Tb)))));
		BOMLl = -Xbr + Lc -XbL;
		BOM.Format(_T("%.3f"), BOMLl);
		SetDlgItemText(IDC_EDIT200, BOM);
	}

	else if (Me == 101 || Me || 102 && Me || 103 && Me || 104) // 모관 
	{
	BOMLl = LcR + LcL;
	BOM.Format(_T("%.3f"), BOMLl);
	SetDlgItemText(IDC_EDIT200, BOM);
	}

	
	/////////////////////////////////////////////////////////////////////////////////////////////////////

	
}

void last::OnBnClickedButton4()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT1, NUM);
}


void last::OnBnClickedButton47()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT2, NUM);
}


void last::OnBnClickedButton5()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT29, NUM);
}


void last::OnBnClickedButton61()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT3, NUM);
}


void last::OnBnClickedButton31()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT45, NUM);
}


void last::OnBnClickedButton32()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT115, NUM);
}

/*
[2020년 06월 11일 BOM(최장길이)에 키보드 제거 - 공병주]
void last::OnBnClickedButton33()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT116, NUM);
}
*/

//################################################################    VERIFY    ###############################################
void last::OnBnClickedButton151()
{
	GetDlgItemText(IDC_EDIT1, WC);
	GetDlgItemText(IDC_EDIT2, CH);

	float PI = 3.141592654;
	float Wc = _tstof(WC);
	float hc = _tstof(CH) - 1;
	float hrot = _tstof(Zgap);
	float error = 1;
	float Blimit = _tstof(RBelimit);

	//[<>2020.09.01 - 김대현]
	//@@ float Bevvv[12] = { _tstof(Bv1),_tstof(Bv2),_tstof(Bv3),_tstof(Bv4),_tstof(Bv1_1),_tstof(Bv2_2),_tstof(Bv3_3),_tstof(Bv4_4),_tstof(Bv1_11),_tstof(Bv2_22),_tstof(Bv3_33),_tstof(Bv4_44) };
	
	//Consider Reverse Beveling [+2020.09.01 - 김대현]
	float A = fabs(_tstof(Bv1));
	float B = fabs(_tstof(Bv2));
	float C = fabs(_tstof(Bv3));
	float D = fabs(_tstof(Bv4));
	float E = fabs(_tstof(Bv1_1));
	float F = fabs(_tstof(Bv2_2));
	float G = fabs(_tstof(Bv3_3));
	float H = fabs(_tstof(Bv4_4));
	float I = fabs(_tstof(Bv1_11));
	float J = fabs(_tstof(Bv2_22));
	float K = fabs(_tstof(Bv3_33));
	float L = fabs(_tstof(Bv4_44));

	//[+2020.09.01 - 김대현]
	float Bevvv[12] = { A,B,C,D,E,F,G,H,I,J,K,L };

	float inspec = -asin((hrot - hc) / sqrt(Wc*Wc + hrot * hrot)) * (180.0 / PI) + atan(hrot / Wc) * (180.0 / PI);

	for (int Q=0; Q<12; Q++)
	{
		if (inspec < Bevvv[Q])
		{
			error = 2;
		}
	}
	if (inspec > Blimit) 
	{ 
		inspec = Blimit;
	}
	if (error == 1)
	{
		CString Mess;
		Mess.Format(L"이상 없음\n최대 베벨링 값 : %.3f", inspec);
		AfxMessageBox(Mess, MB_OK);
	}
	else if (error == 2)
	{
		CString Mess;
		Mess.Format(L"베벨링 값이 크므로 수정바람\n최대 베벨링 값 : %.3f", inspec);
		AfxMessageBox(Mess, MB_OK);
	}
}
// #####################################################################################################################

void last::OnBnClickedButton34()////////////////////////////////SNet
{
	TCHAR path[_MAX_PATH];
	GetModuleFileName(NULL, path, sizeof(path));

	CString strPath = path;
	int i = strPath.ReverseFind('\\');      // 실행파일 이름을 지우기 위해서 왼쪽에 있는 '/'를 찾는다.
	strPath = strPath.Left(i);              // 뒤에 있는 현재 실행파일 이름을 지운다.

	CString File = strPath + L"\\SNet\\SNet.exe"; // 주소 합치기

	SHELLEXECUTEINFO execinfo; // 실행을 위해 구조체 세트 
	ZeroMemory(&execinfo, sizeof(execinfo));
	execinfo.cbSize = sizeof(execinfo);
	execinfo.lpVerb = _T("open");
	execinfo.lpFile = File;
	//execinfo.lpParameters = _T("\"c:\\octavescript.txt\"");
	execinfo.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS;
	execinfo.nShow = SW_SHOWDEFAULT;

	// 프로그램을 실행한다.
	int r = (int)ShellExecuteEx(&execinfo);

	//프로세스가 종료될 때까지 무한정 기다림
	WaitForSingleObject(execinfo.hProcess, INFINITE);
}

//////////////////////////////////////////////////////////////// Zoom 관련 부분
void last::OnBnClickedButton7()/////////////////////Zoom In
{
	Zoom += 0.0001f;
}


void last::OnBnClickedButton8()/////////////////////Zoom Out
{
	if (Zoom < 0.0001f)
	{
		Zoom = Zoom;
	}
	else
	{
		Zoom -= 0.0001f;
	}
}
////////////////////////////////////////////////////////////////////




void last::OnBnClickedButton88()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT118, NUM);
	GetDlgItemText(IDC_EDIT118, Boon);
}


void last::OnBnClickedButton153()
{
	GetDlgItemText(IDC_EDIT118, Boon);
	float PI = 3.141592654;
	float Rb = _tstof(Db) / 2;
	float Rm = _tstof(Dm) / 2;
	float bh = _tstof(Boon);
	float alfa;
	if (Me == 1)
	{
		alfa = atan(Rb / Rm) * (180.0 / PI);
	}
	else if (Me == 101)
	{
		alfa = 0.0;
	}
	else
	{
		alfa = _tstof(a);
	}

	float TaWon = PI * ((5.0 / 4.0) * (Rb + (Rb / cos(alfa * (PI / 180.0)))) - Rb * (Rb / cos(alfa * (PI / 180.0))) / (Rb + (Rb / cos(alfa * (PI / 180.0)))));

	CString BHL;
	float msbk = TaWon / bh;
	BHL.Format(L"%.3f", msbk);
	SetDlgItemText(IDC_EDIT119, BHL);
}


void last::OnBnClickedButton92()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT10, NUM);
}


void last::OnBnClickedButton149()
{
	keyboard O;
	O.DoModal();
	UpdateData(TRUE);
	SetDlgItemText(IDC_EDIT37, NUM);
}


void last::OnBnClickedRadio3()
{
	hole_print = 1;
}


void last::OnBnClickedRadio4()
{
	hole_print = 0;
}

