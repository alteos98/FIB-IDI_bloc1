#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"

#define GLM_FORCE_RADIANS

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);
    
    //Quan premem una tecla
    virtual void keyPressEvent (QKeyEvent* e);

    /*
    //Quan premem un botó del ratolí
    virtual void mousePressEvent (QMouseEvent* e);
    
    //Quan deixem anar un botó del ratolí
    virtual void mouseReleaseEvent (QMouseEvent* e);*/
    
    //Quan movem el ratolí per la pantalla (botó del ratolí premut)
    virtual void mouseMoveEvent (QMouseEvent* e);

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform ();
    
    float xt, yt, zt; //translation
    float xs, ys, zs; //scale
    glm::mat4 TG; //matriu transformació
    
    QPoint xq, yq; //per saber pos anterior mouse

    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint transLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1;
};
