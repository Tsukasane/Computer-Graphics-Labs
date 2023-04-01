// fragment shader 在 vertex shader 之后执行 //
// 物体每个表面的每个像素都会执行该 fragment shader //

#version 400 core

//out vec4 outColor;
in vec3 Color;
out vec4 outColor;

void main()
{
    // fragment shader 中必须输出一个 vec4 类型变量，表示物体表面每个像素的颜色，格式为 rgba //
   outColor = vec4(Color, 1.0f);
}
