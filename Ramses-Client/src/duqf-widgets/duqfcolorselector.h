#ifndef DUQFCOLORSELECTOR_H
#define DUQFCOLORSELECTOR_H

#include <QLineEdit>
#include <QToolButton>
#include <QHBoxLayout>
#include <QColorDialog>

class DuQFColorSelector : public QWidget
{
    Q_OBJECT
public:
    explicit DuQFColorSelector(QWidget *parent = nullptr);


    const QColor &color() const;
    void setColor(const QColor &newColor);

signals:
   void colorChanged(QColor);

private slots:
   void updateColorEditStyle();
   void selectColor();

private:
    void setupUi();
    void connectEvents();

    QColor m_color;

    QLineEdit *ui_colorEdit;
    QToolButton *ui_colorButton;
};

#endif // DUQFCOLORSELECTOR_H
