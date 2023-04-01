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
#include <iostream>
#include <QFile>
#include <QDebug>
#include <vector>
#include <math.h>
using namespace std;

OpenGLWidget::OpenGLWidget(QWidget * parent) :QOpenGLWidget(parent) {

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    this->setFocusPolicy(Qt::ClickFocus);

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
    vertexShaderFile.setFileName(":/shaders/vertex.glsl");
    fragmentShaderFile.setFileName(":/shaders/fragment.glsl");
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



    /******************** 从 obj 文件中读入数据 *********************/
    QFile modelFile(":/models/StanfordDragon/dragon.obj");
    if (!modelFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Model file open failed." << endl;
        return;
    }

    int cntver=0;
    int cnti=0;
    QTextStream modelfileTextStream(&modelFile);
    while(!modelfileTextStream.atEnd()){  // while(!modelfileTextStream.atEnd())
        // 读入每一行
        QString line = modelfileTextStream.readLine();
        if(line[0]=='v'){
            QVector3D vv;
            int cntv=0;
            int l=line.length();
            QString temp;
            float tt;
            for(int i=2;i<l;i++){
                if(line[i]==' '){
                    tt = temp.toFloat();
                    if(tt!=0){
                        vv[cntv++]=tt;
                        //cout<<"t1:"<<tt<<endl;
                    }
                    temp="";
                }
                else{
                    temp=temp+line[i];
                }
            }
            tt = temp.toFloat();
            //cout<<"t1:"<<tt<<endl;
            vv[cntv++]=tt;
            vertices.push_back(vv);
            cntver++;
        }
        else if(line[0]=='f'){
            int l=line.length();
            QString temp;
            for(int i=2;i<l;i++){
                if(line[i]==' '){
                    int t = temp.toInt();  //atoi(temp.c_str())不行，没有c_str()
                    //cout<<"t2:"<<t<<endl;
                    indices.push_back(t-1);  //obj中存储的坐标从1开始，ebo中从0开始，所以需要-1
                    cnti++;
                    temp="";
                }
                else{
                    temp=temp+line[i];
                }
            }
        }
        // 读取obj文件
        // 并处理平滑法向
    }
    //QVector3D tt;
    //tt[0]=vertices[1][1]-vertices[2][1];
    //cout<<tt[0]<<endl;

    //平滑法向（？

    cout<<cnti<<endl;
    int cntn=0;
    QVector3D nnormal;

    for(int i=0;i<cntver;i++){
        normals.push_back({0.0,0.0,0.0});
    }
    cout<<normals[0][0]<<endl;

    QVector3D a;
    QVector3D b;

    //cout<<indices[1]-1<<endl;
    cout<<vertices[indices[1]-1][0]<<endl;   //indices从下标1开始

    a[0] = vertices[indices[2]-1][0]-vertices[indices[1]-1][0];

    cout<<a[0]<<endl;

    cout<<cnti<<endl;
    for(int i=0;i<cnti-3;i+=3){
        //nnormal = cross((vertices[indices[i+1]]-vertices[indices[i]]),(vertices[indices[i+2]]-vertices[indices[i]])); 
        a[0] = vertices[indices[i+1]][0]-vertices[indices[i]][0];
        a[1] = vertices[indices[i+1]][1]-vertices[indices[i]][1];
        a[2] = vertices[indices[i+1]][2]-vertices[indices[i]][2];

        b[0] = vertices[indices[i+2]][0]-vertices[indices[i]][0];
        b[1] = vertices[indices[i+2]][1]-vertices[indices[i]][1];
        b[2] = vertices[indices[i+2]][2]-vertices[indices[i]][2];

        nnormal[0] = a[1]*b[2]-a[2]*b[1];
        nnormal[1] = a[2]*b[0]-a[0]*b[2];
        nnormal[2] = a[0]*b[1]-a[1]*b[0];

        cntn++;
        for(int j=0;j<3;j++){  //一个三角片的三个顶点，对应记录的法向都要加上算出的一个面的法向
            normals[indices[i]][j]+=nnormal[j];
            normals[indices[i+1]][j]+=nnormal[j];
            normals[indices[i+2]][j]+=nnormal[j];
        }
    }

//注释掉手动normalize是偏灰的smooth

//    cout<<cntn<<endl;
//    cout<<normals[0][0]<<endl;
//    for(int i=0;i<=cntn;i++){  //cntn=99999
//        float fm = pow(pow(normals[i][0],2)+pow(normals[i][1],2)+pow(normals[i][2],2),0.5);
//        normals[i][0] = normals[i][0]/fm;
//        normals[i][1] = normals[i][1]/fm;
//        normals[i][2] = normals[i][2]/fm;
//    }

    modelFile.close();

    /***************************************************************/

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(2, VBO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(QVector3D), &vertices[0], GL_STATIC_DRAW);
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (const void*)0);
    glEnableVertexAttribArray(0);


    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(QVector3D), &normals[0], GL_STATIC_DRAW);
    // 法向
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (const void*)0);
    glEnableVertexAttribArray(1);


    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    // 启用 depth test
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();
    camera->setPosition({0.0f, 0.0f, 5.0f});
    camera->lookTo({0.0f, 0.0f, -1.0f});

    // 点光源位置
    lightPos = QVector3D(100.0f, 100.0f, 100.0f);

}

void OpenGLWidget::paintGL() {

    update(); // necessary
    camera->processKeyboard();

    int viewportWidth = this->width();
    int viewportHeight = this->height();

    float nearPlane = 0.1f;
    float farPlane = 10000.0f;


    model = QMatrix4x4();
    view = QMatrix4x4();
    projection = QMatrix4x4();

    view.lookAt(camera->getPosition(), camera->getPosition() + camera->getFrontDirection(), camera->getWorldUpDirection());
    projection.perspective(camera->getZoom(), viewportWidth * 1.0f / viewportHeight, nearPlane, farPlane);


    glUseProgram(program);

    GLint modelUniformLocation = glGetUniformLocation(program, "model");
    GLint viewUniformLocation = glGetUniformLocation(program, "view");
    GLint projUniformLocation = glGetUniformLocation(program, "proj");

    glUniformMatrix4fv(modelUniformLocation, 1, GL_FALSE, model.constData());
    glUniformMatrix4fv(viewUniformLocation, 1, GL_FALSE, view.constData());
    glUniformMatrix4fv(projUniformLocation, 1, GL_FALSE, projection.constData());

    //
    GLint lightUniformLocation = glGetUniformLocation(program, "lightPosition");
    GLint cameraUniformLocation = glGetUniformLocation(program, "cameraPosition");

    glUniform3f(lightUniformLocation, lightPos.x(), lightPos.y(), lightPos.z());
    glUniform3f(cameraUniformLocation, camera->getPosition().x(), camera->getPosition().y(), camera->getPosition().z());


    glViewport(0, 0, viewportWidth*2, viewportHeight*2);   //using mac

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // 清除 color buffer 和 depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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
