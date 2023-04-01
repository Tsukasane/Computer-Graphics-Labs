/*!
*	File: OpenGLWidget.cpp
*	Date: 2018/03/01 19:42
*
*	Author: Zhang Yu
*	Contact: YuCrazing@mail.bnu.edu.cn
*
*	Description:
*	https://learnopengl.com/
*
*
*/

#include "OpenGLWidget.h"

#include <QFile>
#include <QDebug>

OpenGLWidget::OpenGLWidget(QWidget * parent) :QOpenGLWidget(parent) {

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    this->setFocusPolicy(Qt::ClickFocus);
    this->setMouseTracking(true);
    Q_INIT_RESOURCE(Resources);
}

void OpenGLWidget::initializeGL() {

    // 在任何类中使用 OpenGL 函数，需要先调用 initializeOpenGLFunctions();
    // 除了 initializeGL() 与 paintGL() 之外，其他函数中如需要使用 OpenGL 函数，需要先调用 makeCurrent()

    if (!initializeOpenGLFunctions()) {
        qFatal("OpenGLWidget::initializeOpenGLFunctions() failed.\n");
        return;
    }

    QFile vertexShaderFile, fragmentShaderFile;
    vertexShaderFile.setFileName(":/shaders/shader.vert");
    fragmentShaderFile.setFileName(":/shaders/shader.frag");
    if (!vertexShaderFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Vertex shader file open failed." << endl;
        return;
    }
    if (!fragmentShaderFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Fragment shader file open failed." << endl;
        return;
    }
    QTextStream vertexShaderTextStream(&vertexShaderFile);
    QTextStream fragmentShaderTextStream(&fragmentShaderFile);
    QByteArray vertexShaderSourceTemp = vertexShaderTextStream.readAll().toLocal8Bit();
    const char * vertexShaderSource = vertexShaderSourceTemp.data();
    QByteArray fragmentShaderSourceTemp = fragmentShaderTextStream.readAll().toLocal8Bit();
    const char * fragmentShaderSource = fragmentShaderSourceTemp.data();

	vertexShaderFile.close();
    fragmentShaderFile.close();


    const int bufSize = 1 << 10;
    GLint success;
    GLchar infoLog[bufSize];

    GLuint vertexShader, fragmentShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);

    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, bufSize, NULL, infoLog);
        qDebug("Vertex shader compiled error: %s\n", infoLog);
    }
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, bufSize, NULL, infoLog);
        qDebug("Fragment shader compiled error: %s\n", infoLog);
    }

    program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, bufSize, NULL, infoLog);
        qDebug("Shader program linked error: %s\n", infoLog);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    // 绘制一个正方体，包含六个面，每个面绘制两个三角形，因此需要绘制36个顶点
    float vertices[] = {
        // 背面（绿色）
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
        // 正面（紫色）
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
        // 左面（金黄色）
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
        // 右面（青色）
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
        // 底面（蓝色）
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
        // 顶面（红色）
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
    };


    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 配置顶点位置属性
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (const void*)0);
    glEnableVertexAttribArray(0);
    // 配置顶点颜色属性
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (const void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    // 启用 depth test
    glEnable(GL_DEPTH_TEST);
    

    // 新建一个 camera 对象
    camera = new Camera();
    // 设置 camera 位置
    camera->setPosition({0.0f, 0.0f, 5.0f});
    // 设置 camera 的两个自由度（第三个自由度固定）
    camera->lookTo({0.0f, 0.0f, -1.0f});

}

int Rotate = 45;

void OpenGLWidget::paintGL() {
    update();
    camera->processKeyboard();

    // 视窗宽度与视窗高度
    int viewportWidth = this->width();
    int viewportHeight = this->height();

    // 近平面距离与远平面距离（棱台的顶部与底部）
    float nearPlane = 0.1f;
    float farPlane = 10000.0f;

    // model = QMatrix4x4(); // 下方循环中进行初始化｜｜单位矩阵，初始化不要删掉
    view = QMatrix4x4();
    projection = QMatrix4x4();


    // 若物体本身需要变换，可以修改 model 矩阵
    // model.?

    view.lookAt({camera->getPosition()},{camera->getFrontDirection()},{camera->getWorldUpDirection()});  //eye(position),center(coi),up

    // 第二个位置写camera->getPosition()+camera->getFrontDirection()或者camera->getFrontDirection()都可以
    // 计算 view 矩阵
    // 可能用到的函数:
    // 相机位置: camera->getPosition()  返回x,y,z
    // 相机前方: camera->getFrontDirection()
    // 相机（世界）上方: camera->getWorldUpDirection()
    // view.lookAt();

    projection.perspective({camera->getZoom()},1.0f*viewportWidth/viewportHeight,nearPlane,farPlane);  //float需要*1.0才是小数

    // 计算 projection 矩阵
    // 可能用到的函数:
    // 相机 FOV （角度制）: camera->getZoom()
    // 视窗宽、高: viewportWidth, viewportHeight
    // 近平面、远平面: nearPlane, farPlane
    // projection.perspective();

    // model.translate(0.0f,-1.0f,0.0f);
    glUseProgram(program);

    // 将变量传递到 shader （从 CPU 传递到 GPU）
    // GLint modelUniformLocation = glGetUniformLocation(program, "model"); 放在了后面的循环里
    GLint viewUniformLocation = glGetUniformLocation(program, "view");
    GLint projUniformLocation = glGetUniformLocation(program, "proj");

    // glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, model.constData());
    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, view.constData());
    glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, projection.constData());

    // 设置视窗的位置与大小
    // multiply devicePixelRatio() on retina display
    // retina 显示屏需要把 viewportWidth 和 viewportHeight 乘以 2
    glViewport(0, 0, viewportWidth*2,  viewportHeight*2);  //已乘2
    //projection.ortho(-1.5f,1.5f,-0.5f,0.5f,nearPlane,farPlane);

    //基于Qt的opengl可能与原opengl不同
    //float aspectRatio = (GLfloat)viewportHeight/(GLfloat)viewportWidth;
    //glMatrixMode(GL_PROJECTION);
    //glLoadIdentity();
    //if (aspectRatio<=1)
    //    glOrtho(-1.5, 1.5, -1.5 * aspectRatio, 1.5 * aspectRatio, -10.0, 10.0);
    //else
    //    glOrtho(-1.5*(1/aspectRatio), 1.5*(1/aspectRatio), -1.5, 1.5, -10.0, 10.0);
    //glMatrixMode(GL_MODELVIEW);
    //glLoadIdentity();



    glClearColor(0.0, 0.0, 0.0, 1.0);
    // 清除 color buffer 和 depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //绘制第一个正方体
    glBindVertexArray(VAO);

    for(int i = 0; i < 2; i++)
            {
                model = QMatrix4x4();  //循环里面，这句就相当于model.setToIdentity();
                if( i == 0 )
                {
                    model.translate(0.0f,-0.5f,0.0f); //只要一个三维的向量即可
                    model.rotate(Rotate,1,1,0); //绕x=y旋转
                    model.scale(0.5f,0.5f,0.5f);  //允许每个分量有不同的缩放大小
                    Rotate+=15; //一直在刷新
                }
                else
                {
                    model.rotate(150,1,1,0);
                    model.translate(-0.3f,0.5f,0.0f);
                    model.scale(0.3f);
                }
                GLint modelUniformLocation = glGetUniformLocation(program, "model");
                glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, model.constData());
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }




   // glDrawArrays(GL_TRIANGLES, 0, 36);

    // 改变 model 矩阵前，注意将其先重置为单位矩阵，然后将其传递至 shader
    // 绘制第二个正方体
    // 不用循环的方法
    //model.setToIdentity();
    //model.rotate(150,1,1,0);
    //model.translate(-0.3f,0.5f,0.0f);
    //model.scale(0.3f);
    //GLint modelUniformLocation = glGetUniformLocation(program, "model");
    //glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, model.constData());
    //glDrawArrays(GL_TRIANGLES, 0, 36);


}

void OpenGLWidget::resizeGL(int w, int h) {
    makeCurrent(); // necessary
}

void OpenGLWidget::mousePressEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    camera->mousePressEvent(event);
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    camera->mouseReleaseEvent(event);
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent * event)
{
    makeCurrent(); // necessary
    camera->mouseMoveEvent(event);
    emit cursorPosition("Cursor Position: (" + QString::number(event->x()) + ", " + QString::number(event->y()) + ")");
}

void OpenGLWidget::keyPressEvent(QKeyEvent * event)
{
    makeCurrent(); // necessary
    camera->keyPressEvent(event);
}

void OpenGLWidget::keyReleaseEvent(QKeyEvent * event)
{
    makeCurrent(); // necessary
    camera->keyReleaseEvent(event);
}
