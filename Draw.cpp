//draw.cpp
#include "Draw.h"
#include "Camera.h"
GLUquadricObj *quadratic;
GLfloat	yrot=0;
GLfloat	rot=1.0f;				// Used To Rotate The Text
GLfloat	rot1=1.0f;				// Used To Rotate The Text
int m_on=FALSE;
CCamera g_Camera;
BYTE g_HeightMap[MAP_SIZE*MAP_SIZE];			// 保存高程数据

//  本文件主要包含了所有与地形有关的函数。在地形的渲染过程中使用的是三角形面片，而不是四边形面片。
//  下面的函数的功能是获取高程值
int Height(BYTE *pHeightMap, int X, int Y)
{
	// 确保x,y没有超过数组大小
	int x = X % MAP_SIZE;
	int y = Y % MAP_SIZE;

	if(!pHeightMap) return 0;				// 确保高程数据存在

	// 获取高程值
	return pHeightMap[x + (y * MAP_SIZE)];	
}


//  下面的函数功能是根据X和Z坐标设置当前位置的纹理坐标
void SetTextureCoord(float x, float z)
{
	// 给出当前x和z位置处的纹理坐标
	glTexCoord2f(   (float)x / (float)MAP_SIZE,	
				  - (float)z / (float)MAP_SIZE	);
}


//  下面的函数的功能是以三角形面片的形式渲染地形
void RenderHeightMap(BYTE pHeightMap[])
{
	int X = 0, Y = 0;						// 地形的网格的位置
	int x, y, z;
	bool bSwitchSides = false;

	// 判断高程数据是否存在
	if(!pHeightMap) return;		
	
	// 捆绑纹理
	glBindTexture(GL_TEXTURE_2D, texture[0]);

	// 以三角形面片的形式绘制地形
	glBegin( GL_TRIANGLE_STRIP );			

	// 遍历高程数据中所有行
	for ( X = 0; X <= MAP_SIZE; X += STEP_SIZE )
	{
		// 判断渲染的面
		if(bSwitchSides)
		{	
			//  遍历所有列
			for ( Y = MAP_SIZE; Y >= 0; Y -= STEP_SIZE )
			{
				// 获得高程值		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// 设置当前的纹理坐标
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		

				// 获得高程值		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// 设置当前的纹理坐标
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);			
			}
		}
		else
		{	
			//  遍历所有的行
			for ( Y = 0; Y <= MAP_SIZE; Y += STEP_SIZE )
			{
				// 获得高程值		
				x = X + STEP_SIZE; 
				y = Height(pHeightMap, X + STEP_SIZE, Y ); 
				z = Y;

				// 设置纹理坐标
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);

				// 获得高程值		
				x = X;							
				y = Height(pHeightMap, X, Y );	
				z = Y;							

				// 设置当前的纹理坐标
				SetTextureCoord( (float)x, (float)z );
				glVertex3i(x, y, z);		
			}
		}

		bSwitchSides = !bSwitchSides;
	}

	// 绘制完成
	glEnd();

}

//  下面的函数的功能是将raw文件中的数据读出到一个字节队列中，其中的值就为高程值。
void LoadRawFile(LPSTR strName, int nSize, BYTE *pHeightMap)
{
	FILE *pFile = NULL;

	// 以只读/二进制模式打开文件
	pFile = fopen( strName, "rb" );

	// 判断文件是否存在
	if ( pFile == NULL )	
	{
		// 如果文件没有打开，显示错误信息
		MessageBox(NULL, "Can't find the height map!", "Error", MB_OK);
		return;
	}

	// 将数据读出到数据队列中
	fread( pHeightMap, 1, nSize, pFile );

	// 判断操作是否成功
	int result = ferror( pFile );

	// 如果收到错误信息
	if (result)
	{
		MessageBox(NULL, "Can't get data!", "Error", MB_OK);
	}

	// 关闭文件
	fclose(pFile);
}


int DrawGlobal() //开始时旋转的地球
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

int DrawTopic()//开始时的标题:SPACE三维地质学习系统
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

//  下面的函数的功能是生成天空
void CreateSkyBox(float x, float y, float z, float width, float height, float length)
{
	// 使用纹理
	glEnable(GL_TEXTURE_2D);

	// 设置BACK纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[BACK_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 天空的位置
	x = x - width  / 2;
	y = y - height / 2;
	z = z - length / 2;

	// 开始绘制
	glBegin(GL_QUADS);		
		
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height, z); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z);
		
	glEnd();

	// 设置FRONT部分的纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[FRONT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


	// 开始绘制
	glBegin(GL_QUADS);	
	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height, z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z + length);
	glEnd();

	// 设置BOTTOM部分的纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[BOTTOM_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 开始绘制
	glBegin(GL_QUADS);		
	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y,			z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
	glEnd();

	// 设置TOP部分的纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[TOP_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 开始绘制
	glBegin(GL_QUADS);		
		
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height, z);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y + height, z + length); 
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y + height,	z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);
		
	glEnd();

	// 设置LEFT部分的纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[LEFT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 开始绘制
	glBegin(GL_QUADS);		
		
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x,			y + height,	z);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x,			y + height,	z + length); 
		glTexCoord2f(0.0f, 0.0f); glVertex3f(x,			y,			z + length);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x,			y,			z);		
		
	glEnd();

	// 设置RIGHT部分的纹理参数
	glBindTexture(GL_TEXTURE_2D, texture[RIGHT_ID]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 开始绘制
	glBegin(GL_QUADS);		

		glTexCoord2f(0.0f, 0.0f); glVertex3f(x + width, y,			z);
		glTexCoord2f(1.0f, 0.0f); glVertex3f(x + width, y,			z + length);
		glTexCoord2f(1.0f, 1.0f); glVertex3f(x + width, y + height,	z + length); 
		glTexCoord2f(0.0f, 1.0f); glVertex3f(x + width, y + height,	z);
	glEnd();
}
float yrotc=0;
int DrawCubic()//绘制地形中的一个立方体
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

int DrawIntro()//绘制进入时的全部介面
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
		m_on=TRUE;				//鼠标是否停犹留?
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
//	glTranslatef(0.0f,0.0f,-2.755555555f);						//未修改
	glTranslatef(0.0f,0.0f,-5.570036f);						// 修改过的.Info的深度
	glColor4f(1,1,1,1);
	glEnable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D, texture[INFO]);
/*	glBegin(GL_QUADS);
		// 显示信息:	
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -0.75f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -0.75f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  0.75f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  0.75f,  1.0f);
	glEnd();
*/	glBegin(GL_QUADS);
		// 显示信息:	
		glNormal3f( 0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, -1.875f,  1.0f);
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f, -1.875f,  1.0f);
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f,  1.875f,  1.0f);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,  1.875f,  1.0f);
	glEnd();
/*	测试Info深度的显示:
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

int DrawMain()//绘制主介面的所有
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
	// 获得当前摄像机的位置

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
{	//  判断摄像机是否低于地平面
	if(vPos.y > Height(g_HeightMap, (int)vPos.x, (int)vPos.z )+50.0000001||vPos.y < Height(g_HeightMap, (int)vPos.x, (int)vPos.z )+50)
	{
		// 设置新的位置
		vNewPos.y = (float)Height(g_HeightMap, (int)vPos.x, (int)vPos.z ) + 50;

		// 获得y方向的差
		float temp = vNewPos.y - vPos.y;

		//  获得当前摄像机的方向
		CVector3 vView = g_Camera.View();
		vView.y += temp;

		// 设置新的摄像机位置
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
/*	glDisable(GL_TEXTURE_2D);			//为调整skybox所写:
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
		MessageBox(NULL,"文件不存在!","错误",MB_OK|MB_ICONERROR);

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
	//  如果文件不存在，在返回
	if(!strFileName)								
	{
		return;
	}
	//  读入位图文件中的数据		
	pBitmap = LoadBMP(strFileName);				
	if(pBitmap == NULL)									
		exit(0);
	//  生成纹理
	glGenTextures(1, &textureArray[textureID]);
	//  捆绑纹理
	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);
	//  设置纹理参数
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);
	//  释放资源
	if (pBitmap)									
	{
		if (pBitmap->data)								
		{
			free(pBitmap->data);					
		}
		free(pBitmap);									
	}
}
