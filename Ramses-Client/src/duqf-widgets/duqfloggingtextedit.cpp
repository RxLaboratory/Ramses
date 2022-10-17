#include "duqfloggingtextedit.h"

DuQFLoggingTextEdit::DuQFLoggingTextEdit(QWidget *parent): QTextEdit(parent)
{
    //this->setReadOnly(true);
    _loggerObject = nullptr;
    _level = DuQFLog::Debug;

    connect(DuQFLogger::instance(), &DuQFLogger::newLog, this, &DuQFLoggingTextEdit::log);
}

DuQFLoggingTextEdit::DuQFLoggingTextEdit(DuQFLoggerObject *o, QWidget *parent): QTextEdit(parent)
{
    _loggerObject = o;
    _level = DuQFLog::Debug;
    connect(o, &DuQFLoggerObject::newLog, this, &DuQFLoggingTextEdit::log);
}

void DuQFLoggingTextEdit::log(DuQFLog m)
{
    if (m.type() < _level) return;


    const bool atBottom = this->verticalScrollBar()->value() == this->verticalScrollBar()->maximum();
    QTextDocument* doc = this->document();
    QTextCursor cursor(doc);
    cursor.movePosition(QTextCursor::End);
    cursor.beginEditBlock();
    cursor.insertBlock();

    // Time
    QTextCharFormat f;
    f.setForeground( QColor(109,109,109) );
    f.setFontWeight(300);
    cursor.insertText(m.timeString(), f);

    // Title
    QColor c = m.color();
    if (c.isValid()) f.setForeground( c );
    else f.setForeground(QColor(227,227, 227));
    f.setFontWeight(800);
    f.setFontItalic(true);
    cursor.insertText(m.typeString() + " " + tr("says:") + " ", f);

    // Content
    if (m.type() != DuQFLog::Data) f.setForeground(QColor(227,227, 227));
    f.setFontWeight(400);
    f.setFontItalic(false);
    cursor.insertText(m.message(), f);


    cursor.endEditBlock();

    //scroll scrollarea to bottom if it was at bottom when we started
    //(we don't want to force scrolling to bottom if user is looking at a
    //higher position)
    if (atBottom) {

        QScrollBar* bar =  this->verticalScrollBar();
        bar->setValue(bar->maximum());
    }
}

void DuQFLoggingTextEdit::setLevel(const DuQFLog::LogType &level)
{
    _level = level;
}
