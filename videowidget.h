#ifndef VIDEOWIDGET_H
#define VIDEOWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QWidget>

class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    VideoWidget(QWidget *parent );
    ~VideoWidget();

private:
    void paintGL();
    void initializeGL();
    void resizeGL(int width, int height);

    QOpenGLShaderProgram program;

    GLuint unis[3] = {0};
    GLuint texs[3] = { 0 };

    int width = 256;
    int height = 144;

    unsigned char *datas[3] = { 0 };
};

#endif // VIDEOWIDGET_H
