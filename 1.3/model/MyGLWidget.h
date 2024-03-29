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

  private:
    void createBuffers ();
    void carregaShaders ();
    void modelTransform1 ();
    void modelTransform2 ();
    
    float xt1, yt1, zt1; //translation1
    float xt2, yt2, zt2; //translation2
    float xs, ys, zs; //scale
    float rot1; //rotation1
    float rot2; //rotation2

    glm::mat4 TG1; //matriu transformació
    glm::mat4 TG2; //matriu transformació
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint transLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1;
    GLuint VAO2;
};
