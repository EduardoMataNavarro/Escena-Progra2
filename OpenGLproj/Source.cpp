#include <windows.h>
#include <windowsx.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <gl\glut.h>
#include <mmsystem.h>
#include "Graphics.h"
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "glu32.lib")

#define renderTimer 100

HDC gHDC;


Graph *OGLobj;
bool render = false;

bool levUp = true, backSwim = false, backSwim2 = false, upBucket = false, ufoBack = false;
LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
void SetUpPixelFormat(HDC hDC);

//debemos declarar una variable global que maneje el acceso al dispositivo grafico


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HWND hWnd;

	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "Proyecto final progra2";
	RegisterClassEx(&wc);
	RECT wr = { 0, 0, 800, 600 };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	hWnd = CreateWindowEx(NULL,
		"Proyecto final progra2",
		"OpenGL - Escenario",
		WS_OVERLAPPEDWINDOW,
		100,
		100,
		wr.right - wr.left,
		wr.bottom - wr.top,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hWnd, nCmdShow);

	//Creamos al objeto y se lo pasamos al puntero
	OGLobj = new Graph(hWnd);

	SetTimer(hWnd, renderTimer, 60, NULL);

	MSG msg = { 0 };
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT)
				break;
		}
		else
		{
			if (render)
			{
				OGLobj->Render(gHDC);
				render = false;
			}
		}
	}

	return msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	static HGLRC hRC;
	static HDC hDC;
	int ancho, alto;
	float movcam = 1.5;

	switch (message)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		gHDC = hDC;
		SetUpPixelFormat(hDC);
		hRC = wglCreateContext(hDC);
		wglMakeCurrent(hDC, hRC);
		PlaySound("birdSound.wav", NULL, SND_ASYNC);
		PlaySound("waterEffect.wav", NULL, SND_ASYNC | SND_LOOP);
		return 0;
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case 0x57:						//W key
			OGLobj->poscz -= movcam;
			render = true;
			return 0;
		case 0x53:						//S key 
			OGLobj->poscz += movcam;
			render = true;
			return 0;
		case 0x41:						//A key
			OGLobj->poscx -= movcam;
			render = true;
			return 0;
		case 0x44:						//D key
			OGLobj->poscx += movcam;
			render = true;
			return 0;
		case 0x42:						//B de bucket
			if (OGLobj->bucketY > 5 && !upBucket)
			{
				OGLobj->bucketY -= .2;
				OGLobj->ropeH += .2;
			}
			else
			{
				upBucket = true;
				if (OGLobj->bucketY >= 4 && upBucket)
				{
					OGLobj->bucketY += .2;
					OGLobj->ropeH -= .2;
					if (OGLobj->bucketY == 11)
						upBucket = false;
				}
			}
			render = true;
			return 0;
		case 0x49:
			OGLobj->ufoZ -= 1;			
			render = true;
		return 0;
		case 0x4B:
			OGLobj->ufoZ += 1;
			render = true;
			return 0;
		case 0x4A:
			OGLobj->ufoX -= 1;
			render = true;
			return 0;
		case 0x4C:
			OGLobj->ufoX += 1;
			render = true;
			return 0;
		case 0x55:
			if (!OGLobj->ufoRay)
				OGLobj->ufoRay = true;
			else
				OGLobj->ufoRay = false;
			render = true;
			return 0;
		case VK_NUMPAD1:
			OGLobj->LightPos1[0] -= 3;
			render = true;
			return 0;
		case VK_NUMPAD2:
			OGLobj->LightPos1[0] += 3;
			render = true;
			return 0;
		case VK_UP:
			OGLobj->dircy += movcam;
			render = true;
			return 0;
		case VK_DOWN:
			OGLobj->dircy -= movcam;
			render = true;
			return 0;
		case VK_LEFT:
			OGLobj->dircx -= movcam;
			render = true;
			return 0;
		case VK_RIGHT:
			OGLobj->dircx += movcam;
			return 0;
		case 0x45:
			OGLobj->poscy -= movcam;
			return 0;
		case 0x51:
			OGLobj->poscy += 3;
			return 0;
		case VK_ESCAPE:
			KillTimer(hWnd, renderTimer);
			wglMakeCurrent(hDC, NULL);
			wglDeleteContext(hRC);
			PostQuitMessage(0);
			return 0;
		}
		return 0;
	}
	case WM_TIMER:
	{
		render = true;
		switch (wParam)
		{
			case renderTimer:
				OGLobj->meshRotation += 1;
				OGLobj->meshRotation = OGLobj->meshRotation == 360 ? 0 : OGLobj->meshRotation;
				if (OGLobj->smokeSize <= 3.98)
					OGLobj->smokeSize += .03;
				else
					OGLobj->smokeSize = 1;

				OGLobj->smokeHeight += .15;
				OGLobj->smokeHeight = OGLobj->smokeHeight >= 35 ? 20 : OGLobj->smokeHeight;
				if (OGLobj->levSheep < 17 && levUp)
					OGLobj->levSheep += .05;
				else
				{
					levUp = false;
					if (OGLobj->levSheep <= 18 && !levUp)
					{
						OGLobj->levSheep -= .05;
						if (OGLobj->levSheep == 15)
							levUp = true;
					}
				}
				//Animación del patito 1
				if (OGLobj->duck1movZ > -5 && !backSwim)
					OGLobj->duck1movZ -= .15;
				else
				{
					backSwim = true;
					if (OGLobj->duck1movZ >= -6 && backSwim)
					{
						OGLobj->duck1movZ += .15;
						if (OGLobj->duck1movZ >= 10)
							backSwim = false;
					}
				}
				//Animación de patito 2 
				if (OGLobj->duck2movZ > -30 && !backSwim2)
					OGLobj->duck2movZ -= .15;
				else
				{
					backSwim2 = true;
					if (OGLobj->duck2movZ >= -31 && backSwim2)
					{
						OGLobj->duck2movZ += .15;
						if (OGLobj->duck2movZ >= -10)
							backSwim2 = false;
					}
				}
				
			break;
		}
		return 0;
	}
	case WM_DESTROY:
	{
		KillTimer(hWnd, renderTimer);
		//en caos de salir desocpuar los recursos del opengl
		wglMakeCurrent(hDC, NULL);
		wglDeleteContext(hRC);
		PostQuitMessage(0);
		return 0;
	} break;

	case WM_SIZE:
	{
		//esta opcion del switch se ejecuta una sola vez al arrancar y si se
		//afecta el tamaño de la misma se dispara de nuevo
		alto = HIWORD(lParam);
		ancho = LOWORD(lParam);
		if (alto == 0)
			alto = 1;
		glViewport(0, 0, ancho, alto);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluPerspective(45.0f, (GLfloat)ancho / (GLfloat)alto, 1.0f, 1000.0f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void SetUpPixelFormat(HDC hDC)
{
	int PixForm;

	static PIXELFORMATDESCRIPTOR pixfordes = {
		sizeof(PIXELFORMATDESCRIPTOR), //tamaño de la estructura
		1, //numero de version
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, //soporta la ventana, el opengl y manejara doble buffer
		PFD_TYPE_RGBA, //formato de 32 bits rgba
		32, //tamaño del color en 32 bits
		0,0,0,0,0,0, //bits de color, no se usan
		0, //no hay buffer para el alfa
		0, //ignore el bit de corrimiento
		0, //no hay buffer de acumulacion
		0,0,0,0, //no hay bits de acumulacion
		16, //tamaño del flotante para el buffer z
		0, //no hay buffers de stencil
		0, //no hay buffers auxiliares
		PFD_MAIN_PLANE, //plano principal para dibujo
		0, //reservado
		0,0,0 //mascaras de capas ignoradas
	};

	PixForm = ChoosePixelFormat(hDC, &pixfordes);
	SetPixelFormat(hDC, PixForm, &pixfordes);
}