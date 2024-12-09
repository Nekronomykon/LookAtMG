#ifndef View_FileContent_h__
#define View_FileContent_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ModelFileContent.h"

#include <QListView>

class ViewFileContent
    : public QListView
{
    Q_OBJECT
public:
    explicit ViewFileContent(QWidget * /*parent*/ = nullptr);
    ~ViewFileContent() override = default;

private:
    // QPointer<ModelFileContent> model_;
};

#endif // View_FileContent_h__
