#version 400 core

struct Light{  //光源
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct Material{  //材质对光的反射度
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

// 位置 //
in vec3 vPosition;
in vec3 vNormal;

// 点光源位置 //
uniform vec3 lightPosition;

// 相机位置 //
uniform vec3 cameraPosition;

out vec4 outColor;

void main()
{
    Light light;
    Material material;

    light.ambient = vec3(0.3f);
    light.diffuse = vec3(1.0f);
    light.specular = vec3(0.5f);

    material.ambient = vec3(0.3f);
    material.diffuse = vec3(0.5f);
    material.specular = vec3(0.7f);
    material.shininess = 0.078125 * 128;

    //ambient
    vec3 ambient = light.ambient*material.ambient;

    //diffuse
    vec3 normal = normalize(vNormal);
    vec3 lightDir = normalize(lightPosition-vPosition);
    float diff = max(dot(normal,lightDir),0.0);
    vec3 diffuse = light.diffuse*(diff*material.diffuse);

    //specular
    vec3 viewDir = normalize(cameraPosition-vPosition);
    vec3 reflectDir = reflect(-lightDir,normal);
    float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    vec3 specular = light.specular*(spec*material.specular);

    vec3 result = ambient+diffuse+specular;
    //vec3 result = ambient+diffuse;
//    vec3 result = ambient;
    outColor = vec4(result,1.0f);
}
