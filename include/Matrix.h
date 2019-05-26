#ifndef MATRIX_H
#define MATRIX_H
#include"vector3d.h"
#include<iostream>
///Copyright (C) <2019>  <Eliseo Rivera> curso.tareas@gmail.com
class Matrix
{
public:
   double **aij;
   int n;
   int m;
   Matrix(vector3d v);
   Matrix (vector3d v, float w);
   double cofactor (int i, int j);
   Matrix inversa();
   int size () const;
   Matrix Mij(int a,int b);
   vector3d operator *(const vector3d &P);
   Matrix(int nn,int mm);
   Matrix();
   void zero(int nn, int mm);
   void identity(int nn);
   void resetIdentity();
   void definir();
   void mostrar();
   double& ij(int i,int j) const;
   Matrix(const Matrix &B);
   Matrix operator +( const Matrix &A);
   Matrix& operator =( const Matrix &A);
   Matrix operator -( Matrix A);
   Matrix operator *( Matrix B);
   double& entry(int i, int j) const;
   Matrix crossproduct(const Matrix &B); //va  funcionar con matrices columna de  4*1
    double dotProduct(const Matrix &B);
   double angle (Matrix  &B);//va  funcionar con matrices columna de  4*1
   double magnitud ();//va  funcionar con matrices columna de  4*1
   Matrix  normalizar ();//va  funcionar con matrices columna de  4*1
   Matrix Transpuesta();


   friend  Matrix operator*(double tt, const Matrix &A);
  ~Matrix();
   double determinante();
};

#endif // MATRIX_H
