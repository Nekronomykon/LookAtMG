#ifndef View_List_PathString_h__
#define View_List_PathString_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ModelListPathString.h"

#include <QListView>

class ViewListPathString
    : public QListView
{
    Q_OBJECT

public:
    explicit ViewListPathString(QWidget * /*parent*/ = nullptr);
    ~ViewListPathString() override = default;

private:
    // QPointer<ModelListPathString> model_;
};

#endif // View_List_PathString_h__
