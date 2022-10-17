#ifndef DUQFELIDEDLABEL_H
#define DUQFELIDEDLABEL_H

#include <QLabel>
#include <QPainter>
#include <QStyleOptionFrame>

class DuQFElidedLabel : public QLabel
{
public:
    DuQFElidedLabel(QWidget *parent = nullptr);

    Qt::TextElideMode elideMode() const;
    void setElideMode(Qt::TextElideMode newElideMode);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    virtual void paintEvent(QPaintEvent *event) override;

private:
   Qt::TextElideMode m_elideMode = Qt::ElideLeft;
};

#endif // DUQFELIDEDLABEL_H
