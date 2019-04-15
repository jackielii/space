#include "CBProc.h"

float xt, yt;
static int mx,my;
int ClickEnter=FALSE;


LRESULT CALLBACK WndProc(	HWND	hWnd,			// Handle For This Window
							UINT	uMsg,			// Message For This Window
							WPARAM	wParam,			// Additional Message Information
							LPARAM	lParam)			// Additional Message Information
{
	switch (uMsg)									// Check For Windows Messages
	{
		case WM_ACTIVATE:							// Watch For Window Activate Message
		{
			if (!HIWORD(wParam))					// Check Minimization State
			{
				active=TRUE;						// Program Is Active
			}
			else
			{
				active=FALSE;						// Program Is No Longer Active
			}

			return 0;								// Return To The Message Loop
		}

		case WM_SYSCOMMAND:							// Intercept System Commands
		{
			switch (wParam)							// Check System Calls
			{
				case SC_SCREENSAVE:					// Screensaver Trying To Start?
				case SC_MONITORPOWER:				// Monitor Trying To Enter Powersave?
				return 0;							// Prevent From Happening
			}
			break;									// Exit
		}

		case WM_CLOSE:								// Did We Receive A Close Message?
		{
			PostQuitMessage(0);						// Send A Quit Message
			return 0;								// Jump Back
		}

		case WM_KEYDOWN:							// Is A Key Being Held Down?
		{
			keys[wParam] = TRUE;					// If So, Mark It As TRUE
			return 0;								// Jump Back
		}

		case WM_LBUTTONUP:
			{
				if(sys==INTRO&&m_on)
				{
					ShowCursor(FALSE);							
					sys=MAIN;
				}
				return 0;
			}

		case WM_RBUTTONDOWN:
			{
				sys=INTRO;
				ClickEnter=FALSE;
				ShowCursor(TRUE);							
				glEnable(GL_LIGHTING);
			}

		case WM_KEYUP:								
		{
			keys[wParam] = FALSE;					
			return 0;							
		}

		case WM_MOUSEMOVE: // ≈–∂œ Û±Í «∑Ò“∆∂Ø
			{
				mx = LOWORD(lParam);
				my = HIWORD(lParam);
				if  (mx & 1 << 15) mx -= (1 << 16);
				if  (my & 1 << 15) my -= (1 << 16);
				xt = ((float)(mx-(wWidth/2))/(float)(wWidth/2))*0.66666f;
				yt = -((float)(my-(wHeight/2))/(float)(wHeight/2))*0.5f;
				return 0;
			}


		case WM_SIZE:							
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;								
		}
	}

	// Pass All Unhandled Messages To DefWindowProc
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
