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
#include <iostream>
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

    /**************************** 创建 texture *****************************/

    // 读入图片文件
    image[0].load(":/models/paimeng/Texture/face.jpg");
    image[1].load(":/models/paimeng/Texture/hair.jpg");
    image[2].load(":/models/paimeng/Texture/face.jpg");
    image[3].load(":/models/paimeng/Texture/clothes.jpg");
    image[4].load(":/models/paimeng/Texture/clothes.jpg");
    image[5].load(":/models/paimeng/Texture/hair.jpg");
    image[6].load(":/models/paimeng/Texture/cloak2.jpg");
    image[7].load(":/models/paimeng/Texture/expression.png"); //material_8

    for(int i = 0; i < 8; ++i){
        if(image[i].isNull()){
            qFatal("Image loading failed.\n");
            return;
        }
        else{
            image[i] = image[i].convertToFormat(QImage::Format_RGBA8888);
        }
    }


    // 创建 texture object
//    glGenTextures(8, texture);  //生成纹理的数量，存储纹理索引数据

//    for(int i = 0; i < 8; ++i){
//        if(image[i].isNull()){
//            qFatal("Image loading failed.\n");
//            return ;
//        }
//        // 将图片的数据存储格式转换为 RGBA 格式
//        image[i] = image[i].convertToFormat(QImage::Format_RGBA8888);

//        // 激活 texture unit
//        texUnit[i] = i;
//        glActiveTexture(GL_TEXTURE0 + texUnit[i]);

//        // 绑定纹理 (texture object)
//        glBindTexture(GL_TEXTURE_2D, texture[i]);  //纹理被绑定的目标，纹理名称

//        // 设置纹理属性
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//        // 传递图片像素数据到绑定的纹理
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i].width(), image[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i].constBits());


//        // 生成 Mipmap
//        glGenerateMipmap(GL_TEXTURE_2D);
//    }


//    // 传递变量到 shader
//    glUseProgram(program);
//    GLint texUniformLocation = glGetUniformLocation(program, "tex");


//    glUniform1iv(texUniformLocation, 1, texUnit);
//    glUniform1iv(texUniformLocation, 2, texUnit);
//    glUniform1iv(texUniformLocation, 3, texUnit);
//    glUniform1iv(texUniformLocation, 4, texUnit);
//    glUniform1iv(texUniformLocation, 5, texUnit);
//    glUniform1iv(texUniformLocation, 6, texUnit);
//    glUniform1iv(texUniformLocation, 7, texUnit);
//    glUniform1iv(texUniformLocation, 8, texUnit);

    /***********************************************************************/




    /******************** 从 obj 文件中读入数据 *********************/

    QFile modelFile(":/models/paimeng/paimeng.obj");
    if (!modelFile.open(QFile::ReadOnly | QFile::Text)){
        qDebug() << "Model file open failed." << endl;
        return;
    }

    int test=0;

    QTextStream modelfileTextStream(&modelFile);
    std:: vector<unsigned int> Allindices;
    int cnti2=0;  //三角形面片总数
    int cntv2=0;
    int cntvn=0;
    int cntvt = 0;
    int cntm = 0;
    while(!modelfileTextStream.atEnd()){  // while(!modelfileTextStream.atEnd())
        QString line = modelfileTextStream.readLine();
        if(line[0]=='v'&& line[1]==' '){
            QTextStream in(&line);
            QString temp;
            float a, b, c;
            in >> temp >> a >> b >> c;
            vertices.push_back({a, b, c});
            cntv2++;
        }
        else if(line[1]=='t'){
            QTextStream in(&line);
            QString temp;
            float a, b;
            in >> temp >> a >> b;
            tvertices.push_back({a, b});
            cntvt++;
        }
        else if(line[1] =='n'){
            QTextStream in(&line);
            QString temp;
            float a, b, c;
            in >> temp >> a >> b >> c;
            nvertices.push_back({a, b, c});
            cntvn++;
        }
        else if(line[0]=='f'){  //每个f,ft,fn
            QTextStream in(&line);
            QString temp;
            char x;
            unsigned int a, b, c,d,e,f,g,h,i;
            in >> temp >> a >> x >> b >> x >> c >> d >> x >> e >> x >> f >> g >> x >> h >> x >> i;
            //cout<<a<<b<<c<<d<<e<<f<<g<<h<<i<<endl;
            indices.push_back(a-1);
            tindices.push_back(b-1);
            nindices.push_back(c-1);
            indices.push_back(d-1);
            tindices.push_back(e-1);
            nindices.push_back(f-1);
            indices.push_back(g-1);
            tindices.push_back(h-1);
            nindices.push_back(i-1);
            cnti2++;
        }
        else if(line[0] =='u'){
            int a;
            QString temp;
            temp = line[17];
            a = temp.toInt();
            cout<<a<<endl;
            vbreak[cntm]=cnti2*3;  //下一个子模型起始的顶点 前2964个顶点用材质1
            materials[cntm++]=a;


        }
    }

    vbreak[8]=cnti2*3;

    cout<<"cnti2*3:"<<cnti2*3<<endl;
    cout<<"vbreak[7]:"<<vbreak[7]<<endl;
    cout<<"vbreak[8]:"<<vbreak[8]<<endl;

    cout<<"cntm"<<cntm<<endl;
    cout<<"vbreak"<<vbreak[1]<<endl;
    modelFile.close();

    cout<<"materials"<<materials[0]<<endl;
    for(int i=0;i<cntvt-1;i++){
        tvertices[i][0] = tvertices[i+1][0];
        tvertices[i][1] = tvertices[i+1][1];
    }
    cntvt--;
    modelFile.close();

    float *verticess = new float[cnti2*3*8];  //顶点数cnti2*3，每个顶点信息维度8
    int cntii = cnti2*3;  //重用后的顶点总数
    for(int i=0;i<cntii;i++){  //顶点下标
        verticess[8*i]=vertices[indices[i]][0];
        verticess[8*i+1]=vertices[indices[i]][1];
        verticess[8*i+2]=vertices[indices[i]][2];
        verticess[8*i+3]=tvertices[tindices[i]][0];
        verticess[8*i+4]=tvertices[tindices[i]][1];
        verticess[8*i+5]=nvertices[nindices[i]][0];
        verticess[8*i+6]=nvertices[nindices[i]][1];
        verticess[8*i+7]=nvertices[nindices[i]][2];
    }

    cout<<tindices[0]<<" "<<tindices[1]<<endl;
    cout<<tvertices[0][0]<<" "<<tvertices[0][1]<<endl;

    cout<<verticess[1]<<endl;

    /***************************************************************/


    glGenVertexArrays(8, VAOs);
    glGenBuffers(8, VBOs);

    cout<<"sizeof(verticess)"<<sizeof(verticess)<<endl;
    cout<<"sizeof(GL_FLOAT)"<<sizeof(GL_FLOAT)<<endl;
    cout<<"sizeof(float)"<<sizeof(float)<<endl;


    for(int j=0;j<8;j++){  //用8个vao分别解释8个vbo的信息


//        // 创建 texture object
//        glGenTextures(1, &texture);
//        texUnit = 0;
//        // 激活纹理绑定位置 0 (texture unit 0)
//        glActiveTexture(GL_TEXTURE0 + texUnit);

//        // 绑定纹理 (texture object)
//        glBindTexture(GL_TEXTURE_2D, texture);

//        // 设置纹理属性
//        // 设置环绕属性的参数
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
//        // 设置过滤属性的参数
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//        // 传递图片像素数据到绑定的纹理
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[j].width(), image[j].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image[j].constBits());

//        // 生成 Mipmap
//        glGenerateMipmap(GL_TEXTURE_2D);

//        // 传递变量到 shader
//        glUseProgram(program);
//        GLint texUniformLocation = glGetAttribLocation(program, "tex");
//        //GLint texUniformLocation = glGetUniformLocation(program, "tex");
//        glUniform1i(texUniformLocation, texUnit);
//        /***********************************************************************/

        glBindVertexArray(VAOs[j]);
        glBindBuffer(GL_ARRAY_BUFFER, VBOs[j]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float)*cnti2*3*8, verticess, GL_STATIC_DRAW);   //sizeof不能求动态数组分配的内存！只能对静态数组使用
        // 位置
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void*)(vbreak[j]*8* sizeof(GL_FLOAT)+0* sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(0);
        // 纹理
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void*)(vbreak[j]*8* sizeof(GL_FLOAT)+3 * sizeof(GL_FLOAT)));   //注意是(3 * sizeof(GL_FLOAT))而不是3
        glEnableVertexAttribArray(1);
        // 法向
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (const void*)(vbreak[j]*8* sizeof(GL_FLOAT)+5 * sizeof(GL_FLOAT)));
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);  //解绑当前vbo
        glBindVertexArray(0);  //解绑当前vao
        glBindTexture(GL_TEXTURE_2D, 0);

    }


    // 启用 depth test
    glEnable(GL_DEPTH_TEST);

    camera = new Camera();
    camera->setPosition({0.0f, 0.0f, 5.0f});
    camera->lookTo({0.0f, 0.0f, -1.0f});

    //
    lightPos = QVector3D(100.0f, 100.0f, 100.0f);

}

void OpenGLWidget::paintGL() {

    update(); // necessary 刷新显示
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


    glViewport(0, 0, viewportWidth*2, viewportHeight*2);

    glClearColor(0.0, 0.0, 0.0, 1.0);

    // 清除 color buffer 和 depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

//    for(int i = 0; i < 8; ++i){   //循环次数
//        // 激活 texture unit
//        glActiveTexture(GL_TEXTURE0 + texUnit[i]);
//        // 绑定 texture
//        //glBindTexture(GL_TEXTURE_2D, texture[i]);
//    }



    for(int i=0;i<8;i++){
        // 创建 texture object
        glGenTextures(1, &texture);
        texUnit = 0;
        // 激活纹理绑定位置 0 (texture unit 0)
        glActiveTexture(GL_TEXTURE0 + texUnit);

        // 绑定纹理 (texture object)
        glBindTexture(GL_TEXTURE_2D, texture);

        // 设置纹理属性
        // 设置环绕属性的参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
        // 设置过滤属性的参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 传递图片像素数据到绑定的纹理
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image[i].width(), image[i].height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image[i].constBits());

        // 生成 Mipmap
        glGenerateMipmap(GL_TEXTURE_2D);

        // 传递变量到 shader
        glUseProgram(program);
        GLint texUniformLocation = glGetAttribLocation(program, "tex");
        //GLint texUniformLocation = glGetUniformLocation(program, "tex");
        glUniform1i(texUniformLocation, texUnit);
        /***********************************************************************/

        // 绑定 texture
        glActiveTexture(GL_TEXTURE0 + texUnit);
        glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAOs[i]);
        glDrawArrays(GL_TRIANGLES, 0, vbreak[i+1]-vbreak[i]);
        //cout<<"vbreak--i"<<vbreak[i+1]-vbreak[i]<<" "<<i<<endl;
        glBindTexture(GL_TEXTURE_2D, 0);
    }

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
