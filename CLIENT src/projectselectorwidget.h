#ifndef PROJECTSELECTORWIDGET_H
#define PROJECTSELECTORWIDGET_H

#include <QWidget>
#include <QComboBox>

namespace Ui {
class ProjectSelectorWidget;
}

class ProjectSelectorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectSelectorWidget(QWidget *parent = 0);
    QComboBox* projectsBox();
    QComboBox* stagesBox();

    ~ProjectSelectorWidget();

private:
    Ui::ProjectSelectorWidget *ui;
};

#endif // PROJECTSELECTORWIDGET_H
