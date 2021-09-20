#ifndef SHOWPROYECTINWIDGET_H
#define SHOWPROYECTINWIDGET_H
#include <QOpenGLWidget>
#include <QOpenGLFunctions>

#include <QGLFramebufferObjectFormat>
class ShowProyectInWidget : public QOpenGLWidget , protected QOpenGLFunctions
{
public:
    explicit ShowProyectInWidget(QWidget *parent = nullptr);
protected:
void initializeGL() Q_DECL_OVERRIDE;
void resizeGL(int w, int h)Q_DECL_OVERRIDE;
void paintGL()Q_DECL_OVERRIDE;
void graficarLineas();

public:
double rotate_y=0;
double rotate_x=0;
double rotate_z=0;

};

#endif // SHOWPROYECTINWIDGET_H
