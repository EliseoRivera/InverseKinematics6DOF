#include "Robot.h"
#define PI 3.14159265358979323846264338327
///Copyright (C) <2019>  <Eliseo Rivera> curso.tareas@gmail.com
Robot::Robot()
{
   theta1=0;theta2=0;theta3=0; theta4=0; theta5=0; theta6=0,theta7=0;
   THx.identity(4);
   THy.identity(4);
   THz.identity(4);
   int d=4;
   TH.identity(4);
   initVectores();

   v.zero(4,1);
}

Robot::~Robot()
{

delete base;
delete b1;
delete b2;
delete b3;
delete b4;
delete b5;
delete b6;

    //dtor
}

void Robot::inicializar(){
base=new modelo3D();
b1=new modelo3D();
b2=new modelo3D();
b3=new modelo3D();
b4=new modelo3D();
b5=new modelo3D();
b6=new modelo3D();
gripe=new modelo3D();


base->leer("base.STL");
b1->leer("b1.STL");
b2->leer("b2.STL");
b3->leer("b3.STL");
b4->leer("b4.STL");
b5->leer("b5.STL");
b6->leer("b6.STL");
//gripe->leer("gripe.STL");
base->color={0.5,0.5,0.5};
b1->color={0.5,1,0.5};
b2->color={1,0.5,0.5};
b3->color={0.5,0.5,1};
b4->color={1,0.5,0};
b5->color={1.0,0.0,0.0};
b6->color={0.0,0.0,1};
modelos.push_back(base);
modelos.push_back(b1);
modelos.push_back(b2);
modelos.push_back(b3);
modelos.push_back(b4);
modelos.push_back(b5);
modelos.push_back(b6);

}

void Robot::initVectores(){
iL.zero(4,1); jL.zero(4,1); kL.zero(4,1);
iL.entry(0,0)=1;jL.entry(1,0)=1;kL.entry(2,0)=1;
k3.zero(4,1);k4.zero(4,1);k5.zero(4,1);k6.zero(4,1);k60.zero(4,1);
i3.zero(4,1);i4.zero(4,1);i5.zero(4,1);i6.zero(4,1);i60.zero(4,1);

TPL.zero(4,1); TPG.zero(4,1);TPVL.zero(4,1); TPG0.zero(4,1);
TPVG.zero(4,1);
O4.zero(4,1); O5.zero(4,1);O6.zero(4,1);
TPL.entry(0,0)=-4.25;
TPL.entry(1,0)=0;
TPL.entry(2,0)=3.65;
TPL.entry(3,0)=1; //coordenadas homogeneas

TPVL.entry(0,0)=-4.25;
TPVL.entry(1,0)=0;
TPVL.entry(2,0)=3.65;
TPVL.entry(3,0)=0; //coordenadas homogeneas

}

void Robot::configurarTH(){
//home
AplicarTHz(0,{0,0,0}); //base
THList.push_back(THz);
AplicarTHx(0,{0,0,0}); //base
THList.push_back(THx);
 theta1=0;theta2=-PI/2.0;theta3=0; theta4=0; theta5=PI/2.0; theta6=0,theta7=0;
 z1=6.5; z2=0;z3=0;z4=10.0;z5=0,z6=3.5;z7=3.0;
x2=12.0;
AplicarTHz( theta1,{0,0,z1}); //b1
THList.push_back(THz);
AplicarTHx(-PI/2.0,{0,0,0}); //b1
THList.push_back(THx);

AplicarTHz( theta2,{0,0,z2}); //b2
THList.push_back(THz);
AplicarTHx(0,{x2,0,0}); //b2
THList.push_back(THx);

AplicarTHz( theta3,{0,0,z3}); //b3
THList.push_back(THz);
AplicarTHx(-PI/2.0,{0,0,0}); //b3
THList.push_back(THx);


AplicarTHz( theta4,{0,0,z4}); //b4
THList.push_back(THz);
AplicarTHx(-PI/2.0,{0,0,0}); //
THList.push_back(THx);

AplicarTHz( theta5,{0,0,z5}); //b5
THList.push_back(THz);
AplicarTHx(-PI/2.0,{0,0,0}); //
THList.push_back(THx);

AplicarTHz( theta6,{0,0,z6}); //b6
THList.push_back(THz);
AplicarTHx(0,{0,0,0});
THList.push_back(THx);




initInverseKinematics2();
}

void Robot::initInverseKinematics2(){
t=0;
///calcular la posicion de O6,k6,i6 en base global y respecto del sistema de referencia global
TH.resetIdentity();
modelo3D *model;
for (int m=0;m<modelos.size();m++){

    model=modelos[m];
    TH=TH* THList[2*m+0]*THList[2*m+1];}
k60=TH*kL; /// siempre se conoce del control manual
i60=TH*iL;
i6=i60;
k6=k60;
TPG=TH*TPL;
TPG0=TPG;
TPVG=TH*TPVL;
O6.entry(0,0)=TH.entry(0,3);
O6.entry(1,0)=TH.entry(1,3);
O6.entry(2,0)=TH.entry(2,3);
O6.entry(3,0)=1;
///calcular O4 a partir de O6 y k6,i6;

curva.resize(0);
curva.push_back(vector3d(TPG.entry(0,0),TPG.entry(1,0),TPG.entry(2,0)));

}
void Robot::InverseKinematics2(){

O4=O6-z6*k6;
xg=O4.entry(0,0);
yg=O4.entry(1,0);
zg=O4.entry(2,0);
bool status=InverseKinematics();
if (status==false){ cout<<" configuracion no alcanzable, utilizar el control manual"<<endl ;return ;}
///continua

TH.resetIdentity();
modelo3D *model;
for (int m=0;m<4;m++){

    model=modelos[m];
    TH=TH* THList[2*m+0]*THList[2*m+1];}
k3=TH*kL; /// TH transforma un vector en base B3 a base global
i3=TH*iL;
k4=k6.crossproduct(k3);
k4=k4.normalizar();


i4=k4.crossproduct(k3);
i4.normalizar(); //i4 en base global
///TH.transpuesta transforma  un vector de base global a base B3
i4=TH.Transpuesta()*i4;  //i4 esta en base local B3
theta4=iL.angle(i4)*signo(iL.crossproduct(i4));
AplicarTHz(theta4,{0,0,z4});
THList[8]=THz;
TH=TH*THz*THList[9];

i5=k6.crossproduct(k4);
i5=i5.normalizar();
i5=TH.Transpuesta()*i5; //en base local B4
theta5=iL.angle(i5)*signo(iL.crossproduct(i5));
AplicarTHz(theta5,{0,0,z5});
THList[10]=THz;
TH=TH*THz*THList[11];

i6=TH.Transpuesta()*i60; // i6 en base local B5
theta6=iL.angle(i6)*signo(iL.crossproduct(i6));
AplicarTHz(theta6,{0,0,z6});
THList[12]=THz;
TH=TH*THz*THList[13];
TPG=TH*TPL;
curva.push_back(vector3d(TPG.entry(0,0),TPG.entry(1,0),TPG.entry(2,0)));
Parametrica();
O6=TPG-TPVG;

}

void Robot::InverseKinematics3(){

O4=O6-z6*k6;
xg=O4.entry(0,0);
yg=O4.entry(1,0);
zg=O4.entry(2,0);
bool status=InverseKinematics();
if (status==false){ cout<<" configuracion no alcanzable, utilizar el control manual"<<endl ;return ;}
///continua

TH.resetIdentity();
modelo3D *model;
for (int m=0;m<4;m++){

    model=modelos[m];
    TH=TH* THList[2*m+0]*THList[2*m+1];}
k3=TH*kL; /// TH transforma un vector en base B3 a base global
i3=TH*iL;
k4=k6.crossproduct(k3);
k4=k4.normalizar();


i4=k4.crossproduct(k3);
i4.normalizar(); //i4 en base global
///TH.transpuesta transforma  un vector de base global a base B3
i4=TH.Transpuesta()*i4;  //i4 esta en base local B3
theta4=iL.angle(i4)*signo(iL.crossproduct(i4));
AplicarTHz(theta4,{0,0,z4});
THList[8]=THz;
TH=TH*THz*THList[9];

i5=k6.crossproduct(k4);
i5=i5.normalizar();
i5=TH.Transpuesta()*i5; //en base local B4
theta5=iL.angle(i5)*signo(iL.crossproduct(i5));
AplicarTHz(theta5,{0,0,z5});
THList[10]=THz;
TH=TH*THz*THList[11];

i6=TH.Transpuesta()*i60; // i6 en base local B5
theta6=iL.angle(i6)*signo(iL.crossproduct(i6));
AplicarTHz(theta6,{0,0,z6});
THList[12]=THz;
TH=TH*THz*THList[13];

curva.push_back(vector3d(TPG.entry(0,0),TPG.entry(1,0),TPG.entry(2,0)));


}
void Robot::Parametrica(){


TPG.entry(0,0)=TPG0.entry(0,0)+t;
TPG.entry(1,0)=TPG0.entry(1,0)+0;
TPG.entry(2,0)=TPG0.entry(2,0)+0;
TPG.entry(3,0)=1;
};
void Robot::ProtarZEfector(){
Matrix u(4,1);
float dtheta=0.051;
Matrix v=-1*TPVG;
u=TH*kL;
v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
i60=cos(dtheta)*i60+sin(dtheta)*(u.crossproduct(i60))+(u.dotProduct(i60)*(1-cos(dtheta)))*u;
k6=u;
O6=TPG+v;
O6.entry(3,0)=1;
O6.mostrar();
TPVG=-1*v;
InverseKinematics3();

}


void Robot::ProtarYEfector( ){
    Matrix u(4,1);
float dtheta=0.051;

Matrix v=-1*TPVG;
u=TH*jL;


v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
i60=cos(dtheta)*i60+sin(dtheta)*(u.crossproduct(i60))+(u.dotProduct(i60)*(1-cos(dtheta)))*u;
k60=cos(dtheta)*k60+sin(dtheta)*(u.crossproduct(k60))+(u.dotProduct(k60)*(1-cos(dtheta)))*u;

k6=k60;
O6=TPG+v;
O6.entry(3,0)=1;

TPVG=-1*v;
InverseKinematics3();

}
void Robot::ProtarXEfector(){
    Matrix u(4,1);
float dtheta=0.051;

Matrix v=-1*TPVG;
u=TH*iL;


v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
k60=cos(dtheta)*k60+sin(dtheta)*(u.crossproduct(k60))+(u.dotProduct(k60)*(1-cos(dtheta)))*u;

k6=k60;
O6=TPG+v;
O6.entry(3,0)=1;

TPVG=-1*v;
InverseKinematics3();

}
void Robot::NrotarZEfector(){

    Matrix u(4,1);
float dtheta=-0.051;

Matrix v=-1*TPVG;
u=TH*kL;


v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
i60=cos(dtheta)*i60+sin(dtheta)*(u.crossproduct(i60))+(u.dotProduct(i60)*(1-cos(dtheta)))*u;
k6=u;

O6=TPG+v;
O6.entry(3,0)=1;
O6.mostrar();

TPVG=-1*v;
InverseKinematics3();


}

void Robot::NrotarYEfector( ){
    Matrix u(4,1);
float dtheta=-0.051;

Matrix v=-1*TPVG;
u=TH*jL;


v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
i60=cos(dtheta)*i60+sin(dtheta)*(u.crossproduct(i60))+(u.dotProduct(i60)*(1-cos(dtheta)))*u;
k60=cos(dtheta)*k60+sin(dtheta)*(u.crossproduct(k60))+(u.dotProduct(k60)*(1-cos(dtheta)))*u;

k6=k60;
O6=TPG+v;
O6.entry(3,0)=1;

TPVG=-1*v;
InverseKinematics3();

}
void Robot::NrotarXEfector(){
    Matrix u(4,1);
float dtheta=-0.051;

Matrix v=-1*TPVG;
u=TH*iL;


v=cos(dtheta)*v+sin(dtheta)*(u.crossproduct(v))+(u.dotProduct(v)*(1-cos(dtheta)))*u;
k60=cos(dtheta)*k60+sin(dtheta)*(u.crossproduct(k60))+(u.dotProduct(k60)*(1-cos(dtheta)))*u;

k6=k60;
O6=TPG+v;
O6.entry(3,0)=1;

TPVG=-1*v;
InverseKinematics3();

}
bool Robot::InverseKinematics(){

theta1=atan2(yg,xg);
double e,f,c,a,b;
double q2;
a=2.0*yg*x2*sin(theta1);
b=2.0*(zg-z1)*x2*sin(theta1)*sin(theta1);
if (b<0) return false;
c=sqrt(a*a+b*b);
e=-(z4*z4*sin(theta1)*sin(theta1)-yg*yg-x2*x2*sin(theta1)*sin(theta1)-(zg-z1)*(zg-z1)*sin(theta1)*sin(theta1));


float ec1,ec2,ec3;


double dis1=(4.0*e*e*a*a)-4.0*(a*a+b*b)*(e*e-b*b);
if (dis1<0 ){ cout<<"Congiuracion no alcanzable. Retorne."<<endl;  return false;}
double q21,q22;
if (fabs((2.0*e*a+sqrt(dis1))/(2.0*(a*a+b*b)))>1){ cout<<"Congiuracion no alcanzable. Retorne."<<endl;  return false;}
if (fabs((2.0*e*a-sqrt(dis1))/(2.0*(a*a+b*b)))>1) { cout<<"Congiuracion no alcanzable. Retorne."<<endl;  return false;}

q21=asin((2.0*e*a+sqrt(dis1))/(2.0*(a*a+b*b)));
q22=asin((2.0*e*a-sqrt(dis1))/(2.0*(a*a+b*b)));

if(fabs(q21-q2)<fabs(q22-q2))q2=q21;
else q2=q22;
theta2=q2-PI/2.0;
double q=(zg-x2*cos(q2)-z1)/(-1.0*z4);
if (fabs(q)>0.9) { cout<<"Congiuracion no alcanzable. Retorne usando control manual."<<endl;  return false;}
theta3=asin(q)-q2;

AplicarTHz( theta1,{0,0,z1}); //b1
THList[2]=THz;
AplicarTHz( theta2,{0,0,z2}); //b2
THList[4]=THz;
AplicarTHz( theta3,{0,0,z3}); //b3
THList[6]=THz;


return true;
};

void Robot::renderizar(){

TH.resetIdentity();

modelo3D *model;

for (int m=0;m<modelos.size();m++){

    model=modelos[m];
    TH=TH* THList[2*m+0]*THList[2*m+1];


vector3d ux,uy,uz,O;
ux={1,0,0};
uy={0,1,0};
uz={0,0,1};

Matrix ux4(ux,1),uy4(uy,1),uz4(uz,1),O4(O,1);


ux4=TH*ux4-TH*O4;
uy4=TH*uy4-TH*O4;
uz4=TH*uz4-TH*O4;
O4=TH*O4;


ux={ux4.aij[0][0],ux4.aij[1][0],ux4.aij[2][0]};
uy={uy4.aij[0][0],uy4.aij[1][0],uy4.aij[2][0]};
uz={uz4.aij[0][0],uz4.aij[1][0],uz4.aij[2][0]};
O={O4.aij[0][0],O4.aij[1][0],O4.aij[2][0]};

//if (m<2){
         Drawarrow3D(O,O+4*ux,{1,0.1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uy,{.1,1,0.2},0.03,0.1);
         Drawarrow3D(O,O+4*uz,{0.1,0.2,1},0.03,0.1);
       //  }
         glColor4f(model->color.x,model->color.y,model->color.z, 0.5);

glEnable(GL_BLEND);
 glBegin(GL_TRIANGLES);

  glFrontFace(GL_FRONT_AND_BACK);
    for (int i=0;i<model->ntriangles;i++){

vector3d v1=model->triangulos[i].vertices[0];   //posiciones locales
vector3d v2=model->triangulos[i].vertices[1];
vector3d v3=model->triangulos[i].vertices[2];
Matrix v14(v1,1),v24(v2,1),v34(v3,1);

v14=TH*v14;
v24=TH*v24;
v34=TH*v34;
v1={v14.entry(0,0),v14.entry(1,0),v14.entry(2,0)};
v2={v24.entry(0,0),v24.entry(1,0),v24.entry(2,0)};
v3={v34.entry(0,0),v34.entry(1,0),v34.entry(2,0)};



Matrix N(4,1),d14(4,1),d24(4,1);
d14=v24-v14;
d24=v34-v14;
vector3d d1,d2,n;
d1={d14.entry(0,0),d14.entry(1,0),d14.entry(2,0)};
d2={d24.entry(0,0),d24.entry(1,0),d24.entry(2,0)};
n=d1*d2;  ///devuelve el producto vectorial
n.normalize();



        glNormal3f(n.x,n.y,n.z);
        glVertex3f(v1.x,v1.y,v1.z);
        glVertex3f(v2.x,v2.y,v2.z);
        glVertex3f(v3.x,v3.y,v3.z);
    }
glEnd();
// }
 glDisable(GL_BLEND);


///DIBUJAR EJES


//}
}

}




void Robot::DefinirTHx(float dtheta, vector3d d){

THx.aij[0][0]=1;
THx.aij[0][1]=0;
THx.aij[0][2]=0;
THx.aij[0][3]=d.x;

THx.aij[1][0]=0;
THx.aij[1][1]=cos(dtheta);
THx.aij[1][2]=-sin(dtheta);
THx.aij[1][3]=d.y;

THx.aij[2][0]=0;
THx.aij[2][1]=sin(dtheta);
THx.aij[2][2]=cos(dtheta);
THx.aij[2][3]=d.z;

THx.aij[3][0]=0;
THx.aij[3][1]=0;
THx.aij[3][2]=0;
THx.aij[3][3]=1;

}
void Robot::DefinirTHy(float dtheta, vector3d d){


THy.aij[0][0]=cos(dtheta);
THy.aij[0][1]=0;
THy.aij[0][2]=sin(dtheta);

THy.aij[1][0]=0;
THy.aij[1][1]=1;
THy.aij[1][2]=0;

THy.aij[2][0]=-sin(dtheta);
THy.aij[2][1]=0;
THy.aij[2][2]=cos(dtheta);

THy.aij[3][0]=0;
THy.aij[3][1]=0;
THy.aij[3][2]=0;
THy.aij[3][3]=1;

THy.aij[0][3]=d.x;
THy.aij[1][3]=d.y;
THy.aij[2][3]=d.z;
}
void Robot::DefinirTHz(float dtheta, vector3d d){

THz.aij[0][0]=cos(dtheta);
THz.aij[0][1]=-sin(dtheta);
THz.aij[0][2]=0;
THz.aij[0][3]=d.x;

THz.aij[1][0]=sin(dtheta);
THz.aij[1][1]=cos(dtheta);
THz.aij[1][2]=0;
THz.aij[1][3]=d.y;

THz.aij[2][0]=0;
THz.aij[2][1]=0;
THz.aij[2][2]=1;
THz.aij[2][3]=d.z;

THz.aij[3][0]=0;
THz.aij[3][1]=0;
THz.aij[3][2]=0;
THz.aij[3][3]=1;

}

void Robot::AplicarTHx(float theta, vector3d d){


DefinirTHx(theta,d);

}
void Robot::AplicarTHy(float theta, vector3d d){

DefinirTHy(theta,d);

}
void Robot::AplicarTHz(float theta, vector3d d){

DefinirTHz(theta,d);

}

void Robot::Drawarrow3D( vector3d A,  vector3d B, vector3d color, double cota1,double R)
{

double color1,color2,color3,a,b,c,d,e;



color1=color.x;//abs(color1/255);
color2=color.y;//abs(color2/255);
color3=color.z;//abs(color3/255);

glColor3f( color1,color2, color3);

vector3d n=B-A,np,vertex[10],normallight;
n.normalize();
if(n.z!=0)np={1,1,(-1/n.z)*(n.x+n.y)};
else if(n.y!=0)np={1,(-1/n.y)*(n.x+n.z),1};
else np={(-1/n.x)*(n.y+n.z),1,1};

np.normalize();
vertex[0]=R*np;
vertex[2]=R*(n*np).normalize();
vertex[1]=R*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=R*(n*vertex[2]).normalize();
vertex[3]=R*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=R*(n*vertex[4]).normalize();
vertex[5]=R*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=R*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
int nx=8;
double d_thetha,fraccion=0.1,radioflecha=R+.7*R;
d_thetha=2.0f*PI/nx;


  ///tubos
 glBegin( GL_TRIANGLE_STRIP );

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+A.x,vertex[i].y+A.y,vertex[i].z+A.z);

glVertex3f(vertex[i].x+B.x-fraccion*(B.x-A.x),vertex[i].y+B.y-fraccion*(B.y-A.y),vertex[i].z+B.z-fraccion*(B.z-A.z));

    // top face

                }

glEnd();



//flecha
vertex[0]=radioflecha*np;
vertex[2]=radioflecha*(n*np).normalize();
vertex[1]=radioflecha*((0.5)*(vertex[2]-vertex[0])+vertex[0]).normalize();
vertex[4]=radioflecha*(n*vertex[2]).normalize();
vertex[3]=radioflecha*((0.5)*(vertex[4]-vertex[2])+vertex[2]).normalize();
vertex[6]=radioflecha*(n*vertex[4]).normalize();
vertex[5]=radioflecha*((0.5)*(vertex[6]-vertex[4])+vertex[4]).normalize();
vertex[7]=radioflecha*((0.5)*(vertex[0]-vertex[6])+vertex[6]).normalize();
vertex[8]=vertex[0];
vertex[9]=vertex[1];
vector3d Ap(B-fraccion*(B-A));



 glBegin( GL_TRIANGLE_STRIP );  //flecha

         for(int i=0;i<9;i++)
               {

normallight=n*(vertex[i-1]-vertex[i+1]);
normallight.normalize();
glNormal3f(normallight.x, normallight.y, normallight.z);
                 glVertex3f(vertex[i].x+Ap.x,vertex[i].y+Ap.y,vertex[i].z+Ap.z);


glNormal3f(n.x, n.y, n.z);
glVertex3f(Ap.x+fraccion*(B-A).x,Ap.y+fraccion*(B-A).y,Ap.z+fraccion*(B-A).z);

    // top face

                }

glEnd();


}
void Robot::DibujarCurva(){

glBegin( GL_LINES);
for (int i=1;i<curva.size();i++){
    glVertex3f(curva[i-1].x,curva[i-1].y,curva[i-1].z);
     glVertex3f(curva[i].x,curva[i].y,curva[i].z);
    glColor3f(0,0,0);
}
glEnd();
return;
}
