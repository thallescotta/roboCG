#define GL_SILENCE_DEPRECATION
#include <windows.h>
#include <iostream>
#include <stdlib.h>
#include <GL/glut.h>
#include "RgbImage.h"

#define PI 3.141592654

using namespace std;

char* filenameTexMetal1 = (char*)"./simpatico.bmp";
char* filenameTexMetal2 = (char*)"./fofo6.bmp";
char* filenameTexMetal3 = (char*)"./menosazul.bmp";
char* filenameTexTop = (char*)"./metalTexture1.bmp";
char* filenameTexFront = (char*)"./metalTexture1.bmp";
char* filenameTexSide = (char*)"./metalTexture1.bmp";

GLuint _textureIdMetal1;
GLuint _textureIdMetal2;
GLuint _textureIdMetal3;
GLuint _textureIdSphere;
GLuint _textureIdAux;
GLuint _textureIdCylinder;
GLuint _textureIdPlatformTop;
GLuint _textureIdPlatformFront;
GLuint _textureIdPlatformSide;

bool textureOn = true;

GLUquadric* quadSphere;
GLUquadric* quadCylinder;

float diameterCylinder = 0.3;
float diameterSphere = 0.4;
float diameterBase = 2.0;
float lenghtPlatform = 11.0;
float heightPlatform = 0.4;

float eyeDistance = 30.0;
float eyeX = 0.0f;
float eyeY = 0.0f;
float eyeZ = 0.0f;
float viewAngleX = 90.0;
float viewAngleZ = 15.0;



float angleHead = 0.0f;
float angleTorso = 0.0f;

float angleArmFrenteD = 0.0f;
float angleArmLadoD = 0.0f;
float angleForearmDobrarD = 0.0f;
float angleForearmGirarD = 0.0f;
float anglePrimeiraFalangeD = 0.0f;
float angleSegundaFalangeD = 0.0f;


float angleArmFrenteE = 0.0f;
float angleArmLadoE = 0.0f;
float angleForearmDobrarE = 0.0f;
float angleForearmGirarE = 0.0f;
float anglePrimeiraFalangeE = 0.0f;
float angleSegundaFalangeE = 0.0f;


GLfloat lightposition[4] = { 0.0f, 30.0f, 0.0f, 0.0 };
GLfloat luzAmbiente[4] = { 0.19, 0.19, 0.19, 0.0 };
GLfloat luzDifusa[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat luzEspecular[4] = { 0.5, 0.5, 0.5, 1.0 };
GLfloat especularidade[4] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat semEspecularidade[4] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat especMaterialTexture = 50;
GLfloat especMaterialNoTexture = 60;


//movimento do corpo

float bodyX = 0.0f;
float bodyY = 0.0f;
float bodyZ = 0.0f;
float bodyStepsH = 0;
float bodyStepsV = 0;
float bodyAngle = 0.0f;
float bodyAxisMoveX = 0.0f;
float bodyAxisMoveY = 0.0f;
float bodyAxisMoveZ = 0.0f;
bool leftPress = false;
bool rightPress = false;
bool upPress = false;
bool downPress = false;

// motor
float motorMoveAngle = 0.0f;
int animationFrames = 30;

//defines light source (LIGHT0)
void initLighting(void)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_NORMALIZE);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);

	// Especifica que a cor de fundo da janela será preta

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	// Habilita o modelo de colorização de Gouraud
	glShadeModel(GL_SMOOTH);

	// Ativa o uso da luz ambiente 
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, luzAmbiente);
	//glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR);

	// Define os parâmetros da luz de número 0
	glLightfv(GL_LIGHT0, GL_AMBIENT, luzAmbiente);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, luzDifusa);
	glLightfv(GL_LIGHT0, GL_SPECULAR, luzEspecular);
	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// Materials will follow current color
	glEnable(GL_COLOR_MATERIAL);
}

//makes the image into a texture, and returns the id of the texture
GLuint loadTexture(char* filename) {
	GLuint textureId;

	RgbImage theTexMap(filename); //Image with texture

	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);	//Tell OpenGL which texture to edit
												//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,	//Always GL_TEXTURE_2D
		0,						//0 for now
		GL_RGB,					//Format OpenGL uses for image
		theTexMap.GetNumCols(),	//Width 
		theTexMap.GetNumRows(),	//Height
		0,						//The border of the image
		GL_RGB,					//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,		//GL_UNSIGNED_BYTE, because pixels are stored as unsigned numbers
		theTexMap.ImageData());	//The actual pixel data
	return textureId; //Returns the id of the texture
}

void initRendering(void) {

	quadSphere = gluNewQuadric();
	quadCylinder = gluNewQuadric();
	_textureIdMetal1 = loadTexture(filenameTexMetal1);
	_textureIdMetal2 = loadTexture(filenameTexMetal2);
	_textureIdMetal3 = loadTexture(filenameTexMetal3);
	_textureIdCylinder = _textureIdMetal2;
	_textureIdSphere = _textureIdMetal2;
	_textureIdAux = _textureIdMetal3;
	_textureIdPlatformTop = loadTexture(filenameTexTop);
	_textureIdPlatformFront = loadTexture(filenameTexFront);
	_textureIdPlatformSide = loadTexture(filenameTexSide);
}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	case 'w': //Increase view angle z axis
		if (viewAngleZ < 180) viewAngleZ += 3;
		glutPostRedisplay();
		break;
	case 'q': //Decrease view angle z axis
		if (viewAngleZ > 0)
			viewAngleZ -= 3;
		glutPostRedisplay();
		break;

	case 'a': //Decrease view angle x axis
		if (viewAngleX > 0) viewAngleX -= 3;
		glutPostRedisplay();
		break;
	case 's': //Increase view angle x axis
		if (viewAngleX < 180) viewAngleX += 3;
		glutPostRedisplay();
		break;
	case 'z': //Trocar textura
		textureOn = true;
		if (_textureIdSphere == _textureIdMetal1) {
			_textureIdSphere = _textureIdMetal2;
			_textureIdCylinder = _textureIdMetal2;
		}
		else if (_textureIdSphere == _textureIdMetal2) {
			_textureIdSphere = _textureIdMetal1;
			_textureIdCylinder = _textureIdMetal1;
		}
		glutPostRedisplay();
		break;
	case 'f': //Sem textura
		textureOn = false;
		glutPostRedisplay();
		break;

	case 'i': //Com textura
		textureOn = true;
		glutPostRedisplay();
		break;

	case 'c': //Rodar Cabeça
		if (angleHead < 90) angleHead += 3;
		glutPostRedisplay();
		break;
	case 'x': //Desrodar Cabeça
		if (angleHead > -90) angleHead -= 3;
		glutPostRedisplay();
		break;

	case 't': //Girar Torso para Esquerda
		if (angleTorso < 90)
		{
			angleTorso += 3;
			glutPostRedisplay();
		}
		break;

	case 'r': //Girar torso para Direita
		if (angleTorso > -90)
		{
			angleTorso -= 3;;
			glutPostRedisplay();
		}
		break;


	case 'p': //Braco Esquerdo para frente
		if (angleArmFrenteE < 180)
		{
			angleArmFrenteE += 3;
			glutPostRedisplay();
		}
		break;

	case 'o': //Braco Esquerdo para tras
		if (angleArmFrenteE > 0 || angleArmFrenteE > -69)
		{
			angleArmFrenteE -= 3;
			glutPostRedisplay();
		}
		break;

	case 'l': //Braco Esquerdo para lado
		if (angleArmLadoE >= -90)
		{
			angleArmLadoE -= 3;
			glutPostRedisplay();
		}
		break;

	case 'k': //Braco Esquerdo volta ao centro 
		if (angleArmLadoE < 0)
		{
			angleArmLadoE += 3;
			glutPostRedisplay();
		}
		break;

	case 'j': //Dobra Antebraco Esquerdo
		if (angleForearmDobrarE < 120)
		{
			angleForearmDobrarE += 3;
			glutPostRedisplay();
		}
		break;

	case 'h': //Desdobra Antebraco Esquerdo
		if (angleForearmDobrarE > 0)
		{
			angleForearmDobrarE -= 3;
			glutPostRedisplay();
		}
		break;

	case 'm': //Gira antebraco Esquerdo
		if (angleForearmGirarE < 180)
		{
			angleForearmGirarE += 3;
			glutPostRedisplay();
		}
		break;

	case 'n': //Desgira antebraco Esquerdo
		if (angleForearmGirarE > 0)
		{
			angleForearmGirarE -= 3;
			glutPostRedisplay();
		}
		break;

	case 'b': //Fecha Mao Esquerda
		if (anglePrimeiraFalangeE < 60 && angleSegundaFalangeE < 100)
		{
			anglePrimeiraFalangeE += 2;
			angleSegundaFalangeE += 4;
			glutPostRedisplay();
		}
		break;

	case 'v': //Abre Mao Esquerda
		if (anglePrimeiraFalangeE > 0 && angleSegundaFalangeE > 0)
		{
			anglePrimeiraFalangeE -= 2;
			angleSegundaFalangeE -= 4;
			glutPostRedisplay();
		}
		break;

	case '1': //Braco Direito para frente
		if (angleArmFrenteD < 180)
		{
			angleArmFrenteD += 3;
			glutPostRedisplay();
		}
		break;

	case '2': //Braco Direito para tras
		if (angleArmFrenteD > 0 || angleArmFrenteD > -69)
		{
			angleArmFrenteD -= 3;
			glutPostRedisplay();
		}
		break;

	case '3': //Braco Direito para lado
		if (angleArmLadoD >= -90)
		{
			angleArmLadoD -= 3;
			glutPostRedisplay();
		}
		break;

	case '4': //Braco Direito volta ao centro
		if (angleArmLadoD < 0)
		{
			angleArmLadoD += 3;
			glutPostRedisplay();
		}
		break;

	case '5': //Dobra Antebraco Direito
		if (angleForearmDobrarD < 120)
		{
			angleForearmDobrarD += 3;
			glutPostRedisplay();
		}
		break;

	case '6': //Desdobra Antebraco Direito
		if (angleForearmDobrarD > 0)
		{
			angleForearmDobrarD -= 3;
			glutPostRedisplay();
		}
		break;

	case '7': //Gira Antebraco Direito
		if (angleForearmGirarD < 180)
		{
			angleForearmGirarD += 3;
			glutPostRedisplay();
		}
		break;

	case '8': //Desgira Antebraco Direito
		if (angleForearmGirarD > 0)
		{
			angleForearmGirarD -= 3;
			glutPostRedisplay();
		}
		break;

	case '9': //Fecha Mao Direita
		if (anglePrimeiraFalangeD < 60 && angleSegundaFalangeD < 100)
		{
			anglePrimeiraFalangeD += 2;
			angleSegundaFalangeD += 4;
			glutPostRedisplay();
		}
		break;

	case '0': //Abre Mao Direita
		if (anglePrimeiraFalangeD > 0 && angleSegundaFalangeD > 0)
		{
			anglePrimeiraFalangeD -= 2;
			angleSegundaFalangeD -= 4;
			glutPostRedisplay();
		}
		break;
	}
	//cout << "Arm (" << angleArm << "); Forearm (" << angleForearm << "); Hand (" << angleHand << "); ";
	//cout << "Clamp Y (" << angleClampY << "); Clamp Z (" << angleClampZ << ") \n";
	//cout << "Eye X (" << eyeX << "); Eye Y (" << eyeY << "); Eye Z (" << eyeZ << ") \n";
}

//Movimento do robo com setas
void SpecialInput(int key, int x, int y) {
	switch (key)
	{
	case GLUT_KEY_LEFT: //Anda para Esquerda
		if (bodyStepsH < 5)
		{
			bodyAxisMoveY = 1.0f;
			leftPress = true;
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_RIGHT: //Anda para Direita
		if (bodyStepsH > -5)
		{
			bodyAxisMoveY = 1.0f;
			rightPress = true;
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_UP: //Anda para Frente
		if (bodyStepsV < 5)
		{
			bodyAxisMoveX = 1.0f;
			upPress = true;
		}
		glutPostRedisplay();
		break;

	case GLUT_KEY_DOWN: //Anda para tras
		if (bodyStepsV > -5)
		{
			bodyAxisMoveX = 1.0f;
			downPress = true;
		}
		glutPostRedisplay();
		break;

	}
}


// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance < 30) eyeDistance += 1;
		}
	if (button == GLUT_RIGHT_BUTTON)
		if (state == GLUT_DOWN) {  // Zoom-out
			if (eyeDistance > 0) eyeDistance -= 1;
		}
	glutPostRedisplay();
}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (float)w / (float)h, 1.0, 50.0);
}

void drawCube(float lenghtX, float lenghtY, float height)
{
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformFront);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face posterior
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face frontal
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformSide);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face lateral esquerda
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face lateral direita
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, _textureIdPlatformTop);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBegin(GL_QUADS);			// Face superior
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-lenghtX, lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(lenghtX, lenghtY, height);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(lenghtX, lenghtY, -height);
	glEnd();
	glBegin(GL_QUADS);			// Face inferior
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(lenghtX, -lenghtY, -height);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(lenghtX, -lenghtY, height);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-lenghtX, -lenghtY, height);
	glEnd();
}

void drawCylinder(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, diameter, lenght, 40.0, lenght * 30.0);
}

void drawCone(float diameter, float lenght) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluCylinder(quadCylinder, diameter, 0, lenght, 40.0, lenght * 30.0);
}

void drawDisk(float diameterInner, float diameterOuter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdCylinder);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadCylinder, 1);
	}
	else
		gluQuadricTexture(quadCylinder, 0);
	gluDisk(quadCylinder, diameterInner, diameterOuter, 40.0, 30.0);
}

void drawSphere(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdSphere);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void drawAux(float diameter) {
	if (textureOn) {
		glBindTexture(GL_TEXTURE_2D, _textureIdAux);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		gluQuadricTexture(quadSphere, 1);
	}
	else
		gluQuadricTexture(quadSphere, 0);
	gluSphere(quadSphere, diameter, 40.0, 40.0);
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(animationFrames, timer, 0);
}

void handleSpecialKeyReleased(int key, int x, int y) {

	if (key == GLUT_KEY_LEFT)
	{
		bodyAngle = 0.0f;
		bodyAxisMoveY = 0.0f;
		angleArmLadoD = 0.0f;
		angleArmLadoE = 0.0f;
		leftPress = false;
	}
	if (key == GLUT_KEY_RIGHT)
	{
		bodyAngle = 0.0f;
		bodyAxisMoveY = 0.0f;
		angleArmLadoD = 0.0f;
		angleArmLadoE = 0.0f;
		rightPress = false;
	}
	if (key == GLUT_KEY_UP)
	{
		bodyAngle = 0.0f;
		bodyAxisMoveX = 0.0f;
		angleArmFrenteE = 0.0f;
		angleArmFrenteD = 0.0f;
		upPress = false;
	}
	if (key == GLUT_KEY_DOWN)
	{
		bodyAngle = 0.0f;
		bodyAxisMoveX = 0.0f;
		angleArmFrenteE = 0.0f;
		angleArmFrenteD = 0.0f;
		downPress = false;
	}
}

void smoothMovesXY() {
	if (leftPress and bodyStepsH < 5)
	{
		eyeX += 5;
		bodyStepsH += 1;
		bodyX += 2.0;
		if (angleArmLadoE > -25.0)
		{
			angleArmLadoD -= 6.0;
			angleArmLadoE -= 12.0;
		}
	}
	if (rightPress and bodyStepsH > -5)
	{
		eyeX -= 5;
		bodyStepsH -= 1;
		bodyX -= 2.0;
		if (angleArmLadoD > -25.0)
		{
			angleArmLadoD -= 12.0;
			angleArmLadoE -= 6.0;
		}
	}
	if (upPress and bodyStepsV < 5)
	{
		eyeX += 5;
		bodyStepsV += 1;
		bodyY += 2.0;
		if (angleArmFrenteE < 20.0 and angleArmFrenteD > -20.0)
		{
			angleArmFrenteD -= 12.0;
			angleArmFrenteE += 12.0;
		}
	}
	if (downPress and bodyStepsV > -5)
	{
		eyeX -= 5;
		bodyStepsV -= 1;
		bodyY -= 2.0;
		if (angleArmFrenteE > -20.0 and angleArmFrenteD < 20.0)
		{
			angleArmFrenteD += 12.0;
			angleArmFrenteE -= 12.0;
		}
	}
}

void moveBodyAngle() {
	if (leftPress and bodyAngle < 15.0f)
	{
		bodyAngle += 2.0f;
	}
	if (rightPress and bodyAngle > -15.0f)
	{
		bodyAngle -= 2.0f;
	}
	if (upPress and bodyAngle > -15.0f)
	{
		bodyAngle -= 2.0f;
	}
	if (downPress and bodyAngle < 15.0f)
	{
		bodyAngle += 2.0f;
	}
}

void drawScene(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	eyeX = eyeDistance * cos(viewAngleZ * PI / 180) * cos(viewAngleX * PI / 180);
	eyeY = eyeDistance * cos(viewAngleZ * PI / 180) * sin(viewAngleX * PI / 180);
	eyeZ = eyeDistance * sin(viewAngleZ * PI / 180);

	//glTranslated(bodyX, 0, 0);

	if (viewAngleZ < 90)
		gluLookAt(eyeX, eyeY, eyeZ, bodyX, 0.0, 0.0, 0.0, 0.0, 1.0);
	else
		gluLookAt(eyeX, eyeY, eyeZ, bodyX, 0.0, 0.0, 0.0, 0.0, -1.0);


	glLightfv(GL_LIGHT0, GL_POSITION, lightposition);

	// clears color and material parameters before drawing platform
	glColor3f(1.0f, 1.0f, 1.0f);
	glMaterialfv(GL_FRONT, GL_SPECULAR, semEspecularidade);
	glMateriali(GL_FRONT, GL_SHININESS, 0);

	// draws the platform
	glPushMatrix();
	drawCube(lenghtPlatform, lenghtPlatform, heightPlatform);
	glPopMatrix();

	if (textureOn) {
		glColor3f(0.8f, 0.9f, 1.0f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialTexture);
	}
	else {
		glColor3f(0.465f, 0.390f, 0.363f);
		glMaterialfv(GL_FRONT, GL_SPECULAR, especularidade);
		glMateriali(GL_FRONT, GL_SHININESS, especMaterialNoTexture);
	}

	glPushMatrix();
	glTranslatef(bodyX, bodyY, 11.4f);
	glRotatef(bodyAngle, bodyAxisMoveX, bodyAxisMoveY, bodyAxisMoveZ); // Y - horizontal, X - Vertical, Z - Giro em torno do eixo.

	//Rosto
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(angleHead, 0.0f, 0.0f, 1.0f);
	glRotatef(-90, 0.0f, 0.0f, 1.0f);

	drawSphere(1.5f);

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(0.5f, 1.3f, 0.0f);
	drawAux(0.2f);
	glPopMatrix();

	glPushMatrix();
	glRotatef(90, 0.0f, 0.0f, 1.0f);
	glTranslatef(-0.5f, 1.3f, 0.0f);
	drawAux(0.2f);
	glPopMatrix();


	glPopMatrix();

	//Corpo
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, -1.7f);
	drawCylinder(0.8f, 0.8f);
	drawDisk(diameterCylinder, diameterBase);
	glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
	glRotatef(angleTorso, 0.0f, 0.0f, 1.0f);
	drawCone(2.0f, 4.5f);
	glTranslatef(0.0f, 0.0f, 4.0f);
	drawSphere(1.0f);

	//Ombro Direito
	glTranslatef(-2.4f, 0.0f, -3.5f);
	drawSphere(0.7f);

	//Braço Direito
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.2f);
	glRotatef(angleArmFrenteD, 1.0f, 0.0f, 0.0f); //Rotaciona Braco para frente
	glRotatef(angleArmLadoD, 0.0f, 1.0f, 0.0f); //Rotaciona Braco para o lado
	drawCylinder(0.2f, 2.0f);

	//AnteBraço Direito
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2.2f);
	drawAux(0.3f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	glRotatef(angleForearmDobrarD, 1.0f, 0.0f, 0.0f); //Dobra Antebraco
	glRotatef(angleForearmGirarD, 0.0f, 0.0f, 1.0f); //Gira Antebraco
	drawCylinder(0.2f, 1.5f);

	//Mão Direita
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.6f);
	drawSphere(0.3f);

	glPushMatrix(); //Polegar
	glTranslatef(-0.3f, 0.0f, 0.0f);
	drawAux(0.1f);
	glRotatef(270.0f, 0.0f, 0.1f, 0.0f);
	glRotatef(anglePrimeiraFalangeD, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.1f);
	glRotatef(angleSegundaFalangeD, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.15f);
	glPopMatrix();

	glPushMatrix(); //Mindinho
	glTranslatef(0.35f, 0.0f, 0.0f);
	drawAux(0.1f);
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(anglePrimeiraFalangeD, -1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.1f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, -0.05f);
	drawAux(0.1f);
	glRotatef(180.0f, 0.0f, 0.1f, 0.0f);
	glRotatef(angleSegundaFalangeD, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.15f);
	glPopMatrix();

	glPushMatrix(); //Indicador
	drawAux(0.1f);
	glRotatef(anglePrimeiraFalangeD, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.1f);
	glRotatef(angleSegundaFalangeD, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.4f);
	drawAux(0.15f);
	glPopMatrix();

	glPopMatrix(); //Mao Direita
	glPopMatrix(); //AnteBraco Direito
	glPopMatrix(); //Braco Direito

	//Ombro Esquerdo

	glTranslatef(4.7f, 0.0f, 0.0f);
	drawSphere(0.7f);

	//Braço Esquerdo
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.2f);
	glRotatef(angleArmFrenteE, 1.0f, 0.0f, 0.0f); //Rotaciona Braco para frente
	glRotatef(angleArmLadoE, 0.0f, -1.0f, 0.0f); //Rotaciona Braco para o lado
	drawCylinder(0.2f, 2.0f);

	//AnteBraço Esquerdo
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 2.2f);
	drawAux(0.3f);
	glTranslatef(0.0f, 0.0f, 0.1f);
	glRotatef(angleForearmDobrarE, 1.0f, 0.0f, 0.0f); //Dobra Antebraco
	glRotatef(angleForearmGirarE, 0.0f, 0.0f, -1.0f); //Gira Antebraco
	drawCylinder(0.2f, 1.5f);

	//Mão Direita
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 1.6f);
	drawSphere(0.3f);

	glPushMatrix(); //Polegar
	glTranslatef(-0.3f, 0.0f, 0.0f);
	drawAux(0.1f);
	glRotatef(270.0f, 0.0f, 0.1f, 0.0f);
	glRotatef(anglePrimeiraFalangeE, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.1f);
	glRotatef(angleSegundaFalangeE, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.15f);
	glPopMatrix();

	glPushMatrix(); //Mindinho
	glTranslatef(0.35f, 0.0f, 0.0f);
	drawAux(0.1f);
	glRotatef(270.0f, 0.0f, 1.0f, 0.0f);
	glRotatef(anglePrimeiraFalangeE, -1.0f, 0.0f, 0.0f);
	glTranslatef(0.0f, 0.0f, -0.1f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, -0.05f);
	drawAux(0.1f);
	glRotatef(180.0f, 0.0f, 0.1f, 0.0f);
	glRotatef(angleSegundaFalangeE, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.15f);
	glPopMatrix();

	glPushMatrix(); //Indicador
	drawAux(0.1f);
	glRotatef(anglePrimeiraFalangeE, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.35f);
	drawAux(0.1f);
	glRotatef(angleSegundaFalangeE, 1.0f, 0.0f, 0.0f);
	drawCylinder(0.1f, 0.3f);
	glTranslatef(0.0f, 0.0f, 0.4f);
	drawAux(0.15f);
	glPopMatrix();

	glPopMatrix(); //Mao Esquerda
	glPopMatrix(); //AnteBraco Esquerdo
	glPopMatrix(); //Braco Esquerdo

	glPopMatrix(); //Corpo

	glPushMatrix();
	//circulo externo
	glTranslatef(0.0f, 0.0f, -8.3f);
	glRotatef(motorMoveAngle, 0.0f, 0.0f, 1.0f);
	drawCylinder(2.5f, 0.5f);
	drawCone(0.5f, 0.5f);
	//esfera
	glPushMatrix();
	glTranslatef(2.0f, 1.0f, 0.3f);
	drawAux(0.5f);
	glPopMatrix();
	//circulo interno
	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 0.3f);
	glRotatef(-motorMoveAngle, 1.0f, 0.0f, 0.0f);
	drawCylinder(1.5f, 0.5f);
	glPopMatrix();
	glPopMatrix();

	glPopMatrix();
	glutSwapBuffers();

	//smooth moves
	motorMoveAngle -= 1.0;
	smoothMovesXY();
	moveBodyAngle();

}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutCreateWindow("Robo");

	initLighting();
	initRendering();
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutSpecialFunc(SpecialInput);
	glutMouseFunc(GerenciaMouse);
	glutReshapeFunc(handleResize);
	glutTimerFunc(0, timer, 0);
	glutSpecialUpFunc(handleSpecialKeyReleased);

	glutMainLoop();
	return 0;
}