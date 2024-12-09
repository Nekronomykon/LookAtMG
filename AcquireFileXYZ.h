#ifndef AcquireFile_XYZ_h__
#define AcquireFile_XYZ_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "AcquireFileBase.h"

// examples
#include <vtkXYZMolReader.h>
#include <vtkXYZMolReader2.h>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

// #include <MakeBonds.h> // bottom of file...

/*=========================================================================
  Program:   Visualization Toolkit
  Module:    AcquireFileXYZ.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
/**
 * @class   AcquireFileXYZ
 * @brief   read Molecular Data files
 *
 * AcquireFileXYZ is a source object that reads Molecule files
 * The reader will detect multiple timesteps in an XYZ molecule file.
 *
 * @par Thanks:
 * Dr. Jean M. Favre who developed and contributed this class
 */
#include <istream> // for std::istream
#include <vector>  // for std::vector

// namespace vtk
// {

class /* VTKIOCHEMISTRY_EXPORT*/ AcquireFileXYZ
    : public AcquireFileBase
{
public:
  static AcquireFileXYZ *New();
  vtkTypeMacro(AcquireFileXYZ, AcquireFileBase);
  void PrintSelf(ostream &os, vtkIndent indent) override;

protected:
  explicit AcquireFileXYZ();
  ~AcquireFileXYZ() override = default;

  // ----------------------------------------------------------------------------------------------------
  // To be overriden to read information stored in the (file) stream
  // ----------------------------------------------------------------------------------------------------
  // int RequestInformation(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadSizesFrom(InputFile & /*inp*/) override;
  // ----------------------------------------------------------------------------------------------------
  // int RequestData(vtkInformation *, vtkInformationVector **, vtkInformationVector *) override;
  int ReadDataFrom(InputFile & /*inp*/, Molecule * /* pMol */) override;
  // int OnReadDataComplete(Molecule * /* ptrMol */) override;
  // ----------------------------------------------------------------------------------------------------

private:
  AcquireFileXYZ(const AcquireFileXYZ &) = delete;
  void operator=(const AcquireFileXYZ &) = delete;
};

// }; // namespace vtk

using ANewXYZFileParse = vtkNew<AcquireFileXYZ>;
using AFileParseXYZ = vtkSmartPointer<AcquireFileXYZ>;

template <>
struct ReadStructureFile<AcquireFileXYZ>
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
    ANewXYZFileParse read;
    read->resetPath(path);
    ANewMolecule new_mol;
    read->SetOutput(new_mol);
    read->Update();
    dest.getMolecule()->DeepCopy(new_mol);
    dest.resetTitle(read->getTitle());
    if (!dest.hasTitle())
      dest.pathToTitle();
    return CheckStructureAt(dest);
  }
};

template <>
struct ReadStructureFile<vtkXYZMolReader>
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
    vtkNew<vtkXYZMolReader> read; // TEMP:
    read->SetFileName(path.c_str());
    Molecule *pMol = dest.getMolecule();
    read->SetOutput(pMol);
    dest.clearAll();
    read->Update();
    dest.resetPath(path);
    dest.pathToTitle();
    dest.viewMolecule();
    // pMol->DeepCopy(read->GetOutput());
    return CheckStructureAt(dest);
  }
};

template <>
struct ReadStructureFile<vtkXYZMolReader2>
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
    vtkNew<vtkXYZMolReader2> read; // TEMP:
    read->SetFileName(path.c_str());
    Molecule *pMol = dest.getMolecule();
    read->SetOutput(pMol);
    dest.clearAll();
    read->Update();
    dest.resetPath(path);
    dest.pathToTitle();
    dest.viewMolecule();
    // pMol->DeepCopy(read->GetOutput());
    return CheckStructureAt(dest);
  }
};

#endif // !AcquireFile_XYZ_h__
