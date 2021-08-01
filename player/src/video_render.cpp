#include "video_render.h"

#define ATTRIB_VERTEX 0
#define ATTRIB_TEXTURE 1

static const char *vertexShaderSource =
"attribute vec4 vertexIn;\n"
"attribute vec2 textureIn;\n"
"varying vec2 textureOut;\n"
"void main(void){\n"
    "gl_Position = vertexIn;\n"
    "textureOut = textureIn;\n"
"}\n";

static const char *fragmentShaderSource =
"varying vec2 textureOut;\n"
"uniform sampler2D tex_y;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"void main(void){\n"
    "vec3 yuv;\n"
    "vec3 rgb;\n"
    "yuv.x = texture2D(tex_y, textureOut).r;\n"
    "yuv.y = texture2D(tex_u, textureOut).r - 0.5;\n"
    "yuv.z = texture2D(tex_v, textureOut).r - 0.5;\n"
    "rgb = mat3(1.0, 1.0, 1.0, 0.0, -0.39465, 2.03211, 1.13983, -0.58060, 0.0) * yuv;\n"
    "gl_FragColor = vec4(rgb, 1.0);\n"
"}\n";

VideoRender::VideoRender(QWidget *parent):
    QOpenGLWidget(parent)
{
    yuvBuffer = nullptr;
}

VideoRender::~VideoRender()
{

}

void VideoRender::initializeGL()
{
    initializeOpenGLFunctions();
    glEnable(GL_DEPTH_TEST);

    shaderProgram = new QOpenGLShaderProgram();
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    shaderProgram->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);

    shaderProgram->bindAttributeLocation("vertexIn", ATTRIB_VERTEX);
    shaderProgram->bindAttributeLocation("textureIn", ATTRIB_TEXTURE);
    shaderProgram->link();
    shaderProgram->bind();

    textureUniformY = shaderProgram->uniformLocation("tex_y");
    textureUniformU = shaderProgram->uniformLocation("tex_u");
    textureUniformV = shaderProgram->uniformLocation("tex_v");


    // 頂點矩陣
    static const GLfloat vertexVertices[] = {
        -1.0f, -1.0f,
         1.0f, -1.0f,
         -1.0f, 1.0f,
         1.0f, 1.0f,
    };

    //紋理矩陣
    static const GLfloat textureVertices[] = {
        0.0f,  1.0f,
        1.0f,  1.0f,
        0.0f,  0.0f,
        1.0f,  0.0f,
    };

    ///設置讀取 YUV 數據為 1 字節對齊，默認為 4 字節默
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);
    glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);

    glEnableVertexAttribArray(ATTRIB_VERTEX);
    glEnableVertexAttribArray(ATTRIB_TEXTURE);

    textureY = new QOpenGLTexture(QOpenGLTexture::Target2D);
    textureU = new QOpenGLTexture(QOpenGLTexture::Target2D);
    textureV = new QOpenGLTexture(QOpenGLTexture::Target2D);

    textureY->create();
    textureU->create();
    textureV->create();

    id_y = textureY->textureId();
    id_u = textureU->textureId();
    id_v = textureV->textureId();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    // 黑色背景
//    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    shaderProgram->release();

}


void VideoRender::paintGL()
{
    shaderProgram->bind();

//    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);    // 黑色背景
    glClear(GL_COLOR_BUFFER_BIT);

    // y 數據紋理
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, id_y);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, yuvBuffer);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // u 數據紋理
    glActiveTexture(GL_TEXTURE1);//激活纹理单元GL_TEXTURE1
    glBindTexture(GL_TEXTURE_2D, id_u);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, (char*)yuvBuffer+width*height);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // v 數據紋理
    glActiveTexture(GL_TEXTURE2);//激活纹理单元GL_TEXTURE2
    glBindTexture(GL_TEXTURE_2D, id_v);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width/2, height/2, 0, GL_RED, GL_UNSIGNED_BYTE, (char*)yuvBuffer+width*height*5/4);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    glUniform1i(textureUniformY, 0);
    glUniform1i(textureUniformU, 1);
    glUniform1i(textureUniformV, 2);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    shaderProgram->release();
}

void VideoRender::resizeGL(int width, int height)
{

}

void VideoRender::getOneFrame(uint8_t *buffer, int w, int h)
{
    width = w;
    height = h;
    yuvBuffer = buffer;
    update();               // 更新 widget
}
