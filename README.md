render depth image from given view point(listed in camPosList.txt).

The code is tested on codeblocks,Win10. Before you run the code, please compile opencv (into 'opencv_debug' directory for example) 
and configure your codeblock following these steps.
 
1.project->build options->linker libraries: add (1)all .dll and .a files in opencv_debug\install\lib;
                                                (2)libstrlib2.a file compiled from trimesh(this code also provide this file).
                                                (3)glu32,glut32,opengl32,glew32,glew32s files in codeblocks\MinGW\lib(These files are also provided)
2.project->build options->search directories->compiler: add the directory  opencv_debug\install\include  
3.project->build options->search directories->linker: add the directory open cv_debug\install\bin

then you can render the depth image from a .obj file.
