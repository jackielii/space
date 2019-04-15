//draw.cpp
#include "Draw.h"
#include "Camera.h"
GLUquadricObj *quadratic;
GLfloat	yrot=0;
GLfloat	rot=1.0f;				// Used To Rotate The Text
GLfloat	rot1=1.0f;				// Used To Rotate The Text
int m_on=FALSE;
CCamera g_Camera;
BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];			// ����߳�����

//  ���ļ���Ҫ����������������йصĺ������ڵ��ε���Ⱦ������ʹ�õ�����������Ƭ���������ı�����Ƭ��
//  ����ĺ����Ĺ����ǻ�ȡ�߳�ֵ
int Height(BYTE *pHeightMap, int X, int Y)
{
	// ȷ��x,yû�г��������С
	int x = X % MAP_SIZE;
	int y = Y % MAP_SIZE;

	if(!pHeightMap) return 0;				// ȷ���߳����ݴ���

	// ��ȡ�߳�ֵ
	return pHeightMap[x + (y * MAP_SIZE)];	
}


//  ����ĺ��������Ǹ���X��Z�������õ�ǰλ�õ���������
void SetTextureCoord(float x, float z)
{
	// ������ǰx��zλ�ô�����������
	glTexCoord2f(   (float)x / (float)MAP_SIZE,	
				  - (float)z / (float)MAP_SIZE	);
}


//  ����ĺ����Ĺ���������������Ƭ����ʽ��Ⱦ����
void RenderHeightMap(BYTE pHeightMap[])
{
	int X = 0, Y = 0;						// ���ε������λ��
	int x, y, z;
	bool bSwitchSides = false;

	// �жϸ߳������Ƿ����
	if(!pHeightMap) return;		
	
	// ��������
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// ����������Ƭ����ʽ���Ƶ���
	glBegin( GL_TRIANGLE_STRIP );			

	// �����߳�������������
	for ( X = 0; X <= MAP_SIZE; X += STEP_SIZE )
	{
		// �ж���Ⱦ����
		if(bSwitchSides)
		{	
			//  ����������
			for ( Y = MAP_SIZE; Y >= 0; Y -= STEP_SIZE )
			{
				// ��ø߳�ֵ		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// ���õ�ǰ����������
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		

				// ��ø߳�ֵ		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// ���õ�ǰ����������
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);			
			}
		}
		else
		{	
			//  �������е���
			for ( Y = 0; Y <= MAP_SIZE; Y += STEP_SIZE )
			{
				// ��ø߳�ֵ		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// ������������
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);

				// ��ø߳�ֵ		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// ���õ�ǰ����������
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		
			}
		}

		bSwitchSides = !bSwitchSides;
	}

	// �������
	glEnd();

}

//  ����ĺ����Ĺ����ǽ�raw�ļ��е����ݶ�����һ���ֽڶ����У����е�ֵ��Ϊ�߳�ֵ��
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	// ��ֻ��/������ģʽ���ļ�
	pFile = fopen( strName, "rb" );

	// �ж��ļ��Ƿ����
	if ( pFile == NULL )	
	{
		// ����ļ�û�д򿪣���ʾ������Ϣ
		MessageBox(NULL, "Can't find the height map!", "Error", MB_OK);
		return;
	}

	// �����ݶ��������ݶ�����
	fread( pHeightMap, 1, nSize, pFile );

	// �жϲ����Ƿ�ɹ�
	int result = ferror( pFile );

	// ����յ�������Ϣ
	if (result)
	{
		MessageBox(NULL, "Can't get data!", "Error", MB_OK);
	}

	// �ر��ļ�
	fclose(pFile);
}


int DrawGlobal() //��ʼʱ��ת�ĵ���
{
	glLoadIdentity();									
	glColor4f(0.9f,0.9f,0.9f,0.5f);
	glTranslatef(2.0,-0.5,-10.0);
	glRotatef(yrot,0.0f,1.0f,0.0f);
	glRotatef(-90,1.0f,0.0f,0.0f);
	glBindTexture(GL_TEXTURE_2D, texture[WORLD_MAP]);

	gluSphere(quadratic,1.3f,32,32);

	yrot+=0.3f;

	return TRUE;
}

int DrawTopic()//��ʼʱ�ı���:SPACE��ά����ѧϰϵͳ
{
	glLoadIdentity();									
	glTranslatef(0.0f,0.5f,-7.0f);
	glDisable(GL_DEPTH_TEST);	// Turn Depth Testing Off
	glEnable(GL_BLEND);			// Turn Blending On
	glColor4f(1.0f,0.5f,0.0f,1.0f);
	glBindTexture(GL_TEXTURE_2D, texture[TOPIC]);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(-0.1f, 0.0f);  glVertex2f(-3.58f,1.0f);
		glTexCoord2f(0.9f, 0.0f);  glVertex2f(2.51f,1.0f);
		glTexCoord2f(0.9f, 1.0f);  glVertex2f(3.6f,2.0f);
		glTexCoord2f(-0.1f, 1.0f);  glVertex2f(-2.59f,2.0f);
	glEnd();
	glDisable(GL_BLEND);		// Turn Blending Off
	glEnable(GL_DEPTH_TEST);	// Turn Depth Testing On
	return TRUE;
}

//  ����ĺ����Ĺ������������
void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	// ʹ������
	glEnable(GL_TEXTURE_2D);

	// ����BACK�������
	glBindTexture(GL_TEXTURE_2D, texture[BACK_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ��յ�λ��
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	// ��ʼ����
	glBegin(GL_QUADS);		
		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		
	glEnd();

	// ����FRONT���ֵ��������
	glBindTexture(GL_TEXTURE_2D, texture[FRONT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// ��ʼ����
	glBegin(GL_QUADS);	
	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// ����BOTTOM���ֵ��������
	glBindTexture(GL_TEXTURE_2D, texture[BOTTOM_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ��ʼ����
	glBegin(GL_QUADS);		
	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// ����TOP���ֵ��������
	glBindTexture(GL_TEXTURE_2D, texture[TOP_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ��ʼ����
	glBegin(GL_QUADS);		
		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();

	// ����LEFT���ֵ��������
	glBindTexture(GL_TEXTURE_2D, texture[LEFT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ��ʼ����
	glBegin(GL_QUADS);		
		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	// ����RIGHT���ֵ��������
	glBindTexture(GL_TEXTURE_2D, texture[RIGHT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ��ʼ����
	glBegin(GL_QUADS);		

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}
float yrotc=0;
int DrawCubic()//���Ƶ����е�һ��������
{	
//	glLoadIdentity();
	glColor4f(1,1,1,1);	
	glTranslatef(-1600, 30, 200);
	glRotatef(yrotc,0.0f,1.0f,0.0f);

	glBindTexture(GL_TEXTURE_2D, texture[JACKIE]);
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 40.0f,  40.0f,  40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f,  40.0f,  40.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[TWINS]);
	glBegin(GL_QUADS);
		// Back Face
		glNormal3f( 0.0f, 0.0f,-1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -40.0f, -40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f,  40.0f, -40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 40.0f,  40.0f, -40.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 40.0f, -40.0f, -40.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[CHINA]);
	glBegin(GL_QUADS);
		// Top Face
		glNormal3f( 0.0f, 1.0f, 0.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f,  40.0f, -40.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f,  40.0f,  40.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 40.0f,  40.0f,  40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 40.0f,  40.0f, -40.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[JACKIE]);
	glBegin(GL_QUADS);
		// Bottom Face
		glNormal3f( 0.0f,-1.0f, 0.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f, -40.0f, -40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 40.0f, -40.0f, -40.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -40.0f,  40.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[VOL]);
	glBegin(GL_QUADS);
		// Right face
		glNormal3f( 1.0f, 0.0f, 0.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 40.0f, -40.0f, -40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 40.0f,  40.0f, -40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 40.0f,  40.0f,  40.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 40.0f, -40.0f,  40.0f);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texture[EARTH]);
	glBegin(GL_QUADS);
		// Left Face
		glNormal3f(-1.0f, 0.0f, 0.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -40.0f, -40.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-40.0f,  40.0f,  40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f,  40.0f, -40.0f);
	glEnd();
	yrotc+=0.5f;

	return TRUE;
}

int DrawIntro()//���ƽ���ʱ��ȫ������
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();									
	glEnable(GL_COLOR_MATERIAL);
	glColor4f(1.0f,1.0f,1.0f,0.5f);
	glBindTexture(GL_TEXTURE_2D, texture[STARS]);
	glBegin(GL_QUADS);
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-8.0f, -6.0f,  -14.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 8.0f, -6.0f,  -14.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 8.0f,  6.0f,  -14.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-8.0f,  6.0f,  -14.0f);
	glEnd();
	DrawGlobal();
	DrawTopic();
	glLoadIdentity();									

	glDisable(GL_TEXTURE_2D);							// Enable Texture Mapping
	float xt1=xt*5;
	float yt1=yt*5;
	glTranslatef(-2.0f,-0.4f,-8.0f);
	if((xt1>-1.6)&&(xt1<-0.4)&&(yt1>-0.4)&&(yt<0.02))
	{
		if(!m_on)				
		m_on=TRUE;				//����Ƿ�ͣ����?
//		if(ClickEnter)
//		{
//			ShowCursor(FALSE);
//			sys=MAIN;
//		}
		
		glTranslatef(0.5f,0.0f,-1.0f);						
		glRotatef(rot*1.5f,0.0f,1.0f,0.0f);					
		glColor3f(1.0f*float(cos(rot/20.0f)),1.0f*float(sin(rot/25.0f)),1.0f-0.5f*float(cos(rot/17.0f)));
	 	glPrint("ENTER");					
		rot+=0.5f;	
	}
	else
	{
		m_on=FALSE;
		glTranslatef(-0.5f,0.0f,-5.0f);					
		glColor3f(1.0f*float(cos(rot/20.0f)),1.0f*float(sin(rot/25.0f)),1.0f-0.5f*float(cos(rot/17.0f)));
		glPrint("ENTER");
	}	

	glLoadIdentity();									
	glColor3f(1.0f*float(sin(rot1/35.0f)),1.0f*float(cos(rot1/30.0f)),1.0f-0.5f*float(cos(rot1/37.0f)));
	glTranslatef(0.0f,-6.0f,-20.0f);					
	glDisable(GL_DEPTH_TEST);
	glPrint("By Space WorkGroup");
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	rot1+=0.5;
	glDisable(GL_COLOR_MATERIAL);
	return TRUE;
}
int DrawInfo(GLvoid)
{
	glLoadIdentity();	
//	glTranslatef(0.0f,0.0f,-2.755555555f);						//δ�޸�
	glTranslatef(0.0f,0.0f,-5.570036f);						// �޸Ĺ���.Info�����
	glColor4f(1,1,1,1);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[INFO]);
/*	glBegin(GL_QUADS);
		// ��ʾ��Ϣ:	
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -0.75f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -0.75f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  0.75f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  0.75f,  1.0f);
	glEnd();
*/	glBegin(GL_QUADS);
		// ��ʾ��Ϣ:	
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, -1.875f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f, -1.875f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f,  1.875f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,  1.875f,  1.0f);
	glEnd();
/*	����Info��ȵ���ʾ:
	static float zb=-4;
	glDisable(GL_TEXTURE_2D);						
	glDisable(GL_DEPTH_TEST);
	glTranslatef(0.0f,0.0f,-5);						
	glPrint("zr=%f",zb);
 	glTranslatef(0.0f,0.0f,5);					
	if(GetKeyState('U') & 0x80)
		zb-=0.01f;
 	if(GetKeyState('O') & 0x80)
		zb+=0.01f;
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
*/	glDisable(GL_BLEND);
   return TRUE;
}

int DrawMain()//���������������
{
static	float xr=1038,zr=1030;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	glLoadIdentity();
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	g_Camera.Look();
	glColor4f(1.0,1.0,1.0,1.0);
	CreateSkyBox(1038,500,1030, 2048, 2048, 2048);
	RenderHeightMap(g_HeightMap);						
	// ��õ�ǰ�������λ��

	CVector3 vPos		= g_Camera.Position();
	CVector3 vNewPos    = vPos;
static int 
 hp=1,dn=1;
if (KEY_DOWN('H')&&hp)
{
	hp=0;
	dn=!dn;
}
if (KEY_UP('H'))
	hp=1;

if (dn)
{	//  �ж�������Ƿ���ڵ�ƽ��
	if(vPos.y > Height(g_HeightMap, (int)vPos.x, (int)vPos.z )+50.0000001||vPos.y < Height(g_HeightMap, (int)vPos.x, (int)vPos.z )+50)
	{
		// �����µ�λ��
		vNewPos.y = (float)Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 50;

		// ���y����Ĳ�
		float temp = vNewPos.y - vPos.y;

		//  ��õ�ǰ������ķ���
		CVector3 vView = g_Camera.View();
		vView.y += temp;

		// �����µ������λ��
		g_Camera.PositionCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
								vView.x,	vView.y,	vView.z,	0, 1, 0);								
	}
}
	glColor4f(1.0f,0.5f,0.0f,0.8f);
	glTranslatef(150, 50, 1650);
	glRotatef(180.0f,0.0f,1.0f,0.0f);
glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[ABOUT]);
	glBegin(GL_QUADS);
		// Front Face
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 40.0f, -40.0f,  40.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 40.0f,  40.0f,  40.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-40.0f,  40.0f,  40.0f);
	glEnd();
glDisable(GL_BLEND);
/*	glDisable(GL_TEXTURE_2D);			//Ϊ����skybox��д:
	glDisable(GL_DEPTH_TEST);
	glTranslatef(vPos.x,vPos.y,vPos.z-10);
	glPrint("xr=%.2f",xr);
	glTranslatef(0.0f,-1.5f,0.0f);
	glPrint("zr=%.2f",zr);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	if(GetKeyState('J') & 0x80)
	{
		xr+=1;				
	}
	if(GetKeyState('U') & 0x80)
	{
		xr-=1;				
	}
	if(GetKeyState('L') & 0x80) 
	{
		zr+=1;			
	}
	if(GetKeyState('O') & 0x80) 
	{
		zr-=1;			
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
*/	DrawCubic();
	DrawInfo();
	g_Camera.Update();	
	

	return TRUE;
}



AUX_RGBImageRec *LoadBMP(char *Filename)				
{
	FILE *File=NULL;									

	if (!Filename)										
	{
		return NULL;									
	}

	File=fopen(Filename,"r");							
	if (!File)
		MessageBox(NULL,"�ļ�������!","����",MB_OK|MB_ICONERROR);

	if (File)											
	{
		fclose(File);									
		return auxDIBImageLoad(Filename);				
	}

	return NULL;										
}


void CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	AUX_RGBImageRec *pBitmap = NULL;
	//  ����ļ������ڣ��ڷ���
	if(!strFileName)								
	{
		return;
	}
	//  ����λͼ�ļ��е�����		
	pBitmap = LoadBMP(strFileName);				
	if(pBitmap == NULL)									
		exit(0);
	//  ��������
	glGenTextures(1, &textureArray[textureID]);
	//  ��������
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	//  �����������
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	//  �ͷ���Դ
	if (pBitmap)									
	{
		if (pBitmap->data)								
		{
			free(pBitmap->data);					
		}
		free(pBitmap);									
	}
}
