#include<SDL/SDL.h>
#include<stdio.h>
#include"SDL/SDL_opengl.h"
#include<stdbool.h>
#include<stdlib.h>
#include<time.h>


#define BLOCOS 19


bool colisao(float Ax, float Ay, float Acomp, float Aalt, float Bx, float By, float Bcomp,float Balt)
{

if(Ay+Aalt < By) return false;
else if(Ay > By+Balt) return false;
else if(Ax+Acomp < Bx) return false;
else if(Ax > Bx+Bcomp) return false;
else if(Ax > Bx+Bcomp) return false;

return true;
}

struct Bloco
{
float x;
float y;
float comp;
float alt;
bool vivo;
};

int main(int argc, char* args[]){

  SDL_Init(SDL_INIT_EVERYTHING);

  //memória
  SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
  SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,16);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);

  //nome janela
  SDL_WM_SetCaption("Meu primeiro jogo",NULL);
  //tamanho janela
  SDL_SetVideoMode(600,400,32,SDL_OPENGL);

  //cor
  glClearColor(1,1,1,1);

  //area exibida
  glViewport(0,0,600,400);

  //sombra
  glShadeModel(GL_SMOOTH);

  // 2D
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity(); // desenho geometrico

  //3D
  glDisable(GL_DEPTH_TEST);

  bool executando = true;

  SDL_Event eventos;
  //personagem
  float personx = 300;
  float persony = 350;
  float personComp = 50;
  float personAlt = 30;

  Bloco blocos[BLOCOS];

//criacao dos blocos

  for(int n = 0, x = 4, y = 10; n < BLOCOS; n++, x += 64)
  {
    if(x > 552)
    {
      x = 4;
      y += 23;
    }

    blocos[n].x = x;
    blocos[n].y = y;
    blocos[n].comp = 60;
    blocos[n].alt = 20;
    blocos[n].vivo = true;
  }
  //inimigo
  float inimigox = 50;
  float inimigoy = 50;
  float inimigoCA = 30;



  float velx = 4;
  float vely = 4;

  bool esq = false, dir = false;
  bool inimigo = true;

  //loop do jogo
  while(executando)
  {

  //eventos
    while(SDL_PollEvent(&eventos))
    {
      //fecha com o x da janela
      if(eventos.type == SDL_QUIT){
        executando = false;
      }

      if(eventos.type == SDL_KEYUP && eventos.key.keysym.sym == SDLK_ESCAPE){
        executando = false;
      }

      //anda ou não
      if(eventos.type == SDL_KEYDOWN)
      {
        if(eventos.key.keysym.sym == SDLK_LEFT)
        {
          esq = true;
        }

        else if(eventos.key.keysym.sym == SDLK_RIGHT)
        {
          dir = true;
        }

      }
      else if(eventos.type == SDL_KEYUP)
      {
        if(eventos.key.keysym.sym == SDLK_LEFT)
        {
          esq = false;
        }

        if(eventos.key.keysym.sym == SDLK_RIGHT)
        {
          dir = false;
        }
      }
    }

    //movimento do personagem
    if(esq == true){
      personx -= 8;
    }

    else if(dir == true){
      personx += 8;
    }

    //logica personagem

    if(personx < 0)
    personx = 0;

    else if (personx + personComp > 600) {
      personx = 600 - personComp;
    }



    //movimento do inimigo
    inimigox += velx;
    inimigoy += vely;

    //blocos de cimas
    for(int n = 0; n < BLOCOS; n++)
    {
      if(blocos[n].vivo == true)
      {
        if(colisao(inimigox,inimigoy,inimigoCA,inimigoCA,blocos[n].x,blocos[n].y,blocos[n].comp,blocos[n].alt == true))
        {
          velx = -velx;
          vely = -vely;
          blocos[n].vivo = false;
          break;
        }
      }
    }


    //logica inimigox
    if(inimigox < 0)
    velx = -velx;


    else if (inimigox + inimigoCA > 600) {
      velx = -velx;
    }

    else if (inimigoy < 0) {
      vely = -vely;
    }
    else if (inimigoy + inimigoCA > 400) {
      vely = -vely;
      executando = false;
    }

    if(colisao(inimigox,inimigoy,inimigoCA,inimigoCA,personx,persony,personComp,personAlt) == true)
    {
      vely = -vely;
    }

    //RENDERIZAÇÃO
    glClear(GL_COLOR_BUFFER_BIT);

    //INICIA MATRIZ
    glPushMatrix();

    //dimensões da matriz
    glOrtho(0, 600,400,0,-1, 1);

    //cor
    glColor4ub(255,0,0,255); //vermelho

    //inicia o desenho do personagen
    glBegin(GL_QUADS);

    glVertex2f(personx,persony);//primeiro ponto
    glVertex2f(personx+personComp,persony);//segundo pont0
    glVertex2f(personx+personComp,persony+personAlt);
    glVertex2f(personx,persony+personAlt);

    //fecha
      glEnd();

      glColor4ub(0,0,255,255);
      glBegin(GL_QUADS);

    //inicia o desenho do inimigo
    glVertex2f(inimigox,inimigoy);//primeiro ponto
    glVertex2f(inimigox+inimigoCA,inimigoy);//segundo pont0
    glVertex2f(inimigox+inimigoCA,inimigoy+inimigoCA);
    glVertex2f(inimigox,inimigoy+inimigoCA);

    //fecha
      glEnd();

      //inicia os blocos de cima
      glBegin(GL_QUADS);
      glColor4ub(0,0,255,255);

      for(int n = 0; n < BLOCOS; n++)
      {
        if(blocos[n].vivo == true)
        {

        glVertex2f(blocos[n].x,blocos[n].y);
        glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y);
        glVertex2f(blocos[n].x + blocos[n].comp, blocos[n].y + blocos[n].alt);
        glVertex2f(blocos[n].x, blocos[n].y + blocos[n].alt);
        }
      }
      glEnd();

    //FECHA MATRIZ
    glPopMatrix();

    //ANIMAÇÃO
    SDL_GL_SwapBuffers();
    inimigoy += 0,1;
}
  SDL_Quit();

  return 0;

}
