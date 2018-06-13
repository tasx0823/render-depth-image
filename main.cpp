#include <windows.h>
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <TriMesh.h>
#include <vector>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <strings.h>

using namespace std;
using namespace trimesh;
using namespace cv;

TriMesh *mesh;
vec3 camPos;
vec3 centerPos(0.0, 0.0, 0.0);
string imageName;
string imagePath;
string txtPath;
int currentView = 0;
vector<vec3> camPosList;
vector<string> outNames;

void display()
{
	if (currentView >= camPosList.size()) exit(0);
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double ar = w / static_cast< double >(h);
	const float zNear = 1;
	const float zFar = 2;
	gluPerspective(43, ar, zNear, zFar); // simulate kinect
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	vec3 eye = camPosList[currentView];
	gluLookAt(eye[0], eye[1], eye[2], centerPos[0], centerPos[1], centerPos[2], 0,1,0);
    static float angle = 0;
	glColor3ub(255, 0, 0);
	// render mesh
	for (int it = 0; it < mesh->faces.size(); it++)
	{
		glBegin(GL_TRIANGLES);
		for (int iv = 0; iv < 3; iv++)
		{
			glNormal3f(mesh->normals[mesh->faces[it][iv]][0], mesh->normals[mesh->faces[it][iv]][1], mesh->normals[mesh->faces[it][iv]][2]);
			glVertex3f(mesh->vertices[mesh->faces[it][iv]][0], mesh->vertices[mesh->faces[it][iv]][1], mesh->vertices[mesh->faces[it][iv]][2]);
		}
		glEnd();
	}
	glPopMatrix();
	vector< GLfloat > depth(w * h, 0);
	glReadPixels(0, 0, w, h, GL_DEPTH_COMPONENT, GL_FLOAT, &depth[0]); // read depth buffer
	cv::Mat img(glutGet(GLUT_WINDOW_HEIGHT), glutGet(GLUT_WINDOW_WIDTH), CV_32F);// output depth image
	for (int i = 0; i < img.rows; i++)
	{
		for (int j = 0; j < img.cols; j++)
		{
			depth[i*img.cols + j] = (2.0 * zNear * zFar) / (zFar + zNear - (2.0f * depth[i*img.cols + j] - 1) * (zFar - zNear));
			depth[i*img.cols + j] = (depth[i*img.cols + j] - zNear) / (zFar - zNear);
			img.at<float>(i, j) = (1.0f - depth[i*img.cols + j]) * 255;
		}
	}
	cv::Mat flipped(img);
	cv::flip(img, flipped, 0);

	cv::Mat imgRGB(glutGet(GLUT_WINDOW_HEIGHT), glutGet(GLUT_WINDOW_WIDTH), CV_32FC3);// output depth image
	for (int i = 0; i < imgRGB.rows; i++)
	{
		for (int j = 0; j < imgRGB.cols; j++)
		{
			imgRGB.at<cv::Vec3f>(i, j) = cv::Vec3f(img.at<float>(i, j), img.at<float>(i, j), img.at<float>(i, j)); // flip image
																												   //cout << imgRGB.at<cv::Vec3f>(i, j) << " " << img.at<float>(i, j) << endl;
		}
	}
	string currentName = imagePath + "//" + imageName + "_Cam_" + to_string(currentView) + ".png";
	cv::imwrite(currentName, imgRGB);
	currentView++;
	glutSwapBuffers();
}

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(16, timer, 0);
}

int main(int argc,char **argv)
{
	mesh = TriMesh::read("model_1.obj");
	float x, y, z;
	printf("hahaha");
	ifstream fin("camPosList.txt");

	while (fin >> x >> y >> z)
	{
		vec3 c(x * 1.5, y* 1.5, z* 1.5);
		camPosList.push_back(c);
	}
	imageName = string("model");
	imagePath = string("./");
	//txtPath = string(argv[6]);
	//camPos[0] = stof(argv[3]); camPos[1] = stof(argv[4]); camPos[2] = stof(argv[5]);
	//centerPos[0] = stof(argv[6]); centerPos[1] = stof(argv[7]); centerPos[2] = stof(argv[8]);
	mesh->need_neighbors();
	mesh->need_normals();
	mesh->need_bsphere();
	for (int i = 0; i < mesh->vertices.size(); i++) {
		mesh->vertices[i] = (mesh->vertices[i] - mesh->bsphere.center) / mesh->bsphere.r * 0.5f;
	}
	mesh->write("model_1.obj");
	printf("hahaha\n");
	printf("hahaha\n");

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    printf("hahaha\n");
	glutInitWindowSize(256, 256);


	glutCreateWindow("GLUT");

	glewInit();

	glutDisplayFunc(display);
	glutTimerFunc(0, timer, 0);
	glEnable(GL_DEPTH_TEST);
	glutMainLoop();
	return 0;
}
