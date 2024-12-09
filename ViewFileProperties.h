#ifndef View_FileProperties_h__
#define View_FileProperties_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QTabWidget>

class ViewFileProperties
    : public QTabWidget
{
    Q_OBJECT
public:
    ViewFileProperties(QWidget */*parent*/ = nullptr);
    ~ViewFileProperties() override = default;

};
#endif // View_FileProperties_h__