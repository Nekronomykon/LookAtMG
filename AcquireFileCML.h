#ifndef AcquireFile_CML_h__
#define AcquireFile_CML_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "FormatFile.h"
#include "AcquireFileBase.h"
#include <vtkCMLMoleculeReader.h>

using AcquireFileCML = vtkCMLMoleculeReader;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFileCML> NewAcquireCML;
typedef vtkSmartPointer<AcquireFileCML> ToAcquireCML;

template <>
struct ReadStructureFile<AcquireFileCML>
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
    vtkNew<AcquireFileCML> read; // TEMP:
    read->SetFileName(path.c_str());
    Molecule *pMol = dest.getMolecule();
    read->SetOutput(pMol);
    dest.clearAll();
    read->Update();

    dest.viewMolecule();
    // pMol->DeepCopy(read->GetOutput());
    return CheckStructureAt(dest);
  }
};
#endif // !AcquireFile_CML_h__
