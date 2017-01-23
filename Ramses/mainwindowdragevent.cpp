#include "mainwindowdragevent.h"
#include <QtDebug>

mainWindowDragEvent::mainWindowDragEvent(QObject *parent) : QObject(parent)
{

}

bool mainWindowDragEvent::eventFilter(QWidget *obj, QEvent *event)
{
  if (event->type() == QEvent::MouseButtonPress)
  {
      QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
      if (mouseEvent->button() == Qt::LeftButton)
      {
          qDebug() << "Mouse pressed";
        dragPosition = mouseEvent->globalPos() - obj->frameGeometry().topLeft();
        event->accept();
      }
      return true;
  }
  else if (event->type() == QEvent::MouseMove)
  {
    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
    qDebug() << "Mouse drag";
    if (mouseEvent->buttons() & Qt::LeftButton)
    {
        qDebug() << "Mouse drag accepted";
      obj->move(mouseEvent->globalPos() - dragPosition);
      event->accept();
    }
    return true;
  }
  else
  {
      // standard event processing
      return QObject::eventFilter(obj, event);
  }
}