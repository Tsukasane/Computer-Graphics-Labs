/*!
*	File: OpenGLWidget.cpp
*	Date: 2018/03/01 19:42
*
*	Author: Zhang Yu
*	Contact: YuCrazing@mail.bnu.edu.cn
*
*	Description:
*
*
*
*/


#include "OpenGLWidget.h"
#include <iostream>
#include <QFile>
#include <QTime>

OpenGLWidget::OpenGLWidget(QWidget * parent) :QOpenGLWidget(parent) {

    QSurfaceFormat format = QSurfaceFormat::defaultFormat();
    format.setVersion(4, 0);
    format.setProfile(QSurfaceFormat::OpenGLContextProfile::CoreProfile);
    format.setSwapBehavior(QSurfaceFormat::DoubleBuffer);
    format.setSwapInterval(0);
    QSurfaceFormat::setDefaultFormat(format);
    this->setFormat(format);

    this->setUpdateBehavior(QOpenGLWidget::PartialUpdate); //

    this->setFocusPolicy(Qt::ClickFocus);

    Q_INIT_RESOURCE(Resources);
}

void OpenGLWidget::render()
{
    //todo
    //完成光线追踪的主要程序
    img = QImage(nx,ny,QImage::Format_RGB888);
    double aspect_ratio = 1.0*nx / ny;

    //camera
    double viewport_height = 2.0;
    double viewport_width = aspect_ratio * viewport_height;
    double focal_length = 1.0;

    QVector3D origin = QVector3D{0, 0, 0};
    QVector3D horizontal = QVector3D{static_cast<float>(viewport_width), 0, 0};
    QVector3D vertical = QVector3D{0, static_cast<float>(viewport_height), 0};
    QVector3D lower_left_corner = origin - horizontal/2 - vertical/2 - QVector3D{0, 0, static_cast<float>(focal_length)};



//    camera = Camera({0,0,0},{0,0,-1},{0,1,0},focal_length,aspect_ratio,1.0,1.0);
//    Functions f;
//    int samples_per_pixel = 3;
//    for (int j = ny-1; j >= 0; --j) {
//        for (int i = 0; i < nx; ++i) {
//            QVector3D ray_color = {0,0,0};
//            for (int s = 0; s < samples_per_pixel; ++s) {
//                auto u = (i + f.random()) / (nx-1);
//                auto v = (j + f.random()) / (ny-1);
//                Ray r = camera.get_ray(u,v);
//                ray_color += scene.getColor(r,10);
//            }
//            int ir = 255.999*ray_color[0];
//            int ig = 255.999*ray_color[1];
//            int ib = 255.999*ray_color[2];
//            img.setPixel(i,j,qRgb(ir,ig,ib));
//         }
//     }

    //测试image2（ray tracing in one weekend），上下颠倒 (当作背景)
    for (int j = ny-1; j >= 0; --j) {
        for (int i = 0; i < nx; ++i) {
            auto u = double(i) / (nx-1);
            auto v = double(j) / (ny-1);

            Ray myray(origin, lower_left_corner + u*horizontal + v*vertical - origin);
            QVector3D ray_color = scene.getColor(myray,10);  //(ray,递归深度)

            int ir = 255.999*ray_color[0];
            int ig = 255.999*ray_color[1];
            int ib = 255.999*ray_color[2];

            img.setPixel(i,j,qRgb(ir,ig,ib));
        }
    }


    //测试image1（ray tracing in one weekend），上下颠倒
//    for (int j = ny-1; j >= 0; --j) {
//        for (int i = 0; i < nx; ++i) {
//            auto r = double(i) / (nx-1);
//            auto g = double(j) / (ny-1);
//            auto b = 0.25;

//            int ir = static_cast<int>(255.999 * r);
//            int ig = static_cast<int>(255.999 * g);
//            int ib = static_cast<int>(255.999 * b);

//            img.setPixel(i,j,qRgb(ir,ig,ib));
//            //std::cout << ir << ' ' << ig << ' ' << ib << '\n';
//        }
//    }


}

void OpenGLWidget::initializeGL() {

    nx = this->width();
    ny = this->height();

    QVector3D center1 = {-0.1,0.1,-1};
    QVector3D center2 = {0.5,0.3,-1};
    QVector3D center3 = {0.2,-0.3,-1};

    float r1 = 0.3;
    float r2 = 0.2;
    float r3 = 0.3;

    Lambertian m1 = Lambertian({0.9,0.3,0.3});  //{1.5,0.3,0.3}
    Metal m2 = Metal({0.7,0.2,0.3});
    Dielectric m3 = Dielectric(0.02);

    Sphere mySphere1;
    mySphere1.center = center1;
    mySphere1.radius = r1;
    mySphere1.material = &m1;
    scene.objects.push_back(mySphere1);

    Sphere mySphere2;
    mySphere2.center = center2;
    mySphere2.radius = r2;
    mySphere2.material = &m2;
    scene.objects.push_back(mySphere2);

    Sphere mySphere3;
    mySphere3.center = center3;
    mySphere3.radius = r3;
    mySphere3.material = &m3;
    scene.objects.push_back(mySphere3);

    render();

    //在这里调用render()函数
    //利用定义好的sphere类与材质类，定义一个你想要的场景
}

void OpenGLWidget::paintGL() {

    update(); // necessary
    QPainter painter;
    painter.begin(this);
    //painter.setViewport(0, 0, this->width(), this->height());
    painter.drawImage(0, 0, img.scaledToWidth(this->width()));
    painter.end();

}

void OpenGLWidget::resizeGL(int w, int h) {
    makeCurrent(); // necessary
}


