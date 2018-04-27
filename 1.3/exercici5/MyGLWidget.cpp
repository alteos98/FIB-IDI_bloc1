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
  
  xt = yt = zt = 0.0; //translation
  xs = ys = zs = 1.0; //scale
  xq = yq = NULL;
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  createBuffers();
}

void MyGLWidget::paintGL ()
{
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Activem l'Array a pintar 
  glBindVertexArray(VAO1);

  modelTransform();
 
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
  Vertices[0] = glm::vec3(-1.0, -1.0, 0.0);
  Vertices[1] = glm::vec3(1.0, -1.0, 0.0);
  Vertices[2] = glm::vec3(0.0, 1.0, 0.0);
  
  glm::vec3 Colors[3];  // Tres colors
  Colors[0] = glm::vec3(1.0, .0, 0.0);
  Colors[1] = glm::vec3(0.0, 1.0, 0.0);
  Colors[2] = glm::vec3(0.0, 0.0, 1.0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar
  glGenVertexArrays(1, &VAO1);
  glBindVertexArray(VAO1);

  // Creació del buffer amb les dades dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
  // Activem l'atribut que farem servir per vèrtex	
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  // Creació del buffer amb les dades dels colors
  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);
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
			yt += 0.1;
		break;
		
		case Qt::Key_Down:
			yt -= 0.1;
		break;
		
		case Qt::Key_Left:
			xt -= 0.1;
		break;
		
		case Qt::Key_Right:
			xt += 0.1;
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

		default: e->ignore();
	}
	update();
}


void MyGLWidget::modelTransform () {
	glm::mat4 TG(1.0);

  /*El que posem més avall és el primer canvi que apliquem*/
  TG = glm::scale(TG, glm::vec3(xs, ys, zs));
  TG = glm::translate(TG, glm::vec3(xt, yt, zt));
  //TG = glm::rotate(TG, (float)M_PI/4, glm::vec3(0.0, 0.0, 1.0));

	glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouseMoveEvent (QMouseEvent* e) {
  makeCurrent();

  if (xq.isNull() or yq.isNull()) {
    xq = e.pos().x();
    yq = e.pos().y();
  }
  else if (xq > e.pos().x()) {
    xs -= 0.1;
  }
  else if (xq < e.pos().x()) {
    xs += 0.1;
  }
  else if (yq > e.pos().y()) {
    ys -= 0.1;
  }
  else if (yq < e.pos().y()) {
    ys += 0.1;
  }

  update();
}