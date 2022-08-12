#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLfloat tx = 0, ty = -22, win = 25;

typedef struct
{
    char tipo;
    int tamanho;
    float posicao[2];
    float colors[3];
} Obstaculo;


Obstaculo SetObstaculo(char tipo, int tamanho, float posicaoum, float posicaodois, float colorum, float colordois, float colortres)
{ 
    Obstaculo Ob;
    Ob.tipo = tipo; 
    Ob.posicao[0] = posicaoum;  
    Ob.posicao[1] = posicaodois;   
    Ob.tamanho = tamanho;  
    Ob.colors[0] = colorum;  
    Ob.colors[1] = colordois;  
    Ob.colors[2] = colortres;
    return Ob;
}

Obstaculo obstaculos[] = {
	SetObstaculo('t', 2, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
	SetObstaculo('r', 3, -17.0f, 18.0f, 0.0f, 1.0f, 0.0f),
	SetObstaculo('p', 5, 5.0f, 5.0f, 1.0f, 1.0f, 0.0f),
	SetObstaculo('r', 2, 5.0f, 5.0f, 0.0f, 0.0f, 1.0f),
	SetObstaculo('r', 2, 5.0f, 5.0f, 1.0f, 0.0f, 1.0f),
	SetObstaculo('r', 2, 5.0f, 5.0f, 0.0f, 1.0f, 1.0f),
	SetObstaculo('r', 2, 5.0f, 5.0f, 1.0f, 1.0f, 1.0f),
	};

void DesenhaObstaculo(char tipo, float posicao[2], int tamanho, float colors[3] )
{
	int t1, t2, t3, n;	
	float angleIncrement, angle;
	printf ("\t Tipo: %c, \n\t Centro:(X:%i, Y:%i) \n\t Cores: %f %f %f \n\t Pontos: ", 
				tipo, 
				posicao[0], 
				posicao[1], 
				colors[0], 
				colors[1], 
				colors[2]
	);
	glColor3f(colors[0], colors[1], colors[2]);	
	switch(tipo) {
	  case 't':
	  	t1 = posicao[0]-2*tamanho;
	  	t2 = posicao[0]+2*tamanho;
	  	t3 = posicao[1]+3*tamanho;
	  	printf ("[1] => X:%f\t Y:%f \n", t1, posicao[1]);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t1, t3);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t2, posicao[1]);
	    glScalef(1.0f, 1.0f, 1.0f);
	    glBegin(GL_TRIANGLES);
			glVertex2f(t1, posicao[1]);
			glVertex2f(t1, t3);
			glVertex2f(t2, posicao[1]);
		glEnd();
	    break;	  
	  case 'r':
	  	t1 = posicao[0]-2*tamanho;
	  	t2 = posicao[0]+2*tamanho;
	  	t3 = posicao[1]+2*tamanho;
	  	printf ("[1] => X:%f\t Y:%f \n", t1, posicao[1]);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t1, t3);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t2, posicao[1]);
		glScalef(1.0f, 1.0f, 1.0f);
	    glBegin(GL_TRIANGLES);
			glVertex2f(t1, posicao[1]);
			glVertex2f(t1, t3);
			glVertex2f(t2, posicao[1]);
		glEnd();
	    break;	  
	  case 'p':
	  	n= 5;
	  	angleIncrement = 360.0f / n;
		angleIncrement *= M_PI / 180.0f;
		glBegin(GL_TRIANGLE_FAN);
		angle = 0.0f;
		for (int k = 0; k < n; ++k) {
			printf ("\t\t\[%i] => X:%f \t Y:%f \n", k, tamanho * cos(angle), tamanho * sin(angle));
		    glVertex3f(tamanho * cos(angle), tamanho * sin(angle), 0.0f);		    
		    angle += angleIncrement;
		}
		glEnd();
	    break;	  
	  default:	  	
	  break;	    
	}
	printf ("\n");
	glFlush();	
}

void debug(void){
	#if defined(_WIN32) || defined(_WIN64)
    system("cls");
	#else defined(__linux__) || defined(__unix__)
	    system("clear");
	#endif
	printf("\t================================================================================\n");
	printf("\t Setas para esquerda, direita, cima e baixa movimentam o objeto no trajeto\n");
	printf("\t O botão esquerdo do mouse muda as cores da janela, dos obstáculos e do objeto\n");	
	printf("\t================================================================================");
}

void Desenha(void)
{	
	debug();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();
	
	// Objeto
	glPushMatrix();	
	glTranslatef(tx, ty, 0.0f);
	glScalef(1.0f, 1.0f, 1.0f);
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_QUADS);
		glVertex2f(0.9, 2.4);
		glVertex2f(0.9, -2.4);
		glVertex2f(-0.9, -2.4);
		glVertex2f(-0.9, 2.4);
	glEnd();
	glPopMatrix();
	
	//Obstáculos
	printf ("\n\n\n\n Obstáculos: \n");
	for(int i = 0; i<=9; i++){
	if(obstaculos[i].tipo != NULL){
		printf ("\n [%i] =>", i);	
		DesenhaObstaculo(obstaculos[i].tipo, obstaculos[i].posicao, obstaculos[i].tamanho, obstaculos[i].colors);
		}		
    }
	
	glPopMatrix();
	glFlush();
}

void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;
	if (h == 0)
		h = 1;
	largura = w;
	altura = h;

	glViewport(0, 0, largura, altura);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	if (largura <= altura)
	{
		gluOrtho2D(-25.0f, 25.0f, -25.0f * altura / largura, 25.0f * altura / largura);
		win = 25.0f;
	}
	else
	{
		gluOrtho2D(-25.0f * largura / altura, 25.0f * largura / altura, -25.0f, 25.0f);
		win = 25.0f * largura / altura;
	}
}

void TeclasEspeciais(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		tx -= 2;
		if (tx < -win)
			tx = -win;
		break;
	case GLUT_KEY_RIGHT:
		tx += 2;
		if (tx > win)
			tx = win;
		break;
	case GLUT_KEY_DOWN:
		ty -= 2;
		if (ty < -win)
			ty = -win;
		break;
	case GLUT_KEY_UP:
		ty += 2;
		if (ty > win)
			ty = win;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void Teclado(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'd':		
		break;
	case 27:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


void Inicializa(void)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

int main(void)
{
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(450, 450);
	glutCreateWindow("3 ª Atividade - 1ª Unidade (Desafio dos Obstáculos)");
	glutDisplayFunc(Desenha);
	glutReshapeFunc(AlteraTamanhoJanela);
	glutSpecialFunc(TeclasEspeciais);
	glutKeyboardFunc(Teclado);
	Inicializa();
	glutMainLoop();
	return 0;
}
