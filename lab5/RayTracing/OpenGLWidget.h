/*!
*	File: OpenGLWidget.h
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

#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H


#include <QDebug>
#include <QVector3D>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_0_Core>
//#include <QOpenGLShaderProgram>
#include <QPainter>

#include <ctime>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class Functions{
public:

    // 生成 [0, 1] 范围的随机数
    static float random() {
        return rand()*1.0f/RAND_MAX;
    }

    // 反射
    // n 是一个单位向量
    static QVector3D reflect(const QVector3D & v, const QVector3D & n){
        return v - 2.0f*(QVector3D::dotProduct(v,n))*n; // v*u is not dot(v, u) !!!
    }

    // 折射
    // n 是一个单位向量
    static bool refract(const QVector3D & v, const QVector3D & n, float ni_over_nt, QVector3D & refracted){
        QVector3D uv = v.normalized();
        float dt = QVector3D::dotProduct(uv, n);
        float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt*dt);
        if(discriminant > 0){
            refracted = ni_over_nt * (uv - n*dt) - n * sqrt(discriminant);
            return true;
        }
        return false;
    }

    // Christophe Schlick polynomial approximation of glass reflection
    static float schlick(float cosine, float ref_idx){
        float r0 = (1-ref_idx) / (1+ref_idx);
        r0 = r0*r0;
        return r0 + (1-r0)*pow((1-cosine), 5);
    }

    // 随机生成单位圆中的一点
    static QVector3D randomPointInUnitDisk(){
        QVector3D p;
        do{
            p = 2.0f * QVector3D(random(), random(), 0.0f) - QVector3D(1.0f, 1.0f, 0.0f);
        }while(QVector3D::dotProduct(p, p) >= 1.0f);
        return p;
    }

    // 随机生成单位球中的一点
    static QVector3D randomPointInUnitSphere(){
            QVector3D p;
            do{
                p = 2.0*QVector3D(random(), random(), random()) - QVector3D(1.0f, 1.0f, 1.0f);
            }while(QVector3D::dotProduct(p, p) >= 1.0f);
            return p;
    }
};

class Ray{
    QVector3D ori, dir;

public:

    Ray(const QVector3D & _ori = QVector3D(), const QVector3D & _dir = QVector3D(1.0f, 0.0f, 0.0f)):
       ori(_ori), dir(_dir)
    {}

    QVector3D origin() const {return ori;}

    // normalized direction
    QVector3D direction() const {return dir.normalized();}

    QVector3D point(float t) const {return ori + t * direction();}

};

class Light{
public:
    QVector3D position;
};

class Material;

class hitPointPara{
public:
    QVector3D normal;
    float u;
    Material * material;
};

class Material{
public:
//    Material(const QVector3D & a) : albedo(a) {}
//    QVector3D albedo;
    virtual bool scatter(const Ray & r, const hitPointPara & para, QVector3D & attenuation, Ray & scatterd) const = 0;
};

class Lambertian: public Material{
public:
    QVector3D albedo;
    Lambertian(const QVector3D & a) : albedo(a){}
    bool scatter(const Ray & r, const hitPointPara & para, QVector3D & attenuation, Ray & scatterd) const override{
        attenuation = albedo;
        scatterd = {r.point(para.u), para.normal + Functions::randomPointInUnitSphere()};
        return true;
    }
};

class Metal: public Material{
public:
    float fuzz;
    QVector3D albedo;
    Metal(const QVector3D & a, float f=0.0f) : albedo(a){
        if(f < 0.0f) f = 0.0f;
        else if(f > 1.0f) f = 1.0f;
        fuzz = f;
    }
    bool scatter(const Ray & r, const hitPointPara & para, QVector3D & attenuation, Ray & scattered) const override{
        attenuation = albedo;
        QVector3D reflectedDir = Functions::reflect(r.direction(), para.normal);
        scattered = {r.point(para.u), reflectedDir + fuzz * Functions::randomPointInUnitSphere()};
        if(QVector3D::dotProduct(para.normal, scattered.direction()) > 0){
            return true;
        }
        return false;
    }
};

class Dielectric: public Material{
public:
    float ref_idx;
    Dielectric(float ri) : ref_idx(ri){}
    bool scatter(const Ray & r, const hitPointPara & para, QVector3D & attenuation, Ray & scattered) const override{
        QVector3D outward_normal;
        QVector3D reflected = Functions::reflect(r.direction(), para.normal);
        float ni_over_nt;
        attenuation = QVector3D(1.0f, 1.0f, 1.0f);
        QVector3D refracted;
        float reflect_prob, cosine;

        if(QVector3D::dotProduct(r.direction(), para.normal) > 0){
            outward_normal = -para.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * QVector3D::dotProduct(r.direction(), para.normal); // r.direction() should be normalized.
        }else{
            outward_normal = para.normal;
            ni_over_nt = 1.0f / ref_idx;
            cosine = -QVector3D::dotProduct(r.direction(), para.normal); // r.direction() should be normalized.
        }

        if(Functions::refract(r.direction(), outward_normal, ni_over_nt, refracted)){
            reflect_prob = Functions::schlick(cosine, ref_idx);
        }else{
            reflect_prob = 1.0f;
        }

        if(Functions::random() < reflect_prob){
            scattered = {r.point(para.u), reflected};
        }else{
            scattered = {r.point(para.u), refracted};
        }
        return true;
    }
};

class Object{
public:
    virtual bool hit(const Ray & r, hitPointPara & para, float u_min, float u_max) const{
        return false;
    }
};

class Sphere: public Object{
public:
    QVector3D center;
    float radius;

    Material * material;

    Sphere(){}
    Sphere(const QVector3D & c, float r, Material * mat): center(c), radius(r), material(mat){}

    bool hit(const Ray & r, hitPointPara & para, float u_min, float u_max) const override{
        QVector3D oc = r.origin() - center;
        float a = QVector3D::dotProduct(r.direction(), r.direction());
        float b = QVector3D::dotProduct(oc, r.direction());
        float c = QVector3D::dotProduct(oc, oc) - radius*radius;

        float discriminant = b*b - a*c;

        if(discriminant > 0){
            // 交点 1
            float u = (-b - sqrt(discriminant))/a;
            if(u < u_max && u > u_min){
                para.u = u;
                para.normal = (r.point(u) - center) / radius;
                para.material = material;
                return true;
            }

            // 交点 2
            u = (-b + sqrt(discriminant))/a;
            if(u < u_max && u > u_min){
                para.u = u;
                para.normal = (r.point(u) - center) / radius;
                para.material = material;
                return true;
            }
        }
        return false;
    }
};

class Scene{
public:

    // 记录最大递归深度，用于调试
    int maxDep;

    std::vector<Sphere> objects;

    QVector3D getColor(const Ray & r, int dep=0){

        maxDep = maxDep<dep?dep:maxDep;

        hitPointPara tmpPara, para;

        bool hit = false;
        float closestU = 1e20f;
        float eps = 1e-4f; // 防止浮点误差导致的自交
        for(auto object : objects){
            if(object.hit(r, tmpPara, eps, closestU)){
                hit = true;
                closestU = tmpPara.u;
                para = tmpPara;
            }
        }

        if(hit){

            Ray scattered;
            QVector3D attenuation;

            if(dep < 50 && para.material->scatter(r, para, attenuation, scattered)){
                return attenuation * getColor(scattered, dep+1);
            }else {
                return {0.0f, 0.0f, 0.0f};
            }

//            QVector3D origin = r.point(para.u);
//            QVector3D direction = (origin + para.normal + Functions::randomPointInUnitSphere()) - origin;
//            Ray nextRay(origin, direction);
//            return QVector3D(0.3f, 0.3f, 0.3f) * getColor(nextRay, dep+1);
        }

        float t = 0.5f * (r.direction().y() + 1.0f);
        return (1.0f - t) * QVector3D(1.0f, 1.0f, 1.0f) + t * QVector3D(0.5f, 0.7f, 1.0f);
    }
};

class Camera{
public:
    QVector3D origin;
    QVector3D lower_left_corner;
    QVector3D horizontal;
    QVector3D vertical;
    QVector3D u, v, w; // right, up, behind
    float lens_radius;
    Camera(){}
    Camera(QVector3D lookfrom, QVector3D lookat, QVector3D vup, float vfov, float aspect, float aperture, float focus_dist){
        lens_radius = aperture / 2.0f;
        float theta = vfov * M_PI / 180.0f;
        float half_height = tan(theta/2.0f);
        float half_width = aspect * half_height;
        origin = lookfrom;
        w = (lookfrom - lookat).normalized();
        u = QVector3D::crossProduct(vup, w).normalized();
        v = QVector3D::crossProduct(w, u);
        lower_left_corner = origin - half_width*focus_dist*u-half_height*focus_dist*v-focus_dist*w;
        horizontal = 2.0f * half_width*focus_dist*u;
        vertical = 2.0f * half_height*focus_dist*v;
    }
    Ray get_ray(float s, float t){
        QVector3D rd = lens_radius * Functions::randomPointInUnitDisk();
        QVector3D offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
    }
};




class OpenGLWidget : public QOpenGLWidget, public QOpenGLFunctions_4_0_Core
{

    Q_OBJECT

    Scene scene;

    Camera camera;

    QImage img;

    QPainter painter;

    int nx, ny;


public:

    OpenGLWidget(QWidget * parent = 0);

protected:

    void render();

    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;
};

#endif // OPENGLWIDGET_H
