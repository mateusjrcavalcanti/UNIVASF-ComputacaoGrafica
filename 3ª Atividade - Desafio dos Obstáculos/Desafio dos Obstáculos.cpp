#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

GLfloat tx = 0, ty = -22, win = 25;
float altura = 4.8, largura = 2.2;
int vidas=3;

typedef struct
{
    char tipo;
    int tamanho;
    float posicao[2];
    float colors[3];
} Obstaculo;

Obstaculo SetObstaculo(char tipo, int tamanho, float posicaoX, float posicaoY, float colorum, float colordois, float colortres)
{ 
    Obstaculo Ob;
    Ob.tipo = tipo; 
    Ob.posicao[0] = posicaoX;  
    Ob.posicao[1] = posicaoY;   
    Ob.tamanho = tamanho;  
    Ob.colors[0] = colorum;  
    Ob.colors[1] = colordois;  
    Ob.colors[2] = colortres;
    return Ob;
}

Obstaculo obstaculos[] = {
	SetObstaculo('t', 3, 	-17.0f, 18.0f,	1.0f, 0.0f, 0.0f),
	SetObstaculo('r', 5, 	-16.0f, 5.0f, 	0.0f, 1.0f, 0.0f),
	SetObstaculo('p', 6, 	-1.0f, 8.0f, 	1.0f, 1.0f, 0.0f),
	SetObstaculo('q', 8, 	15.0f, 17.0f, 	0.0f, 0.0f, 1.0f),
	SetObstaculo('g', 7, 	11.0f, -5.5f, 	1.0f, 0.0f, 1.0f),
	SetObstaculo('g', 7, 	-13.0f, -11.0f, 0.0f, 1.0f, 1.0f),
	SetObstaculo('z', 6, 	17.0f, 5.5f, 	0.7f, 0.4f, 0.2f),
	};
	
bool ptInTriangle(float x, float y, float x0, float y0, float x1, float y1, float x2, float y2) {
	float p0[2] = {x0, y0}, p1[2] = {x1,y1}, p2[2] = {x2,y2};
    float dX = x-p2[0];
    float dY = y-p2[1];
    float dX21 = p2[0]-p1[0];
    float dY12 = p1[1]-p2[1];
    float D = dY12*(p0[0]-p2[0]) + dX21*(p0[1]-p2[1]);
    float s = dY12*dX + dX21*dY;
    float t = (p2[1]-p0[1])*dX + (p0[0]-p2[0])*dY;
    if (D<0) return s<=0 && t<=0 && s+t>=D;
    return s>=0 && t>=0 && s+t<=D;
}

bool colidiu(float x0, float y0, float x1, float y1, float x2, float y2) {
	return	ptInTriangle(tx + largura/2, ty + altura/2, x0, y0, x1, y1, x2, y2) || 
			ptInTriangle(tx + largura/2, ty - altura/2, x0, y0, x1, y1, x2, y2) || 
			ptInTriangle(tx - largura/2, ty + altura/2, x0, y0, x1, y1, x2, y2) || 
			ptInTriangle(tx - largura/2, ty - altura/2, x0, y0, x1, y1, x2, y2);
}

void Colisao(void)
{
	tx = 0;
	ty = -22;
	vidas--;
}
	
void DesenhaObstaculo(char tipo, float posicao[2], int tamanho, float colors[3] )
{
	float t1, t2, t3, t4, n;	
	float angleIncrement, angle;
	/*printf ("\t Tipo: %c, \n\t Centro:(X:%f, Y:%f) \n\t Cores: %f %f %f \n\t Pontos:", 
				tipo, 
				posicao[0], 
				posicao[1], 
				colors[0], 
				colors[1], 
				colors[2]
	);*/
	glColor3f(colors[0], colors[1], colors[2]);	
	switch(tipo) {
	  case 't':
	  	t1 = posicao[0]-2*tamanho;
	  	t2 = posicao[0]+2*tamanho;
	  	t3 = posicao[1]+1.5*tamanho;
	  	t4 = posicao[1]-1.5*tamanho;
	  	/*printf (" [1] => X:%f \t Y:%f \n", t1, t4);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t1, t3);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t2, t4);*/
	    glScalef(1.0f, 1.0f, 1.0f);
	    glBegin(GL_TRIANGLES);
			glVertex2f(t1, t4);
			glVertex2f(t1, t3);
			glVertex2f(t2, t4);
		glEnd();	
		if(colidiu(t1, t4, t1, t3, t2, t4)) Colisao();		
	    break;	  
	  case 'r':
	  	t1 = posicao[0]-tamanho;
	  	t2 = posicao[0]+tamanho;
	  	t3 = posicao[1]+tamanho;
	  	t4 = posicao[1]-tamanho;
	  	/*printf (" [1] => X:%f\t Y:%f \n", t1, t4);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t2, t4);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", posicao[0], t3);
		glScalef(1.0f, 1.0f, 1.0f);*/
	    glBegin(GL_TRIANGLES);
			glVertex2f(t1, t4);
			glVertex2f(t2, t4);
			glVertex2f(posicao[0],t3);
		glEnd();
		if(colidiu(t1, t4, posicao[0], t3, t2, t4)) Colisao();	
	    break;	  
	  case 'p':
	  	n= 5;
	  	angleIncrement = 360.0f / n;
		angleIncrement *= M_PI / 180.0f;
		glBegin(GL_TRIANGLE_FAN);
		angle = 0.0f;
		float anterior[2], primeiro[2];
		for (int k = 0; k < n; ++k) {
			if(k == 0){
				printf (" [%i] => X:%lf \t Y:%lf \n", k + 1, tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1]);
				primeiro[0] = tamanho * cos(angle) + posicao[0];
				primeiro[1] = tamanho * sin(angle) + posicao[1];
			}else{
				printf ("\t\t [%i] => X:%f \t Y:%f \n", k + 1, tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1]);
				if(k == n - 1){
					if(colidiu(posicao[0],posicao[1], tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], primeiro[0], primeiro[1])) Colisao();
				}else{
					if(colidiu(posicao[0],posicao[1], tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], anterior[0], anterior[1])) Colisao();	
				}
			}
			anterior[0] = tamanho * cos(angle) + posicao[0];
			anterior[1] = tamanho * sin(angle) + posicao[1];
			
		    glVertex3f(tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], 0.0f);		    
		    angle += angleIncrement;
		}
		glEnd();
	    break;	  
	  case 'q':
	  	t1 = posicao[0]+tamanho/2;
	  	t2 = posicao[0]-tamanho/2;
	  	t3 = posicao[1]+tamanho/2;
	  	t4 = posicao[1]-tamanho/2;
	  	/*printf (" [1] => X:%f\t Y:%f \n", t1, t4);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t2, t4);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t1, t3);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t2, t3);*/
	    glScalef(1.0f, 1.0f, 1.0f);
	    glBegin(GL_QUADS);
			glVertex2f(t1, t3);
			glVertex2f(t2, t3);
			glVertex2f(t2, t4)
			;glVertex2f(t1, t4);			
		glEnd();
		if(colidiu(posicao[0], posicao[1], t1, t3, t2, t3)) Colisao();
		if(colidiu(posicao[0], posicao[1], t2, t4, t2, t3)) Colisao();
		if(colidiu(posicao[0], posicao[1], t2, t4, t1, t4)) Colisao();
		if(colidiu(posicao[0], posicao[1], t1, t3, t1, t4)) Colisao();
	    break; 
	  case 'g':
	  	t1 = posicao[0]+3*tamanho/2;
	  	t2 = posicao[0]-3*tamanho/2;
	  	t3 = posicao[1]+tamanho/2;
	  	t4 = posicao[1]-tamanho/2;
	  	/*printf (" [1] => X:%f\t Y:%f \n", t1, t4);
	  	printf ("\t\t [2] => X:%f \t Y:%f \n", t2, t4);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t1, t3);
	  	printf ("\t\t [3] => X:%f \t Y:%f \n", t2, t3);*/
	    glScalef(1.0f, 1.0f, 1.0f);
	    glBegin(GL_QUADS);
			glVertex2f(t1, t3);
			glVertex2f(t2, t3);
			glVertex2f(t2, t4)
			;glVertex2f(t1, t4);			
		glEnd();
		if(colidiu(posicao[0], posicao[1], t1, t3, t2, t3)) Colisao();
		if(colidiu(posicao[0], posicao[1], t2, t4, t2, t3)) Colisao();
		if(colidiu(posicao[0], posicao[1], t2, t4, t1, t4)) Colisao();
		if(colidiu(posicao[0], posicao[1], t1, t3, t1, t4)) Colisao();
	    break;	  
	  case 'z':
	  	n= 4;
	  	angleIncrement = 360.0f / n;
		angleIncrement *= M_PI / 180.0f;
		glBegin(GL_TRIANGLE_FAN);
		angle = 0.0f;
		for (int k = 0; k < n; ++k) {
			if(k == 0){
				printf (" [%i] => X:%lf \t Y:%lf \n", k + 1, tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1]);
				primeiro[0] = tamanho * cos(angle) + posicao[0];
				primeiro[1] = tamanho * sin(angle) + posicao[1];
			}else{
				printf ("\t\t [%i] => X:%f \t Y:%f \n", k + 1, tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1]);
				if(k == n - 1){
					if(colidiu(posicao[0],posicao[1], tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], primeiro[0], primeiro[1])) Colisao();
				}else{
					if(colidiu(posicao[0],posicao[1], tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], anterior[0], anterior[1])) Colisao();	
				}
			}
			anterior[0] = tamanho * cos(angle) + posicao[0];
			anterior[1] = tamanho * sin(angle) + posicao[1];
			
		    glVertex3f(tamanho * cos(angle) + posicao[0], tamanho * sin(angle) + posicao[1], 0.0f);		    
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
	
	printf ("\n\n\n\n Objeto: %f %f\n", tx, ty);
	glBegin(GL_QUADS);
		glVertex2f(largura/2, altura/2);
		glVertex2f(largura/2, -(altura/2));
		glVertex2f(-(largura/2), -(altura/2));
		glVertex2f(-(largura/2), altura/2);
	glEnd();
	glPopMatrix();
	
	//Obstáculos
	printf ("\n\n Obstáculos: \n");
	for(int i = 0; i<=9; i++){
	if(obstaculos[i].tipo != NULL){
		printf ("\n [%i] =>", i + 1);	
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
		tx -= 1;
		if (tx < -win)
			tx = -win;
		break;
	case GLUT_KEY_RIGHT:
		tx += 1;
		if (tx > win)
			tx = win;
		break;
	case GLUT_KEY_DOWN:
		ty -= 1;
		if (ty < -win)
			ty = -win;
		break;
	case GLUT_KEY_UP:
		ty += 1;
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
