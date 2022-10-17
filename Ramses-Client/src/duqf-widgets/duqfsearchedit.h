#ifndef DUQFSEARCHEDIT_H
#define DUQFSEARCHEDIT_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

class DuQFSearchEdit : public QWidget
{
    Q_OBJECT
public:
    explicit DuQFSearchEdit(QWidget *parent = nullptr);
    void hideSearchButton();

signals:
    void changing(QString);
    void changed(QString);
private slots:
    void search();
private:
    void setupUi();
    void connectEvents();
    QLineEdit *m_searchEdit;
    QToolButton *m_searchButton;

};

#endif // DUQDSEARCHEDIT_H
