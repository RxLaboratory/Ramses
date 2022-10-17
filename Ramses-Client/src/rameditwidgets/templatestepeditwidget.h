#ifndef TEMPLATESTEPEDITWIDGET_H
#define TEMPLATESTEPEDITWIDGET_H

#include <QComboBox>
#include <QFormLayout>

#include "duqf-widgets/autoselectdoublespinbox.h"
#include "objecteditwidget.h"
#include "duqf-widgets/duqfcolorselector.h"
#include "ramtemplatestep.h"

class TemplateStepEditWidget : public ObjectEditWidget
{
    Q_OBJECT

public:
    explicit TemplateStepEditWidget(QWidget *parent = nullptr);
    explicit TemplateStepEditWidget(RamTemplateStep *templateStep, QWidget *parent = nullptr);

    RamTemplateStep *step() const;

protected:
    virtual void reInit(RamObject *o) override;

private slots:
    void updateEstimationSuffix();
    void setType(int t);
    void setColor(QColor c);
    void setEstimationType(int t);
    void setVeryEasy(double e);
    void setEasy(double e);
    void setMedium(double e);
    void setHard(double e);
    void setVeryHard(double e);

private:
    RamTemplateStep *m_step;

    void setupUi();
    void connectEvents();

    // UI Controls
    QComboBox *ui_typeBox;
    QLabel *ui_estimationLabel;
    QWidget *ui_estimationWidget;
    QComboBox *ui_estimationTypeBox;
    QLabel *ui_estimationTypeLabel;
    AutoSelectDoubleSpinBox *ui_veryEasyEdit;
    AutoSelectDoubleSpinBox *ui_easyEdit;
    AutoSelectDoubleSpinBox *ui_mediumEdit;
    AutoSelectDoubleSpinBox *ui_hardEdit;
    AutoSelectDoubleSpinBox *ui_veryHardEdit;
    DuQFColorSelector *ui_colorSelector;
};

#endif // TEMPLATESTEPEDITWIDGET_H
