#ifndef Bar_Lines_h__
#define Bar_Lines_h__

#include <QSize>
#include <QPaintEvent>
#include <QWidget>

class EditMarkLines;

class BarLines : public QWidget
{
    Q_OBJECT
public:
    explicit BarLines(EditMarkLines *parent);

public:
    QSize sizeHint() const override;

    long getActiveID() const { return idActiveString_; }
    long resetActiveID(long id)
    {
        std::swap(idActiveString_, id);
        return id;
    }

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int nStringHeight_ = -1;
    EditMarkLines *ptrHost_ = nullptr;
    long idActiveString_ = -1L;
};

#endif // Bar_Lines_h__
