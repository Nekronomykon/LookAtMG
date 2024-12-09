#ifndef AcquireFile_PDB_h__
#define AcquireFile_PDB_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

// #include "FormatFile.h"
#include "AcquireFileBase.h"
#include <vtkPDBReader.h>

using AcquireFilePDB = vtkPDBReader;

#include <vtkNew.h>
#include <vtkSmartPointer.h>

typedef vtkNew<AcquireFilePDB> NewAcquirePDB;
typedef vtkSmartPointer<AcquireFilePDB> ToAcquirePDB;

template <>
struct ReadStructureFile<vtkPDBReader>
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

template <class Host>
bool ParseFilePDBTo(Path a_path, Host &host)
{
  NewAcquirePDB read;
  read->SetFileName(a_path.c_str());
  read->SetOutput(host.getMolecule());
  read->Update();
  host.resetTitle( /* read->getTitle() */ );
  return bool(host.getMolecule()->GetNumberOfAtoms() > 0);
}

#endif // !AcquireFile_PDB_h__
