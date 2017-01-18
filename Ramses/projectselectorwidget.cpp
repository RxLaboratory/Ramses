#include "projectselectorwidget.h"
#include "ui_projectselectorwidget.h"

ProjectSelectorWidget::ProjectSelectorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectSelectorWidget)
{
    ui->setupUi(this);
}

ProjectSelectorWidget::~ProjectSelectorWidget()
{
    delete ui;
}

QComboBox* ProjectSelectorWidget::projectsBox()
{
    return ui->projectsComboBox;
}

QComboBox* ProjectSelectorWidget::stagesBox()
{
    return ui->stagesComboBox;
}
