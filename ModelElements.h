#ifndef MODELELEMENTS_H
#define MODELELEMENTS_H

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <vtkNew.h>
#include <vtkType.h>

#include <vtkLookupTable.h>
#include <vtkFloatArray.h>

#include "Elements.h"

#include <QAbstractTableModel>

class ModelElements
  : public QAbstractTableModel
{
  Q_OBJECT

protected:
    enum {
    NumberOfElements = 135
    , NumberOfPeriods = 8
    , NumberOfGroups = 18
    , NumberOfSpecialSeq = 3 // Ln --- lantanides, Aq --- actinides, Sq --- superactinides
    , LengthSpecialSec = 15
    , NumberOfTableRows = NumberOfPeriods + NumberOfSpecialSeq
    //, DummyElement = VTK_UNSIGNED_SHORT_MAX
  };

  static const unsigned short NobleGases[NumberOfPeriods];
  static const unsigned short InGroup[];
  static const vtkIdType IdCore[]; // real element code is NobleGases[ IdCore[i] ];
  static const vtkIdType TableForm[];
  static const vtkIdType NumTable;
  static const float DefaultColors[][3];
public:
  explicit ModelElements(QObject* /*parent*/ = nullptr);
  ~ModelElements()override = default;

  int columnCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
  {
    return NumberOfGroups;
  }
  int    rowCount(const QModelIndex &/*parent*/ = QModelIndex()) const override
  {
    return (NumTable + NumberOfGroups - 1) / NumberOfGroups;
  }
  QVariant data(const QModelIndex & /*index*/, int /*role*/) const override;

  Qt::ItemFlags flags(const QModelIndex & /*index*/) const override;

//------------------------------------------------------------------------------
void ExportDefaultColors(vtkLookupTable* /*colors*/);

//------------------------------------------------------------------------------

private:
  // Elements elements_;

};

#endif // MODELELEMENTS_H
