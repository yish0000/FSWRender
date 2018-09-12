/*
 * -----------------------------------------------------------------------
 *  Name:   WinApp.cpp
 *  Desc:   本文件实现了一个窗口应用程序对象。
 *  Author: Yish
 *  Date:   2011/7/20
 *  ---------------------------------------------------------------------
 *  CopyRight (C) YishSoft. All right Observed.
 * -----------------------------------------------------------------------
 */


#define _WIN32_WINNT 0x0400

#include "WinApp.h"
#include "FSWDevice.h"
#include "LogFile.h"
#include "InputFilter.h"
#include "SysInputFilter.h"
#include "CameraInputFilter.h"
#include "FSWScene.h"
#include "FSWSceneNode.h"
#include "FSWModel.h"

#include "SimpleSceneListener.h"


///////////////////////////////////////////////////////////////////////////
//  
//  Local types and global variables
//  
///////////////////////////////////////////////////////////////////////////

const int WND_WIDTH = 800;
const int WND_HEIGHT = 600;


///////////////////////////////////////////////////////////////////////////
//  
//  Implement class WinApp
//  
///////////////////////////////////////////////////////////////////////////

WinApp* WinApp::s_pInst;

// 窗口类
static char g_szWndClass[] = "FSW_TestWindow";

/** 构造函数
*/
WinApp::WinApp(void) : m_hInst(NULL),m_hWnd(NULL),m_pDevice(NULL),
	m_pScene(NULL), m_pInput(NULL), m_pSysFilter(NULL), m_pCamFilter(NULL)
{
	m_bActive = true;
	m_bFullscreen = false;
	m_dwStyle = 0;

	s_pInst = this;
}

/** 析构函数
*/
WinApp::~WinApp(void)
{
    s_pInst = NULL;
}

/** 获取程序对象指针
*/
WinApp* WinApp::GetInstance()
{
	ASSERT(s_pInst);
	return s_pInst;
}

/** 初始化应用程序
*/
bool WinApp::Init( HINSTANCE hInst )
{
    m_hInst = hInst;

    // 注册窗口类
    WNDCLASSEX wnd;
    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject( BLACK_BRUSH );
    wnd.hCursor = LoadCursor( NULL,IDC_ARROW );
    wnd.hIcon = LoadIcon( NULL,IDI_APPLICATION );
    wnd.hIconSm = NULL;
    wnd.hInstance = hInst;
    wnd.lpfnWndProc = (WNDPROC)MsgProc;
    wnd.lpszClassName = g_szWndClass;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW|CS_VREDRAW;
    RegisterClassEx( &wnd );

	RECT rcWnd;
	m_dwStyle = WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX & ~WS_SIZEBOX;
	SetRect( &rcWnd,0,0,WND_WIDTH,WND_HEIGHT );
	AdjustWindowRect( &rcWnd,m_dwStyle,FALSE );

	// 使窗口居中
	int w = rcWnd.right - rcWnd.left;
	int h = rcWnd.bottom - rcWnd.top;
	int screenx = GetSystemMetrics( SM_CXSCREEN );
	int screeny = GetSystemMetrics( SM_CYSCREEN );
	if( w > screenx ) w = screenx;
	if( h > screeny ) h = screeny;
	int x = (screenx - w) / 2;
	int y = (screeny - h) / 2;

    // 创建窗口
    m_hWnd = CreateWindow( g_szWndClass,"FSWTest",m_dwStyle,x,y,w,h,NULL,NULL,m_hInst,NULL );
    if( !m_hWnd )
    {
        MessageBox( NULL,"创建窗口失败，程序即将推出！","错误",MB_OK|MB_ICONERROR );
        return false;
    }

    ShowWindow( m_hWnd,SW_SHOW );
    UpdateWindow( m_hWnd );

    // 初始化图形设备
    return Init3DEngine();
}

/** 应用程序主循环
*/
int WinApp::Run(void)
{
    MSG msg;
	BOOL bGotMsg;
    memset( &msg,0,sizeof(MSG) );

    while( msg.message != WM_QUIT )
    {
		if( m_bActive )
			bGotMsg = PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
		else
			bGotMsg = GetMessage(&msg, NULL, 0, 0);

        if( bGotMsg )
        {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
        }
        else
        {
            // 绘制每一帧
            if( m_bActive ) OnFrame();
        }
    }

    UnregisterClass( g_szWndClass,m_hInst );
    return ExitInstance();
}

/** 退出应用程序
*/
int WinApp::ExitInstance(void)
{
	for( size_t i=0;i<m_Listeners.size();++i )
		SAFE_DELETE( m_Listeners[i] );

	SAFE_DELETE( m_pScene );
	SAFE_DELETE( m_pSysFilter );
	SAFE_DELETE( m_pCamFilter );
	SAFE_DELETE( m_pInput );
    SAFE_DELETE( m_pDevice );
    return 0;
}

/** 初始化图形设备
*/
bool WinApp::Init3DEngine(void)
{
    RECT rc;
    GetClientRect( m_hWnd,&rc );
    m_pDevice = new FSWDevice();
    if( !m_pDevice || !m_pDevice->Init(m_hWnd, rc.right, rc.bottom, m_bFullscreen) )
    {
		SAFE_DELETE( m_pDevice );
        MessageBox( NULL,"创建图形设备失败！","错误",MB_OK|MB_ICONERROR );
        return 0;
    }

    SMaterial mat;
    mat.Ambient = ColorF( 0.5f,0.5f,0.5f );
    mat.Diffuse = ColorF( 1.0f,1.0f,1.0f );
    m_pDevice->SetMaterial( mat );
    SLight light;
    light.Ambient = ColorF( 0.5f,0.5f,0.5f );
    light.Diffuse = ColorF( 1.0f,1.0f,1.0f );
    light.Direction = Vector3( 0.0f,0.0f,1.0f );
    light.Direction.Normalize();
    m_pDevice->SetLight( light );
    m_pDevice->SetLightEnable( true );

	// 场景对象
	m_pScene = new FSWScene(m_pDevice);

	// 初始化输入管理器
	m_pInput = new InputManager();

	// 程序控制
	m_pSysFilter = new SysInputFilter(m_pInput);
	m_pInput->RegisterFilter(m_pSysFilter);

	// 相机控制
	m_pCamFilter = new CameraInputFilter(m_pInput, m_pScene->GetCamera());
	m_pInput->RegisterFilter(m_pCamFilter);

	// 注册帧监听器
	m_Listeners.push_back(new SimpleSceneListener(m_pDevice));

	// 初始化场景
    return OneTimeSceneInit();
}

// 一次性初始化场景
bool WinApp::OneTimeSceneInit()
{
	// 加载一个模型
	FSWModel* pModel = dynamic_cast<FSWModel*>(
		m_pScene->CreateObject("Teapot", FSWScene::OBJECT_MODEL));
	if( !pModel || !pModel->LoadModel("Data\\model\\teapot.smd") )
	{
		FSW_LOG("WinApp::Init3DEngine, failed to load the teapot model!");
	}

	m_pScene->GetSceneRoot()->AddSceneObject(pModel);
	//m_pDevice->AttachPostEffect(POST_BW);
	return true;
}

/** 绘制每一帧
*/
void WinApp::OnFrame(void)
{
    // 更新应用程序的FPS
    if( m_Timer.Update() )
    {
        char caption[80];
        sprintf_s( caption,80,"FSWTest - FPS: %.3f",m_Timer.GetFPS() );
        SetWindowText( m_hWnd,caption );
    }

	OnTick();
	OnRender();
}

// 更新逻辑
void WinApp::OnTick()
{
	float fElapsed = m_Timer.GetElapsed();

	// 更新输入管理器
	m_pInput->Tick();

	// 更新场景
	m_pScene->Update( fElapsed );

	// 更新监听器
	for( size_t i=0;i<m_Listeners.size();i++ )
		m_Listeners[i]->Tick( fElapsed );
}

// 渲染场景
void WinApp::OnRender()
{
	m_pDevice->BeginScene();
	m_pDevice->Clear( true,true );

	// 渲染场景
	m_pScene->Render();

	// 渲染监听器
 	for( size_t i=0;i<m_Listeners.size();i++ )
 		m_Listeners[i]->Render();

	// 应用后期特效
	m_pDevice->ApplyPostEffect();

	// 结束渲染
	m_pDevice->EndScene();
	m_pDevice->Present( NULL,NULL );
}

// 切换全屏模式
void WinApp::ToggleFullscreen()
{
	m_bFullscreen = !m_bFullscreen;
	int width = m_bFullscreen ? GetSystemMetrics(SM_CXSCREEN) : WND_WIDTH;
	int height = m_bFullscreen ? GetSystemMetrics(SM_CYSCREEN) : WND_HEIGHT;
	m_pDevice->ResetDevice(width, height, m_bFullscreen);
}

/** 窗口的回调函数
*/
LRESULT CALLBACK WinApp::MsgProc( HWND hWnd,UINT uMsg,WPARAM wParam,LPARAM lParam )
{
    WinApp* app = WinApp::GetInstance();
	InputManager* pInput = app->GetInputManager();

    switch( uMsg )
    {
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
		if( uMsg == WM_KEYDOWN )
		{
			if( wParam == VK_ESCAPE ) PostQuitMessage(0);
			if( app->IsFullscreen() && (wParam == VK_LWIN || wParam == VK_RWIN) )
				CloseWindow(app->GetMainWnd());
		}

		if( pInput ) pInput->OnKeyMessage(uMsg, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
	case WM_LBUTTONUP:
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
	case WM_RBUTTONUP:
	case WM_MBUTTONDOWN:
	case WM_MBUTTONUP:
	case WM_MOUSEMOVE:
	case WM_MOUSEWHEEL:
		if( pInput ) pInput->OnMouseMessage(uMsg, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_GETMINMAXINFO:
		// 防止窗口比最小值还小
		((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
		((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
		break;
	case WM_KILLFOCUS:
		app->SetActive(false);
		break;
	case WM_SETFOCUS:
		app->SetActive(true);
		break;
    default: return DefWindowProc( hWnd,uMsg,wParam,lParam );
    }

    return 0;
}