
//#pragma comment(lib,"playH264ThreadDLL1.lib")


//解码播放时用到的数据结构
typedef struct
{
    int width;             //实际宽度
    int height;            //实际高度
    int playWidth;         //显示宽度
    int playHeight;        //显示高度
    int fps;               //播放频率
    HWND playHandle;       //播放句柄
    int stopPlay;          //播放控制
    int playChannle;		//播放的通道号
	bool isDecode;
}myparamInput;

typedef int (_cdecl *PFCALLBACK)(int INSTANCE,int width,int height,char *buf,int bufsize,int buftype);
typedef int (_cdecl *TDrawRectCallBack)(char*,int,int);
typedef int (_cdecl *TDrawLineCallBack)(int,HDC);//define my CallBack Func
typedef int (_cdecl *TBmpCallBack)(char*,int,int,int,int ,int ,int,HWND);
typedef int (_cdecl *TYUVCallBack)(char *,int len,int wid,void *);//YUV回调
typedef int (WINAPI *TH264CallBack)(int ,char *,int len,int wid,int height);//H264回调

typedef int (_cdecl *setYUVCallBack)(int ,TYUVCallBack,void *);
typedef int (_cdecl *setH264CallBack)(int ,TH264CallBack);
typedef int (_cdecl *fSetCallBack)(int INSTANCE,PFCALLBACK f1);
typedef void (_cdecl *fDecodeVideo)(int num,unsigned char * pInBuffer, int size);
typedef int (_cdecl *finitVideoDLL)();
typedef int	(_cdecl *fGetIdlevideoINSTANCE)();
typedef int (_cdecl *fInitVideoParam)(int INSTANCE,myparamInput *Myparam,int);
typedef int (_cdecl *fInitVideoParamNew)(int ,myparamInput *,int,int);
typedef int (_cdecl *fpauseVideos)(int INSTANCE);
typedef int (_cdecl *fplayVideos)(int INSTANCE);
typedef int (_cdecl *ffreeVideos)(int INSTANCE);
typedef int (_cdecl *finputBuf)(int INSTANCE,char *buf, int bufsize);
typedef int (_cdecl *fresize)(int INSTANCE,int width, int height);
typedef void (_cdecl *fexitdll)();
typedef int (_cdecl *fSetDrawLineCallBack)(int INSTANCE,TDrawLineCallBack f1);
typedef int (_cdecl *fSetBmpCallBack)(int INSTANCE,TBmpCallBack bmp1);
typedef int (_cdecl *fSetFillBmpCallBack)(int INSTANCE,TDrawRectCallBack bmpf);
typedef int (_cdecl *revoHW)(int INSTANCE);

