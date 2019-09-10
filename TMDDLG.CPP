// TmdDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Tmd.h"
#include "TmdDlg.h"
#include "okapi32.h"
#include "sub.h"
#include <io.h>

#include "math.h"
#include <stdio.h>
#include <vector>
#include <istream>
#include <fstream> 
#include <string>
using namespace std;
  #include   <algorithm>
  #include <functional>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define THRT 30
#define THR2 THRT*2
#define THR4 THR2*THR2

#pragma comment(lib, "OKAPI32")
#pragma comment(lib, "Sub")
//struct SH3 {long k,unsigned short g;float v;};
//struct SH4 {unsigned short i,j,g;float v;};	
//struct SH6 {unsigned short i,j,g1,g2;float v1,v2;};	
//struct RCRD {BYTE no,rng;SH4 s;};
struct ELPS{
	float	a,		//a轴
	  		b,		//b轴
			bt,		//a与x轴夹角
			ag,		//起点角
			me;		//均方差
	FXY		c;		//圆心
};	

struct DW2{DWORD dw[4];};
		



HANDLE hBrd;
PNT *pnt,*pnu,BLACK={0,0,0},WHITE={255,255,255},RED={0,0,255};
//	BYTE *bg=(BYTE *)new BYTE[HW];
BYTE *b0,*b1,*b2,gray;
RECT rct0;
//FXYZ dt[6][25];
short idy[729][6];


FXYZ stdd3(float *a,int n) ;
void Sort7z(FXYZ arr[],long cnt,BYTE flg);
void CTgt2Q(float id,FXYZ *dt1,float *Q);
float tsFx(float a,FXYZ t1,FXYZ c0);
float tsFind(float a,float b,FXYZ t1,FXYZ c0);
ELPS fitElp(FXYZ *xy,long n);
void genXX(FXYZ *xy,long n,float *XX);
FXYZ pdt2vct(FXYZ in,float &r);
float mods(FXYZ in){float r;r=sqrt(in.x*in.x+in.y*in.y+in.z*in.z);return r;};
void dsply();
FXYZ guiy1(FXYZ i,float &r){FXYZ f;r=sqrt(i.x*i.x+i.y*i.y+i.z*i.z);f.x=i.x/r;f.y=i.y/r;f.z=i.z/r;return f;}
float angle(FXYZ fi,FXYZ fj){FXYZ f0,f1;f0=guiy(fi);f1=guiy(fj);return acos(fi.x*fj.x+fi.y*fj.y+fi.z*fj.z);}
FXYZ vctliz(FXYZ fi,float la){FXYZ fj;fj=guiy(fi);fj.x*=la;fj.y*=la;fj.z*=la;return fj;}
void o_Rex(FXYZ ro,float *R);
FXYZ R_Oex(float *R);
//void o2Rex(FXYZ ro,float *R);
void Q2rot(float *Q,FXYZ *rot);
void rot2Q(float *Q,FXYZ *rot);

void Q2r1t(float *Q,FXYZ *rot);
void r1t2Q(float *Q,FXYZ *rot);

float juli(FXYZ a,FXYZ b){float x,y,z;x=b.x-a.x;y=b.y-a.y;z=b.z-a.z;return sqrt(x*x+y*y+z*z);}
void sttc(float *f,long n,float *rs);
float funX(FXYZ *fi,FXYZ *fj,int n,float *U);
void gunX(FXYZ *f,FXYZ *g,int n,float *U,FXYZ *rot);
void Q2v(float *Q,FXYZ *v){
	v[0].x=Q[0];
	v[1].x=Q[1];
	v[2].x= Q[2];
	v[3].x=Q[3];

	v[0].y=Q[4];
	v[1].y=Q[5];
	v[2].y= Q[6];
	v[3].y=Q[7];

	v[0].z=Q[8];
	v[1].z=Q[9];
	v[2].z=Q[10];
	v[3].z=Q[11];
}
void v2Q(float *Q,FXYZ *v){
	FillMemory(Q,64,0);Q[15]=1;
	Q[0]=v[0].x;
	Q[1]=v[1].x;
	Q[2]= v[2].x;
	Q[3] =v[3].x;

	Q[4]=v[0].y;
	Q[5]=v[1].y;
	Q[6]= v[2].y;
	Q[7] =v[3].y;

	Q[8]=v[0].z;
	Q[9]=v[1].z;
	Q[10]=v[2].z;
	Q[11]=v[3].z;
}
FXYZ addttn(FXYZ a,FXYZ b){
	FXYZ c;
	c.x=a.x+b.x;
	c.y=a.y+b.y;
	c.z=a.z+b.z;
	return c;
}

vector <FXYZ> F1;
vector <DW2> D2;
/////////////////////////////////////////////////////////////////////////////
// CTmdDlg dialog

CTmdDlg::CTmdDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTmdDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTmdDlg)
	m_id = 0.0f;
	//}}AFX_DATA_INIT
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTmdDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTmdDlg)
	DDX_Control(pDX, IDC_COMBO2, m_dot);
	DDX_Control(pDX, IDC_COMBO1, m_cmb);
	DDX_Control(pDX, IDC_LIST1, m_lst);
	DDX_Text(pDX, IDC_EDIT1, m_id);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTmdDlg, CDialog)
	//{{AFX_MSG_MAP(CTmdDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, OnButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnButton4)
	ON_BN_CLICKED(IDC_BUTTON5, OnButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnButton6)
	ON_WM_RBUTTONUP()
	ON_EN_CHANGE(IDC_EDIT1, OnChangeEdit1)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTmdDlg message handlers

BOOL CTmdDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	SetWindowPos(NULL,0,0,1280,800,NULL);	
	long lIndex=-2030,l;
	char st[80];
	FILE *fp;
	hBrd= okOpenBoard(&lIndex);
	RECT rct;
	SetRect(&rct,0,0,0,0);
	okSetTargetRect(hBrd,BUFFER,&rct);
	SetRect(&rct,2,40,2+W2,40+HEIGHT);
//	SetRect(&rct,0,0,W2,HEIGHT);

	okSetTargetRect(hBrd,SCREEN,&rct);
//	l=okLoadImageFile(hBrd,"BMP\\a.bmp",0,BUFFER,0,1);
//	l=okLoadImageFile(hBrd,"JPG\\rep0\\00.jpg",0,BUFFER,0,1);
	l=okLoadImageFile(hBrd,"JPG\\000.jpg",0,BUFFER,0,1);
//	l=okLoadImageFile(hBrd,"JPG\\a0.jpg",0,BUFFER,0,1);
	sprintf(st,"l=%d",l);m_lst.AddString(st);
	b0=(BYTE *)okGetBufferAddr(hBrd,0);b1=b0+HW;b2=b1+HW;
	pnt=(PNT *)okGetBufferAddr(hBrd,0);
//	pnu=(PNT *)okGetBufferAddr(hBrd,1);
	CreateDirectory("DAT\\",NULL);
	CreateDirectory("DAT\\0\\",NULL);
	CreateDirectory("DAT\\1\\",NULL);
	CreateDirectory("DBT\\",NULL);
	CreateDirectory("JPG\\",NULL);
	CreateDirectory("JPG\\0\\",NULL);
	CreateDirectory("JPG\\1\\",NULL);
	CreateDirectory("JPH\\",NULL);
	CreateDirectory("JPH\\0\\",NULL);
	CreateDirectory("JPH\\1\\",NULL);
	CreateDirectory("BMP\\",NULL);
	CreateDirectory("BMP\\0\\",NULL);
	CreateDirectory("BMP\\1\\",NULL);
	m_cmb.SetCurSel(0);m_dot.SetCurSel(0);

	if(!(fp=fopen("DAT\\id.dat","rb"))){m_id=1528.75;UpdateData(false);}
	else{fread(&m_id,sizeof(float),1,fp);UpdateData(false);}
	if(!(fp=fopen("DAT\\idy.dat","rb")))return 0;fread(idy,sizeof(short),729*3,fp);fclose(fp);
//	okConvertRect(hBrd,SCREEN,0,BUFFER,0,1);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTmdDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

HCURSOR CTmdDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

#define WRTX l=strlen(st);st[l]=13;st[l+1]=10;fwrite(st,1,l+2,fp);
#define SPACE st[0]=13;st[1]=10;fwrite(st,1,2,fp);

#define THRD 170
	int idx[9][2]={-1,-1,
		           -1,0,
				   -1,1,
				   0,-1,
				   0,0,
				   0,1,
				   1,-1,
				   1,0,
				   1,1};


double getN4rmPnts(FXYZ *f){
	int i,i1;
	double ds[3];
	double x,y,z,max,min;
	for(i=0;i<3;i++){i1=i+1;i1%=3;x=f[i1].x-f[i].x;y=f[i1].y-f[i].y;z=f[i1].z-f[i].z;ds[i]=sqrt(x*x+y*y+z*z);}
	max=0;for(i=0;i<3;i++)if(ds[i]>max)max=ds[i];
	min=99999;for(i=0;i<3;i++)if(ds[i]<min)min=ds[i];	
	return max-min;
}


FXYZ vLize(FXYZ V,float &l)	//one point V to a vector v and a number l
{
	l=sqrt(V.x*V.x+V.y*V.y+V.z*V.z);
	return guiy(V);
}
FXYZ VLize(FXYZ v,float l)	//a vector and a number to one point V
{
	FXYZ V=v;
	V.x*=l;V.y*=l;V.z*=l;
	return V;
}
void GetQ4rmDot(FXYZ *f,float edge,float *Q,FXYZ *La){
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());//char st[80];
	int k,k1;
	FXYZ mn[3],g[3],v0,v1,v2;
	float stp,zn,min,la,r,x,y,z,fwk;
	CopyMemory(mn,f,36);
	stp=getN4rmPnts(f);
	zn=9999;

	while(stp>0.00001)
	{
		CopyMemory(f,mn,36);
		stp=getN4rmPnts(f);
		min=stp;
		for(k=0;k<9;k++){
			CopyMemory(g,f,36);
			v1=vLize(g[1],r);g[1]=VLize(v1,r+idx[k][0]*stp);
			v2=vLize(g[2],r);g[2]=VLize(v2,r+idx[k][1]*stp);
			stp=getN4rmPnts(g);
			if(stp<min){min=stp;CopyMemory(mn,g,36);}
		}
		if(min==zn)break;
		zn=min;
	}
	CopyMemory(f,mn,12*3);
//	pDlg->m_lst.AddString("");
//	for(i=0;i<3;i++){sprintf(st,"mn %9.3f %9.3f %9.3f ",mn[i].x,mn[i].y,mn[i].z);pDlg->m_lst.AddString(st);}
//	for(i=0;i<3;i++){i1=i+1;i1%=3;x=mn[i1].x-mn[i].x;y=mn[i1].y-mn[i].y;z=mn[i1].z-mn[i].z;r=sqrt(x*x+y*y+z*z);sprintf(st,"r= %9.3f ",r);pDlg->m_lst.AddString(st);}

	for(k=0;k<3;k++){k1=k+1;k1%=3;x=mn[k1].x-mn[k].x;y=mn[k1].y-mn[k].y;z=mn[k1].z-mn[k].z;r=sqrt(x*x+y*y+z*z);}

	v0=vLize(mn[0],fwk);la=edge*fwk/r;mn[0]=VLize(v0,la);
	v1=vLize(mn[1],fwk);la=edge*fwk/r;mn[1]=VLize(v1,la);
	v2=vLize(mn[2],fwk);la=edge*fwk/r;mn[2]=VLize(v2,la);

	CopyMemory(La,mn,12*3);
	for(k=0;k<3;k++){k1=k+1;k1%=3;x=La[k1].x-La[k].x;y=La[k1].y-La[k].y;z=La[k1].z-La[k].z;r=sqrt(x*x+y*y+z*z);
//sprintf(st,"!r=%9.3f",r);pDlg->m_lst.AddString(st);
	}
	v0=sbstrct(mn[0],mn[1]);v0=guiy(v0);
	v1=sbstrct(mn[0],mn[2]);v1=guiy(v1);
	v2=normal(v0,v1);
	v1=normal(v2,v0);
	Q[0]=v0.x;Q[1]=v1.x;Q[ 2]=v2.x;Q[ 3]=mn[0].x;
	Q[4]=v0.y;Q[5]=v1.y;Q[ 6]=v2.y;Q[ 7]=mn[0].y;
	Q[8]=v0.z;Q[9]=v1.z;Q[10]=v2.z;Q[11]=mn[0].z;
	Q[12]=Q[13]=Q[14]=0;Q[15]=1;
}
void GeneIx(int *ix)
{
	int k0,i,k1,cnt;
	for(k0=0;k0<10;k0++)ix[k0]=-1;cnt=0;
	while(1){
		i=rand();i%=20;
		k1=0;for(k0=0;k0<10;k0++)if(ix[k0]!=i)k1++;
		if(k1==10)ix[cnt++]=i;
		if(cnt==10)break;
	}
}

void sttc(float *a,long n,float *rs)
{
	float sum=0,ave,x,sg,max,min;
	int k;
	min=99999;max=-99999;
	for(k=0;k<n;k++){
		if(a[k]>max)max=a[k];
		if(a[k]<min)min=a[k];
	}
	sum=0;for(k=0;k<n;k++)sum+=a[k];ave=sum/n;
	sum=0;for(k=0;k<n;k++){x=a[k]-ave;sum+=x*x;}
	sg=sqrt(sum/n);
	rs[0]=n;
	rs[1]=min;
	rs[2]=max;
	rs[3]=ave;
	rs[4]=sg;
	rs[5]=sg/(max-min);
}


void CTmdDlg::OnButton1() 
{
	char st[256];
	int i,j,i0,j0,i1,j1,k0,k1,k2,k3,k4,wk,kcnt;
	long k,l,n;
	float x,y,z,r,af,Q[16],A[9],Q0[16],R[12],edge,r3[3],min,max,mean,Qinv[16],U[16],Ua[16],Q2[2][16],A2[2][9];
	FXYZ fi,fj,fk,fl,f[25],g[25],g2[2][25],f3[3],f4[4],f7[73],g7[73],La[4],La2[2][360],v[4],rot[2];
	FILE *fp,*fq,*fr,*fw;
	FILE *fs[4];
	FXYZ offset;
	edge=2*RADIUS*cos(pi/6);

	FABCD S;S.A=S.B=0;S.C=-1;S.D=m_id;
	BYTE flg;
	WORD *wd;
	BYTE *b3=b2+HW;
	BYTE *b4=b3+HW;


//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\dv%d.dat",k4);if(!(fp=fopen(st,"rb")))return;for(k0=0;k0<40;k0++){sprintf(st,"JPG\\%d\\%02d.jpg",k4,k0);SetWindowText(st);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(100);fread(f,12,25,fp);for(k1=0;k1<25;k1++){i0=f[k1].y;j0=f[k1].x;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(20);}dsply();Sleep(100);}}
//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\dv.dat",k4);if(!(fr=fopen(st,"rb")))return;sprintf(st,"DAT\\%d\\La.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);fq=fopen(st,"wb");sprintf(st,"DAT\\%d\\Q.dat",k4);fw=fopen(st,"wb");for(k0=0;k0<360;k0++){fread(f,12,25,fr);for(k2=0;k2<24;k2++){for(k1=0;k1<3;k1++){f3[k1]=f[(k1*8+k2)%24];f3[k1].x-=W2;f3[k1].y-=H2;f3[k1].z=m_id;}GetQ4rmDot(f3,edge,Q,La);sprintf(st,"DAT\\%d\\Q.dat",k4);fwrite(Q,64,1,fw);SPACE;for(k1=0;k1<3;k1++){fi=La[k1];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k2,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}fclose(fr);fclose(fp);fclose(fq);}
//	FXYZ *fx=(FXYZ *)new FXYZ[1080];int ix[3]={0,2,1};for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\Q&A.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(fx,12,1080,fq);cld2eig(fx,1080,Q,A);sprintf(st,"Q[%d]:",k4);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A[%d]:  %12.3f %12.3f %9.3f",k4,A[0],A[4],A[8]);WRTX;Q2rot(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);WRTX;fclose(fp);sprintf(st,"DAT\\%d\\Q.dat",k4);fq=fopen(st,"wb");fwrite(Q,64,1,fq);fclose(fq);CopyMemory(Q2[k4],Q,64);m_lst.AddString("============================");for(k0=0;k0<45;k0++){k1=k0*8*3+ix[k0%3];fb[k4].x+=fx[k1].x;fb[k4].y+=fx[k1].y;fb[k4].z+=fx[k1].z;}fb[k4].x/=45;fb[k4].y/=45;fb[k4].z/=45;sprintf(st,"fb %9.3f %9.3f %9.3f",fb[k4].x,fb[k4].y,fb[k4].z);m_lst.AddString(st);}fq=fopen("DAT\\fb.dat","wb");fwrite(fb,12,2,fq);fclose(fq);delete [] fx;for(k4=0;k4<2;k4++){v[2].x=Q2[k4][2];v[2].y=Q2[k4][6];v[2].z=Q2[k4][10];fi.x=fb[k4].x-Q2[k4][3];fi.y=fb[k4].y-Q2[k4][7];fi.z=fb[k4].z-Q2[k4][11];v[0]=guiy(fi);v[1]=normal(v[2],v[0]);Q2[k4][0]=v[0].x;Q2[k4][1]=v[1].x;Q2[k4][2]=v[2].x;Q2[k4][4]=v[0].y;Q2[k4][5]=v[1].y;Q2[k4][6]=v[2].y;Q2[k4][8]=v[0].z;Q2[k4][9]=v[1].z;Q2[k4][10]=v[2].z;m_lst.AddString("Q2[k4]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);m_lst.AddString(st);}}fq=fopen("DAT\\Q2.dat","wb");fwrite(Q2,64,2,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\00.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(100);}dsply();Sleep(1000);}	
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){m_lst.AddString("=============================");for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);f[k0]=fi;sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);}sprintf(st,"DAT\\%d\\d25.dat",k4);fq=fopen(st,"wb");fwrite(f,12,25,fq);fclose(fq);}
//	float la,lb,lc;if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);m_lst.AddString("");m_lst.AddString("Q2[0]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);m_lst.AddString(st);}m_lst.AddString("Q2[1]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);m_lst.AddString(st);}m_lst.AddString("");inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],U);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2rot(U,rot);r=x=y=0;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);fi=guiy1(fi,lc);fi.x=acos(fi.x);fi.y=acos(fi.y);sprintf(st,"%02d %12.6f %12.6f %12.6f %9.3f",k0,fi.x*180/pi,fi.y*180/pi,fi.z,lc);m_lst.AddString(st);r+=lc;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",x*180/pi,y*180/pi,r);m_lst.AddString(st);rot[1].x=x;rot[1].y=y;rot[1].z=r;rot2Q(U,rot);m_lst.AddString("");fi=rot[0];fj=rot[1];sprintf(st,"rot %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);m_lst.AddString(st);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\U.dat","wb");fwrite(U,64,1,fq);fclose(fq);rot[0].x=rot[0].y=rot[0].z=0;rot2Q(U,rot);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\Ua.dat","wb");fwrite(U,64,1,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\28.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);sprintf(st,"DAT\\%d\\d25.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(f,12,25,fq);fclose(fq);m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j0+j]^=255;dsply();Sleep(200);}}
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);}r=x=y=0;m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);sprintf(st,"dif %02d %12.6f %12.6f %12.6f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);r+=fi.z;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;fi.x=x;;fi.y=y;fi.z=r;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",fi.x,fi.y,fi.z);m_lst.AddString(st);fq=fopen("DAT\\t.dat","wb");fwrite(&fi,12,1,fq);fclose(fq);
//	float bt,gm,lc,la,lb;if(!(fp=fopen("DAT\\Ua.dat","rb")))return;fread(U,64,1,fp);fclose(fp);if(!(fq=fopen("DAT\\dv0.dat","rb")))return;if(!(fr=fopen("DAT\\dv1.dat","rb")))return;fp=fopen("DAT\\fifj.txt","w");fw=fopen("DAT\\fifj.dat","wb");fk.x=U[3],fk.y=U[7],fk.z=U[11],fk=guiy1(fk,lc);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fi.y-=H2;fi.z=m_id;fj.x-=W2;fj.y-=H2;fj.z=m_id;fi=guiy(fi);v[0]=fi;fj=mvct4a(U,fj);fj=guiy(fj);v[1]=fj;gm=angle(fi,fj);af=angle(fi,fk);bt=pi-af-gm;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],la);fwrite(&fi,12,1,fw);fwrite(&fj,12,1,fw);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);	fclose(fq);fclose(fr);fclose(fw);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fq=fopen("DAT\\d25x2.dat","wb");fp=fopen("DAT\\d25x2.txt","w");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);	WRTX;fwrite(&fi,12,1,fq);fwrite(&fj,12,1,fq);}fclose(fq);fclose(fp);
//	if(!(fq=fopen("DAT\\fifj.dat","rb")))return;fp=fopen("DAT\\sbstrctFifj.txt","w");for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fq);fk=sbstrct(fi,fj);r=sqrt(fk.x*fk.x+fk.y*fk.y+fk.z*fk.z);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f",k0,fk.x,fk.y,fk.z,r);WRTX;}fclose(fq);fclose(fp);
//	fq=fopen("DAT\\idy.dat","wb");fp=fopen("DAT\\idy.txt","w");for(k=0;k<729;k++){idy[k][5]=(k/243);idy[k][4]=(k%243)/81;idy[k][3]=(k%81)/27;idy[k][2]=(k%27)/9;idy[k][1]=(k%9)/3;idy[k][0]=(k%3);}for(k=0;k<729;k++)for(i=0;i<6;i++)idy[k][i]-=1;for(k=0;k<729;k++){sprintf(st,"%03d |  %3d %3d %3d %3d %3d %3d " ,k,idy[k][5],idy[k][4],idy[k][3],idy[k][2],idy[k][1],idy[k][0]);m_lst.AddString(st);WRTX;}fwrite(idy,2,729*6,fq);fclose(fq);fclose(fp);
//Qa	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ua.dat","rb")))return;fread(Ua,64,1,fq);fclose(fq);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);}
//Q2 	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],Ua);gunX(f,g,25,Ua,rot);r1t2Q(Ua,rot);fq=fopen("DAT\\Ub.dat","wb");fp=fopen("DAT\\Ub.txt","w");sprintf(st,"Ub:");WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);WRTX;}fwrite(Ua,64,1,fq);fclose(fq);fclose(fp);
//crtfy	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2r1t(U,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z);m_lst.AddString(st);m_lst.AddString("");m_lst.AddString("===========================");for(k0=0;k0<25;k0++){m_lst.AddString("");fi=f[k0];fj=g[k0];fj=mvct4a(U,fj);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fj.x,fj.y,fj.z);m_lst.AddString(st);}
//crtfu		if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}if(!(fq=fopen("DAT\\0\\La.dat","rb")))return;if(!(fr=fopen("DAT\\1\\La.dat","rb")))return;fp=fopen("DAT\\crtfu.txt","w");sprintf(st,"=========================");WRTX;for(k0=0;k0<1080;k0++){SPACE;fread(&fi,12,1,fq);fread(&fj,12,1,fr);;sprintf(st,"%04d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;fj=mvct4a(U,fj);sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,fj.x,fj.y,fj.z);WRTX;}fclose(fq);fclose(fr);fclose(fp);
//crtfv	FXYZ t;float bt,gm,la,lb,lc;fp=fopen("DAT\\crtfv.txt","w");if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);sprintf(st,"U:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}CopyMemory(Qinv,U,64);inv4(Qinv);sprintf(st,"Qinv:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Qinv[i*4+0],Qinv[i*4+1],Qinv[i*4+2],Qinv[i*4+3]);m_lst.AddString(st);WRTX;}
//		if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fr=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fr);fclose(fr);t.x=U[3];t.y=U[7];t.z=U[11];v[2]=guiy1(t,lc);sprintf(st,"labc+abg+SINE cert=========================");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f %9.4f",k0,la,lb,lc,af*180/pi,bt*180/pi,gm*180/pi,(af+bt+gm)*180/pi);m_lst.AddString(st);WRTX;}sprintf(st,"SINE cert");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %12.6f %12.6f %12.6f |",k0,la/sin(af),lb/sin(bt),lc/sin(gm));m_lst.AddString(st);WRTX;}sprintf(st,"9000 afbtgm=========================");m_lst.AddString(st);WRTX;if(!(fq=fopen("DAT\\0\\dv.dat","rb")))return;;if(!(fr=fopen("DAT\\1\\dv.dat","rb")))return;;for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,af*180/pi,bt*180/pi,gm*180/pi);WRTX;}sprintf(st,"=========================");m_lst.AddString(st);WRTX;fseek(fq,0,SEEK_SET);fseek(fr,0,SEEK_SET);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],lb);sprintf(st,"%04d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);fclose(fq);fclose(fr);
//==================================================================================	
//JPH	BLOCKINFO blk;FillMemory(&blk,sizeof(blk),0);blk.iBitCount=8;blk.iFormType=FORM_GRAY8;blk.iHeight=HEIGHT;blk.iWidth=WIDTH;blk.lpBits = b0;for(k0=0;k0<9;k0++){for(k1=0;k1<40;k1++){sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k1);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)okGetBufferAddr(hBrd,0);b0=(BYTE *)pnt;b1=b0+HW*2;for(k=0;k<HW*2;k++)b0[k]=pnt[k].r;CopyMemory(b1,b0,HW*2);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+j];}sprintf(st,"JPH\\0\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+WIDTH+j];}sprintf(st,"JPH\\1\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);}}
//dt	float sum,isum,jsum;fp=fopen("DBT\\dt.txt","w");fq=fopen("DBT\\dt.dat","wb");int *bg5=(int *)new int[HW];for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,1,1);im2bw(b1,b0,HEIGHT,WIDTH);for(i=0;i<10;i++)for(j=0;j<WIDTH;j++)b0[i*WIDTH+j]=0;FillMemory(bg5,sizeof(int)*HW,0);for(n=0;n<HW;n++)if(b0[n]>0)bg5[n]=1;wk=BWLabel(bg5,HEIGHT,WIDTH);sprintf(st,"wk=%d",wk);WRTX;	kcnt=0;for(k1=1;k1<=wk;k1++){sum=isum=jsum=0;for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){if(bg5[i*WIDTH+j]==k1){sum++;isum+=i;jsum+=j;}}if(sum>10){fi.x=jsum/sum;fi.y=isum/sum;fi.z=sum;sprintf(st,"%d %02d %03d %9.3f %9.3f %7.0f",k4,kcnt++,k0,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}}delete[]bg5;fclose(fp);fclose(fq);
//dv	if(!(fq=fopen("DBT\\dt.dat","rb")))return;fp=fopen("DBT\\dv.txt","w");fr=fopen("DBT\\dv.dat","wb");for(k0=0;k0<720;k0++){SPACE;fread(f,12,25,fq);for(k=0;k<25;k++){f[k].x-=W2;f[k].y-=H2;}Sort7z(f,25,1);for(k=0;k<24;k++){x=f[k].x-f[24].x;y=f[k].y-f[24].y;r=sqrt(x*x+y*y);af=asin(y/r);if(x<0)af=pi-af;if(af<0)af+=2*pi;f[k].z=af;}for(k=23;k>=0;k--){f[k].z-=f[0].z;if(f[k].z<0)f[k].z+=2*pi;}Sort7z(f,24,0);	for(k=0;k<25;k++){fi=f[k];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k,fi.x,fi.y,fi.z);WRTX;}fwrite(f,12,25,fr);}fclose(fq);fclose(fp);fclose(fr);
//La	if(!(fq=fopen("DBT\\dv.dat","rb")))return;fp=fopen("DBT\\La.txt","w");fr=fopen("DBT\\La.dat","wb");fw=fopen("DBT\\Q.dat","wb");for(k0=0;k0<720;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k=0;k<24;k++){for(i=0;i<3;i++){f3[i]=f[(k+8*i)%24];f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);fwrite(Q,64,1,fw);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){fi=La[i];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);fk=guiy1(fk,r3[i]);}sprintf(st,"r: %9.4f %9.4f %9.4f ",r3[0],r3[1],r3[2]);WRTX;}			}fclose(fq);fclose(fw);fclose(fr);fclose(fp);	
//rot	if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\Q.txt","w");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);sprintf(st,"Q[%03d]:",k0);WRTX;for(i1=0;i1<4;i1++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i1*4+0],Q[i1*4+1],Q[i1*4+2],Q[i1*4+3]);WRTX;}}}fclose(fq);fclose(fp);if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\rot.txt","w");fr=fopen("DBT\\rot.dat","wb");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fr);r=sqrt(fj.x*fj.x+fj.y*fj.y+fj.z*fj.z);sprintf(st,"rot[%03d]: %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}}fclose(fq);fclose(fp);fclose(fr);	
//cld	if(!(fq=fopen("DBT\\La.dat","rb")))return;fp=fopen("DBT\\cld.txt","w");FXYZ *L7=(FXYZ *)new FXYZ[72];fr=fopen("DBT\\Q7.dat","wb");fw=fopen("DBT\\rt7.dat","wb");for(k0=0;k0<720;k0++){fread(L7,12,72,fq);SPACE;cld2eig(L7,72,Q,A);sprintf(st,"Q[%03d]:",k0);WRTX;fwrite(Q,64,1,fr);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A: %9.3f %9.3f %9.6f",A[0],A[4],A[8]);WRTX;Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fw);fk=guiy1(fj,r);sprintf(st,"%9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}fclose(fp);fclose(fr);fclose(fw);fclose(fq);delete[]L7;
//log	FXYZ *f1st=(FXYZ *)new FXYZ[720];FXYZ *cntr=(FXYZ *)new FXYZ[720];FXYZ *v0=(FXYZ *)new FXYZ[720];if(!(fq=fopen("DBT\\La.dat","rb")))return;for(k0=0;k0<720;k0++){fread(f7,12,72,fq);fi.x=(f7[0].x+f7[26].x+f7[49].x)/3;fi.y=(f7[0].y+f7[26].y+f7[49].y)/3;fi.z=(f7[0].z+f7[26].z+f7[49].z)/3;f1st[k0]=fi;}if(!(fq=fopen("DBT\\Q7.dat","rb")))return;fp=fopen("DBT\\log.txt","w");fr=fopen("DBT\\Q2.dat","wb");for(k0=0;k0<720;k0++){fread(Q,64,1,fq);fi.x=cntr[k0].x=Q[3];fi.y=cntr[k0].y=Q[7];fi.z=cntr[k0].z=Q[11];fj=f1st[k0];fi=sbstrct(fi,fj);fi=guiy(fi);v0[k0]=fi;SPACE;Q2r1t(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%03d %9.6f %9.6f %9.6f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;Q2v(Q,v);v[0]=v0[k0];v[1]=normal(v[2],v[0]);v2Q(Q,v);Q2r1t(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%03d %9.6f %9.6f %9.6f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;fwrite(Q,64,1,fr);}delete[]f1st,cntr,v0;fclose(fq);fclose(fr);fp=fopen("DBT\\loh.txt","w");fr=fopen("DBT\\U7.dat","wb");if(!(fq=fopen("DBT\\Q2.dat","rb")))return;for(k0=0;k0<360;k0++){fseek(fq,k0*64,SEEK_SET);fread(Q2[0],64,1,fq);fseek(fq,(k0+360)*64,SEEK_SET);fread(Q2[1],64,1,fq);SPACE;sprintf(st,"Q2[0][%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);WRTX;}sprintf(st,"Q2[1][%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);WRTX;}CopyMemory(Qinv,Q2[1],64);inv4(Qinv);mmtrx4(Q2[0],Qinv,U);sprintf(st,"U[%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);WRTX;}fwrite(U,64,1,fr);Q2r1t(U,rot);fi=rot[0];fj=rot[1];fk=guiy1(fj,r);sprintf(st,"%03d %9.4f %9.4f %9.4f| %9.4f %9.4f %9.4f %9.4f ",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}fclose(fq);fclose(fp);fclose(fr);
//-------------------------------------------------------------
//La		if(!(fq=fopen("DBT\\dv.dat","rb")))return;fp=fopen("DAT\\La.txt","w");fr=fopen("DAT\\La.dat","wb");fw=fopen("DAT\\Q.dat","wb");for(k0=0;k0<720;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k=0;k<24;k++){for(i=0;i<3;i++){f3[i]=f[(k+8*i)%24];f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);fwrite(Q,64,1,fw);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){fi=La[i];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);fk=guiy1(fk,r3[i]);}sprintf(st,"r: %9.4f %9.4f %9.4f ",r3[0],r3[1],r3[2]);WRTX;}			}fclose(fq);fclose(fw);fclose(fr);fclose(fp);	
//fifj		if(!(fq=fopen("DAT\\UbOld.dat","rb")))return;fread(U,64,1,fq);fclose(fq);if(!(fq=fopen("DBT\\La.dat","rb")))return;fp=fopen("DAT\\fifj.txt","w");for(k0=0;k0<25920;k0++){fseek(fq,k0*12,SEEK_SET);fread(&fi,12,1,fq);fseek(fq,(k0+25920)*12,SEEK_SET);fread(&fj,12,1,fq);fj=mvct4a(U,fj);fk=sbstrct(fi,fj);fk=guiy1(fk,r);sprintf(st,"%05d %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z,r);WRTX;}fclose(fq);fclose(fp);
//b0.bmp	BLOCKINFO blk;;FillMemory(&blk,sizeof(blk),0);blk.iBitCount=8;blk.iFormType=FORM_GRAY8;blk.iHeight=HEIGHT;blk.iWidth=WIDTH;blk.lpBits = b0;for(k4=0;k4<2;k4++){sprintf(st,"BMP\\a%d.bmp",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)b0;for(k=0;k<HW;k++)b0[k]=pnt[k].r;sprintf(st,"BMP\\b%d.bmp",k4);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);}
//dif.dat	BYTE flg;float f9[9]={0},a[4]={0},cnt,stp;int iwk,ix[9]={-WIDTH-1,-WIDTH+0,-WIDTH+1,-1,0,1,WIDTH-1, WIDTH+0, WIDTH+1};for(k0=0;k0<2;k0++){sprintf(st,"BMP\\b%d.bmp",k0);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);b0=(BYTE *)okGetBufferAddr(hBrd,0);FillMemory(b1,HW,0);for(k=0;k<HW;k++){if(b0[k]==0)continue;flg=0;for(i=0;i<9;i++){if(b0[k+ix[i]]==0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)f9[i]=(b0[k+ix[i]]-b0[k]);for(i=0;i<4;i++){a[i]=f9[i]+f9[8-i];if(a[i]<0)a[i]=-a[i];}iwk=a[0]+a[1]+a[2]+a[3];if(iwk>255)iwk=255;b1[k]=iwk;}sprintf(st,"DAT\\dif%d.dat",k0);fq=fopen(st,"wb");fwrite(b1,1,HW,fq);fclose(fq);CopyMemory(b0,b1,HW);dsply();Sleep(1000);sprintf(st,"BMP\\df%d.bmp",k0);okSaveImageFile(hBrd,st,100,BUFFER,0,1);}
//	for(k4=0;k4<2;k4++){sprintf(st,"BMP\\df%d.bmp",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=255;dsply();Sleep(1000);}
//===============================================
//wd	PNT *pnu,p;for(k4=0;k4<2;k4++){sprintf(st,"BMP\\a%d.bmp",k4);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)okGetBufferAddr(hBrd,0);pnu=pnt+HW*3; wd=(WORD *)pnu;FillMemory(wd,2,HW,0);for(k=0;k<HW;k++)wd[k]=(114*pnt[k].b+587*pnt[k].g+299*pnt[k].r)/10;sprintf(st,"DAT\\wd%d.dat",k4);fq=fopen(st,"wb");fwrite(wd,2,HW,fq);fclose(fq);}
//bw.bmp	if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[0],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);for(k4=0;k4<2;k4++){for(k=0;k<HW;k++)b0[k]=wd2[k4][k]/100;dsply();Sleep(1000);}for(k4=0;k4<2;k4++){FillMemory(b0,HW,0);for(k=0;k<HW;k++)if(wd2[k4][k]>0)b0[k]=255;sprintf(st,"BMP\\bw%d.bmp",k4);okSaveImageFile(hBrd,st,100,BUFFER,0,1);dsply();Sleep(1000);}
//offset	FXYZ cntr[2]={0};long cnt;okLoadImageFile(hBrd,"BMP\\bw0.bmp",0,BUFFER,0,1);cnt=0;for(k=0;k<HW;k++){if(b0[k]>0){cntr[0].x+=k%WIDTH;cntr[0].y+=k/WIDTH;cnt++;}}cntr[0].x/=cnt;cntr[0].y/=cnt;okLoadImageFile(hBrd,"BMP\\bw1.bmp",0,BUFFER,1,1);cnt=0;for(k=0;k<HW;k++){if(b1[k]>0){cntr[1].x+=k%WIDTH;cntr[1].y+=k/WIDTH;cnt++;}}cntr[1].x/=cnt;cntr[1].y/=cnt;offset=sbstrct(cntr[0],cntr[1]);sprintf(st,"offset x=%9.3f y=%9.3f",offset.x,offset.y);m_lst.AddString(st);fq=fopen("DAT\\offset.dat","wb");fwrite(&offset,12,1,fq);fclose(fq);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=85;for(k=0;k<HW;k++)if(b1[k]>0)b0[k]+=170;okSaveImageFile(hBrd,"BMP\\offset.bmp",100,BUFFER,0,1);	
//0to1		if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);okLoadImageFile(hBrd,"BMP\\bw0.bmp",0,BUFFER,0,1);FillMemory(b1,HW,0);for(k=0;k<HW;k++){i=k/WIDTH+offset.y;j=k%WIDTH+offset.x;if(b0[k]>0)b1[i*WIDTH+j]=85;}okLoadImageFile(hBrd,"BMP\\bw1.bmp",0,BUFFER,0,1);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=170;for(k=0;k<HW;k++)b0[k]+=b1[k];dsply();
//grid		for(k=0;k<25;k++){i=(k/5)-2;j=(k%5)-2;idz[k]=i*WIDTH+j;}for(k=0;k<5;k++){sprintf(st,"%6d %6d %6d %6d %6d ",idz[k*5+0],idz[k*5+1],idz[k*5+2],idz[k*5+3],idz[k*5+4]);m_lst.AddString(st);}fq=fopen("DAT\\idz25.dat","wb");fwrite(idz,sizeof(int),25,fq);fclose(fq);if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);FillMemory(b2,HW,0);okLoadImageFile(hBrd,"BMP\\df0.bmp",0,BUFFER,0,1);for(k=0;k<HW;k++){if(b0[k]>0){i=k/WIDTH+offset.y;j=k%WIDTH+offset.x;b2[i*WIDTH+j]=b0[k];}}CopyMemory(b0,b2,HW);okLoadImageFile(hBrd,"BMP\\df1.bmp",0,BUFFER,0,1);FillMemory(b2,HW,0);fq=fopen("DAT\\grid.dat","wb");for(i=0;i<HEIGHT;i+=5)for(j=0;j<WIDTH;j+=5){flg=0;for(i0=0;i0<5;i0++)for(j0=0;j0<5;j0++)if(b0[(i+i0)*WIDTH+j+j0]==0)flg++;if(flg==0){b2[(i+2)*WIDTH+j+2]=255;fi.x=j+2;fi.y=i+2;fi.z=0;fwrite(&fi,12,1,fq);}}CopyMemory(b0,b2,HW);dsply();okSaveImageFile(hBrd,"BMP\\c.bmp",100,BUFFER,0,1);fclose(fq);
//bw.bmp	if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[0],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);for(k4=0;k4<2;k4++){for(k=0;k<HW;k++)b0[k]=wd2[k4][k]/100;dsply();Sleep(1000);}for(k4=0;k4<2;k4++){FillMemory(b0,HW,0);for(k=0;k<HW;k++)if(wd2[k4][k]>0)b0[k]=255;sprintf(st,"BMP\\bw%d.bmp",k4);okSaveImageFile(hBrd,st,100,BUFFER,0,1);dsply();Sleep(1000);}
//wdx3		wd2[0]=(WORD *)b0;wd2[1]=(WORD *)b2;wd2[2]=(WORD *)b4;if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);sprintf(st,"offset %9.3f %9.3f",offset.x,offset.y);m_lst.AddString(st);if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);FillMemory(wd2[0],HW*2,0);for(k=0;k<HW;k++){if(wd2[2][k]>0){i=k/WIDTH+offset.y;j=(k%WIDTH)+offset.x;wd2[0][i*WIDTH+j]=wd2[2][k];}}fq=fopen("DAT\\wd2.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);wd=(WORD *)b0;for(k4=0;k4<3;k4++){sprintf(st,"DAT\\wd%d.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(wd,2,HW,fq);for(k=0;k<HW;k++)b0[k]=wd[k]/100;dsply();Sleep(1000);FillMemory(wd,HW*2,0);}
//cmpr		wd=(WORD*)b0;for(k4=1;k4<3;k4++){sprintf(st,"DAT\\wd%d.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(wd,2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd[k]/100;dsply();Sleep(1000);}
//wdf		int cha[9],a[4];int ix9[9]={-WIDTH-1,-WIDTH,-WIDTH+1,-1,0,1,WIDTH-1,WIDTH,WIDTH+1};if(!(fq=fopen("DAT\\wd2.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);FillMemory(wd2[0],2*HW,0);for(k=0;k<HW;k++){if(wd2[2][k]<=0)continue;flg=0;for(i=0;i<9;i++){if(wd2[2][k+ix9[i]]<=0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)cha[i]=wd2[2][k+ix9[i]]-wd2[2][k];for(i=0;i<4;i++)a[i]=abs(cha[i]+cha[8-i]);wd2[0][k]=a[0]+a[1]+a[2]+a[3];}fq=fopen("DAT\\wdf2.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd2[0][k]/100;dsply();Sleep(1000);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);FillMemory(wd2[0],2*HW,0);for(k=0;k<HW;k++){if(wd2[1][k]<=0)continue;flg=0;for(i=0;i<9;i++){if(wd2[1][k+ix9[i]]<=0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)cha[i]=wd2[1][k+ix9[i]]-wd2[1][k];for(i=0;i<4;i++)a[i]=abs(cha[i]+cha[8-i]);wd2[0][k]=a[0]+a[1]+a[2]+a[3];}fq=fopen("DAT\\wdf1.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd2[0][k]/100;dsply();Sleep(1000);
//addr		DW2 d;if(!(fq=fopen("DAT\\wdf2.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wdf1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);D2.clear();for(i0=0;i0<HEIGHT;i0+=11)for(j0=0;j0<WIDTH;j0+=11){FillMemory(&d,sizeof(D2),0);min=99999;max=0;for(i=0;i<=10;i++)for(j=0;j<=10;j++){k=(i0+i)*WIDTH+j0+j;if(wd2[2][k]>max){max=wd2[2][k];l=k;}if(wd2[2][k]<min){min=wd2[2][k];}}if(min<=0)max=0;d.dw[0]=l;d.dw[1]=max;min=99999;max=0;for(i=0;i<=10;i++)for(j=0;j<=10;j++){k=(i0+i)*WIDTH+j0+j;if(wd2[1][k]>max){max=wd2[1][k];l=k;}if(wd2[1][k]<min){min=wd2[1][k];}}if(min<=0)max=0;d.dw[2]=l;d.dw[3]=max;if(d.dw[1]>0&&d.dw[3]>0)D2.push_back(d);}sprintf(st,"size=%d",D2.size());m_lst.AddString(st);fp=fopen("DAT\\log3.txt","w");for(k=0;k<D2.size();k++){sprintf(st,"%04d %6d %6d %6d %6d ",k,D2[k].dw[0],D2[k].dw[1],D2[k].dw[2],D2[k].dw[3]);WRTX;}fclose(fp);fp=fopen("DAT\\addr.txt","w");for(k=0;k<D2.size();k++){i0=D2[k].dw[0]/WIDTH;j0=D2[k].dw[0]%WIDTH;i1=D2[k].dw[2]/WIDTH;j1=D2[k].dw[2]%WIDTH;x=j1-j0;y=i1-i0;r=sqrt(x*x+y*y);sprintf(st,"%04d %6d %6d %9.3f",k,D2[k].dw[0],D2[k].dw[2],r);WRTX;}fclose(fp);fq=fopen("DAT\\addr.dat","wb");for(k=0;k<D2.size();k++)fwrite(&D2[k],sizeof(D2),1,fq);fclose(fq);
//X&XX	{float x[9]={-1,0,-1,-1,0,-1,-1,0,-1};float y[9]={-1,-1,-1,0,0,0,-1,-1,-1};float X[9][6];float XX[6][6]={0};for(i=0;i<9;i++){X[i][0]=x[i]*x[i];X[i][1]=x[i]*y[i];X[i][2]=y[i]*y[i];X[i][3]=x[i];X[i][4]=y[i];X[i][5]=1;}for(i=0;i<9;i++){sprintf(st,"%d %5.0f %5.0f %5.0f %5.0f %5.0f %5.0f ",i,X[i][0],X[i][1],X[i][2],X[i][3],X[i][4],X[i][5]);m_lst.AddString(st);}for(i=0;i<6;i++){for(j=0;j<6;j++){for(k=0;k<9;k++)XX[i][j]+=X[i][k]*X[j][k];}}m_lst.AddString("");for(i=0;i<6;i++){sprintf(st,"%d %5.0f %5.0f %5.0f %5.0f %5.0f %5.0f ",i,XX[i][0],XX[i][1],XX[i][2],XX[i][3],XX[i][4],XX[i][5]);m_lst.AddString(st);}fq=fopen("DAT\\X.dat","wb");fwrite(X,4,6*9,fq);fclose(fq);fq=fopen("DAT\\XX.dat","wb");fwrite(XX,4,6*6,fq);fclose(fq);}
//===============================================


//img2dv	float sum,isum,jsum;int *bg5=(int *)new int[HW];fp=fopen("DAT\\dv.txt","w");fq=fopen("DAT\\dv.dat","wb");for(k4=0;k4<2;k4++){sprintf(st,"===================");WRTX;for(k0=0;k0<360;k0++){SPACE;sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);okLoadImageFile(hBrd,st,0,BUFFER,0,1);im2bw(b0,b1,HEIGHT,WIDTH);CopyMemory(b0,b1,HW);FillMemory(bg5,sizeof(int)*HW,0);for(i=0;i<10;i++)for(j=0;j<WIDTH;j++)b1[i*WIDTH+j]=0;for(n=0;n<HW;n++)if(b1[n]>0)bg5[n]=1;wk=BWLabel(bg5,HEIGHT,WIDTH);kcnt=0;for(k1=1;k1<=wk;k1++){sum=isum=jsum=0;for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){if(bg5[i*WIDTH+j]==k1){sum++;isum+=i;jsum+=j;}}if(sum>10){fi.x=jsum/sum;fi.y=isum/sum;fi.z=sum;f[kcnt++]=fi;}}sprintf(st,"wk=%d kcnt=%d",wk,kcnt);WRTX;Sort7z(f,25,1);for(k1=0;k1<24;k1++){x=f[k1].x-f[24].x;y=f[k1].y-f[24].y;r=sqrt(x*x+y*y);af=asin(y/r);if(x<0)af=pi-af;f[k1].z=af;}for(k1=23;k1>=0;k1--)f[k1].z-=f[0].z;for(k1=0;k1<24;k1++)if(f[k1].z<0)f[k1].z+=2*pi;Sort7z(f,24,0);fwrite(f,12,25,fq);for(i=0;i<25;i++){fi=f[i];sprintf(st,"%d %03d %02d %9.3f %9.3f %9.3f ",k4,k0,i,fi.x,fi.y,fi.z);WRTX;}}}delete[]bg5;fclose(fq);fclose(fp);
//La		if(!(fq=fopen("DAT\\dv.dat","rb")))return;fp=fopen("DAT\\La.txt","w");fr=fopen("DAT\\La.dat","wb");for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k1=0;k1<24;k1++){for(i=0;i<3;i++){fi=f[(k1+i*8)%24];fi.x-=W2;fi.y-=H2;fi.z=m_id;f3[i]=fi;}GetQ4rmDot(f3,edge,Q,La);SPACE;for(i=0;i<3;i++){sprintf(st,"%d %03d %02d %9.4f %9.4f %9.4f ",k4,k0,k1,La[i].x,La[i].y,La[i].z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);guiy1(fk,r3[i]);}sprintf(st,"%d %03d %02d %9.4f %9.4f %9.4f ",k4,k0,k1,r3[0],r3[1],r3[2]);WRTX;fwrite(La,12,3,fr);}}}fclose(fq);fclose(fp);fclose(fr);
//log		FXYZ cntr;if(!(fq=fopen("DAT\\La.dat","rb")))return;fp=fopen("DAT\\log.txt","w");fr=fopen("DAT\\rot.dat","wb");fw=fopen("DAT\\d25.dat","wb");for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){SPACE;fread(f7,12,72,fq);cld2eig(f7,72,Q,A);sprintf(st,"Q[%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A: %9.3f %9.3f %9.3f",A[0],A[4],A[8]);WRTX;Q2v(Q,v);cntr.x=Q[3];cntr.y=Q[7];cntr.z=Q[11];fk=sbstrct(cntr,f7[0]);v[0]=guiy(fk);v[1]=normal(v[2],v[0]);v2Q(Q,v);sprintf(st,"Q[%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fk=guiy1(fj,r);sprintf(st,"%d %03d %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f ",k4,k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;fwrite(rot,12,2,fr);for(i=0;i<25;i++){af=pi*i*15/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(i==24)fi.x=fi.y=0;fi=mvct4a(Q,fi);sprintf(st,"%d %03d %02d %9.3f %9.3f %9.3f",k4,k0,i,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fw);}}}
//Urot&log1		if(!(fq=fopen("DAT\\rot.dat","rb")))return;fp=fopen("DAT\\log1.txt","w");fr=fopen("DAT\\Urot.dat","wb");for(k0=0;k0<360;k0++){SPACE;fseek(fq,24*k0,SEEK_SET);fread(rot,12,2,fq);r1t2Q(Q2[0],rot);sprintf(st,"Q2[0_%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);WRTX;}fseek(fq,24*(k0+360),SEEK_SET);fread(rot,12,2,fq);r1t2Q(Q2[1],rot);sprintf(st,"Q2[1_%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);WRTX;}CopyMemory(Qinv,Q2[1],64);inv4(Qinv);mmtrx4(Q2[0],Qinv,U);sprintf(st,"U[%03d:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);WRTX;}Q2r1t(U,rot);fi=rot[0];fj=rot[1];fk=guiy1(fj,r);sprintf(st,"%03d %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f ",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;fwrite(rot,12,2,fr);}
//d25fifj	if(!(fq=fopen("DAT\\Urot.dat","rb")))return;if(!(fr=fopen("DAT\\d25.dat","rb")))return;fp=fopen("DAT\\log2.txt","w");for(k0=0;k0<360;k0++){fread(rot,12,2,fq);r1t2Q(U,rot);fseek(fr,k0*12*25,SEEK_SET);fread(f,12,25,fr);fseek(fr,(k0+360)*12*25,SEEK_SET);fread(g,12,25,fr);sprintf(st,"====================");WRTX;for(i=0;i<25;i++){fi=f[i];fj=g[i];fj=mvct4a(U,fj);SPACE;sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,i,fi.x,fi.y,fi.z);WRTX;sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,i,fj.x,fj.y,fj.z);WRTX;}}
//d25see	if(!(fq=fopen("DAT\\d25.dat","rb")))return;for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(50);fseek(fq,(k0+k4*360)*12*25,SEEK_SET);fread(f,12,25,fq);for(k1=0;k1<25;k1++){fi=f[k1];fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;}dsply();Sleep(500);}}
//Lb		if(!(fq=fopen("DAT\\d25.dat","rb")))return;fp=fopen("DAT\\Lb.txt","w");fr=fopen("DAT\\Lb.dat","wb");for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){fread(f,12,25,fq);for(k1=0;k1<24;k1++){for(i=0;i<3;i++){fi=f[(k1+i*8)%24];fi=intsct(fi,S);f3[i]=fi;}GetQ4rmDot(f3,edge,Q,La);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){sprintf(st,"%d %03d %02d %9.3f %9.3f %9.3f ",k4,k0,k1,La[i].x,La[i].y,La[i].z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);guiy1(fk,r3[i]);}sprintf(st,"%d %03d %02d %9.3f %9.3f %9.3f ",k4,k0,k1,r3[0],r3[1],r3[2]);WRTX;}}}
//LaStdd	float er[25920];long cnt;FXYZ std;if(!(fq=fopen("DAT\\Urot.dat","rb")))return;if(!(fr=fopen("DAT\\La.dat","rb")))return;fp=fopen("DAT\\La_fifj.txt","w");cnt=0;for(k0=0;k0<360;k0++){fread(rot,12,2,fq);r1t2Q(U,rot);fseek(fr,12*72*k0,SEEK_SET);fread(f7,12,72,fr);fseek(fr,12*72*(k0+360),SEEK_SET);fread(g7,12,72,fr);SPACE;for(i=0;i<72;i++){fi=f7[i];fj=g7[i];fj=mvct4a(U,fj);fk=sbstrct(fi,fj);fk=guiy1(fk,r);er[cnt++]=r;	sprintf(st,"%03d %02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f |%9.6f",k0,i,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z,r);WRTX;}}std=stdd3(er,25920);SPACE;sprintf(st,"mean=%9.6f er=%9.6f",std.x,std.y);WRTX;
//LbStdd	float er[25920];long cnt;FXYZ std;if(!(fq=fopen("DAT\\Urot.dat","rb")))return;if(!(fr=fopen("DAT\\Lb.dat","rb")))return;fp=fopen("DAT\\Lb_fifj.txt","w");cnt=0;for(k0=0;k0<360;k0++){fread(rot,12,2,fq);r1t2Q(U,rot);fseek(fr,12*72*k0,SEEK_SET);fread(f7,12,72,fr);fseek(fr,12*72*(k0+360),SEEK_SET);fread(g7,12,72,fr);SPACE;for(i=0;i<72;i++){fi=f7[i];fj=g7[i];fj=mvct4a(U,fj);fk=sbstrct(fi,fj);fk=guiy1(fk,r);er[cnt++]=r;	sprintf(st,"%03d %02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f |%9.6f",k0,i,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z,r);WRTX;}}std=stdd3(er,25920);SPACE;sprintf(st,"mean=%9.6f er=%9.6f",std.x,std.y);WRTX;

	m_lst.AddString("");
	m_lst.AddString("Fininshe");
}


void gunX(FXYZ *f,FXYZ *g,int n,float *U,FXYZ *rot)
{
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());char st[80];

	float stp[6];

	int k,i,k0,k1,cnt=0;
	FXYZ rat[2],rbt[2],rct[2];
	float Ua[16],Ub[16],Uc[16],sum,min,zsm;
	CopyMemory(Uc,U,64);
	zsm=-1;sum=0;cnt=0;
	pDlg->m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);pDlg->m_lst.AddString(st);}
//	stp[0]=stp[1]=stp[2]=0.001*pi/180;
//	stp[3]=stp[4]=stp[5]=0.001;

	stp[0]=stp[1]=stp[2]=0.01*pi/180;
	stp[3]=stp[4]=stp[5]=0.01;

	while(zsm!=sum){
		zsm=sum;
		min=999999;
		for(k=0;k<729;k++){
			CopyMemory(Ua,Uc,64);
			Q2r1t(Ua,rat);
			rbt[0]=rat[0];rbt[1]=rat[1];

			rbt[0].x+=idy[k][0]*stp[0];
			rbt[0].y+=idy[k][1]*stp[1];
			rbt[0].z+=idy[k][2]*stp[2];
			rbt[1].x+=idy[k][3]*stp[3];
			rbt[1].y+=idy[k][4]*stp[4];
			rbt[1].z+=idy[k][5]*stp[5];				

			r1t2Q(Ub,rbt);
			sum= funX(f,g,n,Ub);
			if(sum<min){min=sum;k0=k;rct[0]=rbt[0];rct[1]=rbt[1];
			sprintf(st,"cnt=%05d min=%12.9f",cnt,min);pDlg->SetWindowText(st);			
			}
		}
		r1t2Q(Uc,rct);
		cnt++;
	}
	rot[0]=rct[0];rot[1]=rct[1];
	sprintf(st,"cnt=%05d min=%12.9f",cnt,min);pDlg->m_lst.AddString(st);
	pDlg->m_lst.AddString("Uc:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Uc[i*4+0],Uc[i*4+1],Uc[i*4+2],Uc[i*4+3]);pDlg->m_lst.AddString(st);}

}

float funX(FXYZ *fi,FXYZ *fj,int n,float *U)
{
	int k0;
	float sum=0,r;
	FXYZ fk;
	for(k0=0;k0<n;k0++){
		fk=mvct4a(U,fj[k0]);
		fk=sbstrct(fi[k0],fk);
		guiy1(fk,r);
		sum+=r;
	}
	sum/=n;
	return sum;
}






FXYZ pdt2vct(FXYZ in,float &r)
{
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());//char st[80];
	FXYZ o;
	float x,y,z;
	x=in.x-W4;y=in.y-H2;z=pDlg->m_id;r=sqrt(x*x+y*y+z*z);
	o.x=x/r;o.y=y/r;o.z=z/r;
	return o;
}

//		LOGFONT lfLogFont={32, 24, 0, 0, 100, 0, 0, 0, 0, 1, 2, 1, 26, "Arial"};
//		LOGFONT lfLogFont={64, 48, 0, 0, 400, 0, 0, 0, 0, 1, 2, 1, 26, "Arial"};
//		SETTEXTMODE	textmode={0x000000,0xffffff,0,0,0};//°×é?
//		SETTEXTMODE	textmode={0x000000,0x0000ff,0,0,0};//oìé?
//		RECT rect0={30,20,0,0};
//		rect0.top=100;
//		sprintf(st,"|¤b=%7.2f mm",ep.b-RADIUS);
//		okSetTextTo(hBrd,TARGET(&blk),&rect0,&lfLogFont,&textmode,st,20);


void CTmdDlg::OnButton2() 
{
	char st[256];
	int i,j,i0,j0,i1,k0,k1,k2,k3,k4,wk,kcnt;
	long k,l,n;
	float x,y,z,r,af,Q[16],A[9],Q0[16],R[12],edge,r3[3],min,max,mean,Qinv[16],U[16],Ua[16],Q2[2][16],A2[2][9];
	FXYZ fi,fj,fk,fl,f[25],g[25],g2[2][25],f3[3],f4[4],f7[73],La[4],La2[2][360],v[4],rot[2],rst[2]={0};
	FILE *fp,*fq,*fr,*fw;
	FILE *fs[2];
	edge=2*RADIUS*cos(pi/6);
	FXYZ fb[2]={0};FABCD S;S.A=S.B=0;S.C=-1;S.D=m_id;
//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\dv%d.dat",k4);if(!(fp=fopen(st,"rb")))return;for(k0=0;k0<40;k0++){sprintf(st,"JPG\\%d\\%02d.jpg",k4,k0);SetWindowText(st);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(100);fread(f,12,25,fp);for(k1=0;k1<25;k1++){i0=f[k1].y;j0=f[k1].x;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(20);}dsply();Sleep(100);}}
//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\dv.dat",k4);if(!(fr=fopen(st,"rb")))return;sprintf(st,"DAT\\%d\\La.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);fq=fopen(st,"wb");sprintf(st,"DAT\\%d\\Q.dat",k4);fw=fopen(st,"wb");for(k0=0;k0<360;k0++){fread(f,12,25,fr);for(k2=0;k2<24;k2++){for(k1=0;k1<3;k1++){f3[k1]=f[(k1*8+k2)%24];f3[k1].x-=W2;f3[k1].y-=H2;f3[k1].z=m_id;}GetQ4rmDot(f3,edge,Q,La);sprintf(st,"DAT\\%d\\Q.dat",k4);fwrite(Q,64,1,fw);SPACE;for(k1=0;k1<3;k1++){fi=La[k1];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k2,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}fclose(fr);fclose(fp);fclose(fq);}
//	FXYZ *fx=(FXYZ *)new FXYZ[1080];int ix[3]={0,2,1};for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\Q&A.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(fx,12,1080,fq);cld2eig(fx,1080,Q,A);sprintf(st,"Q[%d]:",k4);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A[%d]:  %12.3f %12.3f %9.3f",k4,A[0],A[4],A[8]);WRTX;Q2rot(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);WRTX;fclose(fp);sprintf(st,"DAT\\%d\\Q.dat",k4);fq=fopen(st,"wb");fwrite(Q,64,1,fq);fclose(fq);CopyMemory(Q2[k4],Q,64);m_lst.AddString("============================");for(k0=0;k0<45;k0++){k1=k0*8*3+ix[k0%3];fb[k4].x+=fx[k1].x;fb[k4].y+=fx[k1].y;fb[k4].z+=fx[k1].z;}fb[k4].x/=45;fb[k4].y/=45;fb[k4].z/=45;sprintf(st,"fb %9.3f %9.3f %9.3f",fb[k4].x,fb[k4].y,fb[k4].z);m_lst.AddString(st);}fq=fopen("DAT\\fb.dat","wb");fwrite(fb,12,2,fq);fclose(fq);delete [] fx;for(k4=0;k4<2;k4++){v[2].x=Q2[k4][2];v[2].y=Q2[k4][6];v[2].z=Q2[k4][10];fi.x=fb[k4].x-Q2[k4][3];fi.y=fb[k4].y-Q2[k4][7];fi.z=fb[k4].z-Q2[k4][11];v[0]=guiy(fi);v[1]=normal(v[2],v[0]);Q2[k4][0]=v[0].x;Q2[k4][1]=v[1].x;Q2[k4][2]=v[2].x;Q2[k4][4]=v[0].y;Q2[k4][5]=v[1].y;Q2[k4][6]=v[2].y;Q2[k4][8]=v[0].z;Q2[k4][9]=v[1].z;Q2[k4][10]=v[2].z;m_lst.AddString("Q2[k4]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);m_lst.AddString(st);}}fq=fopen("DAT\\Q2.dat","wb");fwrite(Q2,64,2,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\00.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(100);}dsply();Sleep(1000);}	
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){m_lst.AddString("=============================");for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);f[k0]=fi;sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);}sprintf(st,"DAT\\%d\\d25.dat",k4);fq=fopen(st,"wb");fwrite(f,12,25,fq);fclose(fq);}
//	float la,lb,lc;if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);m_lst.AddString("");m_lst.AddString("Q2[0]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);m_lst.AddString(st);}m_lst.AddString("Q2[1]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);m_lst.AddString(st);}m_lst.AddString("");inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],U);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2rot(U,rot);r=x=y=0;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);fi=guiy1(fi,lc);fi.x=acos(fi.x);fi.y=acos(fi.y);sprintf(st,"%02d %12.6f %12.6f %12.6f %9.3f",k0,fi.x*180/pi,fi.y*180/pi,fi.z,lc);m_lst.AddString(st);r+=lc;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",x*180/pi,y*180/pi,r);m_lst.AddString(st);rot[1].x=x;rot[1].y=y;rot[1].z=r;rot2Q(U,rot);m_lst.AddString("");fi=rot[0];fj=rot[1];sprintf(st,"rot %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);m_lst.AddString(st);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\U.dat","wb");fwrite(U,64,1,fq);fclose(fq);rot[0].x=rot[0].y=rot[0].z=0;rot2Q(U,rot);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\Ua.dat","wb");fwrite(U,64,1,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\28.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);sprintf(st,"DAT\\%d\\d25.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(f,12,25,fq);fclose(fq);m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j0+j]^=255;dsply();Sleep(200);}}
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);}r=x=y=0;m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);sprintf(st,"dif %02d %12.6f %12.6f %12.6f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);r+=fi.z;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;fi.x=x;;fi.y=y;fi.z=r;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",fi.x,fi.y,fi.z);m_lst.AddString(st);fq=fopen("DAT\\t.dat","wb");fwrite(&fi,12,1,fq);fclose(fq);
//	float bt,gm,lc,la,lb;if(!(fp=fopen("DAT\\Ua.dat","rb")))return;fread(U,64,1,fp);fclose(fp);if(!(fq=fopen("DAT\\dv0.dat","rb")))return;if(!(fr=fopen("DAT\\dv1.dat","rb")))return;fp=fopen("DAT\\fifj.txt","w");fw=fopen("DAT\\fifj.dat","wb");fk.x=U[3],fk.y=U[7],fk.z=U[11],fk=guiy1(fk,lc);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fi.y-=H2;fi.z=m_id;fj.x-=W2;fj.y-=H2;fj.z=m_id;fi=guiy(fi);v[0]=fi;fj=mvct4a(U,fj);fj=guiy(fj);v[1]=fj;gm=angle(fi,fj);af=angle(fi,fk);bt=pi-af-gm;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],la);fwrite(&fi,12,1,fw);fwrite(&fj,12,1,fw);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);	fclose(fq);fclose(fr);fclose(fw);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fq=fopen("DAT\\d25x2.dat","wb");fp=fopen("DAT\\d25x2.txt","w");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);	WRTX;fwrite(&fi,12,1,fq);fwrite(&fj,12,1,fq);}fclose(fq);fclose(fp);
//	if(!(fq=fopen("DAT\\fifj.dat","rb")))return;fp=fopen("DAT\\sbstrctFifj.txt","w");for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fq);fk=sbstrct(fi,fj);r=sqrt(fk.x*fk.x+fk.y*fk.y+fk.z*fk.z);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f",k0,fk.x,fk.y,fk.z,r);WRTX;}fclose(fq);fclose(fp);
//	fq=fopen("DAT\\idy.dat","wb");fp=fopen("DAT\\idy.txt","w");for(k=0;k<729;k++){idy[k][5]=(k/243);idy[k][4]=(k%243)/81;idy[k][3]=(k%81)/27;idy[k][2]=(k%27)/9;idy[k][1]=(k%9)/3;idy[k][0]=(k%3);}for(k=0;k<729;k++)for(i=0;i<6;i++)idy[k][i]-=1;for(k=0;k<729;k++){sprintf(st,"%03d |  %3d %3d %3d %3d %3d %3d " ,k,idy[k][5],idy[k][4],idy[k][3],idy[k][2],idy[k][1],idy[k][0]);m_lst.AddString(st);WRTX;}fwrite(idy,2,729*6,fq);fclose(fq);fclose(fp);
//Qa	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ua.dat","rb")))return;fread(Ua,64,1,fq);fclose(fq);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);}
//Q2 	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],Ua);gunX(f,g,25,Ua,rot);r1t2Q(Ua,rot);fq=fopen("DAT\\Ub.dat","wb");fp=fopen("DAT\\Ub.txt","w");sprintf(st,"Ub:");WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);WRTX;}fwrite(Ua,64,1,fq);fclose(fq);fclose(fp);
//crtfy	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2r1t(U,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z);m_lst.AddString(st);m_lst.AddString("");m_lst.AddString("===========================");for(k0=0;k0<25;k0++){m_lst.AddString("");fi=f[k0];fj=g[k0];fj=mvct4a(U,fj);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fj.x,fj.y,fj.z);m_lst.AddString(st);}
//crtfu		if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}if(!(fq=fopen("DAT\\0\\La.dat","rb")))return;if(!(fr=fopen("DAT\\1\\La.dat","rb")))return;fp=fopen("DAT\\crtfu.txt","w");sprintf(st,"=========================");WRTX;for(k0=0;k0<1080;k0++){SPACE;fread(&fi,12,1,fq);fread(&fj,12,1,fr);;sprintf(st,"%04d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;fj=mvct4a(U,fj);sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,fj.x,fj.y,fj.z);WRTX;}fclose(fq);fclose(fr);fclose(fp);
//crtfv	FXYZ t;float bt,gm,la,lb,lc;fp=fopen("DAT\\crtfv.txt","w");if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);sprintf(st,"U:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}CopyMemory(Qinv,U,64);inv4(Qinv);sprintf(st,"Qinv:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Qinv[i*4+0],Qinv[i*4+1],Qinv[i*4+2],Qinv[i*4+3]);m_lst.AddString(st);WRTX;}
//		if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fr=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fr);fclose(fr);t.x=U[3];t.y=U[7];t.z=U[11];v[2]=guiy1(t,lc);sprintf(st,"labc+abg+SINE cert=========================");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f %9.4f",k0,la,lb,lc,af*180/pi,bt*180/pi,gm*180/pi,(af+bt+gm)*180/pi);m_lst.AddString(st);WRTX;}sprintf(st,"SINE cert");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %12.6f %12.6f %12.6f |",k0,la/sin(af),lb/sin(bt),lc/sin(gm));m_lst.AddString(st);WRTX;}sprintf(st,"9000 afbtgm=========================");m_lst.AddString(st);WRTX;if(!(fq=fopen("DAT\\0\\dv.dat","rb")))return;;if(!(fr=fopen("DAT\\1\\dv.dat","rb")))return;;for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,af*180/pi,bt*180/pi,gm*180/pi);WRTX;}sprintf(st,"=========================");m_lst.AddString(st);WRTX;fseek(fq,0,SEEK_SET);fseek(fr,0,SEEK_SET);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],lb);sprintf(st,"%04d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);fclose(fq);fclose(fr);
//==================================================================================	
//JPH	BLOCKINFO blk;FillMemory(&blk,sizeof(blk),0);blk.iBitCount=8;blk.iFormType=FORM_GRAY8;blk.iHeight=HEIGHT;blk.iWidth=WIDTH;blk.lpBits = b0;for(k0=0;k0<9;k0++){for(k1=0;k1<40;k1++){sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k1);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)okGetBufferAddr(hBrd,0);b0=(BYTE *)pnt;b1=b0+HW*2;for(k=0;k<HW*2;k++)b0[k]=pnt[k].r;CopyMemory(b1,b0,HW*2);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+j];}sprintf(st,"JPH\\0\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+WIDTH+j];}sprintf(st,"JPH\\1\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);}}
//dt	float sum,isum,jsum;fp=fopen("DBT\\dt.txt","w");fq=fopen("DBT\\dt.dat","wb");int *bg5=(int *)new int[HW];for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,1,1);im2bw(b1,b0,HEIGHT,WIDTH);for(i=0;i<10;i++)for(j=0;j<WIDTH;j++)b0[i*WIDTH+j]=0;FillMemory(bg5,sizeof(int)*HW,0);for(n=0;n<HW;n++)if(b0[n]>0)bg5[n]=1;wk=BWLabel(bg5,HEIGHT,WIDTH);sprintf(st,"wk=%d",wk);WRTX;	kcnt=0;for(k1=1;k1<=wk;k1++){sum=isum=jsum=0;for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){if(bg5[i*WIDTH+j]==k1){sum++;isum+=i;jsum+=j;}}if(sum>10){fi.x=jsum/sum;fi.y=isum/sum;fi.z=sum;sprintf(st,"%d %02d %03d %9.3f %9.3f %7.0f",k4,kcnt++,k0,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}}delete[]bg5;fclose(fp);fclose(fq);
//dv	if(!(fq=fopen("DBT\\dt.dat","rb")))return;fp=fopen("DBT\\dv.txt","w");fr=fopen("DBT\\dv.dat","wb");for(k0=0;k0<720;k0++){SPACE;fread(f,12,25,fq);for(k=0;k<25;k++){f[k].x-=W2;f[k].y-=H2;}Sort7z(f,25,1);for(k=0;k<24;k++){x=f[k].x-f[24].x;y=f[k].y-f[24].y;r=sqrt(x*x+y*y);af=asin(y/r);if(x<0)af=pi-af;if(af<0)af+=2*pi;f[k].z=af;}for(k=23;k>=0;k--){f[k].z-=f[0].z;if(f[k].z<0)f[k].z+=2*pi;}Sort7z(f,24,0);	for(k=0;k<25;k++){fi=f[k];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k,fi.x,fi.y,fi.z);WRTX;}fwrite(f,12,25,fr);}fclose(fq);fclose(fp);fclose(fr);
//La	if(!(fq=fopen("DBT\\dv.dat","rb")))return;fp=fopen("DBT\\La.txt","w");fr=fopen("DBT\\La.dat","wb");fw=fopen("DBT\\Q.dat","wb");for(k0=0;k0<720;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k=0;k<24;k++){for(i=0;i<3;i++){f3[i]=f[(k+8*i)%24];f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);fwrite(Q,64,1,fw);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){fi=La[i];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);fk=guiy1(fk,r3[i]);}sprintf(st,"r: %9.4f %9.4f %9.4f ",r3[0],r3[1],r3[2]);WRTX;}			}fclose(fq);fclose(fw);fclose(fr);fclose(fp);	
//rot	if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\Q.txt","w");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);sprintf(st,"Q[%03d]:",k0);WRTX;for(i1=0;i1<4;i1++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i1*4+0],Q[i1*4+1],Q[i1*4+2],Q[i1*4+3]);WRTX;}}}fclose(fq);fclose(fp);if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\rot.txt","w");fr=fopen("DBT\\rot.dat","wb");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fr);r=sqrt(fj.x*fj.x+fj.y*fj.y+fj.z*fj.z);sprintf(st,"rot[%03d]: %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}}fclose(fq);fclose(fp);fclose(fr);	
//cld	if(!(fq=fopen("DBT\\La.dat","rb")))return;fp=fopen("DBT\\cld.txt","w");FXYZ *L7=(FXYZ *)new FXYZ[72];fr=fopen("DBT\\Q7.dat","wb");fw=fopen("DBT\\rt7.dat","wb");for(k0=0;k0<720;k0++){fread(L7,12,72,fq);SPACE;cld2eig(L7,72,Q,A);sprintf(st,"Q[%03d]:",k0);WRTX;fwrite(Q,64,1,fr);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A: %9.3f %9.3f %9.6f",A[0],A[4],A[8]);WRTX;Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fw);fk=guiy1(fj,r);sprintf(st,"%9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}fclose(fp);fclose(fr);fclose(fw);fclose(fq);delete[]L7;
//aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa
//d40	FXYZ *d40=(FXYZ *)new FXYZ[1000];if(!(fq=fopen("DBT\\dv.dat","rb")))return;fr=fopen("DBT\\d40.dat","wb");fseek(fq,400*25*12,SEEK_SET);fread(d40,12,1000,fq);fwrite(d40,12,1000,fr);fseek(fq,40*25*12,SEEK_SET);fread(d40,12,1000,fq);fwrite(d40,12,1000,fr);fclose(fr);fclose(fq);delete[]d40;


	FXYZ fst[2]={0};
	if(!(fq=fopen("DBT\\d40.dat","rb")))return;
	fp=fopen("DBT\\L40.txt","w");
	fr=fopen("DBT\\L40.dat","wb");
	int cnt=0;
	for(k0=0;k0<80;k0++){
		fread(f,12,25,fq);
		for(k1=0;k1<25;k1++)f[k1].z=m_id;
		for(k1=0;k1<24;k1++){
			for(i=0;i<3;i++)f3[i]=f[(i*8+k1)%24];
			GetQ4rmDot(f3,edge,Q,La);fwrite(La,12,3,fr);
			for(i=0;i<3;i++){sprintf(st,"%04d %02d %d %9.3f %9.3f %9.3f ",cnt++,k0,i,La[i].x,La[i].y,La[i].z);WRTX;
			}
			if(k1%8==0){
				i=k1/8;
//				if(i==0)m_lst.AddString("");				
				if(i==0)j=0;else if(i==1)j=2;else j=1;
				sprintf(st,"%02d %d %12.6f %12.6f %12.6f ",k0,i,La[j].x,La[j].y,La[j].z);//WRTX;
				fst[k0/40]=addttn(fst[k0/40],La[j]);
				}
			}
	}
	for(k0=0;k0<2;k0++){
		fst[k0].x/=120;fst[k0].y/=120;	fst[k0].z/=120;
		fi=fst[k0];
//		sprintf(st,"fst: %9.3f %9.3f %9.3f ",fi.x,fi.y,fi.z);m_lst.AddString(st);
	}
	fclose(fq);fclose(fp);fclose(fr);
	fq=fopen("DBT\\fst.dat","wb");fwrite(fst,12,2,fq);fclose(fq);

	FXYZ *f28=(FXYZ *)new FXYZ[2880];
	if(!(fq=fopen("DBT\\L40.dat","rb")))return;
	fp=fopen("DBT\\log2.txt","w");
	for(k0=0;k0<2;k0++){
		fread(f28,12,2880,fq);
		cld2eig(f28,2880,Q2[k0],A);
		sprintf(st,"Q2[k0]:");m_lst.AddString(st);WRTX;
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k0][i*4+0],Q2[k0][i*4+1],Q2[k0][i*4+2],Q2[k0][i*4+3]);m_lst.AddString(st);WRTX;}
		sprintf(st,"A: %12.3f %12.3f %9.3f",A[0],A[4],A[8]);m_lst.AddString(st);WRTX;
	}
	fclose(fq);
	delete[]f28;
	FXYZ v0[2];
	for(k0=0;k0<2;k0++){
		fi.x=Q2[k0][3];fi.y=Q2[k0][7];fi.z=Q2[k0][11];
		fj=fst[k0];fk=sbstrct(fi,fj);v0[k0]=guiy(fk);
		Q2v(Q2[k0],v);v[0]=v0[k0];
		v[1]=normal(v[2],v[0]);
		v2Q(Q2[k0],v);
		sprintf(st,"Q2[k0]:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k0][i*4+0],Q2[k0][i*4+1],Q2[k0][i*4+2],Q2[k0][i*4+3]);m_lst.AddString(st);WRTX;}
	}
	CopyMemory(Qinv,Q2[1],64);inv4(Qinv);
	mmtrx4(Q2[0],Qinv,U);
	sprintf(st,"U:");m_lst.AddString(st);WRTX;
	for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}


	fclose(fp);



	m_lst.AddString("");
	m_lst.AddString("Finished!");
}

/*	char st[80];
	int i,k0,k1;
	float *f=(float *)new float[HW*2];
	BYTE *b=(BYTE *)new BYTE[HW*2];
	BLOCKINFO blk;
	FillMemory(&blk,sizeof(blk),0);
	blk.iBitCount=8;
	blk.iFormType=FORM_GRAY8;
	blk.iHeight=HEIGHT;
	blk.iWidth=WIDTH*2;
	blk.lpBits = b;

	for(k0=0;k0<9;k0++)
	{
		FillMemory(f,HW*2*sizeof(float),0);
		for(k1=0;k1<40;k1++){
			sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k1);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
			pnt=(PNT *)okGetBufferAddr(hBrd,0);
			for(k=0;k<HW*2;k++)f[k]+=(114*pnt[k].b+587*pnt[k].g+299*pnt[k].r);
		}
		for(k=0;k<HW*2;k++)b[k]=f[k]/40000;
		for(k=0;k<HW*2;k++)if(b[k]>THRD)b[k]=255;else b[k]=0;
		for(i=0;i<HEIGHT;i++){
			for(j=0;j<WIDTH;j++){
				l=i*WIDTH*2+WIDTH*2-1-j;
				by=b[i*WIDTH*2+j];b[i*WIDTH*2+j]=b[l];b[l]=by;
			}
		}
		sprintf(st,"JPG\\%02d.jpg",k0);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);
		SetWindowText(st);
	}
	m_lst.AddString("here");return;*/
//=======================================================
/*	for(k0=0;k0<9;k0++)
	{
		for(k2=0;k2<4;k2++){
			FillMemory(f,HW*2*sizeof(float),0);
			for(k1=0;k1<10;k1++){
				sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k2*10+k1);m_lst.AddString(st);
				l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
				pnt=(PNT *)okGetBufferAddr(hBrd,0);
				for(k=0;k<HW*2;k++)f[k]+=(114*pnt[k].b+587*pnt[k].g+299*pnt[k].r);
			}
			for(k=0;k<HW*2;k++)b[k]=f[k]/10000;
			for(k=0;k<HW*2;k++)if(b[k]>THRD)b[k]=255;else b[k]=0;
			for(i=0;i<HEIGHT;i++){
				for(j=0;j<WIDTH;j++){
					l=i*WIDTH*2+WIDTH*2-1-j;
					by=b[i*WIDTH*2+j];b[i*WIDTH*2+j]=b[l];b[l]=by;
				}
			}
			sprintf(st,"JPG\\a%02d_%d.jpg",k0,k2);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);
			SetWindowText(st);
		}
	}
	m_lst.AddString("here");return;*/
//=======================================================

/*	b1=b0+HW*2;
	for(k0=0;k0<9;k0++)
	{
		for(k2=0;k2<4;k2++){
			k1=k2+1;k1%=4;
			sprintf(st,"JPG\\a%02d_%d.jpg",k0,k2);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
			sprintf(st,"JPG\\a%02d_%d.jpg",k0,k1);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,1,1);
			for(k=0;k<HW*2;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			for(k=0;k<HW*2;k++)if(b1[k]>THRD)b1[k]=255;else b1[k]=0;
			l=0;for(k=0;k<HW*2;k++)if(b0[k]>0)l++;
			n=0;for(k=0;k<HW*2;k++)if(b0[k]!=b1[k])n++;
			sprintf(st,"%d %d %d %d %d",k0,k2,k1,n,l);m_lst.AddString(st);
		}
	}

	for(k0=0;k0<9;k0++)
	{
		for(k1=0;k1<40;k1++){
			sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k1);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
			pnt=(PNT *)okGetBufferAddr(hBrd,0);
			for(k=0;k<HW*2;k++)f[k]=(114*pnt[k].b+587*pnt[k].g+299*pnt[k].r);
			for(k=0;k<HW*2;k++)b[k]=f[k]/1000;
			for(k=0;k<HW*2;k++)if(b[k]>THRD)b[k]=255;else b[k]=0;
			for(i=0;i<HEIGHT;i++){
				for(j=0;j<WIDTH;j++){
					l=i*WIDTH*2+WIDTH*2-1-j;
					by=b[i*WIDTH*2+j];b[i*WIDTH*2+j]=b[l];b[l]=by;
				}
			}
			sprintf(st,"JPG\\b%d_%02d.jpg",k0,k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);
			SetWindowText(st);
		}
	}
	b1=b0+HW*2;
	for(k0=0;k0<9;k0++)
	{
		for(k2=0;k2<40;k2++){
			k1=k2+1;k1%=40;
			sprintf(st,"JPG\\b%d_%02d.jpg",k0,k2);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
//			sprintf(st,"l=%d",l);m_lst.AddString(st);
			sprintf(st,"JPG\\b%d_%02d.jpg",k0,k1);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,1,1);
//			sprintf(st,"l=%d",l);m_lst.AddString(st);
			for(k=0;k<HW*2;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			for(k=0;k<HW*2;k++)if(b1[k]>THRD)b1[k]=255;else b1[k]=0;
			n=0;
			for(k=0;k<HW*2;k++)if(b0[k]!=b1[k])n++;
			sprintf(st,"%d %02d %02d %d",k0,k2,k1,n);m_lst.AddString(st);
		}
	}
		m_lst.AddString("here");return;*/
//=======================================================
/*	blk.iWidth=WIDTH;
	for(k0=0;k0<9;k0++)
	{
		for(k2=0;k2<4;k2++){
			sprintf(st,"JPG\\a%02d_%d.jpg",k0,k2);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);
			for(k=0;k<HW*2;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++)b[i*WIDTH+j]=b0[i*WIDTH*2+j];
			sprintf(st,"JPG\\c%d_%d.jpg",k0,k2*2);m_lst.AddString(st);
			okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);
			for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++)b[i*WIDTH+j]=b0[i*WIDTH*2+WIDTH+j];
			sprintf(st,"JPG\\c%d_%d.jpg",k0,k2*2+1);m_lst.AddString(st);
			okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);
		}
	}	
	m_lst.AddString("here");return;*/
//==========================================
/*
delete [] b;
int *bg5=(int *)new int[HW];
FXYZ fi,fj;
FILE *fq;
	FXYZ dv[18][25],f3[3],La[3],ro,v0,v1,r9[9];
	float Q[16],Q0[18][16],R[12],edge,Qinv[16],U[9][16];


	fq=fopen("DAT\\dt.dat","wb");
	for(k0=0;k0<9;k0++)
//	for(k0=0;k0<1;k0++)
	{
		for(k2=0;k2<8;k2++)
//		for(k2=0;k2<1;k2++)
		{
			sprintf(st,"JPG\\c%d_%d.jpg",k0,k2);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);//sprintf(st,"l=%d",l);m_lst.AddString(st);
			for(k=0;k<HW;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			dsply();

			FillMemory(bg5,sizeof(int)*HW,0);
			for(n=0;n<HW;n++)if(b0[n]>0)bg5[n]=1;

			wk=BWLabel(bg5,HEIGHT,WIDTH);
			sprintf(st,"wk=%d",wk);m_lst.AddString(st);

			kcnt=0;
			for(k1=1;k1<=wk;k1++){		
				sum=isum=jsum=0;
				for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){
					if(bg5[i*WIDTH+j]==k1)
					{
						sum++;
						isum+=i;jsum+=j;
					}
				}

				if(sum>10){
					fi.x=jsum/sum;
					fi.y=isum/sum;
					fi.z=sum;
					fwrite(&fi,12,1,fq);

					sprintf(st,"%02d %9.3f %9.3f %9.3f",k1,fi.x,fi.y,fi.z);m_lst.AddString(st);
					i0=fi.y;j0=fi.x;
					for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i+i0)*WIDTH+j+j0]^=255;

				}
			}
			dsply();Sleep(500);
		}
	}
	fclose(fq);
	delete [] bg5;
	m_lst.AddString("here");return;*/
//==================================================
/*	blk.iWidth=WIDTH;
	for(k0=0;k0<9;k0++)
	{
			sprintf(st,"JPG\\%02d.jpg",k0);//m_lst.AddString(st);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);//sprintf(st,"l=%d",l);m_lst.AddString(st);
			b0=(BYTE *)okGetBufferAddr(hBrd,0);

			for(k=0;k<HW*2;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++)b[i*WIDTH+j]=b0[i*WIDTH*2+j];
			sprintf(st,"JPG\\d%02d.jpg",k0*2);m_lst.AddString(st);
			okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);

			for(k=0;k<HW*2;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++)b[i*WIDTH+j]=b0[i*WIDTH*2+WIDTH+j];
			sprintf(st,"JPG\\d%02d.jpg",k0*2+1);m_lst.AddString(st);
			okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);		
	}	
	m_lst.AddString("here");return;*/


/*	fq=fopen("DAT\\du.dat","wb");
	for(k0=0;k0<18;k0++)
	{
			sprintf(st,"JPG\\d%02d.jpg",k0);
			l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);//sprintf(st,"l=%d",l);m_lst.AddString(st);
			for(k=0;k<HW;k++)if(b0[k]>THRD)b0[k]=255;else b0[k]=0;
			dsply();
			FillMemory(bg5,sizeof(int)*HW,0);
			for(n=0;n<HW;n++)if(b0[n]>0)bg5[n]=1;
			wk=BWLabel(bg5,HEIGHT,WIDTH);
			sprintf(st,"wk=%d",wk);m_lst.AddString(st);
			kcnt=0;
			for(k1=1;k1<=wk;k1++){		
				sum=isum=jsum=0;
				for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){
					if(bg5[i*WIDTH+j]==k1)
					{
						sum++;
						isum+=i;jsum+=j;
					}
				}
				if(sum>10){
					fi.x=jsum/sum;
					fi.y=isum/sum;
					fi.z=sum;
					fwrite(&fi,12,1,fq);
					sprintf(st,"%02d %9.3f %9.3f %9.3f",k1,fi.x,fi.y,fi.z);m_lst.AddString(st);
					i0=fi.y;j0=fi.x;
					for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
				}
			}
			dsply();Sleep(1000);
	}
	fclose(fq);
	delete [] bg5;
	m_lst.AddString("here");return;*/



/*	if(!(fq=fopen("DAT\\du.dat","rb")))return;
	fread(du,12,18*25,fq);fclose(fq);
	for(k0=0;k0<18;k0++){
		sprintf(st,"JPG\\d%02d.jpg",k0);
		okLoadImageFile(hBrd,st,0,BUFFER,0,1);
//		for(k1=0;k1<25;k1++){i0=du[k0][k1].y;j0=du[k0][k1].x;for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(40);}
		Sort7z(du[k0],25,1);fj=fi=du[k0][24];
		for(k1=0;k1<24;k1++){fi=du[k0][k1];
			x=fi.x-fj.x;y=fi.y-fj.y;r=sqrt(x*x+y*y);
			af=asin(y/r);if(x<0)af=pi-af;
			if(af<0)af+=2*pi;
			du[k0][k1].z=af;
		}
		for(k1=23;k1>=0;k1--){
			du[k0][k1].z-=du[k0][0].z;
			if(du[k0][k1].z<0)du[k0][k1].z+=2*pi;
		}
		Sort7z(du[k0],25,0);
		m_lst.AddString("");for(k1=0;k1<25;k1++){fi=du[k0][k1];sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z*180/pi);m_lst.AddString(st);}
		fq=fopen("DAT\\dv.dat","wb");fwrite(du,12,18*25,fq);fclose(fq);
//		dsply();Sleep(500);
	}
	m_lst.AddString("Fininshe");
	m_lst.AddString("here");return;	*/

//==============================================

/*	edge=2*RADIUS*cos(pi/6);
	m_id=1520;//1427.5;
//	m_lst.AddString("");sprintf(st,"m_id=%9.3f",m_id);m_lst.AddString(st);m_lst.AddString("");
	if(!(fq=fopen("DAT\\dv.dat","rb")))return;
	fread(dv,12,18*25,fq);fclose(fq);
	fp=fopen("DAT\\midDat.txt","w");
	for(k0=0;k0<18;k0++){
		m_lst.AddString("");SPACE;
		for(k1=0;k1<25;k1++){fi=dv[k0][k1];sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z*180/pi);
		m_lst.AddString(st);WRTX;}
//		sprintf(st,"JPG\\d%02d.jpg",k0);okLoadImageFile(hBrd,st,0,BUFFER,0,1);for(k1=0;k1<25;k1++){fi=dv[k0][k1];i0=fi.y;j0=fi.x;for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i+i0)*WIDTH+j+j0]^=255;dsply();Sleep(50);}dsply();Sleep(500);
	}
	for(k0=0;k0<18;k0++){for(k=0;k<25;k++){fi=dv[k0][k];fi.x-=W2;fi.y-=H2;fi.z=m_id;dv[k0][k]=fi;}}
	m_lst.AddString("");SPACE;
	for(k0=0;k0<18;k0++){
		for(k=0;k<24;k++){
			for(k1=0;k1<3;k1++){
				f3[k1]=dv[k0][(k+k1*8)%24];
			}
			GetQ4rmDot(f3,edge,Q,La);
			for(k1=0;k1<3;k1++)dt[k*3+k1]=La[k1];
		}
		cld2eig(dt,72,Q0[k0],A[k0]);
//		m_lst.AddString("Q0[k0]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q0[k0][i*4+0],Q0[k0][i*4+1],Q0[k0][i*4+2],Q0[k0][i*4+3]);m_lst.AddString(st);}
//		sprintf(st,"A[k0]:  %9.3f %9.3f %9.3f",A[k0][0],A[k0][4],A[k0][8]);m_lst.AddString(st);
		GetR3(R,Q0[k0]);
		ro=R_Oex(R);
//		sprintf(st,"ro: %9.4f %9.4f %9.4f",ro.x*180/pi,ro.y*180/pi,ro.z*180/pi);m_lst.AddString(st);
		t.x=Q0[k0][3];t.y=Q0[k0][7];t.z=Q0[k0][11];
		
		sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f",k0,ro.x*180/pi,ro.y*180/pi,ro.z*180/pi,t.x,t.y,t.z);
		m_lst.AddString(st);WRTX;
		rot[k0][0]=ro;rot[k0][1]=t;
	}
	m_lst.AddString("");SPACE;
	m_lst.AddString("");SPACE;
	;sprintf(st,"m_id=%9.3f",m_id);m_lst.AddString(st);WRTX;
	m_lst.AddString("");SPACE;
	for(k0=0;k0<8;k0++){
		x=rot[(k0+1)*2][1].x-rot[(k0)*2][1].x;
		y=rot[(k0+1)*2][1].y-rot[(k0)*2][1].y;
		z=rot[(k0+1)*2][1].z-rot[(k0)*2][1].z;r=sqrt(x*x+y*y+z*z);
		sprintf(st,"k0=%d %9.3f",k0,r);m_lst.AddString(st);WRTX;
	}
	
	m_lst.AddString("");SPACE;
	for(k0=0;k0<8;k0++){
		x=rot[(k0+1)*2+1][1].x-rot[(k0)*2+1][1].x;
		y=rot[(k0+1)*2+1][1].y-rot[(k0)*2+1][1].y;
		z=rot[(k0+1)*2+1][1].z-rot[(k0)*2+1][1].z;r=sqrt(x*x+y*y+z*z);
		sprintf(st,"k0=%d %9.3f",k0,r);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;
	for(k0=0;k0<18;k0++){
		sprintf(st,"Q0[%02d]:",k0);m_lst.AddString(st);WRTX;
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q0[k0][i*4+0],Q0[k0][i*4+1],Q0[k0][i*4+2],Q0[k0][i*4+3]);
		m_lst.AddString(st);WRTX;}
	}

	m_lst.AddString("");SPACE;
	for(k0=0;k0<18;k0++){
		sprintf(st,"A[%02d]:  %12.3f %12.3f %9.3f",k0,A[k0][0],A[k0][4],A[k0][8]);m_lst.AddString(st);WRTX;
	}

	m_lst.AddString("");SPACE;
	for(k0=0;k0<9;k0++){
		GetR3(R,Q0[k0*2]);
		ro=R_Oex(R);
		GetR3(R,Q0[k0*2+1]);
		rp=R_Oex(R);	
		sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f",k0*2,ro.x*180/pi,ro.y*180/pi,ro.z*180/pi,rp.x*180/pi,rp.y*180/pi,rp.z*180/pi);	
		m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;
	m_lst.AddString("");SPACE;
		sprintf(st,"z axis's Angles of Two cameras(deg.):");m_lst.AddString(st);WRTX;
	for(k0=0;k0<9;k0++){
		v0.x=Q0[k0*2][2];v0.y=Q0[k0*2][6];v0.z=Q0[k0*2][10];
		v1.x=Q0[k0*2+1][2];v1.y=Q0[k0*2+1][6];v1.z=Q0[k0*2+1][10];
		r=v0.x*v1.x+v0.y*v1.y+v0.z*v1.z;
		af=acos(r);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;
	m_lst.AddString("");SPACE;
	m_lst.AddString("=======================");
//	for(k0=0;k0<18;k0++){GetR3(R,Q0[k0]);ro=R_Oex(R);o_Rex(ro,R);GetR4(R,Q);sprintf(st,"Q0[%02d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);m_lst.AddString(st);}}
	fq=fopen("DAT\\Q0,dat","wb");fwrite(Q0,64,18,fq);fclose(fq);
//	m_lst.AddString("here");return;		

//	fq=fopen("DAT\\Q0,dat","rb");fread(Q0,64,18,fq);fclose(fq);	
//	for(k0=0;k0<18;k0++){sprintf(st,"Q0[%02d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q0[k0][i*4+0],Q0[k0][i*4+1],Q0[k0][i*4+2],Q0[k0][i*4+3]);m_lst.AddString(st);}}	
//	for(k0=0;k0<9;k0++){CopyMemory(Qinv,Q0[k0*2+1],64);inv4(Qinv);mmtrx4(Q0[k0*2],Qinv,U[k0]);sprintf(st,"U[%d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[k0][i*4+0],U[k0][i*4+1],U[k0][i*4+2],U[k0][i*4+3]);m_lst.AddString(st);}	}
	sprintf(st,"================");m_lst.AddString(st);WRTX;
	sprintf(st,"front-back comapars of z,y,x axises:");m_lst.AddString(st);WRTX;
	sprintf(st,"left camera:");m_lst.AddString(st);WRTX;
	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2][2];  v0.y=Q0[k0*2][6];  v0.z=Q0[k0*2][10];
		v1.x=Q0[k0*2+2][2];v1.y=Q0[k0*2+2][6];v1.z=Q0[k0*2+2][10];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}

	m_lst.AddString("");SPACE;

	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2][1];  v0.y=Q0[k0*2][5];  v0.z=Q0[k0*2][9];
		v1.x=Q0[k0*2+2][1];v1.y=Q0[k0*2+2][5];v1.z=Q0[k0*2+2][9];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;

	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2][0];  v0.y=Q0[k0*2][4];  v0.z=Q0[k0*2][8];
		v1.x=Q0[k0*2+2][0];v1.y=Q0[k0*2+2][4];v1.z=Q0[k0*2+2][8];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;
SPACE;

	sprintf(st,"right camera:");m_lst.AddString(st);WRTX;
	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2+1][2];  v0.y=Q0[k0*2+1][6];  v0.z=Q0[k0*2+1][10];
		v1.x=Q0[k0*2+3][2];v1.y=Q0[k0*2+3][6];v1.z=Q0[k0*2+3][10];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}

	m_lst.AddString("");SPACE;

	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2+1][1];  v0.y=Q0[k0*2+1][5];  v0.z=Q0[k0*2+1][9];
		v1.x=Q0[k0*2+3][1];v1.y=Q0[k0*2+3][5];v1.z=Q0[k0*2+3][9];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;

	for(k0=0;k0<8;k0++){
		v0.x=Q0[k0*2+1][0];  v0.y=Q0[k0*2+1][4];  v0.z=Q0[k0*2+1][8];
		v1.x=Q0[k0*2+3][0];v1.y=Q0[k0*2+3][4];v1.z=Q0[k0*2+3][8];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE
SPACE
sprintf(st,"================");m_lst.AddString(st);WRTX;
	sprintf(st,"left-right comapars of z,y,x axises:");m_lst.AddString(st);WRTX;
	for(k0=0;k0<9;k0++){
		v0.x=Q0[k0*2][2];  v0.y=Q0[k0*2][6];  v0.z=Q0[k0*2][10];
		v1.x=Q0[k0*2+1][2];v1.y=Q0[k0*2+1][6];v1.z=Q0[k0*2+1][10];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;

	for(k0=0;k0<9;k0++){
		v0.x=Q0[k0*2][1];  v0.y=Q0[k0*2][5];  v0.z=Q0[k0*2][9];
		v1.x=Q0[k0*2+1][1];v1.y=Q0[k0*2+1][5];v1.z=Q0[k0*2+1][9];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;

	for(k0=0;k0<9;k0++){
		v0.x=Q0[k0*2][0];  v0.y=Q0[k0*2][4];  v0.z=Q0[k0*2][8];
		v1.x=Q0[k0*2+1][0];v1.y=Q0[k0*2+1][4];v1.z=Q0[k0*2+1][8];
		af=acos(v0.x*v1.x+v0.y*v1.y+v0.z*v1.z);
		sprintf(st,"%d %9.4f",k0,af*180/pi);m_lst.AddString(st);WRTX;
	}
	m_lst.AddString("");SPACE;
	fclose(fp);
	m_lst.AddString("here");return;*/
//====================================================

/*FABCD Sm;
FXYZ v2,ctr[18];
	edge=2*RADIUS*cos(pi/6);
	fq=fopen("DAT\\Q0,dat","rb");fread(Q0,64,18,fq);fclose(fq);	
//	for(k0=0;k0<18;k0++){sprintf(st,"Q0[%02d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q0[k0][i*4+0],Q0[k0][i*4+1],Q0[k0][i*4+2],Q0[k0][i*4+3]);m_lst.AddString(st);}}	
	if(!(fq=fopen("DAT\\dv.dat","rb")))return;
	fread(dv,12,18*25,fq);fclose(fq);

	for(k0=0;k0<18;k0++){	
		m_lst.AddString("");
		for(k1=0;k1<3;k1++){
			f3[k1]=dv[k0][(k1*8)%24];
			f3[k1].x-=W2;f3[k1].y-=H2;f3[k1].z=m_id;
		}
		GetQ4rmDot(f3,edge,Q,La);
		ctr[k0].x=(La[0].x+La[1].x+La[2].x)/3;
		ctr[k0].y=(La[0].y+La[1].y+La[2].y)/3;
		ctr[k0].z=(La[0].z+La[1].z+La[2].z)/3;
		for(k1=0;k1<3;k1++){sprintf(st,"La %02d %9.3f %9.3f %9.3f",k0,La[k1].x,La[k1].y,La[k1].z);m_lst.AddString(st);  }
		sprintf(st,"mn %02d %9.3f %9.3f %9.3f",k0,ctr[k0].x,ctr[k0].y,ctr[k0].z);m_lst.AddString(st);  

	}


	fq=fopen("DAT\\Q1,dat","wb");	
//	for(k0=0;k0<18;k0++){m_lst.AddString("");sprintf(st,"JPG\\d%02d.jpg",k0);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);for(k=0;k<25;k++){fi=dv[k0][k];i0=fi.y;j0=fi.x;for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i0+i)*WIDTH+j+j0]^=255;dsply();Sleep(50);sprintf(st,"%02d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);m_lst.AddString(st);}dsply();Sleep(500);}
	for(k0=0;k0<18;k0++){
//		m_lst.AddString("");
//		fj=dv[k0][24];fj.x-=W2;fj.y-=H2;fj.z=m_id;
		fj=ctr[k0];
		Sm.A=Q0[k0][2];
		Sm.B=Q0[k0][6];	
		Sm.C=Q0[k0][10];
		Sm.D=-Sm.A*fj.x-Sm.B*fj.y-Sm.C*fj.z;
		fi=dv[k0][0];fi.x-=W2;fi.y-=H2;fi.z=m_id;
		fi=intsct(fi,Sm);
		v0=sbstrct(fj,fi);v0=guiy(v0);
//		sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);

		v2.x=Q0[k0][2];v2.y=Q0[k0][6];v2.z=Q0[k0][10];//v2=guiy(v2);
		v1=normal(v2,v0);//v1=guiy(v1);
//		sprintf(st,"%02d %9.6f %9.6f %9.6f ",k0,v0.x,v0.y,v0.z);m_lst.AddString(st);
//		sprintf(st,"%02d %9.6f %9.6f %9.6f ",k0,v1.x,v1.y,v1.z);m_lst.AddString(st);
//		sprintf(st,"%02d %9.6f %9.6f %9.6f ",k0,v2.x,v2.y,v2.z);m_lst.AddString(st);
		Q[0]=v0.x;Q[1]=v1.x;Q[2]=v2.x;Q[3]=fj.x;
		Q[4]=v0.y;Q[5]=v1.y;Q[6]=v2.y;Q[7]=fj.y;
		Q[8]=v0.z;Q[9]=v1.z;Q[10]=v2.z;Q[11]=fj.z;
		Q[12]=Q[13]=Q[14]=0;Q[15]=1;
//		m_lst.AddString("Q:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);m_lst.AddString(st);}
		fwrite(Q,64,1,fq);
//		for(k=0;k<25;k++){dv[k0][k].x-=W2;dv[k0][k].y-=H2;dv[k0][k].z=m_id;fi=dv[k0][k];fi=intsct(fi,Sm);sprintf(st,"%9.3f %9.3f %9.3f ",fi.x,fi.y,fi.z);m_lst.AddString(st);}
	}
	fclose(fq);

	fq=fopen("DAT\\Q1,dat","rb");fread(Q0,64,18,fq);fclose(fq);
//	for(k0=0;k0<18;k0++){sprintf(st,"Q1[%02d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q0[k0][i*4+0],Q0[k0][i*4+1],Q0[k0][i*4+2],Q0[k0][i*4+3]);m_lst.AddString(st);}}	
	for(k0=0;k0<9;k0++){
		CopyMemory(Qinv,Q0[k0*2+1],64);inv4(Qinv);
		mmtrx4(Q0[k0*2],Qinv,U[k0]);
		GetR3(R,U[k0]);
		r9[k0]=R_Oex(R);
	}

	for(k0=0;k0<9;k0++){sprintf(st,"U[%02d]:",k0);m_lst.AddString(st);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[k0][i*4+0],U[k0][i*4+1],U[k0][i*4+2],U[k0][i*4+3]);m_lst.AddString(st);}
	sprintf(st,"r9 %9.4f %9.4f %9.4f",r9[k0].x*180/pi,r9[k0].y*180/pi,r9[k0].z*180/pi);m_lst.AddString(st);
	}
	ro.x=ro.y=ro.z=0;
	for(k0=0;k0<9;k0++){
		ro.x+=r9[k0].x;
		ro.y+=r9[k0].y;
		ro.z+=r9[k0].z;
	}
	ro.x/=9;ro.y/=9;ro.z/=9;
	o_Rex(ro,R);
	GetR4(R,Q);
		m_lst.AddString("Q:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);m_lst.AddString(st);}
	sprintf(st,"ro %9.4f %9.4f %9.4f",ro.x*180/pi,ro.y*180/pi,ro.z*180/pi);m_lst.AddString(st);
*/

void Q2rot(float *Q,FXYZ *rot)
{
	float R[12],x,y,z,r;
	FXYZ ro,t;
	x=Q[3];y=Q[7];z=Q[11];r=sqrt(x*x+y*y+z*z);t.z=r;
	t.x=acos(x/r);t.y=acos(y/r);
	GetR3(R,Q);ro=R_Oex(R);
	rot[0]=ro;rot[1]=t;
}
void rot2Q(float *Q,FXYZ *rot)
{
	float R[12],x,y,z,r;
	FXYZ ro,t;	
	ro=rot[0];t=rot[1];
	r=t.z;x=cos(t.x);y=cos(t.y);z=sqrt(1-x*x-y*y);
	t.x=r*x;t.y=r*y;t.z=r*z;
	o_Rex(ro,R);R[9]=t.x;R[10]=t.y;R[11]=t.z;
	GetR4(R,Q);
}

void Q2r1t(float *Q,FXYZ *rot)
{
	float R[12];
	GetR3(R,Q);rot[0]=R_Oex(R);
	rot[1].x=Q[3];rot[1].y=Q[7];rot[1].z=Q[11];
}

void r1t2Q(float *Q,FXYZ *rot)
{
	float R[12];
	o_Rex(rot[0],R);R[9]=rot[1].x;R[10]=rot[1].y;R[11]=rot[1].z;
	GetR4(R,Q);
}




void CTmdDlg::OnButton3() 
{
	char st[256];
	int i,j,i0,j0,i1,k0,k1,k2,k3,k4,wk,kcnt;
	long k,l,n;
	float x,y,z,r,af,Q[16],A[9],Q0[16],R[12],edge,r3[3],min,max,mean,Qinv[16],U[16],Ua[16],Q2[2][16],A2[2][9];
	FXYZ fi,fj,fk,fl,f[25],g[25],g2[2][25],f3[3],f4[4],f7[2][73],La[4],La2[2][4],v[4],rot[2],rst[2]={0};
	FILE *fp,*fq,*fr,*fw;
	FILE *fs[2];
	edge=2*RADIUS*cos(pi/6);
	FXYZ fb[2]={0};FABCD S;S.A=S.B=0;S.C=-1;S.D=m_id;
//===========================================
//Q2f7	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\dv.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(f,12,25,fq);fclose(fq);for(k0=0;k0<25;k0++){f[k0].x-=W2;f[k0].y-=H2;f[k0].z=m_id;}for(k0=0;k0<24;k0++){for(i=0;i<3;i++)f3[i]=f[(k0+i*8)%24];GetQ4rmDot(f3,edge,Q,La);for(i=0;i<3;i++)f7[k4][k0*3+i]=La[i];}}for(k4=0;k4<2;k4++){m_lst.AddString("");cld2eig(f7[k4],72,Q,A);f7[k4][72].x=Q[3];f7[k4][72].y=Q[7];f7[k4][72].z=Q[11];m_lst.AddString("Q:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);m_lst.AddString(st);}sprintf(st,"A: %9.3f %9.3f %9.3f ",A[0],A[4],A[8]);m_lst.AddString(st);CopyMemory(Q2[k4],Q,64);}fq=fopen("DAT\\Q2.dat","wb");fwrite(Q2,64,2,fq);fclose(fq);fq=fopen("DAT\\f7.dat","wb");fwrite(f7,12,73*2,fq);fclose(fq);
	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);
	if(!(fq=fopen("DAT\\f7.dat","rb")))return;fread(f7,12,73*2,fq);fclose(fq);
	for(k4=0;k4<2;k4++){
		m_lst.AddString("++++++++++++++");
		Q2v(Q2[k4],v);
//		for(i=0;i<4;i++){fi=v[i];sprintf(st,"%d %12.6f %12.6f %12.6f ",k4,fi.x,fi.y,fi.z);m_lst.AddString(st);}
//		for(i=0;i<73;i++){fi=f7[k4][i];sprintf(st,"%d %02d %12.6f %12.6f %12.6f ",k4,i,fi.x,fi.y,fi.z);m_lst.AddString(st);}
//	fi=f7[k4][ 0];sprintf(st,"%d %12.6f %12.6f %12.6f ",k4,fi.x,fi.y,fi.z);m_lst.AddString(st);
//	fi=f7[k4][26];sprintf(st,"%d %12.6f %12.6f %12.6f ",k4,fi.x,fi.y,fi.z);m_lst.AddString(st);
//	fi=f7[k4][49];sprintf(st,"%d %12.6f %12.6f %12.6f ",k4,fi.x,fi.y,fi.z);m_lst.AddString(st);
		v[0].x=(f7[k4][ 0].x+f7[k4][26].x+f7[k4][49].x)/3;
		v[0].y=(f7[k4][ 0].y+f7[k4][26].y+f7[k4][49].y)/3;
		v[0].z=(f7[k4][ 0].z+f7[k4][26].z+f7[k4][49].z)/3;
		v[0]=sbstrct(v[0],f7[k4][72]);
		v[0]=guiy(v[0]);
		v[1]=normal(v[2],v[0]);
//		m_lst.AddString("");for(i=0;i<4;i++){fi=v[i];sprintf(st,"%d %12.6f %12.6f %12.6f ",k4,fi.x,fi.y,fi.z);m_lst.AddString(st);}
		v2Q(Q,v);CopyMemory(Q2[k4],Q,64);
		for(k0=0;k0<25;k0++){
			af=k0*15*pi/180;
			fi.x=RADIUS*cos(af);
			fi.y=RADIUS*sin(af);fi.z=0;
			if(k0==24){fi.x=fi.y=0;}
			f[k0]=mvct4a(Q,fi);
			fi=f[k0];sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);
		}
		for(k0=0;k0<25;k0++){
			fi=intsct(f[k0],S);
			i0=fi.y+H2;j0=fi.x+W2;
			for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
		}
		dsply();Sleep(200);
		CopyMemory(g2[k4],f,12*25);
	}
	CopyMemory(Qinv,Q2[1],64);inv4(Qinv);
	mmtrx4(Q2[0],Qinv,Ua);
	m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);}
	m_lst.AddString("");
	for(k0=0;k0<25;k0++){
	m_lst.AddString("");
		fi=g2[0][k0];
		sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);
		fi=g2[1][k0];fi=mvct4a(Ua,fi);
		sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);
	}
	gunX(g2[0],g2[1],25,Ua,rot);
//	gunX(f7[0],f7[1],73,Ua,rot);
	r1t2Q(Ua,rot);
	m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);}

	for(k0=0;k0<73;k0++){
	m_lst.AddString("");
		fi=f7[0][k0];
		sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);
		fi=f7[1][k0];fi=mvct4a(Ua,fi);
		sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);
	}



	m_lst.AddString("");
	m_lst.AddString("");m_lst.AddString("finished");

}

/*	for(k=0;k<HW;k++)b0[k]=k%256;
	for(k4=0;k4<2;k4++){
		m_lst.AddString("");

		for(k0=0;k0<24;k0++){
			fi=g2[k4][k0];
			i0=fi.y+H2;j0=fi.x+W2;
			
			for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
			
			sprintf(st,"%d %02d %9.3f %9.3f %9.3f ",k4,k0,fi.x,fi.y,fi.z);m_lst.AddString(st);


		}
		dsply();Sleep(1000);

	}*/

	
//	if(!(fq=fopen("DAT\\g2.dat","rb")))return;fread(g2,12,50,fq);fclose(fq);
//	for(k4=0;k4<2;k4++){for(k0=0;k0<25;k0++){fi=g2[k4][k0];fi.x-=W2;fi.y-=H2;fi.z=m_id;g2[k4][k0]=fi;}}
//	fp=fopen("DAT\\g2.txt","w");for(k4=0;k4<2;k4++){SPACE;for(k0=0;k0<25;k0++){fi=g2[k4][k0];sprintf(st,"%d %02d %9.3f %9.3f %9.3f",k4,k0,fi.x,fi.y,fi.z);WRTX;}}fclose(fp);

/*
	fp=fopen("DAT\\midDt.txt","w");
	fq=fopen("DAT\\La2.dat","wb");
	fr=fopen("DAT\\Q2.dat","wb");
	fw=fopen("DAT\\rot.dat","wb");
	for(k0=0;k0<24;k0++){
		SPACE;
		for(k4=0;k4<2;k4++){
			sprintf(st,"%d %02d",k4,k0);WRTX;
			for(i=0;i<3;i++)f3[i]=g2[k4][(k0+i*8)%24];
			GetQ4rmDot(f3,edge,Q2[k4],La2[k4]);

			sprintf(st,"La2[%d]:",k4);WRTX;
			for(i=0;i<3;i++){
				fi=La2[k4][i];
				sprintf(st,"%d %02d %9.3f %9.3f %9.3f",k4,k0,fi.x,fi.y,fi.z);WRTX;
				fj=La2[k4][(i+1)%3];
				x=fj.x-fi.x;y=fj.y-fi.y;z=fj.z-fi.z;r3[i]=sqrt(x*x+y*y+z*z);
			}
			sprintf(st,"r %9.4f %9.4f %9.4f",r3[0],r3[1],r3[2]);WRTX;
			sprintf(st,"Q2[%d]:",k4);WRTX;
			for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);WRTX;}
		}
		fwrite(La2,12,6,fq);
		fwrite(Q2,64,2,fr);
		CopyMemory(Qinv,Q2[1],64);inv4(Qinv);
		mmtrx4(Q2[0],Qinv,U);
		sprintf(st,"U:");WRTX;
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);WRTX;}
		Q2rot(U,rot);fi=rot[0];fj=rot[1];
		sprintf(st,"rot: %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);WRTX;
		fwrite(rot,12,2,fw);
		for(i=0;i<2;i++){
			rst[i].x+=rot[i].x/24;rst[i].y+=rot[i].y/24;rst[i].z+=rot[i].z/24;
		}
		sprintf(st,"fi<->fj");WRTX;
		for(i=0;i<3;i++){
			fi=La2[0][i];fj=La2[1][i];fj=mvct4a(U,fj);
			sprintf(st,"%9.3f %9.3f %9.3f ",fi.x,fi.y,fi.z);WRTX;
			sprintf(st,"%9.3f %9.3f %9.3f ",fj.x,fj.y,fj.z);WRTX;
		}

		SPACE;
		sprintf(st,"------------------------------");WRTX;
	}
		sprintf(st,"------------------------------");WRTX;


		fi=rst[0];fj=rst[1];
		sprintf(st,"rst: %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);WRTX;
		rot2Q(Ua,rst);
		sprintf(st,"Ua:");WRTX;
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);WRTX;}
	fclose(fp);fclose(fq);fclose(fr);fclose(fw);
	fq=fopen("DAT\\Ua.dat","wb");fwrite(Ua,64,1,fq);fclose(fq);

	FXYZ ff[72],fg[72];
	if(!(fq=fopen("DAT\\La2.dat","rb")))return;
	for(k0=0;k0<24;k0++){
		m_lst.AddString("");
		fread(La2,12,6,fq);
		for(i=0;i<3;i++){
			fi=ff[k0*3+i]=La2[0][i];
			fj=fg[k0*3+i]=La2[1][i];
			sprintf(st,"%9.3f %9.3f %9.3f |%9.3f %9.3f %9.3f ",fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);
		}
	}
	fclose(fq);
	fq=fopen("DAT\\ff.dat","wb");fwrite(ff,12,72,fq);fclose(fq);
	fq=fopen("DAT\\fg.dat","wb");fwrite(fg,12,72,fq);fclose(fq);
	fp=fopen("DAT\\fffg.txt","w");

	for(k0=0;k0<24;k0++){
		okLoadImageFile(hBrd,"JPG\\0\\00.jpg",0,BUFFER,0,1);
		for(k1=0;k1<3;k1++){
			fi=ff[k0*3+k1];
			fi=intsct(fi,S);
			i0=fi.y+H2;j0=fi.x+W2;
			for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
		}
		sprintf(st,"JPG\\0\\a%02d.jpg",k0);m_lst.AddString(st);
		l=okSaveImageFile(hBrd,st,70,BUFFER,0,1);
		sprintf(st,"l=%d",l);m_lst.AddString(st);
	}
	for(k0=0;k0<24;k0++){
		okLoadImageFile(hBrd,"JPG\\1\\00.jpg",0,BUFFER,0,1);
		for(k1=0;k1<3;k1++){
			fi=fg[k0*3+k1];
			fi=intsct(fi,S);
			i0=fi.y+H2;j0=fi.x+W2;
			for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
		}
		sprintf(st,"JPG\\1\\a%02d.jpg",k0);m_lst.AddString(st);
		l=okSaveImageFile(hBrd,st,70,BUFFER,0,1);
		sprintf(st,"l=%d",l);m_lst.AddString(st);
	}
*/


/*	for(k0=0;k0<72;k0++)
	{
		fi=ff[k0];fj=fg[k0];
		sprintf(st,"%02d %9.3f %9.3f %9.3f |%9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;
	}
	fclose(fp);
	okLoadImageFile(hBrd,"JPG\\0\\00.jpg",0,BUFFER,0,1);
	for(k0=0;k0<24;k0++){
		fi=ff[k0];fi=intsct(fi,S);
		i0=fi.y+H2;j0=fi.x+W2;
		for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
		dsply();Sleep(100);
	}
	dsply();Sleep(1000);
	okLoadImageFile(hBrd,"JPG\\1\\00.jpg",0,BUFFER,0,1);
	for(k0=0;k0<24;k0++){
		fi=fg[k0];fi=intsct(fi,S);
		i0=fi.y+H2;j0=fi.x+W2;
		for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
		dsply();Sleep(100);
	}

	dsply();Sleep(1000);*/


/*	cld2eig(ff,72,Q2[0],A);
		sprintf(st,"Q2[0]:");m_lst.AddString(st);
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);m_lst.AddString(st);}
		sprintf(st,"A: %9.3f %9.3f %9.3f ",A[0],A[4],A[8]);m_lst.AddString(st);
		Q2v(Q2[0],v);
		
	cld2eig(fg,72,Q2[1],A);
		sprintf(st,"Q2[1]:");m_lst.AddString(st);
		for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);m_lst.AddString(st);}
		sprintf(st,"A: %9.3f %9.3f %9.3f ",A[0],A[4],A[8]);m_lst.AddString(st);
		fi.x=fi.y=fi.z=0;
		fi.x+=(ff[0].x+ff[26].x+ff[49.x)/3;
*/
//	gunX(ff,fg,72,Ua,rot);





	


void CTmdDlg::OnButton4() 
{
	// TODO: Add your control notification handler code here
	char st[256];
	int i,j,i0,j0,i1,j1,k0,k1,k2,k3,k4,wk,kcnt;
	long k,l,n;
	float x,y,z,r,af,Q[16],A[9],Q0[16],R[12],edge,r3[3],min,max,mean,Qinv[16],U[16],Ua[16],Q2[2][16],A2[2][9];
	FXYZ fi,fj,fk,fl,f[25],g[25],g2[2][25],f3[3],f4[4],f7[73],La[4],La2[2][360],v[4],rot[2];
	FILE *fp,*fq,*fr,*fw;
	FILE *fs[2];
	FXYZ offset,cntr[2];
	edge=2*RADIUS*cos(pi/6);

	FABCD S;S.A=S.B=0;S.C=-1;S.D=m_id;
	BYTE flg;
	WORD *wd;
	BYTE *b3=b2+HW;
	BYTE *b4=b3+HW;


//YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY

//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\dv%d.dat",k4);if(!(fp=fopen(st,"rb")))return;for(k0=0;k0<40;k0++){sprintf(st,"JPG\\%d\\%02d.jpg",k4,k0);SetWindowText(st);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(100);fread(f,12,25,fp);for(k1=0;k1<25;k1++){i0=f[k1].y;j0=f[k1].x;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(20);}dsply();Sleep(100);}}
//	for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\dv.dat",k4);if(!(fr=fopen(st,"rb")))return;sprintf(st,"DAT\\%d\\La.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);fq=fopen(st,"wb");sprintf(st,"DAT\\%d\\Q.dat",k4);fw=fopen(st,"wb");for(k0=0;k0<360;k0++){fread(f,12,25,fr);for(k2=0;k2<24;k2++){for(k1=0;k1<3;k1++){f3[k1]=f[(k1*8+k2)%24];f3[k1].x-=W2;f3[k1].y-=H2;f3[k1].z=m_id;}GetQ4rmDot(f3,edge,Q,La);sprintf(st,"DAT\\%d\\Q.dat",k4);fwrite(Q,64,1,fw);SPACE;for(k1=0;k1<3;k1++){fi=La[k1];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k2,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}fclose(fr);fclose(fp);fclose(fq);}
//	FXYZ *fx=(FXYZ *)new FXYZ[1080];int ix[3]={0,2,1};for(k4=0;k4<2;k4++){sprintf(st,"DAT\\%d\\Q&A.txt",k4);fp=fopen(st,"w");sprintf(st,"DAT\\%d\\La.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(fx,12,1080,fq);cld2eig(fx,1080,Q,A);sprintf(st,"Q[%d]:",k4);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A[%d]:  %12.3f %12.3f %9.3f",k4,A[0],A[4],A[8]);WRTX;Q2rot(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);WRTX;fclose(fp);sprintf(st,"DAT\\%d\\Q.dat",k4);fq=fopen(st,"wb");fwrite(Q,64,1,fq);fclose(fq);CopyMemory(Q2[k4],Q,64);m_lst.AddString("============================");for(k0=0;k0<45;k0++){k1=k0*8*3+ix[k0%3];fb[k4].x+=fx[k1].x;fb[k4].y+=fx[k1].y;fb[k4].z+=fx[k1].z;}fb[k4].x/=45;fb[k4].y/=45;fb[k4].z/=45;sprintf(st,"fb %9.3f %9.3f %9.3f",fb[k4].x,fb[k4].y,fb[k4].z);m_lst.AddString(st);}fq=fopen("DAT\\fb.dat","wb");fwrite(fb,12,2,fq);fclose(fq);delete [] fx;for(k4=0;k4<2;k4++){v[2].x=Q2[k4][2];v[2].y=Q2[k4][6];v[2].z=Q2[k4][10];fi.x=fb[k4].x-Q2[k4][3];fi.y=fb[k4].y-Q2[k4][7];fi.z=fb[k4].z-Q2[k4][11];v[0]=guiy(fi);v[1]=normal(v[2],v[0]);Q2[k4][0]=v[0].x;Q2[k4][1]=v[1].x;Q2[k4][2]=v[2].x;Q2[k4][4]=v[0].y;Q2[k4][5]=v[1].y;Q2[k4][6]=v[2].y;Q2[k4][8]=v[0].z;Q2[k4][9]=v[1].z;Q2[k4][10]=v[2].z;m_lst.AddString("Q2[k4]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);m_lst.AddString(st);}}fq=fopen("DAT\\Q2.dat","wb");fwrite(Q2,64,2,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\00.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i0+i)*WIDTH+j0+j]^=255;dsply();Sleep(100);}dsply();Sleep(1000);}	
//	if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);for(k4=0;k4<2;k4++){m_lst.AddString("=============================");for(k0=0;k0<25;k0++){af=k0*15*pi/180;fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);fi.z=0;if(k0==24){fi.x=fi.y=0;}fi=mvct4a(Q2[k4],fi);f[k0]=fi;sprintf(st,"%02d %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);}sprintf(st,"DAT\\%d\\d25.dat",k4);fq=fopen(st,"wb");fwrite(f,12,25,fq);fclose(fq);}
//	float la,lb,lc;if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);m_lst.AddString("");m_lst.AddString("Q2[0]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);m_lst.AddString(st);}m_lst.AddString("Q2[1]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);m_lst.AddString(st);}m_lst.AddString("");inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],U);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2rot(U,rot);r=x=y=0;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);fi=guiy1(fi,lc);fi.x=acos(fi.x);fi.y=acos(fi.y);sprintf(st,"%02d %12.6f %12.6f %12.6f %9.3f",k0,fi.x*180/pi,fi.y*180/pi,fi.z,lc);m_lst.AddString(st);r+=lc;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",x*180/pi,y*180/pi,r);m_lst.AddString(st);rot[1].x=x;rot[1].y=y;rot[1].z=r;rot2Q(U,rot);m_lst.AddString("");fi=rot[0];fj=rot[1];sprintf(st,"rot %9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x*180/pi,fj.y*180/pi,fj.z);m_lst.AddString(st);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\U.dat","wb");fwrite(U,64,1,fq);fclose(fq);rot[0].x=rot[0].y=rot[0].z=0;rot2Q(U,rot);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}fq=fopen("DAT\\Ua.dat","wb");fwrite(U,64,1,fq);fclose(fq);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k4=0;k4<2;k4++){sprintf(st,"JPG\\%d\\28.jpg",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);sprintf(st,"DAT\\%d\\d25.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(f,12,25,fq);fclose(fq);m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fi=intsct(fi,S);i0=fi.y+H2;j0=fi.x+W2;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j0+j]^=255;dsply();Sleep(200);}}
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);}r=x=y=0;m_lst.AddString("");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];fi=sbstrct(fi,fj);sprintf(st,"dif %02d %12.6f %12.6f %12.6f",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);r+=fi.z;x+=fi.x;y+=fi.y;}r/=25;x/=25;y/=25;fi.x=x;;fi.y=y;fi.z=r;m_lst.AddString("");sprintf(st,"mean  %12.6f %12.6f %12.6f",fi.x,fi.y,fi.z);m_lst.AddString(st);fq=fopen("DAT\\t.dat","wb");fwrite(&fi,12,1,fq);fclose(fq);
//	float bt,gm,lc,la,lb;if(!(fp=fopen("DAT\\Ua.dat","rb")))return;fread(U,64,1,fp);fclose(fp);if(!(fq=fopen("DAT\\dv0.dat","rb")))return;if(!(fr=fopen("DAT\\dv1.dat","rb")))return;fp=fopen("DAT\\fifj.txt","w");fw=fopen("DAT\\fifj.dat","wb");fk.x=U[3],fk.y=U[7],fk.z=U[11],fk=guiy1(fk,lc);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fi.y-=H2;fi.z=m_id;fj.x-=W2;fj.y-=H2;fj.z=m_id;fi=guiy(fi);v[0]=fi;fj=mvct4a(U,fj);fj=guiy(fj);v[1]=fj;gm=angle(fi,fj);af=angle(fi,fk);bt=pi-af-gm;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],la);fwrite(&fi,12,1,fw);fwrite(&fj,12,1,fw);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);	fclose(fq);fclose(fr);fclose(fw);
//	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fq=fopen("DAT\\d25x2.dat","wb");fp=fopen("DAT\\d25x2.txt","w");for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);	WRTX;fwrite(&fi,12,1,fq);fwrite(&fj,12,1,fq);}fclose(fq);fclose(fp);
//	if(!(fq=fopen("DAT\\fifj.dat","rb")))return;fp=fopen("DAT\\sbstrctFifj.txt","w");for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fq);fk=sbstrct(fi,fj);r=sqrt(fk.x*fk.x+fk.y*fk.y+fk.z*fk.z);sprintf(st,"%04d %9.3f %9.3f %9.3f %9.3f",k0,fk.x,fk.y,fk.z,r);WRTX;}fclose(fq);fclose(fp);
//	fq=fopen("DAT\\idy.dat","wb");fp=fopen("DAT\\idy.txt","w");for(k=0;k<729;k++){idy[k][5]=(k/243);idy[k][4]=(k%243)/81;idy[k][3]=(k%81)/27;idy[k][2]=(k%27)/9;idy[k][1]=(k%9)/3;idy[k][0]=(k%3);}for(k=0;k<729;k++)for(i=0;i<6;i++)idy[k][i]-=1;for(k=0;k<729;k++){sprintf(st,"%03d |  %3d %3d %3d %3d %3d %3d " ,k,idy[k][5],idy[k][4],idy[k][3],idy[k][2],idy[k][1],idy[k][0]);m_lst.AddString(st);WRTX;}fwrite(idy,2,729*6,fq);fclose(fq);fclose(fp);
//Qa	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ua.dat","rb")))return;fread(Ua,64,1,fq);fclose(fq);m_lst.AddString("Ua:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);}
//Q2 	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Q2.dat","rb")))return;fread(Q2,64,2,fq);fclose(fq);inv4(Q2[1]);mmtrx4(Q2[0],Q2[1],Ua);gunX(f,g,25,Ua,rot);r1t2Q(Ua,rot);fq=fopen("DAT\\Ub.dat","wb");fp=fopen("DAT\\Ub.txt","w");sprintf(st,"Ub:");WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Ua[i*4+0],Ua[i*4+1],Ua[i*4+2],Ua[i*4+3]);m_lst.AddString(st);WRTX;}fwrite(Ua,64,1,fq);fclose(fq);fclose(fp);
//crtfy	if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fq);fclose(fq);if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}Q2r1t(U,rot);fi=rot[0];fj=rot[1];sprintf(st,"%9.4f %9.4f %9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z);m_lst.AddString(st);m_lst.AddString("");m_lst.AddString("===========================");for(k0=0;k0<25;k0++){m_lst.AddString("");fi=f[k0];fj=g[k0];fj=mvct4a(U,fj);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);sprintf(st,"%02d %12.6f %12.6f %12.6f ",k0,fj.x,fj.y,fj.z);m_lst.AddString(st);}
//crtfu		if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);}if(!(fq=fopen("DAT\\0\\La.dat","rb")))return;if(!(fr=fopen("DAT\\1\\La.dat","rb")))return;fp=fopen("DAT\\crtfu.txt","w");sprintf(st,"=========================");WRTX;for(k0=0;k0<1080;k0++){SPACE;fread(&fi,12,1,fq);fread(&fj,12,1,fr);;sprintf(st,"%04d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;fj=mvct4a(U,fj);sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,fj.x,fj.y,fj.z);WRTX;}fclose(fq);fclose(fr);fclose(fp);
//crtfv	FXYZ t;float bt,gm,la,lb,lc;fp=fopen("DAT\\crtfv.txt","w");if(!(fq=fopen("DAT\\Ub.dat","rb")))return;fread(U,64,1,fq);fclose(fq);sprintf(st,"U:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}CopyMemory(Qinv,U,64);inv4(Qinv);sprintf(st,"Qinv:");m_lst.AddString(st);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Qinv[i*4+0],Qinv[i*4+1],Qinv[i*4+2],Qinv[i*4+3]);m_lst.AddString(st);WRTX;}
//		if(!(fq=fopen("DAT\\0\\d25.dat","rb")))return;fread(f,12,25,fq);fclose(fq);if(!(fr=fopen("DAT\\1\\d25.dat","rb")))return;fread(g,12,25,fr);fclose(fr);t.x=U[3];t.y=U[7];t.z=U[11];v[2]=guiy1(t,lc);sprintf(st,"labc+abg+SINE cert=========================");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %9.4f %9.4f %9.4f | %9.4f %9.4f %9.4f %9.4f",k0,la,lb,lc,af*180/pi,bt*180/pi,gm*180/pi,(af+bt+gm)*180/pi);m_lst.AddString(st);WRTX;}sprintf(st,"SINE cert");m_lst.AddString(st);WRTX;for(k0=0;k0<25;k0++){fi=f[k0];fj=g[k0];v[0]=guiy1(fi,la);v[1]=guiy1(fj,lb);gm=acos((la*la+lb*lb-lc*lc)/(2*la*lb));af=acos((lc*lc+lb*lb-la*la)/(2*lc*lb));bt=acos((lc*lc+la*la-lb*lb)/(2*lc*la));sprintf(st,"%02d %12.6f %12.6f %12.6f |",k0,la/sin(af),lb/sin(bt),lc/sin(gm));m_lst.AddString(st);WRTX;}sprintf(st,"9000 afbtgm=========================");m_lst.AddString(st);WRTX;if(!(fq=fopen("DAT\\0\\dv.dat","rb")))return;;if(!(fr=fopen("DAT\\1\\dv.dat","rb")))return;;for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;sprintf(st,"%04d %9.4f %9.4f %9.4f",k0,af*180/pi,bt*180/pi,gm*180/pi);WRTX;}sprintf(st,"=========================");m_lst.AddString(st);WRTX;fseek(fq,0,SEEK_SET);fseek(fr,0,SEEK_SET);for(k0=0;k0<9000;k0++){fread(&fi,12,1,fq);fread(&fj,12,1,fr);fi.x-=W2;fj.x-=W2;fi.y-=H2;fj.y-=H2;fi.z=fj.z=m_id;v[0]=guiy(fi);v[1]=guiy(fj);af=angle(v[0],v[2]);fk.x=Qinv[3];fk.y=Qinv[7];fk.z=Qinv[11];fk=guiy(fk);bt=angle(v[1],fk);gm=pi-af-bt;la=lc*sin(bt)/sin(gm);lb=lc*sin(af)/sin(gm);fi=VLize(v[0],la);fj=VLize(v[1],lb);sprintf(st,"%04d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;}fclose(fp);fclose(fq);fclose(fr);
//==================================================================================	
//JPH	BLOCKINFO blk;FillMemory(&blk,sizeof(blk),0);blk.iBitCount=8;blk.iFormType=FORM_GRAY8;blk.iHeight=HEIGHT;blk.iWidth=WIDTH;blk.lpBits = b0;for(k0=0;k0<9;k0++){for(k1=0;k1<40;k1++){sprintf(st,"..\\..\\D%d\\%d.bmp",k0,k1);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)okGetBufferAddr(hBrd,0);b0=(BYTE *)pnt;b1=b0+HW*2;for(k=0;k<HW*2;k++)b0[k]=pnt[k].r;CopyMemory(b1,b0,HW*2);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+j];}sprintf(st,"JPH\\0\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){b0[i*WIDTH+j]=b1[i*WIDTH*2+WIDTH+j];}sprintf(st,"JPH\\1\\%03d.jpg",k0*40+k1);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);}}
//dt	float sum,isum,jsum;fp=fopen("DBT\\dt.txt","w");fq=fopen("DBT\\dt.dat","wb");int *bg5=(int *)new int[HW];for(k4=0;k4<2;k4++){for(k0=0;k0<360;k0++){sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);l=okLoadImageFile(hBrd,st,0,BUFFER,1,1);im2bw(b1,b0,HEIGHT,WIDTH);for(i=0;i<10;i++)for(j=0;j<WIDTH;j++)b0[i*WIDTH+j]=0;FillMemory(bg5,sizeof(int)*HW,0);for(n=0;n<HW;n++)if(b0[n]>0)bg5[n]=1;wk=BWLabel(bg5,HEIGHT,WIDTH);sprintf(st,"wk=%d",wk);WRTX;	kcnt=0;for(k1=1;k1<=wk;k1++){sum=isum=jsum=0;for(i=0;i<HEIGHT;i++)for(j=0;j<WIDTH;j++){if(bg5[i*WIDTH+j]==k1){sum++;isum+=i;jsum+=j;}}if(sum>10){fi.x=jsum/sum;fi.y=isum/sum;fi.z=sum;sprintf(st,"%d %02d %03d %9.3f %9.3f %7.0f",k4,kcnt++,k0,fi.x,fi.y,fi.z);WRTX;fwrite(&fi,12,1,fq);}}}}delete[]bg5;fclose(fp);fclose(fq);
//dv	if(!(fq=fopen("DBT\\dt.dat","rb")))return;fp=fopen("DBT\\dv.txt","w");fr=fopen("DBT\\dv.dat","wb");for(k0=0;k0<720;k0++){SPACE;fread(f,12,25,fq);for(k=0;k<25;k++){f[k].x-=W2;f[k].y-=H2;}Sort7z(f,25,1);for(k=0;k<24;k++){x=f[k].x-f[24].x;y=f[k].y-f[24].y;r=sqrt(x*x+y*y);af=asin(y/r);if(x<0)af=pi-af;if(af<0)af+=2*pi;f[k].z=af;}for(k=23;k>=0;k--){f[k].z-=f[0].z;if(f[k].z<0)f[k].z+=2*pi;}Sort7z(f,24,0);	for(k=0;k<25;k++){fi=f[k];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f",k0,k,fi.x,fi.y,fi.z);WRTX;}fwrite(f,12,25,fr);}fclose(fq);fclose(fp);fclose(fr);
//La	if(!(fq=fopen("DBT\\dv.dat","rb")))return;fp=fopen("DBT\\La.txt","w");fr=fopen("DBT\\La.dat","wb");fw=fopen("DBT\\Q.dat","wb");for(k0=0;k0<720;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k=0;k<24;k++){for(i=0;i<3;i++){f3[i]=f[(k+8*i)%24];f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);fwrite(Q,64,1,fw);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){fi=La[i];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);fk=guiy1(fk,r3[i]);}sprintf(st,"r: %9.4f %9.4f %9.4f ",r3[0],r3[1],r3[2]);WRTX;}			}fclose(fq);fclose(fw);fclose(fr);fclose(fp);	
//rot	if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\Q.txt","w");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);sprintf(st,"Q[%03d]:",k0);WRTX;for(i1=0;i1<4;i1++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i1*4+0],Q[i1*4+1],Q[i1*4+2],Q[i1*4+3]);WRTX;}}}fclose(fq);fclose(fp);if(!(fq=fopen("DBT\\Q.dat","rb")))return;fp=fopen("DBT\\rot.txt","w");fr=fopen("DBT\\rot.dat","wb");for(k0=0;k0<720;k0++){for(i=0;i<3;i++){fread(Q,64,1,fq);Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fr);r=sqrt(fj.x*fj.x+fj.y*fj.y+fj.z*fj.z);sprintf(st,"rot[%03d]: %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}}fclose(fq);fclose(fp);fclose(fr);	
//cld	if(!(fq=fopen("DBT\\La.dat","rb")))return;fp=fopen("DBT\\cld.txt","w");FXYZ *L7=(FXYZ *)new FXYZ[72];fr=fopen("DBT\\Q7.dat","wb");fw=fopen("DBT\\rt7.dat","wb");for(k0=0;k0<720;k0++){fread(L7,12,72,fq);SPACE;cld2eig(L7,72,Q,A);sprintf(st,"Q[%03d]:",k0);WRTX;fwrite(Q,64,1,fr);for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q[i*4+0],Q[i*4+1],Q[i*4+2],Q[i*4+3]);WRTX;}sprintf(st,"A: %9.3f %9.3f %9.6f",A[0],A[4],A[8]);WRTX;Q2r1t(Q,rot);fi=rot[0];fj=rot[1];fwrite(rot,12,2,fw);fk=guiy1(fj,r);sprintf(st,"%9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}fclose(fp);fclose(fr);fclose(fw);fclose(fq);delete[]L7;
//log	FXYZ *f1st=(FXYZ *)new FXYZ[720];FXYZ *cntr=(FXYZ *)new FXYZ[720];FXYZ *v0=(FXYZ *)new FXYZ[720];if(!(fq=fopen("DBT\\La.dat","rb")))return;for(k0=0;k0<720;k0++){fread(f7,12,72,fq);fi.x=(f7[0].x+f7[26].x+f7[49].x)/3;fi.y=(f7[0].y+f7[26].y+f7[49].y)/3;fi.z=(f7[0].z+f7[26].z+f7[49].z)/3;f1st[k0]=fi;}if(!(fq=fopen("DBT\\Q7.dat","rb")))return;fp=fopen("DBT\\log.txt","w");fr=fopen("DBT\\Q2.dat","wb");for(k0=0;k0<720;k0++){fread(Q,64,1,fq);fi.x=cntr[k0].x=Q[3];fi.y=cntr[k0].y=Q[7];fi.z=cntr[k0].z=Q[11];fj=f1st[k0];fi=sbstrct(fi,fj);fi=guiy(fi);v0[k0]=fi;SPACE;Q2r1t(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%03d %9.6f %9.6f %9.6f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;Q2v(Q,v);v[0]=v0[k0];v[1]=normal(v[2],v[0]);v2Q(Q,v);Q2r1t(Q,rot);fi=rot[0];fj=rot[1];sprintf(st,"%03d %9.6f %9.6f %9.6f %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;fwrite(Q,64,1,fr);}delete[]f1st,cntr,v0;fclose(fq);fclose(fr);fp=fopen("DBT\\loh.txt","w");fr=fopen("DBT\\U7.dat","wb");if(!(fq=fopen("DBT\\Q2.dat","rb")))return;for(k0=0;k0<360;k0++){fseek(fq,k0*64,SEEK_SET);fread(Q2[0],64,1,fq);fseek(fq,(k0+360)*64,SEEK_SET);fread(Q2[1],64,1,fq);SPACE;sprintf(st,"Q2[0][%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[0][i*4+0],Q2[0][i*4+1],Q2[0][i*4+2],Q2[0][i*4+3]);WRTX;}sprintf(st,"Q2[1][%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[1][i*4+0],Q2[1][i*4+1],Q2[1][i*4+2],Q2[1][i*4+3]);WRTX;}CopyMemory(Qinv,Q2[1],64);inv4(Qinv);mmtrx4(Q2[0],Qinv,U);sprintf(st,"U[%03d]:",k0);WRTX;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);WRTX;}fwrite(U,64,1,fr);Q2r1t(U,rot);fi=rot[0];fj=rot[1];fk=guiy1(fj,r);sprintf(st,"%03d %9.4f %9.4f %9.4f| %9.4f %9.4f %9.4f %9.4f ",k0,fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z,r);WRTX;}fclose(fq);fclose(fp);fclose(fr);
//-------------------------------------------------------------
//La		if(!(fq=fopen("DBT\\dv.dat","rb")))return;fp=fopen("DAT\\La.txt","w");fr=fopen("DAT\\La.dat","wb");fw=fopen("DAT\\Q.dat","wb");for(k0=0;k0<720;k0++){sprintf(st,"k0=%03d",k0);SetWindowText(st);fread(f,12,25,fq);for(k=0;k<24;k++){for(i=0;i<3;i++){f3[i]=f[(k+8*i)%24];f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);fwrite(Q,64,1,fw);fwrite(La,12,3,fr);SPACE;for(i=0;i<3;i++){fi=La[i];sprintf(st,"%03d %02d %9.3f %9.3f %9.3f ",k0,k,fi.x,fi.y,fi.z);WRTX;}for(i=0;i<3;i++){fi=La[i];fj=La[(i+1)%3];fk=sbstrct(fi,fj);fk=guiy1(fk,r3[i]);}sprintf(st,"r: %9.4f %9.4f %9.4f ",r3[0],r3[1],r3[2]);WRTX;}			}fclose(fq);fclose(fw);fclose(fr);fclose(fp);	
//fifj		if(!(fq=fopen("DAT\\UbOld.dat","rb")))return;fread(U,64,1,fq);fclose(fq);if(!(fq=fopen("DBT\\La.dat","rb")))return;fp=fopen("DAT\\fifj.txt","w");for(k0=0;k0<25920;k0++){fseek(fq,k0*12,SEEK_SET);fread(&fi,12,1,fq);fseek(fq,(k0+25920)*12,SEEK_SET);fread(&fj,12,1,fq);fj=mvct4a(U,fj);fk=sbstrct(fi,fj);fk=guiy1(fk,r);sprintf(st,"%05d %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z,r);WRTX;}fclose(fq);fclose(fp);
//b0.bmp	BLOCKINFO blk;;FillMemory(&blk,sizeof(blk),0);blk.iBitCount=8;blk.iFormType=FORM_GRAY8;blk.iHeight=HEIGHT;blk.iWidth=WIDTH;blk.lpBits = b0;for(k4=0;k4<2;k4++){sprintf(st,"BMP\\a%d.bmp",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)b0;for(k=0;k<HW;k++)b0[k]=pnt[k].r;sprintf(st,"BMP\\b%d.bmp",k4);okSaveImageFile(hBrd,st,70,TARGET(&blk),0,1);}
//dif.dat	BYTE flg;float f9[9]={0},a[4]={0},cnt,stp;int iwk,ix[9]={-WIDTH-1,-WIDTH+0,-WIDTH+1,-1,0,1,WIDTH-1, WIDTH+0, WIDTH+1};for(k0=0;k0<2;k0++){sprintf(st,"BMP\\b%d.bmp",k0);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);b0=(BYTE *)okGetBufferAddr(hBrd,0);FillMemory(b1,HW,0);for(k=0;k<HW;k++){if(b0[k]==0)continue;flg=0;for(i=0;i<9;i++){if(b0[k+ix[i]]==0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)f9[i]=(b0[k+ix[i]]-b0[k]);for(i=0;i<4;i++){a[i]=f9[i]+f9[8-i];if(a[i]<0)a[i]=-a[i];}iwk=a[0]+a[1]+a[2]+a[3];if(iwk>255)iwk=255;b1[k]=iwk;}sprintf(st,"DAT\\dif%d.dat",k0);fq=fopen(st,"wb");fwrite(b1,1,HW,fq);fclose(fq);CopyMemory(b0,b1,HW);dsply();Sleep(1000);sprintf(st,"BMP\\df%d.bmp",k0);okSaveImageFile(hBrd,st,100,BUFFER,0,1);}
//	for(k4=0;k4<2;k4++){sprintf(st,"BMP\\df%d.bmp",k4);okLoadImageFile(hBrd,st,0,BUFFER,0,1);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=255;dsply();Sleep(1000);}
//===============================================
//wd	PNT *pnu,p;for(k4=0;k4<2;k4++){sprintf(st,"BMP\\a%d.bmp",k4);l=okLoadImageFile(hBrd,st,0,BUFFER,0,1);pnt=(PNT *)okGetBufferAddr(hBrd,0);pnu=pnt+HW*3; wd=(WORD *)pnu;FillMemory(wd,2,HW,0);for(k=0;k<HW;k++)wd[k]=(114*pnt[k].b+587*pnt[k].g+299*pnt[k].r)/10;sprintf(st,"DAT\\wd%d.dat",k4);fq=fopen(st,"wb");fwrite(wd,2,HW,fq);fclose(fq);}
//bw.bmp	if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[0],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);for(k4=0;k4<2;k4++){for(k=0;k<HW;k++)b0[k]=wd2[k4][k]/100;dsply();Sleep(1000);}for(k4=0;k4<2;k4++){FillMemory(b0,HW,0);for(k=0;k<HW;k++)if(wd2[k4][k]>0)b0[k]=255;sprintf(st,"BMP\\bw%d.bmp",k4);okSaveImageFile(hBrd,st,100,BUFFER,0,1);dsply();Sleep(1000);}
//offset	FXYZ cntr[2]={0};long cnt;okLoadImageFile(hBrd,"BMP\\bw0.bmp",0,BUFFER,0,1);cnt=0;for(k=0;k<HW;k++){if(b0[k]>0){cntr[0].x+=k%WIDTH;cntr[0].y+=k/WIDTH;cnt++;}}cntr[0].x/=cnt;cntr[0].y/=cnt;okLoadImageFile(hBrd,"BMP\\bw1.bmp",0,BUFFER,1,1);cnt=0;for(k=0;k<HW;k++){if(b1[k]>0){cntr[1].x+=k%WIDTH;cntr[1].y+=k/WIDTH;cnt++;}}cntr[1].x/=cnt;cntr[1].y/=cnt;offset=sbstrct(cntr[0],cntr[1]);sprintf(st,"offset x=%9.3f y=%9.3f",offset.x,offset.y);m_lst.AddString(st);fq=fopen("DAT\\offset.dat","wb");fwrite(&offset,12,1,fq);fclose(fq);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=85;for(k=0;k<HW;k++)if(b1[k]>0)b0[k]+=170;okSaveImageFile(hBrd,"BMP\\offset.bmp",100,BUFFER,0,1);	
//0to1		if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);okLoadImageFile(hBrd,"BMP\\bw0.bmp",0,BUFFER,0,1);FillMemory(b1,HW,0);for(k=0;k<HW;k++){i=k/WIDTH+offset.y;j=k%WIDTH+offset.x;if(b0[k]>0)b1[i*WIDTH+j]=85;}okLoadImageFile(hBrd,"BMP\\bw1.bmp",0,BUFFER,0,1);for(k=0;k<HW;k++)if(b0[k]>0)b0[k]=170;for(k=0;k<HW;k++)b0[k]+=b1[k];dsply();
//grid		for(k=0;k<25;k++){i=(k/5)-2;j=(k%5)-2;idz[k]=i*WIDTH+j;}for(k=0;k<5;k++){sprintf(st,"%6d %6d %6d %6d %6d ",idz[k*5+0],idz[k*5+1],idz[k*5+2],idz[k*5+3],idz[k*5+4]);m_lst.AddString(st);}fq=fopen("DAT\\idz25.dat","wb");fwrite(idz,sizeof(int),25,fq);fclose(fq);if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);FillMemory(b2,HW,0);okLoadImageFile(hBrd,"BMP\\df0.bmp",0,BUFFER,0,1);for(k=0;k<HW;k++){if(b0[k]>0){i=k/WIDTH+offset.y;j=k%WIDTH+offset.x;b2[i*WIDTH+j]=b0[k];}}CopyMemory(b0,b2,HW);okLoadImageFile(hBrd,"BMP\\df1.bmp",0,BUFFER,0,1);FillMemory(b2,HW,0);fq=fopen("DAT\\grid.dat","wb");for(i=0;i<HEIGHT;i+=5)for(j=0;j<WIDTH;j+=5){flg=0;for(i0=0;i0<5;i0++)for(j0=0;j0<5;j0++)if(b0[(i+i0)*WIDTH+j+j0]==0)flg++;if(flg==0){b2[(i+2)*WIDTH+j+2]=255;fi.x=j+2;fi.y=i+2;fi.z=0;fwrite(&fi,12,1,fq);}}CopyMemory(b0,b2,HW);dsply();okSaveImageFile(hBrd,"BMP\\c.bmp",100,BUFFER,0,1);fclose(fq);
//bw.bmp	if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[0],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);for(k4=0;k4<2;k4++){for(k=0;k<HW;k++)b0[k]=wd2[k4][k]/100;dsply();Sleep(1000);}for(k4=0;k4<2;k4++){FillMemory(b0,HW,0);for(k=0;k<HW;k++)if(wd2[k4][k]>0)b0[k]=255;sprintf(st,"BMP\\bw%d.bmp",k4);okSaveImageFile(hBrd,st,100,BUFFER,0,1);dsply();Sleep(1000);}
//wdx3		wd2[0]=(WORD *)b0;wd2[1]=(WORD *)b2;wd2[2]=(WORD *)b4;if(!(fq=fopen("DAT\\offset.dat","rb")))return;fread(&offset,12,1,fq);fclose(fq);sprintf(st,"offset %9.3f %9.3f",offset.x,offset.y);m_lst.AddString(st);if(!(fq=fopen("DAT\\wd0.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);FillMemory(wd2[0],HW*2,0);for(k=0;k<HW;k++){if(wd2[2][k]>0){i=k/WIDTH+offset.y;j=(k%WIDTH)+offset.x;wd2[0][i*WIDTH+j]=wd2[2][k];}}fq=fopen("DAT\\wd2.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);wd=(WORD *)b0;for(k4=0;k4<3;k4++){sprintf(st,"DAT\\wd%d.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(wd,2,HW,fq);for(k=0;k<HW;k++)b0[k]=wd[k]/100;dsply();Sleep(1000);FillMemory(wd,HW*2,0);}
//cmpr		wd=(WORD*)b0;for(k4=1;k4<3;k4++){sprintf(st,"DAT\\wd%d.dat",k4);if(!(fq=fopen(st,"rb")))return;fread(wd,2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd[k]/100;dsply();Sleep(1000);}
//wdf		int cha[9],a[4];int ix9[9]={-WIDTH-1,-WIDTH,-WIDTH+1,-1,0,1,WIDTH-1,WIDTH,WIDTH+1};if(!(fq=fopen("DAT\\wd2.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);FillMemory(wd2[0],2*HW,0);for(k=0;k<HW;k++){if(wd2[2][k]<=0)continue;flg=0;for(i=0;i<9;i++){if(wd2[2][k+ix9[i]]<=0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)cha[i]=wd2[2][k+ix9[i]]-wd2[2][k];for(i=0;i<4;i++)a[i]=abs(cha[i]+cha[8-i]);wd2[0][k]=a[0]+a[1]+a[2]+a[3];}fq=fopen("DAT\\wdf2.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd2[0][k]/100;dsply();Sleep(1000);if(!(fq=fopen("DAT\\wd1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);FillMemory(wd2[0],2*HW,0);for(k=0;k<HW;k++){if(wd2[1][k]<=0)continue;flg=0;for(i=0;i<9;i++){if(wd2[1][k+ix9[i]]<=0)flg++;}if(flg>0)continue;for(i=0;i<9;i++)cha[i]=wd2[1][k+ix9[i]]-wd2[1][k];for(i=0;i<4;i++)a[i]=abs(cha[i]+cha[8-i]);wd2[0][k]=a[0]+a[1]+a[2]+a[3];}fq=fopen("DAT\\wdf1.dat","wb");fwrite(wd2[0],2,HW,fq);fclose(fq);for(k=0;k<HW;k++)b0[k]=wd2[0][k]/100;dsply();Sleep(1000);
//addr		DW2 d;if(!(fq=fopen("DAT\\wdf2.dat","rb")))return;fread(wd2[2],2,HW,fq);fclose(fq);if(!(fq=fopen("DAT\\wdf1.dat","rb")))return;fread(wd2[1],2,HW,fq);fclose(fq);D2.clear();for(i0=0;i0<HEIGHT;i0+=11)for(j0=0;j0<WIDTH;j0+=11){FillMemory(&d,sizeof(D2),0);min=99999;max=0;for(i=0;i<=10;i++)for(j=0;j<=10;j++){k=(i0+i)*WIDTH+j0+j;if(wd2[2][k]>max){max=wd2[2][k];l=k;}if(wd2[2][k]<min){min=wd2[2][k];}}if(min<=0)max=0;d.dw[0]=l;d.dw[1]=max;min=99999;max=0;for(i=0;i<=10;i++)for(j=0;j<=10;j++){k=(i0+i)*WIDTH+j0+j;if(wd2[1][k]>max){max=wd2[1][k];l=k;}if(wd2[1][k]<min){min=wd2[1][k];}}if(min<=0)max=0;d.dw[2]=l;d.dw[3]=max;if(d.dw[1]>0&&d.dw[3]>0)D2.push_back(d);}sprintf(st,"size=%d",D2.size());m_lst.AddString(st);fp=fopen("DAT\\log3.txt","w");for(k=0;k<D2.size();k++){sprintf(st,"%04d %6d %6d %6d %6d ",k,D2[k].dw[0],D2[k].dw[1],D2[k].dw[2],D2[k].dw[3]);WRTX;}fclose(fp);fp=fopen("DAT\\addr.txt","w");for(k=0;k<D2.size();k++){i0=D2[k].dw[0]/WIDTH;j0=D2[k].dw[0]%WIDTH;i1=D2[k].dw[2]/WIDTH;j1=D2[k].dw[2]%WIDTH;x=j1-j0;y=i1-i0;r=sqrt(x*x+y*y);sprintf(st,"%04d %6d %6d %9.3f",k,D2[k].dw[0],D2[k].dw[2],r);WRTX;}fclose(fp);fq=fopen("DAT\\addr.dat","wb");for(k=0;k<D2.size();k++)fwrite(&D2[k],sizeof(D2),1,fq);fclose(fq);
//X&XX	{float x[9]={-1,0,-1,-1,0,-1,-1,0,-1};float y[9]={-1,-1,-1,0,0,0,-1,-1,-1};float X[9][6];float XX[6][6]={0};for(i=0;i<9;i++){X[i][0]=x[i]*x[i];X[i][1]=x[i]*y[i];X[i][2]=y[i]*y[i];X[i][3]=x[i];X[i][4]=y[i];X[i][5]=1;}for(i=0;i<9;i++){sprintf(st,"%d %5.0f %5.0f %5.0f %5.0f %5.0f %5.0f ",i,X[i][0],X[i][1],X[i][2],X[i][3],X[i][4],X[i][5]);m_lst.AddString(st);}for(i=0;i<6;i++){for(j=0;j<6;j++){for(k=0;k<9;k++)XX[i][j]+=X[i][k]*X[j][k];}}m_lst.AddString("");for(i=0;i<6;i++){sprintf(st,"%d %5.0f %5.0f %5.0f %5.0f %5.0f %5.0f ",i,XX[i][0],XX[i][1],XX[i][2],XX[i][3],XX[i][4],XX[i][5]);m_lst.AddString(st);}fq=fopen("DAT\\X.dat","wb");fwrite(X,4,6*9,fq);fclose(fq);fq=fopen("DAT\\XX.dat","wb");fwrite(XX,4,6*6,fq);fclose(fq);}
//===============================================
//dv0		
	k3=44;
/*	for(k4=0;k4<2;k4++){
		sprintf(st,"DAT\\%d\\dv.dat",k4);
		if(!(fq=fopen(st,"rb")))return;
		if(fseek(fq,12*25*k3,SEEK_SET)==0)
		{
		fread(g2[k4],12,25,fq);
		fclose(fq);m_lst.AddString("Here1");}
	}*/
	
/*	if(!(fq=fopen("DAT\\0\\dv.dat","rb")))return;fseek(fq,12*25*k3,SEEK_SET);//fseek(fq,12*25*10,SEEK_SET);
	fread(f,12,25,fq);fclose(fq);
	fq=fopen("DAT\\dv0.dat","wb");fwrite(f,12,25,fq);fclose(fq);
	if(!(fq=fopen("DAT\\1\\dv.dat","rb")))return;fseek(fq,12*25*k3,SEEK_SET);//fseek(fq,12*25*10,SEEK_SET);
	fread(g,12,25,fq);fclose(fq);
	fq=fopen("DAT\\dv1.dat","wb");fwrite(g,12,25,fq);fclose(fq);*/

//La&r3		
	for(k4=0;k4<2;k4++){
		sprintf(st,"DAT\\%d\\dv.dat",k4);
		if(!(fq=fopen(st,"rb")))return;
		if(fseek(fq,12*25*k3,SEEK_SET)==0)
		{
		fread(g2[k4],12,25,fq);
		fclose(fq);m_lst.AddString("Here1");}

		sprintf(st,"JPH\\%d\\%03d.jpg",k4,k3);okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(500);
		for(k0=0;k0<25;k0++){i0=g2[k4][k0].y;j0=g2[k4][k0].x;for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;}
		dsply();Sleep(1000);}
return;	
	x=y=0;for(k0=0;k0<25;k0++){x+=g2[1][k0].x-g2[0][k0].x;y+=g2[1][k0].y-g2[0][k0].y;}x/=25;y/=25;sprintf(st,"offset: x=%9.3f y=%9.3f",x,y);m_lst.AddString(st);FillMemory(b0,HW,0);for(k0=0;k0<25;k0++){i0=g2[0][k0].y;j0=g2[0][k0].x;for(i=-5;i<=5;i++)for(j=-5;j<=5;j++)b0[(i+i0)*WIDTH+j+j0]=85;i0=g2[1][k0].y-y;j0=g2[1][k0].x-x;for(i=-5;i<=5;i++)for(j=-5;j<=5;j++)b0[(i+i0)*WIDTH+j+j0]+=170;}dsply();offset.x=x;offset.y=y;offset.z=0;fq=fopen("DAT\\offset.dat","wb");fwrite(&offset,12,1,fq);fclose(fq);fp=fopen("DAT\\r3.txt","w");fq=fopen("DAT\\La.dat","wb");for(k4=0;k4<2;k4++){m_lst.AddString("");for(k0=0;k0<24;k0++){for(i=0;i<3;i++){f3[i]=g2[k4][(k0+i*8)%24];}for(i=0;i<3;i++){f3[i].x-=W2;f3[i].y-=H2;f3[i].z=m_id;}GetQ4rmDot(f3,edge,Q,La);for(i=0;i<3;i++){fj=La[(i+1)%3];fi=La[i];x=fj.x-fi.x;y=fj.y-fi.y;z=fj.z-fi.z;r3[i]=sqrt(x*x+y*y+z*z);}sprintf(st,"r3: %9.5f %9.5f %9.5f ",r3[0],r3[1],r3[2]);m_lst.AddString(st);WRTX;fwrite(La,12,3,fq);}}fclose(fq);fclose(fp);
//La.txt	
	FXYZ f72[2][72];if(!(fq=fopen("DAT\\La.dat","rb")))return;fread(f72,12,144,fq);fclose(fq);fp=fopen("DAT\\La.txt","w");for(k4=0;k4<2;k4++){for(k0=0;k0<24;k0++){m_lst.AddString("");SPACE;for(i=0;i<3;i++){fi=f72[k4][k0*3+i];sprintf(st,"%03d %9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z);m_lst.AddString(st);WRTX;}}}fclose(fp);

	if(!(fq=fopen("DAT\\La.dat","rb")))return;fread(f72,12,144,fq);fclose(fq);
	fp=fopen("DAT\\log.txt","w");
	for(k4=0;k4<2;k4++){
		cld2eig(f72[k4],72,Q2[k4],A);
		SPACE;
		m_lst.AddString("Q2[k4]:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);m_lst.AddString(st);WRTX;}
		sprintf(st,"A: %9.3f %9.3f %9.3f",A[0],A[4],A[8]);m_lst.AddString(st);
		cntr[k4].x=Q2[k4][3];
		cntr[k4].y=Q2[k4][7];
		cntr[k4].z=Q2[k4][11];
	}
	CopyMemory(Qinv,Q2[1],64);inv4(Qinv);
	mmtrx4(Q2[0],Qinv,U);
	m_lst.AddString("U:");SPACE;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}	
	Q2v(Q2[0],v);
	v[0]=sbstrct(cntr[0],f72[0][0]);v[0]=guiy(v[0]);
	v[1]=normal(v[2],v[0]);
	v2Q(Q2[0],v);
	Q2v(Q2[1],v);
	v[0]=sbstrct(cntr[0],f72[0][0]);v[0]=guiy(v[0]);
	v[1]=normal(v[2],v[0]);
	v2Q(Q2[1],v);
	for(k4=0;k4<2;k4++){m_lst.AddString("Q2[k4]:");SPACE;for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",Q2[k4][i*4+0],Q2[k4][i*4+1],Q2[k4][i*4+2],Q2[k4][i*4+3]);m_lst.AddString(st);WRTX;}}
	fq=fopen("DAT\\Q2.dat","wb");fwrite(Q2,64,2,fq);fclose(fq);
	fq=fopen("DAT\\d25.dat","wb");
	for(k4=0;k4<2;k4++){
		for(k0=0;k0<25;k0++){
			af=pi*k0*15/180;
			if(k0<24){fi.x=RADIUS*cos(af);fi.y=RADIUS*sin(af);}else fi.x=fi.y=0;
			fi.z=0;
			fi=mvct4a(Q2[k4],fi);fwrite(&fi,12,1,fq);
			sprintf(st,"%d %02d %9.3f %9.3f %9.3f ",k4,k0,fi.x,fi.y,fi.z);m_lst.AddString(st);WRTX;
		}
	}
	fclose(fq);
	CopyMemory(Qinv,Q2[1],64);inv4(Qinv);
	mmtrx4(Q2[0],Qinv,U);SPACE;
	m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}
	Q2r1t(U,rot);fi=rot[0];fj=rot[1];SPACE;
	sprintf(st,"rot %9.4f %9.4f %9.4f |%9.4f %9.4f %9.4f ",fi.x*180/pi,fi.y*180/pi,fi.z*180/pi,fj.x,fj.y,fj.z);m_lst.AddString(st);WRTX;
	SPACE;
	r1t2Q(U,rot);
	m_lst.AddString("U:");for(i=0;i<4;i++){sprintf(st,"%9.6f %9.6f %9.6f %9.3f",U[i*4+0],U[i*4+1],U[i*4+2],U[i*4+3]);m_lst.AddString(st);WRTX;}
	fq=fopen("DAT\\U.dat","wb");fwrite(U,64,1,fq);fclose(fq);
	fclose(fp);
	fp=fopen("DAT\\Lb.txt","w");
	if(!(fq=fopen("DAT\\d25.dat","rb")))return;fread(g2,12,50,fq);fclose(fq);
	for(k4=0;k4<2;k4++){
		for(k0=0;k0<24;k0++){
			SPACE;
			for(i=0;i<3;i++){
				fi=g2[k4][(k0+i*8)%24];
				sprintf(st,"%02d %9.3f %9.3f %9.3f ",k0*3+i,fi.x,fi.y,fi.z);m_lst.AddString(st);WRTX;
			}
		}
	}

	fp=fopen("DAT\\fifj.txt","w");
	for(k0=0;k0<25;k0++){
		fi=g2[0][k0];fj=g2[1][k0];
		fj=mvct4a(U,fj);
		sprintf(st,"%02d %9.3f %9.3f %9.3f | %9.3f %9.3f %9.3f",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);WRTX;
	}
	SPACE;
	if(!(fq=fopen("DAT\\La.dat","rb")))return;
	for(k0=0;k0<72;k0++){
		fseek(fq,12*k0,SEEK_SET);
		fread(&fi,12,1,fq);
		fseek(fq,12*(k0+72),SEEK_SET);
		fread(&fj,12,1,fq);	fj=mvct4a(U,fj);

		sprintf(st,"%02d %9.3f %9.3f %9.3f |%9.3f %9.3f %9.3f ",k0,fi.x,fi.y,fi.z,fj.x,fj.y,fj.z);m_lst.AddString(st);WRTX;
	}
	fclose(fp);



	m_lst.AddString("");
	m_lst.AddString("Fininshe");
}

void CTmdDlg::OnButton5() 
{
	// TODO: Add your control notification handler code here
	m_lst.ResetContent();


//	m_lst.AddString("");
//	m_lst.AddString("Finished");
}


void CTmdDlg::OnButton6() 
{
	// TODO: Add your control notification handler code here
	char st[256];
	int i,j,i0,j0,i1,j1,k0,k1,k2,k3,k4,wk,kcnt;
	long k,l,n;
	float x,y,z,r,af,Q[16],A[9],Q0[16],R[12],edge,r3[3],min,max,mean,Qinv[16],U[16],Ua[16],Q2[2][16],A2[2][9];
	FXYZ fi,fj,fk,fl,f[25],g[25],g2[2][25],f3[3],f4[4],f7[73],La[4],La2[2][360],v[4],rot[2];
	FILE *fp,*fq,*fr,*fw;
	FILE *fs[2];
	FXYZ offset,cntr[2];
	edge=2*RADIUS*cos(pi/6);
	for(k4=0;k4<2;k4++){
		sprintf(st,"DAT\\%d\\dv.dat",k4);if(!(fq=fopen(st,"rb")))return;
		for(k0=0;k0<360;k0++){
			sprintf(st,"JPH\\%d\\%03d.jpg",k4,k0);SetWindowText(st);
			okLoadImageFile(hBrd,st,0,BUFFER,0,1);dsply();Sleep(50);
			fread(f,12,25,fq);
			for(k1=0;k1<25;k1++){
				i0=f[k1].y;j0=f[k1].x;
				for(i=-3;i<=3;i++)for(j=-3;j<=3;j++)b0[(i+i0)*WIDTH+j+j0]^=255;
			}

			dsply();Sleep(200);
		}
		fclose(fq);


	}

	m_lst.AddString("");
	m_lst.AddString("finished");
}

void CTmdDlg::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default


	CDialog::OnRButtonUp(nFlags, point);
}

FXYZ stdd3(float *a,int n) 
{
	FXYZ ret;
	float sum=0,ave,x,sg;
	int k;
	sum=0;for(k=0;k<n;k++)sum+=a[k];ave=sum/n;
	sum=0;for(k=0;k<n;k++){x=a[k]-ave;sum+=x*x;}
	sg=sqrt(sum/n);
	ret.x=ave;ret.y=sg;ret.z=n;
	return ret;
}
void Sort7z(FXYZ arr[],long cnt,BYTE flg){
	int smallNo;  
	int pass,j;     
	FXYZ temp; 
	if(flg==0){
	for (pass=0;pass<cnt-1;pass++){ 
		smallNo=pass; 
		for(j=pass+1;j<cnt;j++) 
			if(arr[j].z<arr[smallNo].z)smallNo=j; 
		if(smallNo!=pass){ 
			temp=arr[pass]; 
			arr[pass]=arr[smallNo]; 
			arr[smallNo]=temp; 
		} 
	} 
	}
	else{
	for (pass=0;pass<cnt-1;pass++){ 
		smallNo=pass; 
		for(j=pass+1;j<cnt;j++) 
			if(arr[j].z>arr[smallNo].z)smallNo=j; 
		if(smallNo!=pass){ 
			temp=arr[pass]; 
			arr[pass]=arr[smallNo]; 
			arr[smallNo]=temp; 
		} 
	} 
	}
}

void CTmdDlg::OnChangeEdit1() 
{
	UpdateData(true);	
}


void genXX(FXYZ *xy,long n,float *XX)
{
	long k;
	float xx,Xy,yy,x,y,z;
	FillMemory(XX,sizeof(float)*30,0);
	for(k=0;k<n;k++)	
	{
		if(xy[k].x==0.&&xy[k].y==0.)continue;
		{
		xx=xy[k].x*xy[k].x;
		Xy=xy[k].x*xy[k].y;
		yy=xy[k].y*xy[k].y;
		x=xy[k].x;
		y=xy[k].y;
		z=-xx;

		XX[0]+=yy*yy;
		XX[6]+=Xy*Xy;
		XX[12]+=xx;
		XX[18]+=yy;
		XX[24]+=1;

		XX[1]+=yy*Xy;
		XX[2]+=yy*x;
		XX[3]+=yy*y;
		XX[4]+=yy;

		XX[7]+=Xy*x;
		XX[8]+=Xy*y;
		XX[9]+=Xy;

		XX[13]+=Xy;
		XX[14]+=x;

		XX[19]+=y;

		XX[25]+=yy*z;
		XX[26]+=Xy*z;
		XX[27]+=x*z;
		XX[28]+=y*z;	
		XX[29]+=z;	
		}
	}

//对称
	XX[5]=XX[1];

	XX[10]=XX[2];
	XX[11]=XX[7];

	XX[15]=XX[3];
	XX[16]=XX[8];
	XX[17]=XX[13];

	XX[20]=XX[4];
	XX[21]=XX[9];
	XX[22]=XX[14];
	XX[23]=XX[19];
}

ELPS fitElp(FXYZ *xy,long n)
{
	FXYZ zr={0.,0.,0.};FXY p;
//	char st[80];
	ELPS e;
	float *XX=(float *)new float[30];
	long k,kcnt;
	float U,V,W,X,Y,Z,Q,C2,C4,xp,xm;
	float xc,yc,a,b,bt,A,B,C,sume,w0,w3,x,y,r;
	FXYZ xy0;
	int ret;
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());//char st[80];
	xy0=xy[0];
	genXX(xy,n,XX);
//	for(i=0;i<6;i++){sprintf(st,"%16.3f %16.3f %16.3f %16.3f %16.3f",XX[i*5],XX[i*5+1],XX[i*5+2],XX[i*5+3],XX[i*5+4]);pDlg->m_lst.AddString(st);}
	ret=all_gauss(5,XX,&XX[25]);
	if(ret==0){/*AfxMessageBox("all_gauss fail!");*/FillMemory(&e,sizeof(ELPS),0);return e;}

//pDlg->m_lst.AddString("");	sprintf(st,"%16.3f %16.3f %16.3f %16.3f %16.3f",XX[25],XX[26],XX[27],XX[28],XX[29]);pDlg->m_lst.AddString(st);
//pDlg->m_lst.AddString("");

	yc=(2*XX[28]/XX[26]-XX[27])/(XX[26]-4*XX[25]/XX[26]);	//圆心
	xc=(-XX[26]*yc-XX[27])/2.;e.c.x=xc;e.c.y=yc;			//圆心

	Q=(1.-XX[25])/XX[26];

	C=Q+sqrt(Q*Q+1.);
	bt=atan(C);

	e.bt=bt;										//倾角

	U=1./(xc*xc+XX[25]*yc*yc+XX[26]*xc*yc-XX[29]);
	V=XX[25]*U;
	W=XX[26]*U;
	X=XX[27]*U;
	Y=XX[28]*U;
	Z=XX[29]*U-1;

//pDlg->m_lst.AddString("");
//	sprintf(st,"%15.12f %15.12f %15.12f",U,V,W);pDlg->m_lst.AddString(st);
//	sprintf(st,"%15.12f %15.12f %15.12f",X,Y,Z);pDlg->m_lst.AddString(st);
//pDlg->m_lst.AddString("");
	C2=C*C;
	C4=C2*C2;
	A=sqrt((U-C2*V)/(1.-C4));
	B=sqrt((V-C2*U)/(1.-C4));

	a=cos(bt)/A;e.a=a;										//横轴
	b=cos(bt)/B;e.b=b;										//竖轴


//==================================
//==================================
//(b(x-xc))^2+(a(y-yc))^2-(ab)^2=0  
//(a(y-yc))^2=(ab)^2-(b(x-xc))^2
//x=xc+sqrt(b^2-(y-yc)^2)*a/b

/*	sume=0.;kcnt=0;		//计算均方差
	for(k=0;k<n;k++)	
	{
		if(xy[k].x==0.&&xy[k].y==0.)continue;
		p.x=(xy[k].x-xc)*cos(bt)-(xy[k].y-yc)*sin(bt);
		p.y=(xy[k].x-xc)*sin(bt)+(xy[k].y-yc)*cos(bt);
		w0=(b+p.y)*(b-p.y);
		if(w0<0.)continue;
		xp=+sqrt(w0)*a/b;
		xm=-sqrt(w0)*a/b;	
		if(fabs(p.x-xp)<fabs(p.x-xm))xy[k].z=w3=p.x-xp;else xy[k].z=w3=p.x-xm;
		sume+=(w3*w3);kcnt++;		
	}
	sume/=kcnt;
	e.me=sqrt(sume);
	for(k=0;k<n;k++)if(fabs(xy[k].z)>1.*e.me)xy[k]=zr;	*/

	sume=0.;kcnt=0;
	for(k=0;k<n;k++)	
	{
		if(xy[k].x==0.&&xy[k].y==0.)continue;
		p.x=(xy[k].x-xc)*cos(bt)-(xy[k].y-yc)*sin(bt);
		p.y=(xy[k].x-xc)*sin(bt)+(xy[k].y-yc)*cos(bt);
		w0=p.x*p.x/(a*a)+p.y*p.y/(b*b)-1.;
		xy[k].z=w0=fabs(w0);
		sume+=w0;kcnt++;	
	}
	sume/=kcnt;
	e.me=sqrt(sume);
//	for(k=0;k<n;k++)if(fabs(xy[k].z)>0.06)xy[k]=zr;   //计算点到椭圆圆周的距离，并过滤****过滤参数1
	for(k=0;k<n;k++)if(fabs(xy[k].z)>0.03)xy[k]=zr;    //计算点到椭圆圆周的距离，并过滤****过滤参数1

//======================================	
	genXX(xy,n,XX);
//	for(i=0;i<6;i++){sprintf(st,"%16.3f %16.3f %16.3f %16.3f %16.3f",XX[i*5],XX[i*5+1],XX[i*5+2],XX[i*5+3],XX[i*5+4]);pDlg->m_lst.AddString(st);}
	ret=all_gauss(5,XX,&XX[25]);
	if(ret==0){/*AfxMessageBox("all_gauss fail!");*/FillMemory(&e,sizeof(ELPS),0);return e;}

//pDlg->m_lst.AddString("");	sprintf(st,"%16.3f %16.3f %16.3f %16.3f %16.3f",XX[25],XX[26],XX[27],XX[28],XX[29]);pDlg->m_lst.AddString(st);
//pDlg->m_lst.AddString("");

	yc=(2*XX[28]/XX[26]-XX[27])/(XX[26]-4*XX[25]/XX[26]);	//圆心
	xc=(-XX[26]*yc-XX[27])/2.;e.c.x=xc;e.c.y=yc;			//圆心
	
	Q=(1.-XX[25])/XX[26];
	C=Q+sqrt(Q*Q+1.);
	bt=atan(C);

 //	if(bt>pi/4.)bt-=pi/2.;

	e.bt=bt;										//倾角??????????

	U=1./(xc*xc+XX[25]*yc*yc+XX[26]*xc*yc-XX[29]);
	V=XX[25]*U;
	W=XX[26]*U;
	X=XX[27]*U;
	Y=XX[28]*U;
	Z=XX[29]*U-1;

//pDlg->m_lst.AddString("");
//	sprintf(st,"%15.12f %15.12f %15.12f",U,V,W);pDlg->m_lst.AddString(st);
//	sprintf(st,"%15.12f %15.12f %15.12f",X,Y,Z);pDlg->m_lst.AddString(st);
//pDlg->m_lst.AddString("");

	C2=C*C;
	C4=C2*C2;
	A=sqrt((U-C2*V)/(1.-C4));
	B=sqrt((V-C2*U)/(1.-C4));

	a=cos(bt)/A;e.a=a;										//横轴
	b=cos(bt)/B;e.b=b;										//竖轴
	if(a<b){e.a=b;e.b=a;e.bt-=pi/2;}
	
	x=xy0.x-e.c.x;y=xy0.y-e.c.y;r=sqrt(x*x+y*y);
	w0=asin(y/r);if(x<0)w0=pi-w0;
	e.ag=w0;
//	sprintf(st,"xy0 %9.3f %9.3f %9.4f°",xy0.x,xy0.y,w0*180/pi);pDlg->m_lst.AddString(st);
//	sprintf(st,"xyr %9.3f %9.3f %9.3f",x,y,r);pDlg->m_lst.AddString(st);
//	sprintf(st,"e.c %9.3f %9.3f",e.c.x,e.c.y);pDlg->m_lst.AddString(st);

//================================
	sume=0.;kcnt=0;		//计算均方差
	for(k=0;k<n;k++)	
	{
		if(xy[k].x==0.&&xy[k].y==0.)continue;
		p.x=(xy[k].x-xc)*cos(bt)-(xy[k].y-yc)*sin(bt);
		p.y=(xy[k].x-xc)*sin(bt)+(xy[k].y-yc)*cos(bt);
		w0=(b+p.y)*(b-p.y);
		if(w0<0.)continue;
		xp=+sqrt(w0)*a/b;
		xm=-sqrt(w0)*a/b;	
		if(fabs(p.x-xp)<fabs(p.x-xm))xy[k].z=w3=p.x-xp;else xy[k].z=w3=p.x-xm;
		sume+=(w3*w3);kcnt++;		
	}
	sume/=kcnt;
	e.me=sqrt(sume);
//	for(k=0;k<n;k++)if(fabs(xy[k].z)>1.*e.me)xy[k]=zr;	       //计算均方差，并过滤****过滤参数2

//	if(e.bt>1.)e.bt=fabs(e.bt-(pi/2.));
//	if(e.bt>pi/4.)e.bt=fabs(e.bt-(pi/2.));

	delete [] XX;

	return e;
}

float tsFx(float a,FXYZ t1,FXYZ c0)
{
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());//char st[80];
	float x,y,z,r;
	x=a*t1.x-c0.x;
	y=a*t1.y-c0.y;
	z=a*t1.z-c0.z;;
	r=sqrt(x*x+y*y+z*z);
//	sprintf(st,"xyz %9.3f %9.3f %9.3f %9.3f",x,y,z,r-RADIUS);pDlg->m_lst.AddString(st);
//	sprintf(st,"c0 %9.3f %9.3f %9.3f",c0.x,c0.y,c0.z);pDlg->m_lst.AddString(st);
//	sprintf(st,"t1 %9.6f %9.6f %9.6f",t1.x,t1.y,t1.z);pDlg->m_lst.AddString(st);
	return r-RADIUS;
}
float tsFind(float a,float b,FXYZ t1,FXYZ c0)
{
	CTmdDlg *pDlg = (CTmdDlg *)(AfxGetApp()->GetMainWnd());//char st[80];
	float lb,fa,fb,fm,m;
	m=(a+b)/2;
//	sprintf(st,"a=%9.3f b=%9.3f",a,b);pDlg->m_lst.AddString(st);
	fa=tsFx(a,t1,c0);
	fb=tsFx(b,t1,c0);
	if(fa*fb>0){pDlg->m_lst.AddString("fa*fb>0!");return 0;}
	fm=tsFx(m,t1,c0);
	while(fabs(a-b)>0.0001){
		m=(a+b)/2;
		if(fa*fm>0){fa=fm;a=m;}else{fb=fm;b=m;}
		fm=tsFx(m,t1,c0);
//		sprintf(st,"%9.3f %9.3f %9.3f|%9.3f %9.3f %9.3f",a,b,m,fa,fb,fm);pDlg->m_lst.AddString(st);
//		sprintf(st,"fa=%9.3f fb=%9.3f fm=%9.3f",fa,fb,fm);pDlg->m_lst.AddString(st);
	}
	lb=a;
	return lb;
}

void CTmdDlg::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
/*	char st[80];
	int i0,j0,i,j,k0,k1;;
	CPoint lt;
	float sum,isum,jsum;
	FXYZ fi;
	i0=point.y*3/2;j0=point.x*3/2;
//	sprintf(st,"i0=%4d j0=%4d",i0,j0);m_lst.AddString(st);
	lt.x=rct0.left =j0-THRT;rct0.right =j0+THRT;
	lt.y=rct0.top =i0-THRT;rct0.bottom =i0+THRT;	
//	sprintf(st,"%4d %4d %4d %4d",rct0.left,rct0.top,rct0.right,rct0.bottom);m_lst.AddString(st);
	FILE *fp;
	if(!(fp=fopen("DAT\\dt.dat","rb"))){m_lst.AddString("data not found!");return;}
	fread(dt,12,150,fp);fclose(fp);
	k0=m_cmb.GetCurSel();
	k1=m_dot.GetCurSel();
	BLOCKINFO blk;
	BYTE *bf=(BYTE *)new BYTE [THR4];
	BYTE *bw=(BYTE *)new BYTE [THR4];
	FillMemory(&blk,sizeof(blk),0);
	blk.iBitCount=8;
	blk.iFormType=FORM_GRAY8;
	blk.iHeight=THR2;
	blk.iWidth=THR2;
	blk.lpBits = bw;
	for(i=0;i<THR2;i++)for(j=0;j<THR2;j++){
		bf[i*THR2+j]=b0[(lt.y+i)*W2+lt.x+j];
	}
	im2bw(bf,bw,THR2,THR2);
	sum=isum=jsum=0;
	for(i=0;i<THR2;i++)for(j=0;j<THR2;j++){if(bw[i*THR2+j]>127){sum++;isum+=i;jsum+=j;}}
	isum/=sum;jsum/=sum;
	i0=isum;j0=jsum;
	for(i=-1;i<=1;i++)for(j=-1;j<=1;j++)bw[(i0+i)*THR2+j0+j]=0;

	RECT rct;SetRect(&rct,2,40,2+THR2,40+THR2);
	okSetTargetRect(hBrd,SCREEN,&rct);	
	okConvertRect(hBrd,SCREEN,0,TARGET(&blk),0,1);
	delete [] bf,bw;
	dt[k0][k1].x=lt.x+jsum;
	dt[k0][k1].y=lt.y+isum;
	fi=dt[k0][k1];
	sprintf(st,"%d %02d | %9.3f,%9.3f",k0,k1,fi.x,fi.y);m_lst.AddString(st);

	i0=fi.y;j0=fi.x;

	for(i=-2;i<=2;i++)for(j=-2;j<=2;j++)b0[(i+i0)*W2+j+j0]=0;
	dsply();

	k1=m_dot.GetCurSel();
	k1++;k1%=25;m_dot.SetCurSel(k1);
	if(k1==0){
		k0=m_cmb.GetCurSel();
		k0++;k0%=6;m_cmb.SetCurSel(k0);
	}
	fp=fopen("DAT\\dt.dat","wb");fwrite(dt,12,150,fp);fclose(fp);


*/

	CDialog::OnLButtonDblClk(nFlags, point);
}
void dsply(){
	RECT rct;
	SetRect(&rct,2,40,2+WIDTH,40+HEIGHT);
	okSetTargetRect(hBrd,SCREEN,&rct);
	okConvertRect(hBrd,SCREEN,0,BUFFER,0,1);
}


/*void o2Rex(FXYZ ro,float *R)
{
	float 	cz,cy,cx,sz,sy,sx;
	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);
	cz=cos(ro.z);cy=cos(ro.y);cx=cos(ro.x);
	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);
	R[0]=cy*cz; R[1]=sx*sy*cz-cx*sz; R[2]=cx*sy*cz+sx*sz;
	R[3]=cy*sz; R[4]=sx*sy*sz+cx*cz; R[5]=cx*sy*sz+sx*cz;
	R[6]=-sy;   R[7]=sx*cy;          R[8]=cx*cy;
}*/

void o_Rex(FXYZ ro,float *R)
{
	float 	cz,cy,cx,sz,sy,sx;//,W[4];
	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);//教科书
	cz=cos(ro.z);cy=cos(ro.y);cx=cos(ro.x);
	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);

/*	R[0]=cx*cz-sx*sy*sz;  R[1]=-cx*sz-sx*sy*cz; R[2]=-sx*cy;
	R[3]=cy*sz;           R[4]=cy*cz;           R[5]=-sy;
	R[6]=sx*cz+cx*sy*sz;  R[7]=-sx*sz+cx*sy*cz; R[8]=cx*cy;*/


	R[0]=cy*cz-sy*sx*sz;  R[1]=-cy*sz-sy*sx*cz; R[2]=-sy*cx;
	R[3]=cx*sz;           R[4]=cx*cz;           R[5]=-sx;
	R[6]=sy*cz+cy*sx*sz;  R[7]=-sy*sz+cy*sx*cz; R[8]=cy*cx;

//	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);//黄桂平顺序
//	cz=cos(ro.z);cy=cos(ro.y);cx=cos(ro.x);
//	sz=sin(ro.z);sy=sin(ro.y);sx=sin(ro.x);
//	R[0]=cy*cz;           R[1]=-cy*sz;          R[2]=sy;
//	R[3]=sx*sy*cz+cx*sz;  R[4]=-sx*sy*sz+cx*cz; R[5]=-sx*cy;
//	R[6]=-cx*sy*cz+sx*sz; R[7]=cx*sy*sz+sx*cz;  R[8]=cx*cy;

}
FXYZ R_Oex(float *R)
{
	FXYZ ro;
	ro.y= atan(-R[2]/R[8]);//教科书 
	ro.x= asin(-R[5]); 
	ro.z= atan(R[3]/R[4]);

//	ro.x= atan(-R[5]/R[8]);//黄桂平算法 
//	ro.y= asin(R[2]); 
//	ro.z= atan(-R[1]/R[0]);
	return ro;
}
