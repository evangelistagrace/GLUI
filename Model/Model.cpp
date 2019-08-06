#include <iostream>
#include <string>
#include <fstream>
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <math.h>
#include "vertex.h"
#include <GL/glui.h>

using namespace std;
//GLUT Window variables
int WindowWidth = 600, WindowHeight = 600;
bool mousePanMode=false, mouseZoomMode=false, mouseRotationMode=false;
int mouseX = 0, mouseY = 0;
float eyeX=0, eyeY=0, eyeZ=0, focusX=0, focusY=0, focusZ=0, upX=0, upY=1, upZ=0;
// window id for main graphics window
int	MainWindow;

//Object variables
double posX = 0.0, posY = 0.0, posZ = 0.0, posInc = 0.1, angleInc = 2.0;
float rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
float transX=0, transY=0, transZ=0;
float scale = 1;
int option = 0, axisornot = 0, colorSeg = 0;
vertex Manipulation;
bool displayWireFrame = false, displayVertices = false, displayFaces = true;
static int model = 0;
int   wireframe;
//Identity matrix for rotation
float view_rotate[16] = { 1,0,0,0, rotateX,1,0,0, rotateY,0,1,0, rotateZ,0,0,1 };

//Translation matrix
float obj_pos[] = { 0.0, 0.0, 0.0 };
// minimum allowable scale factor:
const float MINSCALE = { 0.05f };
// scaling factor
float	Scale;
//to check for object type
int radbut=0;
//to check if axis is on or off
int check=0;
//rotation value
float rotation_val=0;
//initial object color
float obj_color [] = {0.5, 0.5, 0.5, 1.0};
int COLOR_ID = 20;
int current_color(0);
float transparent = 1.0;
int color_id;

//GLUI variables
GLUI *glui;
GLUI_Panel *obj_panel;
GLUI_Panel *panel;
GLUI_RadioGroup *group;
GLUI_Rotation *rot;
GLUI_Translation *trans;
GLUI_Spinner *Trans_spinner;
GLUI_Listbox* color_list;

//declare functions
void myInit();
void myDisplayFunc(void);
void switch_models(int index);

const int GLUITRUE  = { true  };
const int GLUIFALSE = { false };



enum ButtonVals
{
	RESET_ROTATION,
	RESET_TRANSLATION,
	RESET_ALL,
	QUIT
};


void myGlutIdle( void )
{

    glutSetWindow(MainWindow);
    glutPostRedisplay();
}


void InitTransparent()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glShadeModel(GL_SMOOTH);
}
void myDisplayFunc(void)
{
    glEnable(GL_COLOR_MATERIAL);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    GLfloat amb_light[] = { 0.5f,0.5f,0.5f,1.0f };
    GLfloat diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat specular[] = { 0.5, 1.0, 1.0, 1.0};
    GLfloat lightpos[] = {0.f,0.f,100.0f,1.0f};

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse );
    glLightfv( GL_LIGHT0, GL_SPECULAR, specular );
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glEnable( GL_LIGHT0 );
    glEnable( GL_COLOR_MATERIAL );
    glShadeModel( GL_SMOOTH );
    glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE );
    glDepthFunc( GL_LEQUAL );
    glEnable( GL_DEPTH_TEST );
    glEnable(GL_LIGHTING);
    glColor4fv(obj_color);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(posX, posY, posZ);
    glRotatef(rotateX, 1.0f, 0.0f, 0.0f);
    glRotatef(rotateY, 0.0f, 1.0f, 0.0f);
    glRotatef(rotateZ, 0.0f, 0.0f, 1.0f);



         if ( radbut == 0 ) {
      displayVertices = false, displayFaces = true, displayWireFrame = false;
        }
         if(radbut==1)
        {
            displayVertices = true, displayFaces = false, displayWireFrame = false;
        }
        if(radbut==2)
        {
            displayVertices = false, displayFaces = false, displayWireFrame = true;
        }


    //calling rotation matrix
    glMultMatrixf( view_rotate );
    //Translation matrix
    glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2]);



    glScalef(scale, scale, scale);

    if(displayWireFrame)
        Manipulation.DisplayWireFrame(transparent, colorSeg);
    if(displayFaces)
        Manipulation.DisplayFaces(transparent, colorSeg);
    if(displayVertices)
        Manipulation.DisplayVertices(colorSeg);

    if(check==1)
        Manipulation.DrawAxis();


    glPopMatrix();
    glFlush();
    glutSwapBuffers();
    glutPostRedisplay();
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
        cout<<rotateX<<endl;
        break;
    case GLUT_KEY_UP    :
        rotateX -= angleInc;
         cout<<rotateX<<endl;
        break;
    case GLUT_KEY_RIGHT :
        rotateY += angleInc;
        cout<<rotateY<<endl;
        break;
    case GLUT_KEY_LEFT  :
        rotateY -= angleInc;
        cout<<rotateY<<endl;
        break;
    case GLUT_KEY_HOME  :
        rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;
        posX = 0.0, posY = 0.0, posZ = 0.0;
        scale=1.0;
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
        scale += inc;

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
    string segment_filename;
    string points_filename;

    switch(index)
    {
        case 0:
            points_filename = "data/Animal11Seg.ply";
            segment_filename = "data/Animal11Seg.seg";
            eyeX=0, eyeY=0, eyeZ=3, focusX=0, focusY=0, focusZ=0, upX=0.0, upY=1, upZ=0;
            break;

    }
    cout << "Switched model to " << points_filename << endl;
    Manipulation.read3DModel(points_filename.c_str());

}
void myInit()
{
    glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
    glutInitWindowPosition(100, 100); // Set top-left position
    glutInitWindowSize(WindowWidth, WindowHeight); //Set width and height
    MainWindow = glutCreateWindow("Model");// Create display window

    switch_models(0);

    InitTransparent();
    viewing();
    //black background
    glClearColor(0.0, 0.0, 0.0, 1.0);

    glutDisplayFunc(myDisplayFunc);// Specify the display callback function
    glutReshapeFunc(myReshapeFunc);
    glutKeyboardFunc(myKeyboardFunc);
    glutSpecialFunc(mySpecialFunc);
    glutMouseFunc(myMouseFunc);
    glutMotionFunc(myMotionFunc);


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

void Reset_rotation()
{

    //reset rotation
    rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;

    view_rotate[1]=view_rotate[2]=view_rotate[3]=view_rotate[6]=view_rotate[7]=view_rotate[9]=view_rotate[11]=view_rotate[13]=view_rotate[14]=0.0;
    view_rotate[0]=view_rotate[5]=view_rotate[10]=view_rotate[15]= 1.0;
    view_rotate[4]=rotateX;
    view_rotate[8]=rotateY;
    view_rotate[12]=rotateZ;

}

void Reset_translation()
{
    //reset position
	obj_pos[0] = obj_pos[1] = obj_pos[2] = 0.0;

}

void Reset_all()
{
    //reset position
	obj_pos[0] = obj_pos[1] = obj_pos[2] = 0.0;

	//reset scale
    scale=1.0;

    //reset rotation
    rotateX = 0.0, rotateY = 0.0, rotateZ = 0.0;

    view_rotate[1]=view_rotate[2]=view_rotate[3]=view_rotate[6]=view_rotate[7]=view_rotate[9]=view_rotate[11]=view_rotate[13]=view_rotate[14]=0.0;
    view_rotate[0]=view_rotate[5]=view_rotate[10]=view_rotate[15]= 1.0;
    view_rotate[4]=rotateX;
    view_rotate[8]=rotateY;
    view_rotate[12]=rotateZ;

    //reset color
    obj_color[0] = 0.5;
    obj_color[1] = 0.5;
    obj_color[2] = 0.5;

    //reset object type
    radbut = 0;

    //reset axis
    check = 0;
}

Buttons( int id )
{
	switch( id )
	{
		case RESET_ROTATION:

            Reset_rotation();
			glui -> sync_live();
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

        case RESET_TRANSLATION:

            Reset_translation();
			glui -> sync_live();
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

        case RESET_ALL:

            Reset_all();
			glui -> sync_live();
			glutSetWindow( MainWindow );
			glutPostRedisplay( );
			break;

		case QUIT:

			glui->close( );
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;
	}

}

void Yvalue(int &y)
{
        y = WindowHeight - y;

}

void control_cb(int control)
{
	if (control == COLOR_ID)
	{
		color_id = color_list->get_int_val();
		if (color_id == 0)
		{
			obj_color[0] = 0.5;
			obj_color[1] = 0.5;
			obj_color[2] = 0.5;

		}
		else if (color_id == 1)
		{
			obj_color[0] = 0.5;
			obj_color[1] = 0.0;
			obj_color[2] = 0.0;

		}
		else if (color_id == 2)
		{
			obj_color[0] = 0.0;
			obj_color[1] = 0.5;
			obj_color[2] = 0.0;
		}
		else if (color_id == 3)
		{
			obj_color[0] = 0.0;
			obj_color[1] = 0.0;
			obj_color[2] = 0.5;
		}
	}
}

void initGlui(){

	// setup the glui window:
	glutInitWindowPosition( 700, 100 ); //right of object window
	glui = GLUI_Master.create_glui( "GLUI" );

	//Object type panel
	obj_panel = glui->add_panel( "Object Type" );
    group = glui->add_radiogroup_to_panel( obj_panel,&radbut,3 );
    glui->add_radiobutton_to_group( group, "Faces" );
    glui->add_radiobutton_to_group( group, "Vertices" );
    glui->add_radiobutton_to_group( group, "Wireframe" );

    //Object color
    obj_panel = glui->add_panel("Object Color");
    color_list = glui->add_listbox_to_panel(obj_panel, "Colors: ", NULL, COLOR_ID, control_cb);

    color_list->add_item(0, "Grey");
	color_list->add_item(1, "Red");
	color_list->add_item(2, "Green");
	color_list->add_item(3, "Blue");

	//Scale object
	obj_panel = glui->add_panel("Scale Object");
	glui-> add_spinner_to_panel(obj_panel, "Scale: ", GLUI_SPINNER_FLOAT, &scale);


    //Axis
     obj_panel = glui->add_panel( "Axis" );
     glui->add_checkbox_to_panel( obj_panel, "Show axis?", &check, 1 );

    //Buttons
    obj_panel = glui -> add_rollout("Reset", false);
    glui->add_button_to_panel(obj_panel, "Reset Rotation", RESET_ROTATION, (GLUI_Update_CB) Buttons );
    glui->add_button_to_panel(obj_panel, "Reset Translation", RESET_TRANSLATION, (GLUI_Update_CB) Buttons );
    glui->add_button_to_panel(obj_panel, "Reset All", RESET_ALL, (GLUI_Update_CB) Buttons );
    glui->add_button("Quit", QUIT, (GLUI_Update_CB) Buttons );

     ///////////////////////////////
     glui->add_column(true);

     //Object translation
     obj_panel = glui->add_panel("Object Translation");

     trans = glui->add_translation_to_panel( obj_panel, "Translate X",  GLUI_TRANSLATION_X ,&obj_pos[0] );
     trans->set_speed( 0.010f ); //set speed of translation

     glui-> add_spinner_to_panel(obj_panel, "X: ", GLUI_SPINNER_FLOAT, &obj_pos[0]);

     trans = glui->add_translation_to_panel( obj_panel, "Translate Y",  GLUI_TRANSLATION_Y ,&obj_pos[1] );
     trans->set_speed( 0.010f ); //set speed of translation

     glui-> add_spinner_to_panel(obj_panel, "Y: ", GLUI_SPINNER_FLOAT, &obj_pos[1]);

    glui->add_column_to_panel( obj_panel, GLUIFALSE );

    trans = glui->add_translation_to_panel( obj_panel, "Translate Z",  GLUI_TRANSLATION_Z ,&obj_pos[2] );
    trans->set_speed( 0.010f ); //set speed of translation

    glui-> add_spinner_to_panel(obj_panel, "Z: ", GLUI_SPINNER_FLOAT, &obj_pos[2]);

    trans = glui->add_translation_to_panel( obj_panel, "Translate XY",  GLUI_TRANSLATION_XY ,&obj_pos[0]);
    trans->set_speed( 0.010f ); //set speed of translation

    glui-> add_spinner_to_panel(obj_panel, "X: ", GLUI_SPINNER_FLOAT, &obj_pos[0]);
    glui-> add_spinner_to_panel(obj_panel, "Y: ", GLUI_SPINNER_FLOAT, &obj_pos[1]);

     //Object rotation
     obj_panel = glui->add_panel("Object Rotation");
     glui -> add_statictext_to_panel(obj_panel, "Press ALT key for horizontal movement (rotation along Y-axis");
     glui -> add_statictext_to_panel(obj_panel, "Press CTRL key for vertical movement (rotation along X-axis");
     GLUI_Rotation *view_rot = glui -> add_rotation_to_panel(obj_panel, "Rotate", view_rotate );
    view_rot->set_spin( 0.8 );

    glui -> sync_live();
     glutPostRedisplay();

    GLUI_Master.set_glutIdleFunc( NULL );



}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    myInit();
    initGlui();
    glutMainLoop();
    return 0;
}
