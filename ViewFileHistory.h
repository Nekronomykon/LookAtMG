#ifndef View_FileHistory_h__
#define View_FileHistory_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include "ViewListPathString.h"
#include "ViewFileProperties.h"

#include <QSplitter>

#include <QPointer>

class ViewFileHistory
    : public QSplitter
{
    Q_OBJECT
public:
    explicit ViewFileHistory(QWidget * /*parent*/ = nullptr);
    ~ViewFileHistory() override = default;

private:
    QPointer<ViewListPathString> history_;
    QPointer<ViewFileProperties> properties_;
};

#endif // !View_FileHistory_h__
