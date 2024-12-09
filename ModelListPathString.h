#ifndef Model_ListPathString_h__
#define Model_ListPathString_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QStringListModel>

class ModelListPathString
    : public QStringListModel
{
  Q_OBJECT
public:
  explicit ModelListPathString();
  ~ModelListPathString() override = default;
};

#endif // !Model_ListPathString_h__
