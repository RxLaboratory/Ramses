#include "helpdialog.h"

HelpDialog::HelpDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);

    toolBar = new QToolBar(this);
    toolBar->addAction(actionHelp);
    toolBar->addAction(actionDebug);
    toolBar->addAction(actionAbout);

    //Add window buttons
    SeparatorWidget *sW = new SeparatorWidget();
    QPushButton *quitButton = new QPushButton(QIcon(":/icons/close"),"");
    connect(quitButton,SIGNAL(clicked()),this,SLOT(reject()));
    toolBar->addWidget(sW);
    toolBar->addWidget(quitButton);

    this->layout()->setMenuBar(toolBar);

    this->setWindowFlags(Qt::FramelessWindowHint);

    toolBarClicked = false;
    toolBar->installEventFilter(this);
}

void HelpDialog::hideEvent(QHideEvent *e)
{
    // 'false' means hidden..
    emit visibilityChanged(false);
    QDialog::hideEvent(e);
}

void HelpDialog::showEvent(QShowEvent *e)
{
    // 'true' means visible..
    emit visibilityChanged(true);
    QDialog::showEvent(e);
}

bool HelpDialog::eventFilter(QObject *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = (QMouseEvent*)event;
      if (mouseEvent->button() == Qt::LeftButton)
      {
        toolBarClicked = true;
        dragPosition = mouseEvent->globalPos() - this->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = (QMouseEvent*)event;
    if (mouseEvent->buttons() & Qt::LeftButton && toolBarClicked)
    {
        if (this->isMaximized()) this->showNormal();
      this->move(mouseEvent->globalPos() - dragPosition);
      event->accept();
    }
    return true;
  }
  else if (event->type() == QEvent::MouseButtonRelease)
  {
      toolBarClicked = false;
      return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}
