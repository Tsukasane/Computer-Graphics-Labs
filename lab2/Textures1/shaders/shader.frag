#version 400 core


// 纹理（实际是一个 int 类型的值，表示这个纹理的绑定位置是哪个 texture unit） //
uniform sampler2D tex;


out vec4 outColor;
in vec2 fragTexCoordinate;
in vec3 fragColor;

void main()
{
    // 混合两种颜色可以使用分量乘法 * : //
    // https://en.wikibooks.org/wiki/GLSL_Programming/Vector_and_Matrix_Operations#Operators //
    // 或者使用 mix 函数: //
    // https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/mix.xhtml //


    // 内置函数 texture() //
   outColor = texture(tex, fragTexCoordinate)*vec4(fragColor,1.0);  //分量乘法
}
