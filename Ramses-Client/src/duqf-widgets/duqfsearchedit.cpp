#include "duqfsearchedit.h"

DuQFSearchEdit::DuQFSearchEdit(QWidget *parent) : QWidget(parent)
{
    setupUi();
    connectEvents();
}

void DuQFSearchEdit::hideSearchButton()
{
    m_searchButton->hide();
}

void DuQFSearchEdit::search()
{
    emit changed(m_searchEdit->text());
}

void DuQFSearchEdit::setupUi()
{
    QHBoxLayout *searchLayout = new QHBoxLayout(this);
    searchLayout->setSpacing(0);
    searchLayout->setContentsMargins(0,0,0,0);

    m_searchEdit = new QLineEdit(this);
    m_searchEdit->setClearButtonEnabled(true);
    m_searchEdit->setPlaceholderText("Search...");
    searchLayout->addWidget(m_searchEdit);

    m_searchButton = new QToolButton(this);
    m_searchButton->setIcon(QIcon(":/icons/search"));
    m_searchButton->setIconSize(QSize(10,10));
    searchLayout->addWidget(m_searchButton);
}

void DuQFSearchEdit::connectEvents()
{
    connect(m_searchEdit, &QLineEdit::textChanged, this, &DuQFSearchEdit::changing);
    connect(m_searchEdit, &QLineEdit::editingFinished, this, &DuQFSearchEdit::search);
    connect(m_searchButton, &QToolButton::clicked, this, &DuQFSearchEdit::search);
}
