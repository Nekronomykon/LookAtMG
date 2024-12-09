#ifndef Edit_Mark_Lines_h__
#define Edit_Mark_Lines_h__

#ifdef _MSC_VER
#pragma once
#else  // !_MSC_VER
#endif //  _MSC_VER

#include <QPlainTextEdit>
#include <QWidget>
#include <QTextDocument>

#include "BarLines.h"
#include <QPointer>

class EditMarkLines : public QPlainTextEdit
{
    Q_OBJECT
public:
    explicit EditMarkLines(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();
    QTextDocument *operator->() const { return this->document(); }
    operator QTextDocument *() const { return this->document(); }
    bool isModified() const { return this->document()->isModified(); }
    void setModified(bool bSet = true) const { return this->document()->setModified(bSet); }
    //
    bool loadPath(const QString & /*path*/);

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int /*newBlockCount*/);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QPointer<BarLines> barLines_;
};

#endif // Edit_Mark_Lines_h__
