//*****************************************************************
//
// 								Jogo.cpp
//
// Um jogo OpenGL simples que abre uma janela GLUT, lança "lixo" da 
// parte superior da tela em direção, a parte inferior. O usuário 
// deve utilizar o botão esquerdo do mouse para coletar o lixo 
// antes que ele caia no lago.
//
// DUPLA: @mateusjrcavalcanti e Talisson Cardoso
// Este códio foi criado como Projeto da disciplina de COMPUTAÇÃO
// GRÁFICA 2021.2
//
// COMANDO: g++ Jogo.cpp stb_image.cpp -lGL -lGLU -lglut -o Jogo
//
//****************************************************************

// Bibliotecas Carregadas
#include <GL/glut.h> // FreeGlut: sudo apt-get install freeglut3-dev
#include <iostream>
#include <string>
#include <vector>
#include "stb_image.h" // Download em: https://github.com/nothings/stb/blob/master/stb_image.h

using namespace std;

// Variáveis usadas na Lógica do Jogo
int vidas, pontos;
float velocidade, velocidadeBase;
bool play;

// Variáveis usadas na Janela e Texturas
GLsizei largura, altura;
GLuint objetos[50], texturas[50];

typedef struct
{	
    string arquivo;
    float altura;
    float largura;
} Imagem;

typedef struct
{	
    int numeroImagem;
    float x;
    float y;
    float z;
} Interface;

typedef struct
{	
    int numeroImagem;	
    float x;
    float y;
    float z;
	int pontos;
} Lixo;

typedef struct
{	
    float x;
    float y;
} Coordenadas;

vector<Imagem> imagens;
vector<Interface> interfaces;
vector<Lixo> lixosInicial;
vector<Lixo> lixos;

// Função auxiliar para carregar texturas e armazenas me memória
void carregaTextura(GLuint tex_id, std::string filePath)
{
	unsigned char* imgData;
	int imglargura, imgaltura, imgcanais;

	stbi_set_flip_vertically_on_load(true);
	imgData = stbi_load(filePath.c_str(), &imglargura, &imgaltura, &imgcanais, 4);
	if (imgData)
	{
		glBindTexture(GL_TEXTURE_2D, tex_id);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, imglargura, imgaltura, 0, GL_RGBA, GL_UNSIGNED_BYTE, imgData);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		
		stbi_image_free(imgData);
	}
	else {
		std::cout << "ERRO:: Nao foi possivel carregar a textura!" << filePath.c_str() << std::endl;
	}
	
}

// Função para atribuir texturas a objetos
void imagem(int id, string arquivo, float altura, float largura){
	
	// Se zero, inicia a lista de objetos e texturas
	if(id == 0){
		glGenTextures(imagens.size(), texturas);
		carregaTextura(texturas[0], "texturas/" + arquivo);
		objetos[0] = glGenLists(imagens.size());

		// Caso contrário, apenas obtem a textura
	} else {
		carregaTextura(texturas[id], "texturas/" + arquivo);
		objetos[id] = objetos[0] + id;
	}
	
	// Cria uma lista
	glNewList(objetos[id], GL_COMPILE);
		// Cor para preto
		glColor3f(1.0, 1.0, 1.0);
		// Faz o bind entre o objeto e a Textura
		glBindTexture(GL_TEXTURE_2D, texturas[id]); 
		// Desenha um quadrado
		glBegin(GL_QUADS);
			glTexCoord2f(0.0, 0.0); glVertex3f(-largura/2, -altura/2, 0.0);
			glTexCoord2f(1.0, 0.0); glVertex3f(largura/2, -altura/2, 0.0);
			glTexCoord2f(1.0, 1.0); glVertex3f(largura/2, altura/2, 0.0);
			glTexCoord2f(0.0, 1.0); glVertex3f(-largura/2, altura/2, 0.0);
		glEnd();
	glEndList();
}

// Função para desenha componentes do tipo Interface
void desenhaInterface(int i){
	glPushMatrix();
	glTranslatef(interfaces[i].x, interfaces[i].y, interfaces[i].z);
	glCallList(objetos[interfaces[i].numeroImagem]);
	glPopMatrix();
}

// Função callback de redesenho da janela de visualização
void Desenha(void)
{
	// Limpa a janela de visualização com a cor
	// de fundo definida previamente
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Altera a cor do desenho para preto
	glColor3f(0.0f, 0.0f, 0.0f);

	glLoadIdentity();

	// Rotinas para interface
	for(int i = 0; i < interfaces.size(); i++){
		// Exibir numero de vidas
		if(i == 3 && vidas > 0)
			desenhaInterface(i);
		else if( i == 2 && vidas > 1)
			desenhaInterface(i);
		else if(i == 1 && vidas > 2)
			desenhaInterface(i);

		// Exibe outras interfaces
		else if(i == 0 || i > 3)
			desenhaInterface(i);		
    }

	// Rotinas para lixo
	for(int i = 0; i < lixos.size(); i++){  
		// Incremente negativamente o eixo Y de acordo
		// com a velocidade atual, se o jogo não estiver
		// em Pause
		if(play) lixos[i].y = lixos[i].y - velocidade;   

		// Se atingir o teto minimo, o jogador perde vida
		if(lixos[i].y < -0.8){
			lixos[i].y = lixosInicial[i].y;
			vidas--;
		}  

		// Desenha
		glPushMatrix();
		glTranslatef(lixos[i].x, lixos[i].y, lixos[i].z);
		glCallList(objetos[lixos[i].numeroImagem]);
		glPopMatrix();
    }	

	// Executa os comandos OpenGL
	glFlush();
}

// Função que verifica se um ponto faz parte do objeto
bool verificaPonto(int mouseX, int MouseY, int numeroLixo){
	Imagem imagemPonto = imagens[lixos[numeroLixo].numeroImagem];
	Coordenadas pontoX0Y0 = {((lixos[numeroLixo].x-imagemPonto.largura/2) + 3)*150,((lixos[numeroLixo].y-imagemPonto.altura/2) - 2)*-150};
	Coordenadas pontoX1Y0 = {((lixos[numeroLixo].x+imagemPonto.largura/2) + 3)*150,((lixos[numeroLixo].y-imagemPonto.altura/2) - 2)*-150};
	Coordenadas pontoX1Y1 = {((lixos[numeroLixo].x+imagemPonto.largura/2) + 3)*150,((lixos[numeroLixo].y+imagemPonto.altura/2) - 2)*-150};
	Coordenadas pontoX0Y1 = {((lixos[numeroLixo].x-imagemPonto.largura/2) + 3)*150,((lixos[numeroLixo].y+imagemPonto.altura/2) - 2)*-150};

	if((mouseX > pontoX0Y1.x && mouseX < pontoX1Y1.x) && (MouseY > pontoX1Y1.y && pontoX1Y0.y))
		return true;	
	else 
		return false;	
}

// Função callback chamada quando o tamanho da janela é alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;
	// Evita a divisao por zero
	if(h == 0) h = 1;

	glViewport(0, 0, w, h);

	float aspect = (float)w / (float)h;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, aspect, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
}

// Função callback chamada para gerenciar eventos de teclas
void Teclado (unsigned char key, int x, int y)
{	
	switch (key)
	{
		case 'p':	
			if(play) 
				play = false;
			else
				play = true;
			break;
		case 27:	
			exit(0);
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos de Teclas Especiais
void TeclasEspeciais(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:		
		break;
	case GLUT_KEY_RIGHT:		
		break;
	case GLUT_KEY_DOWN:	
		if(velocidade > velocidadeBase) velocidade = velocidade - velocidadeBase;	
		break;
	case GLUT_KEY_UP:
		velocidade = velocidade + velocidadeBase;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

// Função callback chamada para gerenciar eventos do mouse
void GerenciaMouse(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON){
		if (state == GLUT_DOWN) {
			printf("\n CLICK MOUSE: %d %d", x, y);	
         }
	}
	
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN){
		system("clear");
		for(int i = 0; i < lixos.size(); i++){	
			if(verificaPonto(x, y, i)){
				lixos[i].y = lixosInicial[i].y;
				pontos += lixos[i].pontos;
			}
		}
	} 
         
    glutPostRedisplay();
}

// Função responsável por inicializar parâmetros e variáveis
void Inicializa (void)
{
	// Variáveis e Constantes
	velocidade = velocidadeBase = 0.0001;
	largura = 900;
	altura = 600;
	vidas = 3;
	pontos = 0;
	play = true;

	// Imagens, Interfaces e Lixos
	imagens = {
		{"lago.jpg", 4.124, 6.186},
		{"vida.png", 0.3, 0.3},
		{"lata1.png", 0.5, 0.2},
		{"pneu.png", 0.5, 0.5},
		{"garrafa1.png", 0.5, 0.2},
		{"garrafa2.png", 0.8, 0.5},
		{"papel.png", 0.8, 0.5},
	};

	interfaces = {
		{0, 0.0, 0.0, -5.0},	// Plano de fundo (Lago)
		{1, 2.28, -1.85, -5.0},	// 3 Vidas
		{1, 2.58, -1.85, -5.0}, // 2 Vidas
		{1, 2.88, -1.85, -5.0}, // 1 Vidas
	};

	lixos = {
		{3, -2.5, 2.2, -5.0, 5},// Pneu
		{2, -1.5, 2.2, -5.0, 5},// Lata
		{4, -0.5, 2.2, -5.0, 5},// Garrafa de vidro
		{5, 0.5, 2.2, -5.0, 5},	// Garrafa de Plástico
		{6, 1.5, 2.2, -5.0, 5},	// Papel
		{5, 2.5, 2.2, -5.0, 5},	// Garrafa de Plástico
	};

	lixosInicial = lixos;

	// Define a cor de fundo da janela de visualização como branca
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Texturas e Transparência
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	
	// Ajusta o alinhamento de dados sendo descompactados
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);	

	//Redimensionamento e Reposicionamento a Janela
	glutReshapeWindow(largura, altura);	
	glutPositionWindow((glutGet(GLUT_SCREEN_WIDTH) - largura) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - altura) / 2);

	for(int i = 0; i < imagens.size(); i++){
        imagem(i, imagens[i].arquivo, imagens[i].altura, imagens[i].largura);
    }

}

void debug(void){
	printf("================================================================================\n");
	printf("\n[OBJETIVO]\n");
	printf("\t => Impedir que o lixo contamine o lago!\n");

	printf("\n[REGRAS]\n");
	printf("\t => O jogador inicia com 3 vidas;\n");
	printf("\t => Perde 1 vida cada vez que o lixo contaminar o lago;\n");
	printf("\t => O jogo termina com 0 vidas;\n");

	printf("\n[MOUSE]\n");
	printf("\t => Botão esquerdo coleta o lixo.\n");

	printf("\n[TECLADO]\n");
	printf("\t => Setas para cima e para baixo aumentam/diminuem a velocidade;\n");
	printf("\t => Tecla \"P\" Pause/Play;\n");
	printf("\t => Tecla \"ESC\" Fecha o jogo;\n");	
	printf("================================================================================");
}

// Programa Principal
int main(int argc, char **argv)
{
	debug();

    // Inicia o FreeGlut
    glutInit(&argc, argv);	

	// Define do modo de operação da GLUT
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);	

	// Define a posição inicial da Janela
	glutInitWindowPosition(0, 0);

	// Define o tamanho inicial da janela
	glutInitWindowSize(450, 450);

	// Cria a janela passando como argumento o título da mesma
	glutCreateWindow("Colete o Lixo");

	// Registra a função callback de redesenho da janela de visualização
	glutDisplayFunc(Desenha);

	// Define o retorno de chamada ocioso global como func para que um programa GLUT 
	// possa executar tarefas de processamento em segundo plano ou animação contínua 
	// quando os eventos do sistema de janela não estiverem sendo recebidos.
	glutIdleFunc(Desenha);

	// Registra a função callback de redimensionamento da janela de visualização
	glutReshapeFunc(AlteraTamanhoJanela);

	// Registra a função callback para tratamento das teclas ASCII
	glutKeyboardFunc (Teclado);	

	// Registra a função callback para tratamento das teclas especiais
	glutSpecialFunc(TeclasEspeciais);

	// Registra a função callback para ara gerenciar eventos do mouse
    glutMouseFunc(GerenciaMouse);

	// Chama a função responsável por fazer as inicializações
	Inicializa();	

	// Inicia o processamento e aguarda interações do usuário
	glutMainLoop();

	return 0;
}
