//init.cpp
#include "Init.h"

GLfloat LightAmbient[]=		{ 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat LightDiffuse[]=		{ 1.0f, 1.0f, 1.0f, 1.0f };
GLfloat LightPosition[]=	{ 0.0f, 0.0f, 2.0f, 1.0f };

GLuint	texture[NUMTEXTURES];



int Init(GLvoid)										
{
	CreateTexture(texture,"Data/stars.bmp",STARS);
	CreateTexture(texture,"Data/worldmap.bmp",WORLD_MAP);
	CreateTexture(texture,"Data/topic.bmp",TOPIC);

	CreateTexture(texture,"data/Terrain.bmp",0); 
	CreateTexture(texture,"data/Back.bmp",BACK_ID);
	CreateTexture(texture,"data/Front.bmp",FRONT_ID);
	CreateTexture(texture,"data/Bottom.bmp",BOTTOM_ID);
	CreateTexture(texture,"data/Top.bmp",TOP_ID);	
	CreateTexture(texture,"data/Left.bmp",LEFT_ID);
	CreateTexture(texture,"data/Right.bmp",RIGHT_ID);
	CreateTexture(texture,"data/jackie.bmp",JACKIE);
	CreateTexture(texture,"data/twins.bmp",TWINS);
	CreateTexture(texture,"data/volcanic.bmp",VOL);
	CreateTexture(texture,"data/SPACE.bmp",SPACE);
	CreateTexture(texture,"data/CHINA.bmp",CHINA);
	CreateTexture(texture,"data/EARTH.bmp",EARTH);
	CreateTexture(texture,"data/about.bmp",ABOUT);
	CreateTexture(texture,"data/info.bmp",INFO);

	LoadRawFile("data/Terrain.raw", MAP_SIZE * MAP_SIZE, g_HeightMap);	

	glEnable(GL_TEXTURE_2D);						
	glShadeModel(GL_SMOOTH);							
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				
	glClearDepth(1.0f);								
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LEQUAL);							
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);					

	BuildFont();										// Build The Font


	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);
	glEnable(GL_LIGHT1);								
	glEnable(GL_LIGHTING);								
//	glEnable(GL_COLOR_MATERIAL);

	quadratic=gluNewQuadric();							
	gluQuadricNormals(quadratic, GLU_SMOOTH);		
	gluQuadricTexture(quadratic, GL_TRUE);				

	g_Camera.PositionCamera( 280, 35, 225,  281, 35, 225,  0, 1, 0);
	
	return TRUE;									
}
