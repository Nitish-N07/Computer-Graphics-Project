//g++ -v group4_code.cpp -o output -lopengl32 -lglew32 -lfreeglut -lglu32

#include <Windows.h>
#include<stdio.h>
#include<math.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include<errno.h>
#include<GL\GL.h>
#include<GL\GLU.h>
#include<GL\glut.h>


float y = 1, x = 170, z = -70; 
float deltaMove = 0.0; 

float lx = 0.0, lz = 1; 


float angle = 0.0; 
float deltaAngle = 0.0; 


int isDragging = 0; 
int xDragStart = 0; 
int move=0;
int vertmove=0;
       
unsigned int dataPos;            
unsigned int width, height;
unsigned int imageSize;            
unsigned char * data=NULL;        

void draw_board()
{
    glColor3f(0.177,0.564,1);
    glBegin(GL_QUADS);
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);
        glVertex3f(1,2,0);
        glVertex3f(0,2,0);
        glVertex3f(9,0,0);
        glVertex3f(10,0,0);
        glVertex3f(10,2,0);
        glVertex3f(9,2,0);
    glEnd();
    glColor3f(0.690,0.878,0.901);
    glBegin(GL_QUADS);
        glVertex3f(0,2,0);
        glVertex3f(10,2,0);
        glVertex3f(10,4,0);
        glVertex3f(0,4,0);
    glEnd();
}

void restrict()			
{
    if(x>200)    x=200;
    else if(x<-200)    x=-200;
    if(y>60)    y=60;
    else if(y<0.5)    y=0.5;
    if(z>200)    z=200;
    else if(z<-200)    z=-200;
}

void court2()		
{
    int k;
    
    glPushMatrix();
    glTranslatef(-5,0.1,16);
        glColor3f(0,0,1);
        glBegin(GL_QUADS);
            glVertex3f(0,0.2,0);
            glVertex3f(10,0.2,0);
            glVertex3f(10,0.2,-30);
            glVertex3f(0,0.2,-30);
        glEnd();
        glColor3f(1,1,1);
        glBegin(GL_LINE_LOOP);
            glVertex3f(1,0.3,-1);
            glVertex3f(9,0.3,-1);
            glVertex3f(9,0.3,-13);
            glVertex3f(1,0.3,-13);
        glEnd();
        glBegin(GL_LINE_LOOP);
            glVertex3f(1,0.3,-15);
            glVertex3f(9,0.3,-15);
            glVertex3f(9,0.3,-29);
            glVertex3f(1,0.3,-29);
        glEnd();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-5,0,8);
        glColor3f(1,1,1);
        for(float i=0;i<0.8;i+=0.2)
            for(float j=0;j<9.8;j+=0.2)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex3f(j,i,0);
                    glVertex3f(j+0.2,i,0);
                    glVertex3f(j+0.2,i+0.2,0);
                    glVertex3f(j,i+0.2,0);
                glEnd();
            }
    glPopMatrix();
    glPushMatrix();
        glTranslatef(-5,0,-8);
        glColor3f(1,1,1);
        for(float i=0;i<0.8;i+=0.2)
            for(float j=0;j<9.8;j+=0.2)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex3f(j,i,0);
                    glVertex3f(j+0.2,i,0);
                    glVertex3f(j+0.2,i+0.2,0);
                    glVertex3f(j,i+0.2,0);
                glEnd();
            }
    glPopMatrix(); 
}


class building			
{
    float structure[8][3];
public:
    building(float a, float b, float c)
    {
        structure[0][0]=0;structure[0][1]=0;structure[0][2]=0;
        structure[1][0]=a;structure[1][1]=0;structure[1][2]=0;
        structure[2][0]=a;structure[2][1]=b;structure[2][2]=0;
        structure[3][0]=0;structure[3][1]=b;structure[3][2]=0;
        structure[4][0]=0;structure[4][1]=0;structure[4][2]=c;
        structure[5][0]=a;structure[5][1]=0;structure[5][2]=c;
        structure[6][0]=a;structure[6][1]=b;structure[6][2]=c;
        structure[7][0]=0;structure[7][1]=b;structure[7][2]=c;
    }
    void disp_build(char text[15],char side='/0')
    {
        float door[3];
        glColor3f(1,0.980,0.980);
        glBegin(GL_QUADS);
            glVertex3fv(structure[0]);
            glVertex3fv(structure[1]);
            glVertex3fv(structure[2]);
            glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[0]);
                glVertex3fv(structure[4]);
                glVertex3fv(structure[7]);
                glVertex3fv(structure[3]);
        glEnd();
        glBegin(GL_QUADS);
            glVertex3fv(structure[4]);
            glVertex3fv(structure[5]);
            glVertex3fv(structure[6]);
            glVertex3fv(structure[7]);
        glEnd();
        glBegin(GL_QUADS);
                glVertex3fv(structure[1]);
                glVertex3fv(structure[2]);
                glVertex3fv(structure[6]);
                glVertex3fv(structure[5]);
        glEnd();
        
        if(structure[1][0]>(-1*structure[4][2]))
        {
            for(float i=10; i<structure[2][1]; i+=10)
            {
                glPushMatrix();
                glTranslatef(0,i,0);
                for(float j=5; j<structure[1][0]; j+=15)
                {
                    glColor3f(0,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(j,0,0.1);
                        glVertex3f(j+5,0,0.1);
                        glVertex3f(j+5,5,0.1);
                        glVertex3f(j,5,0.1);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(j,0,structure[4][2]-0.1);
                        glVertex3f(j+5,0,structure[4][2]-0.1);
                        glVertex3f(j+5,5,structure[4][2]-0.1);
                        glVertex3f(j,5,structure[4][2]-0.1);
                    glEnd();
                }
                for(float j=0;j<structure[1][0];j+=15)
                {
                    glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,0.1);
                        glVertex3f(j+2,-10,0.1);
                        glVertex3f(j+2,10,0.1);
                        glVertex3f(j,10,0.1);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(j,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,-10,structure[4][2]-0.1);
                        glVertex3f(j+2,10,structure[4][2]-0.1);
                        glVertex3f(j,10,structure[4][2]-0.1);
                    glEnd();
                }
                glPopMatrix();
            }    
            glColor3f(0,0,0);
            door[0]=(structure[1][0]/2);
            
            
            
            
            
            
            glPushMatrix();
            glTranslatef(10,0,-23);
            glRotatef(180,0,1,0);
            draw_board();
            glPushMatrix();
            glTranslatef(1,2,0.1);
            glScalef(.01, .01, .01);
            glLineWidth(2);
            glColor3f(0,0,0);
            for (int c=0; text[c] != 0; ++c)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
            glPopMatrix();
            glPopMatrix();
        }
        else
        {
            for(float i=10; i<structure[2][1]; i+=10)
            {
                glPushMatrix();
                glTranslatef(0,i,0);
                for(float j=-5; j>structure[4][2]; j-=15)
                {
                    glColor3f(0,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(-0.1,0,j);
                        glVertex3f(-0.1,0,j-5);
                        glVertex3f(-0.1,5,j-5);
                        glVertex3f(-0.1,5,j);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(structure[1][0]+0.1,0,j);
                        glVertex3f(structure[1][0]+0.1,0,j-5);
                        glVertex3f(structure[1][0]+0.1,5,j-5);
                        glVertex3f(structure[1][0]+0.1,5,j);
                    glEnd();
                }
                for(float j=0;j>structure[4][2];j-=15)
                {
                    glColor3f(1,0,0);
                    glBegin(GL_POLYGON);
                        glVertex3f(-0.1,-10,j);
                        glVertex3f(-0.1,-10,j-2);
                        glVertex3f(-0.1,10,j-2);
                        glVertex3f(-0.1,10,j);
                    glEnd();
                    glBegin(GL_POLYGON);
                        glVertex3f(structure[1][0]+0.1,-10,j);
                        glVertex3f(structure[1][0]+0.1,-10,j-2);
                        glVertex3f(structure[1][0]+0.1,10,j-2);
                        glVertex3f(structure[1][0]+0.1,10,j);
                    glEnd();
                }
                glPopMatrix();
            }
            door[2]=(structure[4][2]/2);
            door[0]=structure[1][0];
            glColor3f(0,0,0);
            if(side=='r')
            {
                glBegin(GL_POLYGON);
                    glVertex3f(door[0]+0.2,0,door[2]-4);
                    glVertex3f(door[0]+0.2,0,door[2]+4);
                    glVertex3f(door[0]+0.2,7,door[2]+4);
                    glVertex3f(door[0]+0.2,7,door[2]-4);
                glEnd();
                glPushMatrix();
                glTranslatef(door[0]+3,0,-2);
                glRotatef(90,0,1,0);
                draw_board();
                glPushMatrix();
                glTranslatef(1,2,0.1);
                glScalef(.01, .01, .01);
                glLineWidth(2);
                glColor3f(0,0,0);
                for (int c=0; text[c] != 0; ++c)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
                glPopMatrix();
                glPopMatrix();
            }
            else if(side=='l')
            {
                glBegin(GL_POLYGON);
                    glVertex3f(-0.2,0,door[2]-4);
                    glVertex3f(-0.2,0,door[2]+4);
                    glVertex3f(-0.2,7,door[2]+4);
                    glVertex3f(-0.2,7,door[2]-4);
                glEnd();
                glPushMatrix();
                glTranslatef(-3,0,-10);
                glRotatef(-90,0,1,0);
                draw_board();
                glPushMatrix();
                glTranslatef(1,2,0.1);
                glScalef(.01, .01, .01);
                glLineWidth(2);
                glColor3f(0,0,0);
                for (int c=0; text[c] != 0; ++c)
                    glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
                glPopMatrix();
                glPopMatrix();
            }
        }
        glPushMatrix();
        glTranslatef(0,10,0);
        glColor3f(0,0,1);
        for(int i=0;i<structure[2][1]-5;i+=5)
        {
            glBegin(GL_LINES);
                glVertex3f(structure[0][0],i,structure[0][2]+0.1);
                glVertex3f(structure[1][0],i,structure[0][2]+0.1);
                glVertex3f(structure[0][0]-0.1,i,structure[0][2]);
                glVertex3f(structure[0][0]-0.1,i,structure[4][2]);
                glVertex3f(structure[4][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0],i,structure[4][2]-0.1);
                glVertex3f(structure[5][0]+0.1,i,structure[5][2]);
                glVertex3f(structure[1][0]+0.1,i,structure[1][2]);

            glEnd();
        }
        glPopMatrix();
    }
};
building hostel(20,30,-30);
building admin(60,40,-20);
building mess(20,10,-20);
building power(20,5,-20);
building canteen2(20,40,-40);

void loop(float x,float y,float z)		
{
    float xx,zz,d;
    glColor3f(1,0,0);
    glPointSize(2);
    glBegin(GL_POINTS);
        for(int i=0;i<360;i++)
    {
        d=i*(180/3.14);
        xx=cos(d)+x;
        zz=sin(d)+z;
        glVertex3f(xx,y,zz);
    }
    glEnd();
}


class bball				
{
    float bordr[4][3];
    float bskt[8][3];
public:
    bball()
    {
        bordr[0][0]=0;bordr[0][1]=0;bordr[0][2]=0;
        bordr[1][0]=20;bordr[1][1]=0;bordr[1][2]=0;
        bordr[2][0]=20;bordr[2][1]=0;bordr[2][2]=-20;
        bordr[3][0]=0;bordr[3][1]=0;bordr[3][2]=-20;
        bskt[0][0]=9;bskt[0][1]=4.5;bskt[0][2]=-0.1;
        bskt[1][0]=11;bskt[1][1]=4.5;bskt[1][2]=-0.1;
        bskt[2][0]=11;bskt[2][1]=6.5;bskt[2][2]=-0.1;
        bskt[3][0]=9;bskt[3][1]=6.5;bskt[3][2]=-0.1;
        bskt[4][0]=9,bskt[4][1]=4.5;bskt[4][2]=-19.9;
        bskt[5][0]=11;bskt[5][1]=4.5;bskt[5][2]=-19.9;
        bskt[6][0]=11;bskt[6][1]=6.5;bskt[6][2]=-19.9;
        bskt[7][0]=9;bskt[7][1]=6.5;bskt[7][2]=-19.9;
    }
    void disp_court()
    {
        glPushMatrix();
        glTranslatef(0,0.1,0);
        glColor3f(0.745,0.745,0.745);
        glBegin(GL_QUADS);
            glVertex3fv(bordr[0]);
            glVertex3fv(bordr[1]);
            glVertex3fv(bordr[2]);
            glVertex3fv(bordr[3]);
        glEnd();
        glColor3f(1,0.270,0);
        GLUquadricObj *quadratic;
        quadratic = gluNewQuadric();
        GLUquadricObj *quadratic1;
        quadratic1 = gluNewQuadric();
        glPushMatrix();
        glTranslatef(10,0,0);
            loop(0,5,-1);
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.698,0.133,0.133);
        gluCylinder(quadratic,0.1,0.1,5.0f,32,32);
        glPopMatrix();
        glPopMatrix();
        glPushMatrix();
        glTranslatef(10,0,-20);
            loop(0,5,1);
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        glColor3f(0.698,0.133,0.133);
        gluCylinder(quadratic1,0.1,0.1,5.0f,32,32);
        glPopMatrix();
        glPopMatrix();
        glColor3f(0.745,0.745,0.745);
        glBegin(GL_QUADS);
            for(int i=0;i<8;i++)
                glVertex3fv(bskt[i]);
        glEnd();
        glPopMatrix();
    }
};
bball crt1;

void changeSize(int w, int h) 
{
    float ratio =  ((float) w) / ((float) h); 
    glMatrixMode(GL_PROJECTION); 
    glLoadIdentity(); 
    gluPerspective(45.0, ratio, 0.1, 200.0); 
    glMatrixMode(GL_MODELVIEW); 
    glViewport(0, 0, w, h); 
}

void update(void) 
{
    if (deltaMove) { 
        x += deltaMove*lx * 0.38;
        z += deltaMove*lz * 0.38;
    }
    if(vertmove==1)    y+=0.1;
    if(vertmove==-1) y-=0.1;
    restrict();
    glutPostRedisplay(); 
}
void disp_roads()			
{
    glColor3f(0.411,0.411,0.411);
    glBegin(GL_QUADS);
        glVertex3f(-40,0.1,100);
        glVertex3f(-40,0.1,-70);
        glVertex3f(-20,0.1,-70);
        glVertex3f(-20,0.1,100);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-40,0.1,100);
        glVertex3f(40,0.1,115);
        glVertex3f(40,0.1,130);
        glVertex3f(-20,0.1,100);
        
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(40,0.1,115);
        glVertex3f(190,0.1,115);
        glVertex3f(190,0.1,130);
        glVertex3f(40,0.1,130);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(110,0.1,115);
        glVertex3f(110,0.1,-20);
        glVertex3f(120,0.1,-20);
        glVertex3f(120,0.1,115);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(95,0.1,130);
        glVertex3f(95,0.1,190);
        glVertex3f(85,0.1,190);
        glVertex3f(85,0.1,130);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(160,0.1,115);
        glVertex3f(160,0.1,-20);
        glVertex3f(170,0.1,-20);
        glVertex3f(170,0.1,115);
    glEnd();
    glBegin(GL_QUADS);
        glVertex3f(-80,0.1,-20);
        glVertex3f(190,0.1,-20);
        glVertex3f(190,0.1,-35);
        glVertex3f(-80,0.1,-35);
    glEnd();
    
}

void trees()			
{
    GLUquadricObj *quadratic;
    GLUquadricObj *quadratic1;
        quadratic1 = gluNewQuadric();
    quadratic = gluNewQuadric();
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.721,0.525,0.043);
    gluCylinder(quadratic,1,1,10.0f,32,32);
    glPopMatrix();
    glTranslatef(0,2,0);
        glPushMatrix();
    float k=0;
    for(int i=0,j=0;i<3;i++,j+=0.5,k+=0.15)
    {
        glTranslatef(0,1.8,0);
        glColor3f(0.133+k,0.545+k,0.133-k);
        glPushMatrix();
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
            gluCylinder(quadratic1,4-j,0,4.0f,32,32);
        glPopMatrix();
    }
    glPopMatrix();
}

void draw_arch(char text[5])		
{
    glColor3f(0,0,1);
    glPushMatrix();
    glTranslatef(0,3.5,0);
    glScalef(4,7,2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(16,3.5,0);
    glScalef(4,7,2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glTranslatef(8,9,0);
    glScalef(20,4,2);
    glutSolidCube(1);
    glPopMatrix();
    glPushMatrix();
    glRotatef(180,0,1,0);
    glTranslatef(-10,8,1.1);
    
    glScalef(.02, .02, .02);
    glLineWidth(4.5);
    glColor3f(1,1,1);
    for (int c=0; text[c] != 0; ++c)
        glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
    glPopMatrix();
}

void display()
{
    glClearColor(0.7,0.7,1,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glLoadIdentity();
    gluLookAt(
            x,      y,      z,
            x+lx,    y,        z+lz,
            0.0,    1.0,    0.0);
    printf("\nz=%f\tx=%f\n",z,x);
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
        glVertex3f(-200,0,200);
        glVertex3f(200,0,200);
        glVertex3f(200,0,2);
        glVertex3f(-200,0,2);
        glVertex3f(-200,0,2);
        glVertex3f(-1,0,2);
        glVertex3f(-1,0,-200);
        glVertex3f(-200,0,-200);
        glVertex3f(-1,0,-200);
        glVertex3f(200,0,-200);
        glVertex3f(200,0,-2);
        glVertex3f(-1,0,-2);
        glVertex3f(5,0,-2);
        glVertex3f(200,0,-2);
        glVertex3f(200,0,2);
        glVertex3f(5,0,2);
        glEnd();
    
    disp_roads();
    
    for(int i=25;i<90;i+=10)
    {
        glPushMatrix();
            glTranslatef(i,0,25);
            trees();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(i,0,35);
            trees();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(i,0,45);
            trees();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(i,0,55);
            trees();
        glPopMatrix();
        glPushMatrix();
            glTranslatef(i,0,65);
            trees();
        glPopMatrix();
    }
    glPushMatrix();
    glTranslatef(160,0,-40);
        draw_arch("IIITS");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-70,0,80);
        hostel.disp_build("BH2 hostel",'r');
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-15,0,45);
        crt1.disp_court();
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-70,0,45);
        mess.disp_build("Mess",'r');
    glPopMatrix();
    glPushMatrix();
    glTranslatef(-70,0,20);
        hostel.disp_build("BH1 hostel",'r');
    glPopMatrix();

    court2();
   
    glPushMatrix();
    glTranslatef(50,0,-45);
        power.disp_build("Power Station");
    glPopMatrix();
    glPushMatrix();
    glTranslatef(100,0,180);
        canteen2.disp_build("side",'r');
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(100,0,155);
        admin.disp_build("Academic BLOCK");
    glPopMatrix();

    
    char * text = "Power";
glPushMatrix();
            glTranslatef(50,0,-43);
            
            draw_board();
            glPushMatrix();
            glTranslatef(1,2,0.1);
            glScalef(.01, .01, .01);
            glLineWidth(2);
            glColor3f(0,0,0);
            for (int c=0; text[c] != 0; ++c)
                glutStrokeCharacter(GLUT_STROKE_ROMAN, text[c]);
            glPopMatrix();
            glPopMatrix();
    glutSwapBuffers();
    glFlush();
}

void pressKey(unsigned char key, int xx, int yy)
{
    switch (key) {
        case 'w' : deltaMove = 1.0;glutIdleFunc(update); break;
        case 's' : deltaMove = -1.0;glutIdleFunc(update); break;
        case 'f' : vertmove=1;glutIdleFunc(update);break;
        case 'v' : vertmove=-1;glutIdleFunc(update);break;
    }
} 

void releaseKey(unsigned char key, int x, int y) 
{
    switch (key) {
        case 'w' : deltaMove = 0.0; glutIdleFunc(NULL);break;
        case 's' : deltaMove = 0.0; glutIdleFunc(NULL);break;
        case 'f' : vertmove=0;glutIdleFunc(NULL);break;
        case 'v' : vertmove=0;glutIdleFunc(NULL);break;

    }
} 

void mouseMove(int x, int y) 
{     
    if (isDragging) { 
        
        deltaAngle = (x - xDragStart) * -0.005;
        
        lx = sin(angle + deltaAngle);
        lz = cos(angle + deltaAngle);
        glutIdleFunc(update);
    }
}

void mouseButton(int button, int state, int x, int y) 
{
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) { 
            isDragging = 1;
            xDragStart = x; 
        }
        else  {
            angle += deltaAngle; 
            isDragging = 0; 
            glutIdleFunc(NULL);
        
        }
    }
}
int main(int argc, char **argv){

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500,500);
    glutInitWindowPosition(40,40);
    glutCreateWindow("IIITS");
    glutReshapeFunc(changeSize); 
    glutDisplayFunc(display);
   
    glutIgnoreKeyRepeat(1);
    glutKeyboardFunc(pressKey);
    glutKeyboardUpFunc(releaseKey);
    glutMouseFunc(mouseButton); 
    glutMotionFunc(mouseMove); 
    glutMainLoop();
    return 0;
}

