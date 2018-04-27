#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include "glm/glm.hpp"
#include <QKeyEvent>
#include "glm/gtc/matrix_transform.hpp"

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
    void modelTransform ();
    
    float xi, yi, zi; //per al modelTransform
    glm::mat4 TG;
    
    // attribute locations
    GLuint vertexLoc;
    GLuint colorLoc;
    GLuint transLoc;
    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO1;
};
