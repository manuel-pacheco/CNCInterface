#ifndef WIDGETOPENGL_H
#define WIDGETOPENGL_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QGLFramebufferObjectFormat>

class WidgetOpenGL : public QOpenGLWidget , protected QOpenGLFunctions
{
public:
    WidgetOpenGL();
explicit WidgetOpenGL(QWidget *parent = 0);
protected:
void initializeGL() Q_DECL_OVERRIDE;//reimplementando las funciones virtuales
void resizeGL(int w, int h)Q_DECL_OVERRIDE;
void paintGL()Q_DECL_OVERRIDE;
};

#endif // WIDGETOPENGL_H
