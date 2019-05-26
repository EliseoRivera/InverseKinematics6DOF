#ifndef ROBOT_H
#define ROBOT_H
#include "modelo3D.h"
#include<vector>
#include <cstdlib>
///Copyright (C) <2019>  <Eliseo Rivera> curso.tareas@gmail.com
class Robot
{
    public:
        Robot();
        ~Robot();

        modelo3D *base;
           modelo3D *b1;
              modelo3D *b2;
                 modelo3D *b3;
                    modelo3D *b4;
                       modelo3D *b5;
                          modelo3D *b6;
                               modelo3D *gripe;

void inicializar();
void renderizar();
void configurarTH();
float t;
///
void Parametrica();

bool InverseKinematics();

double xg,yg,zg;
//
void AplicarTHx(float theta, vector3d d);
void AplicarTHy(float theta, vector3d d);
void AplicarTHz(float theta, vector3d d);
Matrix THx,THy,THz,TH;

std::vector<Matrix> THList;
std::vector<vector3d> Origenes;
std::vector<modelo3D*> modelos;


double theta1, theta2, theta3,theta4, theta5, theta6,theta7;
double z1, z2, z3,z4, z5,z6,z7;
double x0,y0,z0;
double x2;
void DibujarCurva();
/// cinematica inversa parte 2//////////////////
Matrix iL, jL, kL;
Matrix k3,k4,k5,k6,k60;
Matrix i3,i4,i5,i6, i60;
Matrix TPL, TPG,TPVL,TPVG,TPG0;
Matrix O4, O5,O6;

void initVectores();
void initInverseKinematics2();
void InverseKinematics2();
///cinematica inversa parte 3: orientación de efector final sin cambiar posición del punto terminal
void ProtarZEfector();
void ProtarYEfector();
void ProtarXEfector();
void NrotarZEfector();
void NrotarYEfector();
void NrotarXEfector();
void InverseKinematics3();
Matrix v;

private :
void DefinirTHx(float theta, vector3d d);
void DefinirTHy(float theta, vector3d d);
void DefinirTHz(float theta, vector3d d);
void  Drawarrow3D( vector3d A,  vector3d B, vector3d color, double cota1,double R) ;
std::vector<vector3d> curva;
double signo(const Matrix &A){
if (A.aij[2][0]>=0) return 1;
else return -1;
}

};

#endif // ROBOT_H
