/********************************************

Course	: TGD2151	Computer Graphics Fundamentals
Session: Trimester 1, 2019/20
ID and	Name	#1 : 1171302056 Evangelista Grace
Contacts	#1 : 0189686774 grace.evangrace@gmail.com
ID and	Name	#2 : 1141127046 Low Ting Wei
Contacts	#2 : 01123676718 tingwei9687@gmail.com
********************************************/


#include<windows.h>
#include <iostream>
#include <string>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "vertex.h"

using namespace std;
int WindowWidth = 600, WindowHeight = 600;
double posX = 0.0, posY = 0.0, posZ = 0.0, posInc = 0.1, angleInc = 2.0;
double rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
static double scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
int option = 0, axisornot = 0, colorSeg = 0;
bool mousePanMode=false, mouseZoomMode=false, mouseRotationMode=false;
int mouseX = 0, mouseY = 0;
float eyeX=0, eyeY=0, eyeZ=0, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;
void myInit();
void myDisplayFunc(void);
void switch_models(int index);

vertex Manipulation;
float transX=0, transY=0, transZ=0, transparent=1.0;
bool displayWireFrame = false, displayVertices = false, displayFaces = true;
static int model = 0;

void InitTransparent()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
    glClearColor(0,0,0,0);
}
void myDisplayFunc(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    //GLfloat specular[] = { 0.1, 0.1, 0.1, 0.1};
    GLfloat lightpos[] = {0.f,0.f,200.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    //glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);
    glScalef(scaleX, scaleY, scaleZ);
    Manipulation.boundingBox();

    if(axisornot%2 == 1)
       Manipulation.DrawAxis();  //Press

    if(displayWireFrame)
        Manipulation.DisplayWireFrame(transparent, colorSeg);
    if(displayFaces)
        Manipulation.DisplayFaces(transparent, colorSeg);
    if(displayVertices)
        Manipulation.DisplayVertices(colorSeg);

    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    //glutPostRedisplay();
}

void viewing()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 0.1, 500.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(eyeX, eyeY, eyeZ, focusX, focusY, focusZ, upX, upY, upZ);
}
void mySpecialFunc(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_DOWN  :
        rotateX += angleInc;
        break;
    case GLUT_KEY_UP    :
        rotateX -= angleInc;
        break;
    case GLUT_KEY_RIGHT :
        rotateY += angleInc;
        break;
    case GLUT_KEY_LEFT  :
        rotateY -= angleInc;
        break;
    case GLUT_KEY_HOME  :
        rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
        posX = 0.0, posY = 0.0, posZ = 0.0;
        scaleX = 1.0, scaleY = 1.0, scaleZ = 1.0;
        break;
    case GLUT_KEY_F1    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        break;
    case GLUT_KEY_F2    :
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        break;
    }
    glutPostRedisplay();
}
void myKeyboardFunc(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 'c':
        case 'C': colorSeg += 1; break;
        case 'm': model += 1; switch_models(model); break;
        case 'n': model -= 1; switch_models(model); break;
        case 's': axisornot += 1; break;//On or Off the axis
        case 'x': transX +=0.005; break;
        case 'X': transX -=0.005; break;
        case 'y': transY +=0.005; break;
        case 'Y': transY -=0.005; break;
        case 'z': transZ +=0.005; break;
        case 'Z': transZ -=0.005; break;
        case 'v':
        case 'V': displayVertices = true, displayFaces = false; displayWireFrame = false; break;
        case 'f':
        case 'F': displayVertices = false, displayFaces = true; displayWireFrame = false; break;
        case 'w':
        case 'W': displayVertices = false, displayFaces = false; displayWireFrame = true; break;

    }

    glutPostRedisplay();
}
void myReshapeFunc(int width, int height)
{
    WindowWidth  = width;
    WindowHeight = height;
    glViewport(0, 0, width, height);
}
void myMotionFunc(int x, int y)
{
    y = WindowHeight - y;
    if(mousePanMode)
    {
        posX += (x - mouseX)*0.5;
        posY += (y - mouseY)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseZoomMode)
    {
        GLdouble inc = (mouseY - y)*0.005;
        scaleX += inc;
        scaleY += inc;
        scaleZ += inc;
        mouseY = y;
        glutPostRedisplay();
    }
    if(mouseRotationMode)
    {
        rotateX += (mouseY - y)*0.5;
        rotateY += (x - mouseX)*0.5;
        mouseX = x;
        mouseY = y;
        glutPostRedisplay();
    }

}
void myMouseFunc(int button, int state, int x, int y)
{
    y = WindowHeight - y;
    switch (button)
    {
    case GLUT_MIDDLE_BUTTON:
        if (state==GLUT_DOWN && !mousePanMode)
        {
            mouseX = x;
            mouseY = y;
            mousePanMode = true;
        }
        if (state==GLUT_UP &&  mousePanMode)
        {
            mousePanMode = false;
        }
        break;
    case GLUT_RIGHT_BUTTON:
        if (state==GLUT_DOWN && !mouseZoomMode)
        {
            mouseY = y;
            mouseZoomMode = true;
        }
        if (state==GLUT_UP && mouseZoomMode)
        {
            mouseZoomMode = false;
        }
        break;
    case GLUT_LEFT_BUTTON:
        if (state==GLUT_DOWN && !mouseRotationMode)
        {
            mouseX = x;
            mouseY = y;
            mouseRotationMode = true;
        }
        if (state==GLUT_UP &&  mouseRotationMode)
        {
            mouseRotationMode = false;
        }
        break;
    }
}

void switch_models(int index)
{
    string skeleton_filename;
    string segment_filename;
    string points_filename;

    switch(index)
    {
        case 0:
            points_filename = "data/Animal11Seg.ply";
            segment_filename = "data/Animal11Seg.seg";
            //skeleton_filename = "data/chair.skel";
            eyeX=0, eyeY=0, eyeZ=3, focusX=0, focusY=0, focusZ=0, upX=0.0, upY=1, upZ=0;
            break;

    }
    cout << "Switched model to " << points_filename << endl;
    //cout << "Display skeleton of " << skeleton_filename << endl;
    Manipulation.read3DModel(points_filename.c_str());
    //Manipulation.read3DSegment(segment_filename.c_str());
    //Manipulation.read3DSkeleton(skeleton_filename.c_str());

    //Start your coding here
    //
}
void myInit()
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(300, 100); // Set top-left position
    glutInitWindowSize(WindowWidth, WindowHeight); //Set width and height
    glutCreateWindow("Model");// Create display window

    switch_models(0);

    InitTransparent();

    viewing();
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glEnable(GL_NORMALIZE);

    glClearColor(1.0, 1.0, 1.0, 1.0);

    glutDisplayFunc(myDisplayFunc);  // Specify the display callback function
    glutReshapeFunc(myReshapeFunc);
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialFunc);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(myMotionFunc);

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);


    cout<<"------------------------------------------------------------"<<endl;
    cout<<"Instruction:"<<endl;
    cout<<"s - On or Off the Axis"
        <<"\nHome - to reset model"
        <<"\nw or W - to display model WireFrame"
        <<"\nv or V - to display model vertices"
        <<"\nf or F - to display model surfaces"
        <<endl;


    cout<<"------------------------------------------------------------"<<endl;
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    myInit();
    glutMainLoop();
    return 0;
}
