#ifndef NamedColors_h__
#define NamedColors_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QComboBox>

#include <QString>
#include <QStringList>

#include <vtkNew.h>
#include <vtkSmartPointer.h>

#include <vtkColor.h>
#include <vtkStdString.h>
#include <vtkNamedColors.h>

class NamedColors
    : public QComboBox
{
  Q_OBJECT
public:
  explicit NamedColors(QWidget * /*parent*/ = nullptr);

private:
  static QStringList ColorNames();
};

#endif // !NamedColors_h__
