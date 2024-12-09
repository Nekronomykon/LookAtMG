#ifndef Model_FileContent_h__
#define Model_FileContent_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QStringListModel>

class ModelFileContent
: public QStringListModel
{
  Q_OBJECT
public:
    explicit ModelFileContent();
    ~ModelFileContent() override = default;
};

#endif // Model_FileContent_h__
