
#ifdef __APPLE__ // MacOS
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else // Windows e Linux
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <windows.h>
#endif
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include "circulo.h"
#define ESC 27

using namespace std;

vector<pair<float,float> > caminho;

vector<pair<float,float> > caminho_2;

GLfloat rosa_caminho[3] = {1.0, 0.0, 1.0};

GLfloat vermelho_caminho[3] = {1.0,0,0};

GLfloat cor_estrelas[3] = {0.988,0.988,0.063};

GLfloat cores_lua[2][3] = {
	{0.870, 0.851, 0.765}, //cor primaria
	{0.772, 0.741, 0.59} //cor secundaria
};

GLfloat cores_foguete[6][3] = {
    {1.0, 0.75, 0.80},   // Rosa claro
    {1.0, 0.43, 0.71},   // Rosa choque
    {0.86, 0.44, 0.58},  // Rosa médio
    {0.74, 0.20, 0.64},  // Roxo rosado
    {1.0, 0.63, 0.63},   // Rosa pêssego
    {0.94, 0.0, 0.5}     // Rosa forte / pink
};

GLfloat cores_foguete_2[6][3] = {
	{1.0,1.0,0},
	{1.0, 0.557, 0.004},
	{0.3098, 0.5059, 0.7373},
	{0.106, 0.2, 0.309},
	{0.749, 0.749, 0.749},
	{1.0,0,0}
};
		
float escala = 0.8f;							
float t_param = 0.0f;   // parâmetro para o coração
float step = 0.02f;     // passo de incremento (velocidade)
float escala_1 = 10.0f;   // escala para ajustar o tamanho
float centroX = 400.0f; // posição central
float centroY = 200.0f;	
float raio = 100;	
float t_param_2 = 2*M_PI; 
GLfloat foguetePosX = 420.0f;
GLfloat foguetePosY = 20.0f;
GLfloat fogueteAngulo = 45.0f;
GLfloat foguetePosX_2 = 420.0f;
GLfloat foguetePosY_2 = 20.0f;
GLfloat fogueteAngulo_2 = 45.0f;	
GLfloat anguloRad = fogueteAngulo * M_PI / 180.0;	
GLfloat velocidadeFoguete = 2.0f;
						
void init(void);
void reshape(int w, int h);
void keyboard(unsigned char key, int x, int y);
void keyboard_special(int key, int x, int y);
void atualiza(int value);
void display(void);
void desenhaCaminho();

class Estrela {
private:
    GLfloat v[3][2];
    GLfloat x, y, scale;
    GLfloat cor[3];
public:
    Estrela(GLfloat posX, GLfloat posY,GLfloat escala, GLfloat cor_estrelas[3]) {
        x = posX;
        y = posY;
        scale = escala;
        for(int i = 0; i < 3;i++) cor[i] = cor_estrelas[i];

		v[0][0] = 0;   v[0][1] = 100;
        v[1][0] = -100; v[1][1] = -50;
        v[2][0] = 100; v[2][1] = -50;
    }

    void draw() const {
        glColor3fv(cor);

        glPushMatrix();
            glTranslatef(x, y, 0);
            glScalef(scale, scale, 1.0);
            glBegin(GL_TRIANGLES);
                glVertex2fv(v[0]);
                glVertex2fv(v[1]);
                glVertex2fv(v[2]);
            glEnd();
        glPopMatrix();

        glPushMatrix();
            glTranslatef(x, y, 0);
            glRotatef(180, 0, 0, 1);
            glScalef(scale, scale, 1.0); 
            glBegin(GL_TRIANGLES);
                glVertex2fv(v[0]);
                glVertex2fv(v[1]);
                glVertex2fv(v[2]);
            glEnd();
            
        glPopMatrix();
    }
};

class Lua {
	private:
    	GLfloat x, y, raio;
    	GLfloat cor[3];
    	GLfloat cor_secundaria[3];
	public:
		Lua(GLfloat posX, GLfloat posY,GLfloat raioLua, GLfloat cores_lua[2][3]){
			x = posX;
        	y = posY;
	        raio = raioLua;
	       	for(int i = 0;i < 3;i++) cor[i] = cores_lua[0][i];
	       	for(int i = 0;i < 3;i++) cor_secundaria[i] = cores_lua[1][i];
    		
		}
		
		void draw() const {
		
		    glPushMatrix(); 
		
		    glTranslatef(x, y, 0); 
		
		    glColor3fv(cor);
		    desenhaCirculo(raio, 360, true);
		
		    glColor3fv(cor_secundaria);
		
		    // Cratera 1
		    glPushMatrix(); 
		    glTranslatef(raio * 0.5f, raio * 0.2f, 0); 
		    desenhaCirculo(raio / 3.5, 360, true);
		    glPopMatrix();
		
		    // Cratera 2
		    glPushMatrix(); 
		    glTranslatef(raio * -0.4f, raio * 0.5f, 0); 
		    desenhaCirculo(raio / 3.5, 360, true); 
		    glPopMatrix();  
		
		    // Cratera 3
		    glPushMatrix(); 
		    glTranslatef(raio * -0.2f, raio * -0.6f, 0);
		    desenhaCirculo(raio / 3.5, 360, true);
		    glPopMatrix();  
		    glPopMatrix(); 
		}
		
};


class Foguete{
	private:
		GLfloat x,y,scala, angulo;
		GLfloat cor_fogo[3];
		GLfloat cor_fogo_2[3];
		GLfloat cor_espelho[3];
		GLfloat cor_espelho_2[3];
		GLfloat cor_principal[3];
		GLfloat cor_asas[3];
	public:
		Foguete(GLfloat posX, GLfloat posY,GLfloat angulo_inclinacao, GLfloat escala,GLfloat cores_foguete[6][3]){
			x = posX;
			y = posY;
			scala = escala;
			angulo = angulo_inclinacao;
			for(int i = 0;i < 3;i++)	cor_fogo[i] = cores_foguete[0][i];
			for(int i = 0;i < 3;i++)	cor_fogo_2[i] = cores_foguete[1][i];
			for(int i = 0;i < 3;i++)	cor_espelho[i] = cores_foguete[2][i];
			for(int i = 0;i < 3;i++)	cor_espelho_2[i] = cores_foguete[3][i];
			for(int i = 0;i < 3;i++)	cor_principal[i] = cores_foguete[4][i];
			for(int i = 0;i < 3;i++)	cor_asas[i] = cores_foguete[5][i];
			
		}
	void draw() const {
		//corpo do foguete
		glColor3fv(cor_principal);
		glPushMatrix(); 
			glTranslated(x,y,0);
			glScaled(scala,scala,0);
			glRotated(angulo,0,0,-1);
			glBegin(GL_QUADS);
	                glVertex2f(0,0);
	                glVertex2f(40,0);
	                glVertex2f(40,90);
	                glVertex2f(0,90);
			glEnd();
			
			glColor3fv(cor_espelho);
				glBegin(GL_QUADS);
	                glVertex2f(0,0);
	                glVertex2f(40,0);
	                glVertex2f(40,8);
	                glVertex2f(0,8);
			glEnd();
			
			//Asas do foguete
			glPushMatrix();
				glScalef(0.9,1,0);
				glTranslatef(47,90,0);
				glRotatef(90,0,0,1);
				glColor3fv(cor_asas);
				glBegin(GL_TRIANGLES);
					glVertex2f(0,0);
					glVertex2f(25,25);
					glVertex2f(0,50);
				glEnd();
			glPopMatrix();
			
			
			
			glPushMatrix();
				glRotatef(23,0,0,1);
				glTranslatef(-25,0,0);
				glBegin(GL_QUADS);
					glVertex2f(0,0);
					glVertex2f(25,0);
					glVertex2f(35,24);
					glVertex2f(10,24);
					
				glEnd();
			glPopMatrix();
			
			glPushMatrix();
				glTranslatef(31,5,0);
				glRotatef(23,0,0,-1);
				glBegin(GL_QUADS);
					glVertex2f(10,0);
					glVertex2f(35,0);
					glVertex2f(25,24);
					glVertex2f(0,24);
					
				glEnd();
			glPopMatrix();		
			
			
			
			//espelho do foguete 
			glPushMatrix();
				glTranslated(20,67,0);
				glColor3fv(cor_espelho_2);
				desenhaCirculo(15,360,true);
				glColor3fv(cor_espelho);
				desenhaCirculo(12,360,true);
			glPopMatrix();
			
			//fogo do foguete
			glPushMatrix();
				glScaled(0.6,0.6,0);
				glTranslated(-4,0,0);
				glRotated(90,0,0,-1);
				glColor3fv(cor_fogo_2);
				glBegin(GL_TRIANGLES);
					glVertex2f(0,0);
					glVertex2f(0,70);
					glVertex2f(70,35);
				glEnd();
			glPopMatrix();
			
			glPushMatrix();
				glColor3fv(cor_fogo);
				glScaled(0.35,0.35,0);
				glTranslated(16,0,0);
				glRotated(90,0,0,-1);
				glBegin(GL_TRIANGLES);
					glVertex2f(0,0);
					glVertex2f(0,70);
					glVertex2f(70,35);
				glEnd();
			glPopMatrix();
			
			//asa do foguete primeiro plano
			glPushMatrix();
			glColor3fv(cor_asas);
				glTranslatef(15,-15,0);
				glBegin(GL_QUADS);
					glVertex2f(0,0);
					glVertex2f(8,0);
					glVertex2f(8,40);
					glVertex2f(0,40);
					
				glEnd();
			glPopMatrix();
		
		glPopMatrix();
		
	}
	void girar(){
		angulo+=5.0f;
	}
};


int main(int argc, char** argv){
    glutInit(&argc, argv); // Passagens de parametros C para o glut
    glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
    glutInitWindowSize (800, 400);  // Tamanho da janela do OpenGL
    glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
    glutCreateWindow ("Imagem Foguete: Trabalho de CG"); // Da nome para uma janela OpenGL
    init(); // Chama a funcao init();
    glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
    glutDisplayFunc(display); //funcao callback de desenho
    glutKeyboardFunc(keyboard); //funcao callback do teclado
  	glutTimerFunc(16, atualiza, 0);
    glutMainLoop(); // executa o loop do OpenGL
    return EXIT_SUCCESS; // retorna 0 para o tipo inteiro da funcao main()
}



void atualiza(int value) {
    
    float x = 16 * pow(sin(t_param), 3);
    float y = 13*cos(t_param) - 5*cos(2*t_param) - 2*cos(3*t_param) - cos(4*t_param);
    float x2 = 16 * pow(sin(t_param_2), 3);
    float y2 = 13*cos(t_param_2) - 5*cos(2*t_param_2) - 2*cos(3*t_param_2) - cos(4*t_param_2);
	//float x = raio*cos(t_param);
	//float y = raio*sin(t_param);
	//float x2 = t_param_2;
	//float y2 = -(1/400.0)*x2*x2+ 2*x2;
    foguetePosX = centroX + escala_1 * x;
    foguetePosY = centroY + escala_1 * y;
     foguetePosX_2 = centroX  + escala_1 * x2;
    foguetePosY_2 = centroY + escala_1 * y2;
    //foguetePosX_2 = x2;
    //foguetePosY_2 = y2;
	//float dx = -raio*sin(t_param);
	//float dy = raio*cos(t_param);
	//float dx_2 = 1.0f;
	//float dy_2 = (-1/200.0)*t_param_2+2;

    float dx = 48 * pow(sin(t_param), 2) * cos(t_param);
    float dy = -13*sin(t_param) + 10*sin(2*t_param) + 6*sin(3*t_param) + 4*sin(4*t_param);
    
    float dx_2 = 48 * pow(sin(t_param_2), 2) * cos(t_param_2);
    float dy_2 = -13*sin(t_param_2) + 10*sin(2*t_param_2) + 6*sin(3*t_param_2) + 4*sin(4*t_param_2);


    fogueteAngulo = atan2(dx, dy)*180.0f / M_PI;
    
    fogueteAngulo_2 =  atan2(dx_2, dy_2)*180.0f / M_PI - 180;

    // Salva no rastro
    caminho.push_back({foguetePosX, foguetePosY});
	caminho_2.push_back({foguetePosX_2,foguetePosY_2});
    // Avança
    t_param += step;
    
    //t_param_2+=velocidadeFoguete;
    t_param_2-=step;

    //if(t_param_2 > 100 && t_param_2 < 200) escala+=0.01;
  	//if(t_param_2 > 200 && t_param_2 < 300) escala -= 0.01;
  
    
    if (t_param >  M_PI) {
        t_param = 0.0f;
        caminho.clear(); // limpa para redesenhar
    }
    if(t_param_2 <  M_PI){
		t_param_2 = 2*M_PI;
		caminho_2.clear();
	}
	glutPostRedisplay();
    glutTimerFunc(16, atualiza, 0);
}

void init(void){
    glClearColor(0.090,0.216,0.369,1.0); //cor do fundo
}



void reshape(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);

    glOrtho (0, 800, 0, 400, -1 ,1);  
	glMatrixMode(GL_MODELVIEW);
}


void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case ESC: exit(EXIT_SUCCESS); break; 
    }
}


void desenhaCaminho(vector<pair<float,float> >c, GLfloat cor[3] ) {
    glColor3fv(cor); // cor rosa para o rastro
    glPointSize(3.0f);        // tamanho dos pontos

    glBegin(GL_POINTS);
    for (int i = 0; i < c.size(); i++) {
        glVertex2f(c[i].first, c[i].second);
    }
    glEnd();
}


void display(void){
	//Limpa o Buffer de Cores e reinicia a matriz
    glClear(GL_COLOR_BUFFER_BIT); 
	glLoadIdentity();

    
	Estrela t1(300,100,0.15, cor_estrelas);
    t1.draw();
    Estrela t2(480,280,0.25, cor_estrelas);
    t2.draw();
 	Estrela t3(560,160,0.15, cor_estrelas);
    t3.draw();
    Estrela t4(680,70,0.25, cor_estrelas);
    t4.draw();
    Estrela t5(90,340,0.25, cor_estrelas);
    t5.draw();
    Lua lua(700,300,70,cores_lua);
    lua.draw();
    desenhaCaminho(caminho,rosa_caminho);
    Foguete foguete(foguetePosX,foguetePosY,fogueteAngulo,0.8,cores_foguete);
    foguete.draw();
    desenhaCaminho(caminho_2,vermelho_caminho);
    glPushMatrix();
    glTranslated(-1,1,0);
     Foguete foguete2(foguetePosX_2,foguetePosY_2,fogueteAngulo_2,escala,cores_foguete_2);
    foguete2.draw();
    glPopMatrix();

    glFlush(); 

}



