#ifndef Critical_Point_h__
#define Critical_Point_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkVector.h>
#include <vtkVectorOperators.h>
#include <vtkObjectFactory.h>

#include <complex>

// #include "Atom.h"
// #include "Molecule.h"

// namespace vtk
//{
typedef std::complex<int> CriticalPointType;

static const CriticalPointType::value_type FullRank(3);

enum /* Sum-Of-Signs */
{
  FullRankTypeMaximum = -3,
  FullRankTypeSaddleB = -1,
  FullRankTypeSaddleR = +1,
  FullRankTypeMinimum = +3
};

// signature of a Cage Critical Point (CCP, local minimum)
static const CriticalPointType cpTypeMinimum(FullRank, FullRankTypeMinimum);

// signature of a Ring Critical Point (RCP, saddle)
static const CriticalPointType cpTypeSaddleR(FullRank, FullRankTypeSaddleR);

// signature of a Bond Critical Point (BCP, reversed saddle point)
static const CriticalPointType cpTypeSaddleB(FullRank, FullRankTypeSaddleB);

// signature of a Atom Critical Point (ACP, local maximum)
static const CriticalPointType cpTypeMaximum(FullRank, FullRankTypeMaximum);

struct CriticalPoint // according to the AIMAll output
// : public Atom
{
public:                    /* data */
  CriticalPointType type_; // Type of the CP
  vtkVector3d xyz_;
  double Rho_ = 0.0;                      // One-electron density, Rho
  vtkVector3d GradRho_;                   // The Rho gradient; ~> 0
  vtkVector3d HessRhoEigVals_;            // Eigenvalues of the Rho Hessian, Ascending Order
  vtkVector3d HessRhoEigVector_[3];       // = Eigenvectors of the Hessian of Rho // vtkMatrix3x3d ???
  double DelSqRho_ = 0.0;                 // DelSqRho = LapRho = trace of the Rho Hessian
  double Ellipticity_ = 0.0;              // in its extended meaning
  double V = 0.0;                         // = Virial Field = Potential Energy Density = Trace of Stress Tensor
  double G = 0.0;                         // = Lagrangian Form of Kinetic Energy Density
  double K = 0.0;                         // = Hamiltonian Form of Kinetic Energy Density
  double L = 0.0;                         // = K - G = Lagrangian Density = (-1/4)DelSqRho
  double Vnuc = 0.0;                      // = Electrostatic Potential from Nuclei
  double Ven = 0.0;                       // = Electron-nuclear attractive contribution to Virial Field V
  double Vrep = 0.0;                      // = V - Ven = Repulsive contribution to Virial Field V
  double DelSqV = 0.0;                    // = Laplacian of V
  double DelSqVen = 0.0;                  // = Laplacian of Ven
  double DelSqVrep = 0.0;                 // = Laplacian of Vrep
  double DelSqG = 0.0;                    // = Laplacian of G
  double DelSqK = 0.0;                    // = Laplacian of K
  vtkVector3d StressEigVals_;             // = Eigenvalues of the Stress Tensor, Ascending Order
  vtkVector3d StressEigVector_[3];        // = Eigenvectors of the Stress Tensor // vtkMatrix3x3d ???

  //
  // explicit CriticalPoint(Molecule & /* mol */, vtkIdType /* id */ = -1U);
  // ~CriticalPoint();
};

//}; // namespace vtk

#endif // !Critical_Point_h__
