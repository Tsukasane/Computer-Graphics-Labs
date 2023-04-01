#version 400 core

layout(location = 0) in vec3 position; // 位置 //
layout(location = 1) in vec3 aNormal;  // 法向 //

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 vPosition;
out vec3 vNormal;


void main(){

    // 在世界坐标系中进行计算 //
    vec4 worldPosition = model * vec4(position, 1.0f);
    gl_Position = proj * view * worldPosition;
    vPosition = vec3(worldPosition) / worldPosition.w;
    vNormal = mat3(transpose(inverse(model))) * aNormal;   //vNormal是三维向量，不能读成四维
    //vPosition = vec3(model * vec4(position, 1.0));
    //vNormal = mat3(transpose(inverse(model) ) )  * aNormal;
    //gl_Position = proj * view * vec4(vPosition, 1.0);

}


