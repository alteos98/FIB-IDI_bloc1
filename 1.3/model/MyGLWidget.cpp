#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  xt1 = yt1 = zt1 = 0.0; //translation1
  xt2 = yt2 = zt2 = 0.0; //translation2
  xs = ys = zs = 1.0; //scale
  rot1 = rot2 = 0.0; //rotation
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);

  modelTransform1();
 
  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);

  // Activem l'Array a pintar 
  glBindVertexArray(VAO2);
 
  modelTransform2();

  // Pintem l'escena
  glDrawArrays(GL_TRIANGLES, 0, 3);
  
  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  glViewport (0, 0, w, h);
}

void MyGLWidget::createBuffers ()
{
  glm::vec3 Vertices[3];  // Tres vèrtexs amb X, Y i Z
  Vertices[0] = glm::vec3(0.25, 0.5, 0.0);
  Vertices[1] = glm::vec3(0.5, 0.5, 0.0);
  Vertices[2] = glm::vec3(0.375, 0.75, 0.0);
  
  glm::vec3 Colors[3];  // Tres colors
  Colors[0] = glm::vec3(1.0, .0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);

  glm::vec3 Vertices2[3];  // Tres vèrtexs amb X, Y i Z
  Vertices2[0] = glm::vec3(0.75, 0.5, 0.0);
  Vertices2[1] = glm::vec3(0.5, 0.5, 0.0);
  Vertices2[2] = glm::vec3(0.625, 0.75, 0.0);
  
  glm::vec3 Colors2[3];  // Tres colors
  Colors2[0] = glm::vec3(1.0, .0, 0.0);
  Colors2[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors2[2] = glm::vec3(0.0, 0.0, 1.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO11;
  glGenBuffers(1, &VBO11);
  glBindBuffer(GL_ARRAY_BUFFER, VBO11);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Creació del buffer amb les dades dels colors
  GLuint VBO12;
  glGenBuffers(1, &VBO12);
  glBindBuffer(GL_ARRAY_BUFFER, VBO12);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO2);
  glBindVertexArray(VAO2);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO21;
  glGenBuffers(1, &VBO21);
  glBindBuffer(GL_ARRAY_BUFFER, VBO21);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices2), Vertices2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Creació del buffer amb les dades dels colors
  GLuint VBO22;
  glGenBuffers(1, &VBO22);
  glBindBuffer(GL_ARRAY_BUFFER, VBO22);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors2), Colors2, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/fragshad.frag");
  vs.compileSourceFile("shaders/vertshad.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation (program->programId(), "TG");
}

void MyGLWidget::keyPressEvent (QKeyEvent* e) {
	makeCurrent();
	switch (e->key()) {
		case Qt::Key_Up:
			yt1 += 0.1;
		break;
		
		case Qt::Key_Down:
			yt1 -= 0.1;
		break;
		
		case Qt::Key_Left:
			xt1 -= 0.1;
		break;
		
		case Qt::Key_Right:
			xt1 += 0.1;
		break;

    case Qt::Key_I:
			yt2 += 0.1;
		break;
		
		case Qt::Key_K:
			yt2 -= 0.1;
		break;
		
		case Qt::Key_J:
			xt2 -= 0.1;
		break;
		
		case Qt::Key_L:
			xt2 += 0.1;
		break;

    case Qt::Key_A:
			xs -= 0.1;
		break;

    case Qt::Key_D:
			xs += 0.1;
		break;

    case Qt::Key_W:
      ys += 0.1;
		break;

    case Qt::Key_S:
      ys -= 0.1;
		break;

    case Qt::Key_P:
      rot1 += (float)M_PI/6;
      rot2 -= (float)M_PI/6;
		break;

		default: e->ignore();
	}
	update();
}


void MyGLWidget::modelTransform1 () {
	glm::mat4 TG1(1.0);

  /*El que posem més avall és el primer canvi que apliquem*/
  TG1 = glm::scale(TG1, glm::vec3(xs, ys, zs));
  TG1 = glm::translate(TG1, glm::vec3(xt1, yt1, zt1));
  TG1 = glm::rotate(TG1, rot1, glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG1[0][0]);
}

void MyGLWidget::modelTransform2 () {
	glm::mat4 TG2(1.0);

  /*El que posem més avall és el primer canvi que apliquem*/
  TG2 = glm::scale(TG2, glm::vec3(xs, ys, zs));
  TG2 = glm::translate(TG2, glm::vec3(xt2, yt2, zt2));
  TG2 = glm::rotate(TG2, rot2, glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG2[0][0]);
}