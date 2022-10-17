#ifndef SEQUENCEEDITWIDGET_H
#define SEQUENCEEDITWIDGET_H

#include "objecteditwidget.h"
#include "objectlistwidget.h"
#include "duqf-widgets/duqfcolorselector.h"
#include "ramsequence.h"

/**
 * @brief The SequenceEditWidget class is used to edit Sequences and can be shown either in the main UI or in the Dock
 */
class SequenceEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit SequenceEditWidget(QWidget *parent = nullptr);
    explicit SequenceEditWidget(RamSequence *s, QWidget *parent = nullptr);

    RamSequence *sequence() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void setColor(QColor c);
    void createShot();

private:
    RamSequence *m_sequence;

    void setupUi();
    void connectEvents();

    DuQFColorSelector *ui_colorSelector;

    ObjectListWidget *ui_shotsList;
};

#endif // SEQUENCEEDITWIDGET_H
