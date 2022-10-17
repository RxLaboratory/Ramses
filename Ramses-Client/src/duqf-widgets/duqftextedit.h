#ifndef DUQFTEXTEDIT_H
#define DUQFTEXTEDIT_H

#include <QTextEdit>

/**
 * @brief The DuQFTextEdit class is a standard QTextEdit but with new signals:
 * editingFinished (using focusOut)
 * enterPressed (the keypad enter) if set to capture enter key
 */
class DuQFTextEdit : public QTextEdit
{
    Q_OBJECT
public:
    DuQFTextEdit(const QString &text, QWidget *parent = nullptr);
    DuQFTextEdit(QWidget *parent = nullptr);
    bool captureEnterKey() const;
    void setCaptureEnterKey(bool newCaptureEnterKey);
    void setUseMarkdown(bool use);

public slots:
    void showMarkdown();
    void showRichText();
    void pasteNoFormatting();
signals:
    void editingFinished();
    void enterPressed();
protected:
    void focusOutEvent(QFocusEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void contextMenuEvent(QContextMenuEvent *event) override;
private slots:
    void finishEditing();
    void setTextChanged();
    void contextMenuHidden();
private:
    void connectEvents();
    void setupUi();
    QMenu *ui_contextMenu;
    QAction *ui_undo;
    QAction *ui_redo;
    QAction *ui_copy;
    QAction *ui_cut;
    QAction *ui_paste;
    QAction *ui_pasteNoFormatting;
    QAction *ui_selectAll;
    QAction *ui_showSource;
    QAction *ui_validate;

    bool m_changed = false;
    bool m_captureEnterKey = true;
    bool m_freezeValidation = false;
    bool m_useMarkdown = true;
};

#endif // DUQFTEXTEDIT_H
