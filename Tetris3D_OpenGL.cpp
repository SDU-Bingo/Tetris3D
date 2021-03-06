// Tetris3D_OpenGL.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <GL/glut.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<iostream>
#ifndef _TIME_H
#define _TIME_H
#include <time.h>
#endif // !_TIME_H
#ifndef _VECTOR
#define _VECTOR
#include <vector>
#endif // !_VECTOR
#include "block.h"
#include<windows.h>
#include<mmsystem.h>
#pragma comment(lib,"winmm.lib");
#define KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? 1:0)
using namespace std;
int lastX, lastY,dropDist=0, centerX, centerY, centerZ,curBlock;
float angleYaw, anglePitch;
bool isLeftDown, wDown, aDown, dDown, sDown,qDown,eDown,rDown,vDown,spaceDown;
#define WIDTH 11
#define HEIGHT 20
bool scene[WIDTH][WIDTH][20];
int sceneColor[WIDTH][WIDTH][20];
int blockSequence[20] = {0,1,2,2,3,10,5,4,6,7,8,0,3,5,9,1,3,11,5,0};
int seqIndex = 0;
int FPS = 0;
block* block1;
vector<block> blockArray;
clock_t lastTime;
GLuint theMeshDL;
int blockIndex = 0;
static void meshDL()
{
	for(int v2=0;v2<20;v2+=3)
	for(int v=0;v<WIDTH;v+=1)
		for (int v1 = 0; v1 < WIDTH; v1 += 1)
		{
			glPushMatrix();
			glTranslatef(v1 * 10, v2*10, v * 10);////////////////
			glColor3f(0, 0.4, 0);
			glutWireCube(10);////////////////////////
			glPopMatrix();
		}
}
static void initDL(void)
{
	theMeshDL = glGenLists(1);
	glNewList(theMeshDL, GL_COMPILE);
	meshDL();
	glEndList();
}
void mouseFunc(int button, int state, int x, int y)
{
	lastX = x;
	lastY = y;
	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			isLeftDown = true;

			//	PlaySound(pBuffer, NULL, SND_MEMORY | SND_ASYNC);
		}
		else
			isLeftDown = false;
		break;
	default:
		break;
	}
}
#define sensitivity 0.5
void mouseMotion(int x, int y)
{
	angleYaw += (x - lastX)*sensitivity;
	anglePitch += (y - lastY)*sensitivity*0.7;
	angleYaw = angleYaw > 360 ? angleYaw = 0 : (angleYaw < 0 ? 360 : angleYaw);
	anglePitch = anglePitch > 90 ? 90: (anglePitch < -30 ? -30: anglePitch);
	lastX = x;
	lastY = y;
}
#undef sensitivity
void myInit()
{
	angleYaw = 0;
	anglePitch = 0;
	memset(scene, false, sizeof(scene));
	glEnable(GL_DEPTH_TEST);
}
void drawBlock(int x,int y,int z)
{
	glPushMatrix();
	glRotatef(anglePitch, 1, 0, 0);
	glRotatef(angleYaw, 0, 1, 0);
	glTranslatef((x - 5) * 10 + 5, (y - 5) * 10 + 5, (z - 5) * 10 + 5);//////////////////////////
	glutSolidCube(9.5);
	glPopMatrix();
}
bool judgeLegal(const block& blockX)
{
	//bool flag = true;
	for (int v = 0; v < blockX.size; v++)
	{
		if ((blockX.blocksi[v].ptr[0] + blockX.pos.ptr[0] < 0) || (blockX.blocksi[v].ptr[0] + blockX.pos.ptr[0] >= WIDTH)
			|| (blockX.blocksi[v].ptr[2] + blockX.pos.ptr[2] < 0) || (blockX.blocksi[v].ptr[2] + blockX.pos.ptr[2] >= WIDTH)
			|| (blockX.blocksi[v].ptr[1] + blockX.pos.ptr[1] < 0))
			return false;
		if (blockX.blocksi[v].ptr[1] + blockX.pos.ptr[1] < 20)
		{
			if (scene[blockX.blocksi[v].ptr[0] + blockX.pos.ptr[0]]
				[blockX.blocksi[v].ptr[2] + blockX.pos.ptr[2]]
			[blockX.blocksi[v].ptr[1] + blockX.pos.ptr[1]])
				return false;
		}
	}
	return true;
}
void myDisplay()
{
	clock_t currentTime;
	currentTime=clock();
	bool nextFrame=false;
	//roll the block
	if (aDown) {
		block1->roll_left();
		if (!judgeLegal(*block1))block1->roll_right();
	}
	if (dDown) {
		block1->roll_right();
		if (!judgeLegal(*block1))block1->roll_left();
	}
	if (wDown) {
		block1->roll_up();
		if (!judgeLegal(*block1))block1->roll_down();
	}
	if (sDown) {
		block1->roll_down();
		if (!judgeLegal(*block1))block1->roll_up();
	}
	if (qDown) {
		block1->pos.ptr[0]--;
		if (!judgeLegal(*block1))block1->pos.ptr[0]++;
	}
	if (eDown) {
		block1->pos.ptr[0]++;
		if (!judgeLegal(*block1))block1->pos.ptr[0]--;
	}
	if (rDown) {
		block1->pos.ptr[2]--;
		if (!judgeLegal(*block1))block1->pos.ptr[2]++;
	}
	if (vDown) {
		block1->pos.ptr[2]++;
		if (!judgeLegal(*block1))block1->pos.ptr[2]--;
	}
	aDown = wDown = sDown = dDown = qDown = eDown = rDown = vDown = false;
	FPS++;
	if ((double)(currentTime - lastTime)/CLOCKS_PER_SEC >= 2.0 || spaceDown)
	{
		std::cout << FPS << std::endl;
		FPS = 0;
		spaceDown = false;
		lastTime = currentTime;
		nextFrame = true;
		/*dropDist++;
		
			dropDist = -10;*/
		block1->pos.ptr[1]--;
		if (!judgeLegal(*block1))
		{
			block1->pos.ptr[1]++;
			for (int v = 0; v < block1->size; v++)
			{
				scene[block1->blocksi[v].ptr[0] + block1->pos.ptr[0]]
					[block1->blocksi[v].ptr[2] + block1->pos.ptr[2]]
				[block1->blocksi[v].ptr[1] + block1->pos.ptr[1]] = true;
				sceneColor[block1->blocksi[v].ptr[0] + block1->pos.ptr[0]]
					[block1->blocksi[v].ptr[2] + block1->pos.ptr[2]]
				[block1->blocksi[v].ptr[1] + block1->pos.ptr[1]] = curBlock;
			}
			block1->pos.ptr[0] = 5;
			block1->pos.ptr[1] = 23;
			block1->pos.ptr[2] = 5;
			curBlock = rand() % blockArray.size();
			//curBlock = blockSequence[seqIndex++];
			block1 = &blockArray.at(curBlock);////////////////////////
		}
	}
	glClear(GL_COLOR_BUFFER_BIT);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);
	glLoadIdentity();
	gluLookAt(0, 50, 200, 0, 50, 0, 0, 1, 0);
	glPushMatrix();
	GLfloat light_position[] = { 1000.0, 1000.0, 1000.0, 0.0 };//////////////
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glPopMatrix();
//judge if current block hit existed blocks:yes dropDist=0 scene=1 : no dropDist++
	glPushMatrix();
	for (int v2 = 0; v2 < 20; v2++)
	{
		bool flag = true;
		for (int v0 = 0; v0 < WIDTH; v0++)
			for (int v1 = 0; v1 < WIDTH; v1++)

			{
				if (scene[v0][v1][v2])
				{
					GLfloat mat_diffuse[] = { blockArray.at(sceneColor[v0][v1][v2]).color.ptr[0] ,blockArray.at(sceneColor[v0][v1][v2]).color.ptr[1] ,blockArray.at(sceneColor[v0][v1][v2]).color.ptr[2] , 1.0 };  //漫射参数
					glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
					drawBlock(v0, v2, v1);
				}
				else flag = false;
			}
		if (flag)
			for (int v0 = 0; v0 < WIDTH; v0++)
				for (int v1 = 0; v1 < WIDTH; v1++)
					scene[v0][v1][v2] = false;
	}
	glPopMatrix();
	glPushMatrix();
	for (int v = 0; v < block1->size; v++)
	{
		glPushMatrix();
		GLfloat mat_diffuse[] = { block1->color.ptr[0] , block1->color.ptr[1], block1->color.ptr[2], 1.0 };  //漫射参数
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		drawBlock(block1->blocksi[v].ptr[0]+block1->pos.ptr[0], block1->blocksi[v].ptr[1] + block1->pos.ptr[1], block1->blocksi[v].ptr[2] + block1->pos.ptr[2]);
		//glRotatef(anglePitch, 1, 0, 0);
		//glRotatef(angleYaw, 0, 1, 0);
		//glTranslatef((block1.blocksi[v].ptr[0] - 5) * 10 + 5, (block1.blocksi[v].ptr[1] - 5) * 10 + 5, (block1.blocksi[v].ptr[2] - 5) * 10 + 5);
		//glTranslatef(0, 150 - dropDist * 10, 0);
		//glutSolidCube(9.5);
		glPopMatrix();
	}
	glPopMatrix();
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glRotatef(anglePitch, 1, 0, 0);
	glRotatef(angleYaw, 0, 1, 0);
	glTranslatef(-45, -45, -45);
	//glCallList(theMeshDL);
	glEnable(GL_LIGHTING);
	glPopMatrix();
	glutSwapBuffers();
}
void normalKey(unsigned char key, int x, int y)
{
	aDown = wDown = sDown = dDown = qDown = eDown = rDown = vDown = spaceDown = false;
	switch (key)
	{
	case'w':
		wDown = true;
		break;
	case'a':
		aDown = true;
		break;
	case'd':
		dDown = true;
		break;
	case's':
		sDown = true;
		break;
	case'q':
		qDown = true;
		break;
	case'e':
		eDown = true;
		break;
	case'r':
		rDown = true;
		break;
	case'v':
		vDown = true;
		break;
	case' ':
		spaceDown = true;
		break;
	default:
		break;
	}
}
void myReshape(int width, int height)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(75, (GLfloat)width / (GLfloat)height, 1, 400);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0, 0, 200, 0, 0, 0, 0, 1, 0);
	lastTime=clock();
	aDown = wDown = sDown = dDown = false;
}
void myTimerFunc(int value)
{
	//aDown = KEY_DOWN('A');
	//wDown = KEY_DOWN('W');
	//sDown = KEY_DOWN('S');
	//dDown = KEY_DOWN('D');
	glutPostRedisplay();
	glutTimerFunc(16, &myTimerFunc, 1);
}
void setLight()
{
	GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };  //镜面反射参数
	GLfloat mat_diffuse[] = { 1, 1, 0, 1.0 };  //漫射参数
	GLfloat mat_shininess[] = { 0.0 };               //高光指数
	GLfloat light1_angle[] = { 15.0 };
	GLfloat light_position[] = { 1000.0, 1000.0, -1000.0, 0.0 };//////////////////////////////
	GLfloat white_light[] = { 1, 1, 1, 1.0 };   //灯位置(1,1,1), 最后1-开关
	GLfloat yellow_light[] = { 1,0.87,0.315,1.0 };
	GLfloat Light_Model_Ambient[] = { 0.5, 0.5, 0.5, 1.0 }; //环境光参数
	glClearColor(0.0, 0.0, 0.0, 0.0);  //背景色
	glShadeModel(GL_SMOOTH);           //多变性填充模式
									   //材质属性
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	//灯光设置
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);   //散射光属性
	glLightfv(GL_LIGHT0, GL_SPECULAR, mat_specular);  //镜面反射光
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, Light_Model_Ambient);  //环境光参数 
	glEnable(GL_LIGHTING);   //开关:使用光
	glEnable(GL_LIGHT0);     //打开0#灯
}
void genBlocks()
{
	block temp0;//H
	temp0.add_block(0, 0, 1);
	temp0.add_block(0, 1, 1);
	temp0.add_block(0, -1, 1);
	temp0.add_block(0, -2, 1);
	temp0.add_block(0, 2, 1);
	temp0.add_block(0, 0, -1);
	temp0.add_block(0, 1, -1);
	temp0.add_block(0, -1, -1);
	temp0.add_block(0, -2, -1);
	temp0.add_block(0, 2, -1);
	temp0.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp0.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp0.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp0);
	block temp1;//A
	temp1.add_block(0, 0, 1);
	temp1.add_block(0, 1, 1);
	temp1.add_block(0, -1, 1);
	temp1.add_block(0, -2, 1);
	temp1.add_block(0, 2, 0);
	temp1.add_block(0, 0, -1);
	temp1.add_block(0, 1, -1);
	temp1.add_block(0, -1, -1);
	temp1.add_block(0, -2, -1);
	temp1.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp1.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp1.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp1);
	block temp2;//P
	temp2.add_block(0, 0, -1);
	temp2.add_block(0, -1, -1);
	temp2.add_block(0, -2, -1);
	temp2.add_block(0, 1, -1);
	temp2.add_block(0, 2, -1);
	temp2.add_block(0, 0, 1);
	temp2.add_block(0, 1, 1);
	temp2.add_block(0, 2, 1);
	temp2.add_block(0, 2, 0);
	temp2.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp2.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp2.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp2);
	block temp3;//Y
	temp3.add_block(0, 0, 1);
	temp3.add_block(0, 1, 1);
	temp3.add_block(0, 2, 1);
	temp3.add_block(0, 0, -1);
	temp3.add_block(0, 1, -1);
	temp3.add_block(0, 2, -1);
	temp3.add_block(0, -1, 0);
	temp3.add_block(0, -2, 0);
	temp3.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp3.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp3.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp3);
	block temp4;//B
	temp4.add_block(0, 1, 1);
	temp4.add_block(0, -1, 1);
	temp4.add_block(0, 2, 1);
	temp4.add_block(0, -2, 1);
	temp4.add_block(0, 2, 0);
	temp4.add_block(0, -2, 0);
	temp4.add_block(0, 2, -1);
	temp4.add_block(0, 1, -1);
	temp4.add_block(0, 0, -1);
	temp4.add_block(0, -1, -1);
	temp4.add_block(0, -2, -1);
	temp4.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp4.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp4.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp4);
	block temp6;//I
	temp6.add_block(0, 1, 0);
	temp6.add_block(0, 2, 0);
	temp6.add_block(0, -1, 0);
	temp6.add_block(0, -2, 0);
	temp6.add_block(0, -2, 1);
	temp6.add_block(0, -2, -1);
	temp6.add_block(0, 2, 1);
	temp6.add_block(0, 2, -1);
	temp6.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp6.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp6.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp6);
	block temp7;//R
	temp7.add_block(0, 1, 1);
	temp7.add_block(0, -1, 1);
	temp7.add_block(0, 2, 1);
	temp7.add_block(0, 2, 0);
	temp7.add_block(0, -2, 1);
	temp7.add_block(0, 2, -1);
	temp7.add_block(0, 1, -1);
	temp7.add_block(0, 0, -1);
	temp7.add_block(0, -1, -1);
	temp7.add_block(0, -2, -1);
	temp7.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp7.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp7.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp7);
	block temp8;//T
	temp8.add_block(0, 1, 0);
	temp8.add_block(0, 2, 0);
	temp8.add_block(0, -1, 0);
	temp8.add_block(0, -2, 0);
	temp8.add_block(0, 2, 1);
	temp8.add_block(0, 2, -1);
	temp8.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp8.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp8.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp8);
	block temp9;//D
	temp9.add_block(0, 2, 0);
	temp9.add_block(0, 2, 1);
	temp9.add_block(0, 1, 0);
	temp9.add_block(0, -1, 0);
	temp9.add_block(0, -2, 0);
	temp9.add_block(0, -2, 1);
	temp9.add_block(0, 1, 2);
	temp9.add_block(0, 0, 2);
	temp9.add_block(0, -1, 2);
	temp9.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp9.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp9.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp9);
	block temp10;//F
	temp10.add_block(0,  2,-1);
	temp10.add_block(0,  1,-1);
	temp10.add_block(0,  0,-1);
	temp10.add_block(0,  -1,-1);
	temp10.add_block(0, -2,-1);
	temp10.add_block(0, 2, 0);
	temp10.add_block(0, 0, 1);
	temp10.add_block(0, 2, 1);
	temp10.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp10.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp10.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp10);
	block temp11;//Q
	temp11.add_block(0, 2, -1);
	temp11.add_block(0, 1, -1);
	temp11.add_block(0, 4, -1);
	temp11.add_block(0, 3, -1);
	temp11.add_block(0, 4, 0);
	temp11.add_block(0, 4, 1);
	temp11.add_block(0, 2, 1);
	temp11.add_block(0, 3, 1);
	temp11.add_block(0, 0, 1);
	temp11.color.ptr[0] = (rand() % 226 + 30) / 255.0;
	temp11.color.ptr[1] = (rand() % 226 + 30) / 255.0;
	temp11.color.ptr[2] = (rand() % 226 + 30) / 255.0;
	blockArray.push_back(temp11);
}
int main(int argc,char** argv)
{
	srand((unsigned)time(NULL));
	genBlocks();
	curBlock = rand() % blockArray.size();
	curBlock = blockSequence[seqIndex++];
	block1 = &blockArray.at(curBlock);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(500, 500);
	glutCreateWindow("");
	initDL();
	myInit();
	setLight();
	glutMouseFunc(&mouseFunc);
	glutMotionFunc(&mouseMotion);
	glutReshapeFunc(&myReshape);
	glutKeyboardFunc(&normalKey);
	glutTimerFunc(17, &myTimerFunc, 1);
	glutDisplayFunc(&myDisplay);
	glutMainLoop();
    return 0;
}

