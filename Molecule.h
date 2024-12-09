#ifndef Molecule_h__
#define Molecule_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "Elements.h"

#include <vtkIdTypeArray.h>

#include <vtkMolecule.h>

namespace vtk {
using Molecule = vtkMolecule;
}; // namespace vtk

using namespace vtk;

typedef vtkNew<Molecule> ANewMolecule;
typedef vtkSmartPointer<Molecule> AMolecule;

///////////////////////////////////////////////////////////////////////////////////////////////////
// паллиатив какой-то, пока совершенно мятый синтаксически...
//
template <class Arg = vtkIdTypeArray>
struct AddSpecialAtom
{
    explicit AddSpecialAtom(Molecule &mol, 
    double X, double y, double z, 
    vtkIdType subtype, Arg context = Arg())
    : the_mol_(mol)
    , idAtomSpecialToAdd_(mol.GetNumberOfAtoms()) {}

    bool bUpdate_ = true;
    Molecule& the_mol_;
    vtkIdType idAtomSpecialToAdd_;
};
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // !Molecule_h__
