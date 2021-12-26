#include "VideoWidget.h"

#include <QOpenGLShader>
#include <QDebug>
#include <QTimer>
#include <QOpenGLExtraFunctions>

#define STR(x) #x
#define A_VER 3
#define T_VER 4

FILE *fp = NULL;

// vertext shader
const char *vString = STR(
    attribute vec4 vertexIn;
    attribute vec2 textureIn;
    varying vec2 textureOut;
    void main(void)
    {
        gl_Position = vertexIn;
        textureOut = textureIn;
    }
);


//fragment shader
const char *tString = STR(
    varying vec2 textureOut;
    uniform sampler2D tex_y;
    uniform sampler2D tex_u;
    uniform sampler2D tex_v;
    void main(void)
    {
        vec3 yuv;
        vec3 rgb;
        yuv.x = texture2D(tex_y, textureOut).r;
        yuv.y = texture2D(tex_u, textureOut).r - 0.5;
        yuv.z = texture2D(tex_v, textureOut).r - 0.5;
        rgb = mat3(
            1.0, 1.0, 1.0,
            0.0, -0.39465, 2.03211,
            1.13983, -0.58060, 0.0) * yuv;
        gl_FragColor = vec4(rgb, 1.0);
    }
);



VideoWidget::VideoWidget(QWidget *parent):QOpenGLWidget(parent)
{

}

VideoWidget::~VideoWidget(){

}


void VideoWidget::initializeGL()
{
    qDebug() << "initializeGL";

    initializeOpenGLFunctions();
    qDebug()<< program.addShaderFromSourceCode(QOpenGLShader::Fragment, tString);
    qDebug() << program.addShaderFromSourceCode(QOpenGLShader::Vertex, vString);

    program.bindAttributeLocation("vertexIn",A_VER);
    program.bindAttributeLocation("textureIn",T_VER);
    qDebug() << "program.link() = " << program.link();
    qDebug() << "program.bind() = " << program.bind();


     static const GLfloat ver[] = {
            -1.0f,-1.0f,
            1.0f,-1.0f,
            -1.0f, 1.0f,
            1.0f, 1.0f
        };

      static const GLfloat tex[] = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f
        };

     glEnableVertexAttribArray(0);
     glVertexAttribPointer(A_VER, 2, GL_FLOAT, 0, 0, ver);
     glEnableVertexAttribArray(A_VER);

     glVertexAttribPointer(T_VER, 2, GL_FLOAT, 0, 0, tex);
     glEnableVertexAttribArray(T_VER);

     unis[0] = program.uniformLocation("tex_y");
     unis[1] = program.uniformLocation("tex_u");
     unis[2] = program.uniformLocation("tex_v");

     glGenTextures(3, texs);

     glBindTexture(GL_TEXTURE_2D, texs[0]);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, 0);


     glBindTexture(GL_TEXTURE_2D, texs[1]);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

     glBindTexture(GL_TEXTURE_2D, texs[2]);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width / 2, height / 2, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

     datas[0] = new unsigned char[width*height];		// Y
     datas[1] = new unsigned char[width*height/4];	    // U
     datas[2] = new unsigned char[width*height/4];      // V

     fp = fopen("/Users/waz/Documents/out.yuv","rb");

     if (!fp)
     {
        qDebug() << "out.yuv file open failed!";
     }

     QTimer *ti = new QTimer(this);
     connect(ti, SIGNAL(timeout()), this, SLOT(update()));
     ti->start(40);
}

void VideoWidget::paintGL()
{
    qDebug() << "paintGL";
    if (feof(fp))
    {
        fseek(fp, 0, SEEK_SET);
    }
    fread(datas[0], 1, width*height, fp);
    fread(datas[1], 1, width*height/4, fp);
    fread(datas[2], 1, width*height/4, fp);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texs[0]);

    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RED, GL_UNSIGNED_BYTE, datas[0]);
    glUniform1i(unis[0], 0);

    glActiveTexture(GL_TEXTURE0+1);
    glBindTexture(GL_TEXTURE_2D, texs[1]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width/2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[1]);
    glUniform1i(unis[1],1);


    glActiveTexture(GL_TEXTURE0+2);
    glBindTexture(GL_TEXTURE_2D, texs[2]);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width / 2, height / 2, GL_RED, GL_UNSIGNED_BYTE, datas[2]);
    glUniform1i(unis[2], 2);

    glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //qDebug() << "paintGL done";

}

void VideoWidget::resizeGL(int width, int height)
{
    qDebug() << "resizeGL "<<width<<":"<<height;
}
