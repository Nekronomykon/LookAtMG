#include "NamedColors.h"

#include <vtkNamedColors.h>

#include <QMessageBox>

#include <string>
#include <sstream>

QStringList NamedColors::ColorNames()
{
  static QStringList allNames;

  if (allNames.isEmpty())
  {
    vtkNew<vtkNamedColors> colorsNamed;
    std::string one_name;
    std::istringstream inpNames(colorsNamed->GetSynonyms());
    std::getline(inpNames, one_name);
    do
    {
      if (one_name.empty())
        continue;
      std::string one_syn;
      std::getline(inpNames, one_syn);
      while (!one_syn.empty())
      {
        one_name += ';';
        one_name += ' ';
        one_name += one_syn;
        if (!std::getline(inpNames, one_syn))
          break;
      }
      // one_syn.clear();
      allNames << one_name.c_str(); /*name; syn0...*/
      // one_name.clear();
    } while (std::getline(inpNames, one_name));

    //
    /*
    vtkNew<vtkStringArray> col_names;
    colorsNamed->GetColorNames(col_names);
    size_t nNames = col_names->GetSize();
    for (size_t j = 0; j < nNames; ++j)
    {
      vtkStdString a_name(col_names->GetValue(j));
      if (a_name.empty())
        continue;
      QString sName(tr(a_name.c_str()));
      if (!sName.isEmpty())
        allNames << sName;
    }*/
  }
  return allNames;
}

NamedColors::NamedColors(QWidget *parent)
    : QComboBox(parent)
{
  this->addItem(tr("-[:user color:]-")); // placeholder
  this->addItems(ColorNames());
}
