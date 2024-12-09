#include "ViewFileHistory.h"

ViewFileHistory::ViewFileHistory(QWidget *parent)
    : QSplitter(Qt::Vertical, parent),
      history_(new ViewListPathString(this)),
      properties_(new ViewFileProperties(this))
{
}