#include "duqftextedit.h"

DuQFTextEdit::DuQFTextEdit(const QString &text, QWidget *parent):
    QTextEdit(text, parent)
{
    setupUi();
    connectEvents();
}

DuQFTextEdit::DuQFTextEdit(QWidget *parent):
    QTextEdit(parent)
{
    setupUi();
    connectEvents();
}

void DuQFTextEdit::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    finishEditing();
    QTextEdit::focusOutEvent(event);
}

void DuQFTextEdit::keyPressEvent(QKeyEvent *event)
{
    int key = event->key();
    Qt::KeyboardModifiers mods = event->modifiers();
    if(key == Qt::Key_Enter && m_captureEnterKey)
    {
        emit enterPressed();
        finishEditing();
        return;
    }
    else if (key == Qt::Key_F2)
    {
        showMarkdown();
    }
    else if (key == Qt::Key_V && mods.testFlag(Qt::ControlModifier) && mods.testFlag(Qt::ShiftModifier)  )
    {
        pasteNoFormatting();
    }
    QTextEdit::keyPressEvent(event);
}

void DuQFTextEdit::finishEditing()
{
    if (m_freezeValidation) return;
    // Remember cursor position to reset it
    if (m_changed) {
        QTextCursor cursor = this->textCursor();

#if (QT_VERSION >= QT_VERSION_CHECK(5, 15, 0))
    if (m_useMarkdown) this->setMarkdown( this->toMarkdown() );
#endif

        this->setTextCursor(cursor);
        emit editingFinished();
    }
    m_changed = false;

}

void DuQFTextEdit::setTextChanged()
{
    m_changed = true;
}

void DuQFTextEdit::contextMenuHidden()
{
    m_freezeValidation = false;
    if (!this->hasFocus()) finishEditing();
}

void DuQFTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    m_freezeValidation = true;
    ui_contextMenu->popup(event->globalPos());
}

void DuQFTextEdit::setupUi()
{
    ui_contextMenu = new QMenu(this);

    ui_undo = new QAction("Undo", this);
    ui_undo->setShortcut(QKeySequence(QKeySequence::Undo));
    ui_undo->setIcon(QIcon(":/icons/undo"));
    ui_contextMenu->addAction(ui_undo);

    ui_redo = new QAction("Redo", this);
    ui_redo->setShortcut(QKeySequence(QKeySequence::Redo));
    ui_redo->setIcon(QIcon(":/icons/redo"));
    ui_contextMenu->addAction(ui_redo);

    ui_contextMenu->addSeparator();

    ui_copy = new QAction("Copy", this);
    ui_copy->setShortcut(QKeySequence(QKeySequence::Copy));
    ui_copy->setIcon(QIcon(":/icons/copy"));
    ui_contextMenu->addAction(ui_copy);

    ui_cut = new QAction("Cut", this);
    ui_cut->setShortcut(QKeySequence(QKeySequence::Cut));
    ui_cut->setIcon(QIcon(":/icons/cut"));
    ui_contextMenu->addAction(ui_cut);

    ui_paste = new QAction("Paste", this);
    ui_paste->setShortcut(QKeySequence(QKeySequence::Paste));
    ui_paste->setIcon(QIcon(":/icons/paste"));
    ui_contextMenu->addAction(ui_paste);

    ui_pasteNoFormatting = new QAction("Paste without formatting", this);
    ui_pasteNoFormatting->setShortcut(QKeySequence("Ctrl+Shift+V"));
    ui_pasteNoFormatting->setIcon(QIcon(":/icons/paste"));
    ui_contextMenu->addAction(ui_pasteNoFormatting);

    ui_contextMenu->addSeparator();

    ui_selectAll = new QAction("Select all", this);
    ui_selectAll->setShortcut(QKeySequence(QKeySequence::SelectAll));
    ui_selectAll->setIcon(QIcon(":/icons/text-file"));
    ui_contextMenu->addAction(ui_selectAll);

    ui_contextMenu->addSeparator();

    ui_showSource = new QAction("Show source", this);
    ui_showSource->setShortcut(QKeySequence(Qt::Key_F2));
    ui_showSource->setIcon(QIcon(":/icons/code"));
    ui_contextMenu->addAction(ui_showSource);

    ui_validate = new QAction("Validate", this);
    ui_validate->setShortcut(QKeySequence(Qt::Key_Enter));
    ui_validate->setIcon(QIcon(":/icons/ok"));
    ui_contextMenu->addAction(ui_validate);
}

void DuQFTextEdit::setUseMarkdown(bool use)
{
    m_useMarkdown = use;
}

void DuQFTextEdit::connectEvents()
{
    connect(this, SIGNAL(textChanged()), this, SLOT(setTextChanged()));

    connect(ui_undo, SIGNAL(triggered(bool)), this, SLOT(undo()));
    connect(ui_redo, SIGNAL(triggered(bool)), this, SLOT(redo()));
    connect(ui_copy, SIGNAL(triggered(bool)), this, SLOT(copy()));
    connect(ui_cut, SIGNAL(triggered(bool)), this, SLOT(cut()));
    connect(ui_paste, SIGNAL(triggered(bool)), this, SLOT(paste()));
    connect(ui_pasteNoFormatting, SIGNAL(triggered(bool)), this, SLOT(pasteNoFormatting()));
    connect(ui_selectAll, SIGNAL(triggered(bool)), this, SLOT(selectAll()));
    connect(ui_showSource, SIGNAL(triggered(bool)), this, SLOT(showMarkdown()));

    connect(ui_contextMenu, SIGNAL(aboutToHide()), this, SLOT(contextMenuHidden()));
}

bool DuQFTextEdit::captureEnterKey() const
{
    return m_captureEnterKey;
}

void DuQFTextEdit::setCaptureEnterKey(bool newCaptureEnterKey)
{
    m_captureEnterKey = newCaptureEnterKey;
}

void DuQFTextEdit::showMarkdown()
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    return;
#else
    if (!m_useMarkdown) return;
    showRichText();
    QString source = this->toMarkdown();
    this->setCurrentCharFormat(QTextCharFormat());
    this->setPlainText( source );
#endif
}

void DuQFTextEdit::showRichText()
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 15, 0))
    return;
#else
    if (!m_useMarkdown) return;
    this->setMarkdown( this->toMarkdown() );
#endif
}

void DuQFTextEdit::pasteNoFormatting()
{
    bool acceptRichText = this->acceptRichText();
    this->setAcceptRichText(false);
    paste();
    this->setAcceptRichText(acceptRichText);
}
