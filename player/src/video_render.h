#ifndef VIDEO_RENDER_H
#define VIDEO_RENDER_H

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>

class VideoRender : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    VideoRender(QWidget *parent = nullptr);
    ~VideoRender();

    void getOneFrame(uint8_t *buffer, int w, int h);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    QOpenGLShaderProgram *shaderProgram;
    GLuint textureUniformY;
    GLuint textureUniformU;
    GLuint textureUniformV;
    QOpenGLTexture *textureY;
    QOpenGLTexture *textureU;
    QOpenGLTexture *textureV;
    GLuint id_y;
    GLuint id_u;
    GLuint id_v;


    int width;
    int height;
    uint8_t *yuvBuffer;
};

#endif // VIDEO_RENDER_H
