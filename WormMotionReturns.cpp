#include <iostream>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#define K 10
#define D 1
#define L 1.0
#define TIME_INTERVAL 50
#define dt 0.5

using namespace std;

const float velocity_loss=1-0.00001;//Velocity lost due to energy loss
float f1 = 0;                    //Force due to spring 1 and damping force 
float f2 = 0;                  //Force due to spring 2 and damping force
int scene_time = 500;

//Define the class used to set force and velocity acting on the 3 mass 2 spring system
class Force3f
{
 public:
        float x, y, z;               //Vectors of position and velocity

        Force3f(){};                 //Default constructor for class Force3f
        Force3f(float, float, float);//Overloaded constructor to instialize position and velocity vectors
                       
        //Operatoring overloading of +, -, *, =, *(float), /(float)
        Force3f operator + (Force3f);
        Force3f operator - (Force3f);
        Force3f operator * (Force3f);
        Force3f operator * (float);
        Force3f operator / (float);
};

//Define the Particle having position, force and velocity component along with its mass
struct Mass
{
 Force3f force;
 Force3f velocity;
 Force3f position;
 
 float mass;

 void add_force(Force3f,float);
};

//Function signatures of all functions implemented
float l(Force3f , Force3f );
float force(Mass ,Mass );
void update(int );

void init();
void lighting();
void display();

void house();
void plane(double );
void petal();
void flowerpot(double , double , double , double , double , double , double , double , double , double , double , double , double , double , double );
void path();
void head(float );

void ellipseMass(Mass );
void findmax(double );

//main() calls the display function and sets the Timer Function
int main(int argc,char **argv)
{
 glutInit(&argc,argv);
 init();
 lighting();
 glClearColor(0.529412, 0.807843, 0.921569, 0.0f);
  
 glutTimerFunc(TIME_INTERVAL, update, 0);
 glutMainLoop();

 return 0;
}

//Overloaded constructor to instialize position and velocity vectors
Force3f::Force3f(float a, float b, float c)
{
 x=a;
 y=b;
 z=c;
}

//Overloading operators for vector operations
Force3f Force3f::operator + (Force3f param)
{
 Force3f position;
 position.x=x+param.x;
 position.y=y+param.y;
 position.z=z+param.z;
 return(position);
}

Force3f Force3f::operator - (Force3f param)
{
 Force3f position;
 position.x=x-param.x;
 position.y=y-param.y;
 position.z=z-param.z;
 return(position);
}

Force3f Force3f::operator * (Force3f param)
{
 Force3f position;
 position.x=x*param.x;
 position.y=y*param.y;
 position.z=z*param.z;
 return(position);
}
Force3f Force3f::operator / (float param)
{
 Force3f position;
 position.x=x/param;
 position.y=y/param;
 position.z=z/param;
 return(position);
}

Force3f Force3f::operator * (float param)
{
 Force3f position;
 position.x=param*x;
 position.y=param*y;
 position.z=param*z;
 return(position);
}

//Calculates scalar distance between either velocity or position vectors
float l(Force3f a, Force3f b)
{
 Force3f c;
 float dist=0;
 c=a-b;
 dist=sqrt((c.x*c.x)+(c.y*c.y)+(c.z*c.z));
 return dist;
}

//add_force() function is defined to compute new postion and velocity vectors due to the force(spring + damping) acting on the particle
void Mass::add_force(Force3f force,float t)
{
 velocity=velocity+(force/mass)*t;
 position=position+velocity*t;
 velocity=velocity*velocity_loss;
}

//Returns the total force acting on a particle (spring + damping)
float force(Mass a,Mass b)
{
 //force = K*(L-l) + D*(dl/dt)   ->   dl/dt is scalar distance between velocity vectors
 return (-(L-(l(a.position,b.position))*K)+(D*l(a.velocity,b.velocity)));
}

//Define intial values of position and velocity vectors 
Force3f mass_A(-19.5,0.0,0.0);
Force3f mass_B(-18.5,0.0,0.0);
Force3f mass_C(-17.5,0.0,0.0);
Force3f mass_A_velocity(0.0,0.0,0.0);
Force3f mass_B_velocity(0.0,0.0,0.0);
Force3f mass_C_velocity(0.0,0.0,0.0);

//Implement a 3 mass 2 spring system
Mass ellipse_A;
Mass ellipse_B;
Mass ellipse_C;

//Draws the 3 mass 2 spring system
void ellipseMass(Mass ellipse)
{
 glPushMatrix();
 glColor3f(0, 1, 0.498039);//Body color of worm
 glTranslated(ellipse.position.x,ellipse.position.y,ellipse.position.z);//Translate to current position of mass
 glScaled(3, 1.5, 1.3);//Drawing an ellipse at the current position of the mass      
 glutSolidSphere(1,15,15);
 glColor3f(1, 0.647059, 0);//Draw an orange torus around the body of the worm (enhances the look of the worm movement)
 glutSolidTorus(0.5, 0.7, 30, 30);
 glPopMatrix();
}

//Draws the head of the worm based on which sphere (A, B, C) is currently maximum along x direction
void head(float max)
{
 glPushMatrix();

 //Head
 glColor3f(0.180392, 0.545098, 0.341176);//Color of the head
 glTranslated(max+3, 0.4, 0.0);//Relative to the maximum of postions of sphere(A, B, C) move 3 units along x-axis
 glutSolidSphere(1.5, 15, 15);
 
 //Nose
 glTranslated(2, 0.0, 0.0);
 glColor3f(1, 0.647059, 0);//Color of nose
 glutSolidSphere(0.3, 15, 15);
 
 //Eyes
 glColor3f(1, 1, 1);
 glTranslated(0, 0.0, 0.75);
 glutSolidSphere(0.25, 15, 15);//Eye 1
 glTranslated(0, 0, -1);   
 glutSolidSphere(0.25, 15, 15);//Eye 2
 glColor3f(0, 0, 0);
 glTranslated(0.17, 0.0, 0.0);
 glutSolidSphere(0.15, 15, 15);//Pupil 1
 glTranslated(0, 0, 1);
 glutSolidSphere(0.15, 15, 15);//Pupil 2
 
 //Antennas
 glColor3f(0.419608, 0.556863, 0.137255);
 glPushMatrix();
 glTranslated(0, 0.9, 1);
 glRotated(90, 0, 1, 0);
 glRotated(45, 0, 1, 1);
 glScaled(0.25, 0.75, 0.25);
 glutSolidSphere(1, 15, 15);//Antenna 1
 glPopMatrix();
 glTranslated(0, 0.8, -0.9);
 glRotated(90, 0, 1, 0);
 glRotated(-45, 0, 1, 1);
 glScaled(0.25, 0.70, 0.25);
 glutSolidSphere(1, 15, 15);//Antenna 2
     
 glPopMatrix();
}

//Used in flowerpot() function to draw petals of sunflower
void petal()
{
 glPushMatrix();
 
 glTranslated(-1.3, -4.6, 0);
 glScaled(0.25, 0.75, 0.25);
 glutSolidSphere(4, 15, 15);
 
 glPopMatrix();
}

//Draw a flowerpot with a sunflower
void flowerpot(double x, double y, double z, double c1, double c2, double c3, double cr1, double cr2, double cr3, double cor1, double cor2, double cor3, double core1, double core2, double core3)
{
 glPushMatrix();
 
 glColor3f(1, 0.498039, 0.313725);//Color of flowerpot
 glTranslated(x, y, z);
 glPushMatrix();
 glRotated(-90, 1, 0, 0);
 GLUquadricObj *quadObj = gluNewQuadric();
 gluCylinder(quadObj, 1.5, 2.3, 3, 30, 30);//Draw the pot
 glutSolidTorus(0.1,1.5,30,30);//Draw a rim at the bottom of the pot
 glPopMatrix();
 glTranslated(0,3,0);
 glPushMatrix();
 glRotated(90, 1, 0, 0);
 glutSolidTorus(0.4,2.3,30,30);//Draw a rim at the top of the pot
 glColor3f(0.647059, 0.164706, 0.164706);//Color of Mud
 glScaled(1 , 1, 0);
 glutSolidTorus(1,1,30,30);//Drawing a circle to represent the Mud
 glPopMatrix();
 glPushMatrix();
 glRotated(-90, 1, 0, 0);
 glColor3f(0.133333, 0.545098, 0.133333);//Stem color
 gluCylinder(quadObj, 0.1, 0.1, 3, 30, 30);//Draw the stem of the sunflower
 glPopMatrix();
 glColor3f(cor1, cor2, cor3);//Color of flower center
 glTranslated(0, 3.4, 0);
 glScaled(0.4,0.5, 0.5);
 glutSolidSphere(1, 15, 15);//Draw the center
 glTranslated(1, 0, 0);
 glColor3f(core1, core2, core3);//Color of inner center 
 glScaled(0.4,0.5, 0.5);
 glutSolidSphere(0.9, 15, 15);//Draw an inner center 
 
 for(int i=0; i<360; i ++)//Drawing petals of sunflower around outer center
 {
  glRotated(i,1,0,0);
  if((i<45)|(i>90)&&(i<135)|(i>180)&&(i<225)|(i>270)&&(i<315))
   glColor3f(c1, c2, c3);//1st shade color
  else
   glColor3f(cr1, cr2, cr3);//2nd shade color
  petal();
 }

 glPopMatrix();
}

//Draw the green plane to denote the grass
void plane(double planeWid)
{
 glPushMatrix();
 glColor3f(0.196078 ,0.803922 ,0.196078);
 glTranslated(-1.4,-3.5,0);
 glScaled(planeWid, 0, planeWid*2); 
 glutSolidCube(45);
 glPopMatrix();
}

//Draw the worm's house with roof, chimney and entrance
void house()
{
 //House
 glPushMatrix();
 glColor3f(0.803922, 0.521569, 0.247059);//Color of house
 glTranslated(-14.75, 1, 0);
 glScaled(2,2,2);
 glutSolidCube(2);//Drawing the house

 //Roof
 glColor3f(0.545098, 0, 0);//Color of roof
 glTranslatef(0,1,0);
 glPushMatrix();                   
 glRotatef(-90,1,0,0);
 glutSolidCone(1.5,1,50,8);//Draw the roof
 glPopMatrix();

 //Chimney
 glColor3f(0.545098, 0.270588, 0.0745098);//Color of chimney
 glTranslatef(0.75,0.5,-0.75);        
 glPushMatrix();                   
 glScalef(1,3,1);
 glutSolidCube(.25);//Draw the Chimney
 glPopMatrix();
 glPopMatrix();
  
 //Entrance
 glPushMatrix();
 glColor3f(0, 0, 0);//Color for Entrance
 glTranslated(-11.75,1, 0.43);
 glPushMatrix();
 glRotated(90, 0, 1, 0);
 glScaled(0.75,1,0.005);
 glutSolidTorus(0.7,0.7,30,30);//Draw a circle
 glPopMatrix();
 glTranslated(0, -0.8, 0);
 glScaled(0.05,1.5,2.087);
 glutSolidCube(1);//Draw a rectangle
 glPopMatrix();
}

//Finds the maximum position along x for the 3 masses
float findmax()
{
 float m = ellipse_A.position.x;
 if(ellipse_A.position.x > ellipse_B.position.x)
 m = ellipse_A.position.x;
 else
     m = ellipse_B.position.x;
 if(m < ellipse_C.position.x)
      m = ellipse_C.position.x;

 return m;
}

//Draw a path from the house leading outside
void path()
{
 glPushMatrix();
 
 glColor3f(0.545098, 0.270588, 0.0745098);//Color of the path
 glTranslated(-0.5, -3.5, -2.45);
 glRotated(90, 1, 0, 0);
 glScaled(45, 2.5, 0.04);
 glutSolidCube(1); //Draw the path
 
 glPopMatrix();
}  

//Updates value of forces acting on particles every 50msec
void update(int value)
{
 f1=force(ellipse_A,ellipse_B);//Spring+Damping force due to 1st spring
 f2=force(ellipse_B,ellipse_C);//Spring+Damping force due to 2nd spring
    
 //Update position and velocity vectors for all 3 masses
 mass_A=ellipse_A.position;
 mass_A_velocity=ellipse_A.velocity;
 mass_B=ellipse_B.position;
 mass_B_velocity=ellipse_B.velocity;
 mass_C=ellipse_C.position;
 mass_C_velocity=ellipse_C.velocity;

 glutPostRedisplay();
 //Decreases scene_time each time TimerFunc is run
 if(scene_time>0)
                 glutTimerFunc(TIME_INTERVAL, update, scene_time--);
}

//Display Function that creates the scene and the worm
void display(void)
{
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 glOrtho(-10.0*64/48.0,10.0*64/48.0,-10.0,10.0,0.1,100.0);//Set up orthographics matrix
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
 gluLookAt(4*6.0,2*6.0,2*6.0,0.0,0.0,0.0,0.0,1.0,0.0);//Set up camera
 glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);//Load values set by glClearColor()
 
 house();//Draw the worm's house
 plane(1);//Draw the green plane (that is meant to represent grass)
 glPushMatrix();
  glScaled(0.80, 0.80, 0.80);
  flowerpot(0, 0, -10, 1, 1, 0, 1, 0.843137, 0, 1, 0.647059, 0, 0.678431, 1, 0.184314);//Draw the 1st flower pot
  flowerpot(4.6, 0, 14, 0, 0, 0.545098, 0, 0, 1, 0.282353, 0.239216, 0.545098, 0.517647, 0.439216, 1);//Draw the 2nd flower pot
 glPopMatrix();
 path();
 glRotated(180, 0, 1, 0);
 
 //Initialize mass, position and velocity of the 3 mass system 
 ellipse_A.mass=100;
 ellipse_A.position=mass_A;
 ellipse_A.velocity=mass_A_velocity;
    
 ellipse_B.mass=100;
 ellipse_B.position=mass_B;
 ellipse_B.velocity=mass_B_velocity;
    
 ellipse_C.mass=100;
 ellipse_C.position=mass_C;
 ellipse_C.velocity=mass_C_velocity;

 //Calculate the direction of motion between for the masses A, B, C
 Force3f dV1=((ellipse_B.position-ellipse_A.position)/l(ellipse_A.position,ellipse_B.position));
 Force3f dV2=((ellipse_C.position-ellipse_B.position)/l(ellipse_B.position,ellipse_C.position));
 //Also know as Spine Vector - gives the direction of motion of the worm
 Force3f spineV=((ellipse_C.position-ellipse_A.position)/l(ellipse_A.position,ellipse_C.position));

 //Calculate spring force acting on the 3 masses
 Force3f springF_A=dV1*f1;
 Force3f springF_B=dV1*f1-dV2*f2;
 Force3f springF_C=dV2*f2;

 //Update position and velocity vectors of the 3 masses
 ellipse_A.add_force(springF_A,dt);
 ellipse_B.add_force(springF_B*-1,dt);
 ellipse_C.add_force(springF_C*-1,dt);
    
 //Implementing the directional friction using Spine Vector
 if(spineV.x < 0.0)
 {//Since motion and velocity are define only along x-axis  ->  V = V - Spine_Vector(Spine_Vector*V) = 0
  if(ellipse_A.velocity.x < 0.0)//Effective condition being checked is if(Spine_Vector*Velocity < 0.0)
   ellipse_A.velocity.x = 0.0;
  if(ellipse_B.velocity.x < 0.0)
   ellipse_B.velocity.x = 0.0;
  if(ellipse_C.velocity.x < 0.0)
   ellipse_C.velocity.x = 0.0;
  }
              
  glPushMatrix();
  glTranslated(ellipse_A.position.x,0,0);//Translate to position of 1st mass
  ellipseMass(ellipse_A);//Draw mass_A
  glPopMatrix();

  glPushMatrix();
  glTranslated(ellipse_B.position.x,0,0);//Translate to position of 2nd mass
  ellipseMass(ellipse_B);//Draw mass_B
  glPopMatrix();

  glPushMatrix();
  glTranslated(ellipse_C.position.x,0,0);//Translate to position of 3rd mass
  ellipseMass(ellipse_C);//Draw mass_C
  glPopMatrix();
  
  //Collision detection has not been implemented. At a particular time any of the 3 particles may be have maximum value of position vector along x-axiz   
  float max = findmax();//Find the position of the mass with maximum value of position vector along x axis
    
  glPushMatrix();
  glTranslated(max, 0.0, 0.0);//Translate to the postion of the mass
  head(max);//Draw the head of the worm
  glPopMatrix();   

  glutSwapBuffers();//Exchange contents of Front and Back buffers
  glFlush();
}

//Initiate values for display
void init()
{
 glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
 glutInitWindowSize(1280,720);
 glutInitWindowPosition(0,0);
 glutCreateWindow("Jatayu the Worm goes for a walk");
 glutDisplayFunc(display);
}

//Enable Lighting, Material and Depth Matrices  ->  Set to default values since display() function does not change Matrix values
void lighting()
{
 glEnable(GL_LIGHTING);
 glEnable(GL_LIGHT0);
 glEnable(GL_COLOR_MATERIAL);
 glShadeModel(GL_SMOOTH);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_NORMALIZE);
} 
