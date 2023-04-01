// vertex shader 是所有 shader 中首先被执行的 //
// 每个顶点都会执行该 vertex shader //
// 第一行指定 GLSL 语言的版本，与 OpenGL 的版本相对应 //

#version 400 core

// 输入的顶点属性数据 //
// 每个属性的编号(location)和数据类型必须与 VAO 中指定的编号和数据类型一致 //
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 aColor;
out vec3 Color;

void main(){
    // vertex shader 中必须给 vec4 类型的内置变量 gl_Position 赋值，表示每个顶点的位置 //
    gl_Position = vec4(position, 1.0f);
    Color = aColor;
}
