# Computer Graphics Lab Notes

Hi there! 

This repo records my lab work in the computer graphics course. It is one of my favorite courses, since its theoretical lectures are profound, its lab tutorials are exquisite, and the professor and T.A.s are really nice. 

All labs is written to run on Qt Creator. However, you can also simply deem them as references and write your own code based on C++ platforms. 

I will briefly describe the requirements of each lab as follows, adding some key points that I find are important or difficult to understand. I think it is important to explore the labs thoroughly by yourself first. Please try not to do meaningless copy-and-pastes.

Also, these codes are not flawless. Please feel free to raise any issues. You are also welcome to contact me through Email(Tsukasazyin@gmail.com).

Hope you enjoy learning computer graphics!



## Lab 0: Triangle

**Aims:** 

Try to understand the workflow of QtOpenGL and the file structure of a project.

**Requirements:** 

Draw two triangles with three vertexes in different colors.

**Keys:**

* Focus on ``OpenGLWidget.cpp`` for the shape. Pay attention to ``shader.vert`` and ``shader.frag `` for colors.
* Understand the function of ``glBufferData(...)`` ``glVertexAttribPointer(...)``	``glDrawArrays(...)``



## Lab 1: Transformation

**Aims:** 

Try to understand the transoformation in rendering process.

**Requirements:** 

* Manipulate two cubes to rotate by themselves with timeline. Note that they should be distinguishable.
* Observe the difference under Orthophoto Projection and Perspective Projection.

**Keys:**

* The window in MacOS is set defaultly Quarter size of WindowsOS. You can solve this problem by making adjustment in one line of code. 
* Timeline is related to the update frequency of the window.
* Having fun to play with various functions!



## Lab 2: Textures

**Aims:** 

Try to understand how EBO works. Get hands on experience on texture wrapping, texture filtering, and Mipmap using.

**Requirements:** 

Flatten the ``1x1`` texture to ``2x4``, with the effect of longitudinal symmetry. Then, add color attribute to all the vertexes and conbine the texture with color.

**Keys:**

* Carefully read the reference(LearningOpenGL website).



## Lab 3: Mesh and Phong Shading

**Aims:** 

Try to understand the structure of ``.obj`` file. Get to know three popular shading methods, i.e. Flat Shading, Gouraud Shading, and Phong Shading. Knowing how to calculate the Flat Normals and the Smooth Normals.

**Requirements:** 

* Using the Phong Shading to show an object of Dragon. 
* Observing the difference in using FlatNormals and SmoothNormals correspondingly. 
* Read in a human 3D model and add textures as well as Phong Shading on it.

**Keys:**

* Be sure to read in your ``.obj`` file correctly before doing anything else. Namely, the position of vertexes and the connection between vertexes should be clearly displayed.
* Although you can manually calculate the Normals with several lines of code as I did in the first time, there are several embeded functions that can support you to directly calculate the normals.
* The human model has several divided parts, which I read them in separately using a ``for loop``. However, it relys on high frequency updates and brings heavy burden to your computer. So it is better to read the model with a new class.



## Lab 4: Solar System

**Aims:** 

To combine the techniques we learnt before in a single project.

**Requirements:** 

Simulate the rotation of planet in solar system. The planets should have textures, and the whole scenario needs lights.

**Keys:** 

This lab is not as difficult for me as lab3. Just remeber to keep the planets in order. If there is any time left, you can also read the code about sky box and try to modify it by yourself.



## Lab 5: Raytracing*

**Aims:** 

Try something advanced in CG.

**Requirements:** 

Implement raytracing on Qt based on [RayTracingInOneWeekend tutorial](https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview).

**Keys:** 

It is the final project of the Computer Graphics class. Actually there are several topics that is open to you but personally I was interested in this vistually striking one.

The tutorial is not as clear as the former labs. Even worse, according to my experience, it is also hard to find references based on Qt Creator. My way is to follow the guidance by reproducing its sample code on C++ platform, and then transfer it to Qt.

By the way, I think it is better to leave more than one weekend for this final project. Although the tutorial says you can get a hang of it in "One Weekend", I actually spent one weekend plus several nights to finish it. 



## References

### Textbooks

《计算机图形学》-- by 倪明田 吴良芝, official textbook for this class.

[《3D Math Primer for Graphics and Game Development》](https://bnu.primo.exlibrisgroup.com.cn/permalink/86BNU_INST/1s2opci/alma9911628953603961) has vivid explanations of concepts and formulas.

### Official Tutorials

[LearnOpenGL-English](https://learnopengl.com/)	[LearningOpenGL-Chinese](https://learnopengl-cn.github.io/) -- friendly tutorials

[OpenGL Reference Doc](https://www.khronos.org/opengl/wiki/OpenGL_Reference)

[Qt Reference Doc](https://doc.qt.io/qt-5/reference-overview.html)

[Foundamental OpenGL Tutorials and Notes (written on Code::Block C++)](http://www.songho.ca/opengl/index.html)

[Guidance of .obj file](https://en.wikipedia.org/wiki/Wavefront_.obj_file)

[Lightings and Shadings](https://opengl-notes.readthedocs.io/en/latest/topics/lighting/shading.html)

[Details about Normals](http://softimage.wiki.softimage.com/xsidocs/poly_shading_AboutNormals.html)

### Blogs

[Phong Shading](https://quanxin.blog.csdn.net/article/details/86694221)

[Solar System](https://blog.csdn.net/Wang_Dou_Dou_/article/details/120708728)

