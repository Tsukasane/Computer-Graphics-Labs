#version 400 core

layout(location = 0) in vec2 position; // 位置坐标 //
layout(location = 1) in vec2 texCoordinate; // 纹理坐标 //
layout(location = 2) in vec3 Color;


uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

out vec3 fragColor;
out vec2 fragTexCoordinate;

void main(){
    gl_Position = proj * view * model * vec4(position, 0.0f, 1.0f);

    // 纹理图片加载时会把图片上下颠倒，这里修改纹理的纵坐标使其正过来
    fragTexCoordinate = vec2(texCoordinate.x, 1.0-texCoordinate.y);
    fragColor = vec3(Color.x,Color.y,Color.z);
}
