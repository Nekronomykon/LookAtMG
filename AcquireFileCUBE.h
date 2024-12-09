#ifndef AcquireFile_CUBE_h__
#define AcquireFile_CUBE_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "FormatFile.h"
#include "AcquireFileBase.h"
#include <vtkGaussianCubeReader.h>
#include <vtkGaussianCubeReader2.h>

using AcquireFileCUBE = vtkGaussianCubeReader2;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileCUBE> NewAcquireCUBE;
typedef vtkSmartPointer<AcquireFileCUBE> ToAcquireCUBE;

template <>
struct ReadStructureFile<vtkGaussianCubeReader>
{
  template <class Dest>
  static inline bool CheckStructureAt(const Dest &dest)
  {
    return bool(dest.getMolecule()->GetNumberOfAtoms() > 0);
  }
  // ==========================================================================================
  template <class Dest>
  static bool FromPathTo(Path path, Dest &dest)
  {
    vtkNew<vtkGaussianCubeReader> read;
    read->SetFileName(path.c_str());
    read->SetOutput(dest.getMolecule());
    read->Update();
    dest.resetPath(path);
    dest.pathToTitle();
    return bool(dest.getMolecule()->GetNumberOfAtoms() > 0);
  }
};

template <>
struct ReadStructureFile<vtkGaussianCubeReader2>
{
  template <class Dest>
  static inline bool CheckStructureAt(const Dest &dest)
  {
    return bool(dest.getMolecule()->GetNumberOfAtoms() > 0);
  }
  // ==========================================================================================
  template <class Dest>
  static bool FromPathTo(Path path, Dest &dest)
  {
    vtkNew<vtkGaussianCubeReader2> read;
    read->SetFileName(path.c_str());
    read->SetOutput(dest.getMolecule());
    read->Update();
    dest.resetPath(path);
    dest.pathToTitle();
    return bool(dest.getMolecule()->GetNumberOfAtoms() > 0);
  }
};

#endif // !AcquireFile_CUBE_h__
