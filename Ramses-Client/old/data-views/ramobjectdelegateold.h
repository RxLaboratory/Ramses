#ifndef RAMOBJECTDELEGATEOLD_H
#define RAMOBJECTDELEGATEOLD_H

#include <QStyledItemDelegate>
#include <QPainter>
#include <QPainterPath>
#include <QStringBuilder>

#include "ramuser.h"

struct PaintParametersOld
{
    QColor bgColor;
    QColor textColor;
    QColor detailsColor;
    QRect bgRect;
    QRect iconRect;
    QRect detailsRect;
    QRect titleRect;
};

/**
 * @brief The RamObjectDelegate class is the main delegate used to paint RamObject in almost all the lists in Ramses.
 */
class RamObjectDelegateOld : public QStyledItemDelegate
{
    Q_OBJECT
public:
    RamObjectDelegateOld(QObject *parent = nullptr);

    void paint(QPainter *painter, const QStyleOptionViewItem &option,
               const QModelIndex &index) const override;
    virtual QSize sizeHint(const QStyleOptionViewItem &option,
                   const QModelIndex &index) const override;

    void setEditable(bool editable);
    void setEditRole(RamUser::UserRole role);

    void setComboBoxMode(bool comboBoxMode);

public slots:
    void setTimeTracking(bool newTimeTracking);
    void setCompletionRatio(bool newCompletionRatio);
    void showDetails(bool s);

signals:
    void edited(RamObject*);

protected:
    bool editorEvent(QEvent *event, QAbstractItemModel *model, const QStyleOptionViewItem &option, const QModelIndex &index) override;

    // drawing specific items
    void drawMore(QPainter *painter, QRect rect, QPen pen) const;
    void drawButton(QPainter *painter, QRect rect, QPixmap icon, bool hover = false) const;

    // Style
    QColor m_abyss;
    QColor m_dark;
    QColor m_medium;
    QColor m_lessLight;
    QColor m_light;
    QColor m_transparent;
    QFont m_textFont;
    QFont m_detailsFont;
    int m_padding;

    // Buttons
    QPixmap m_editIcon;
    QPixmap m_historyIcon;
    QPixmap m_folderIcon;
    QPixmap m_historyDarkIcon;
    QPixmap m_folderDarkIcon;

    // Icons stored as pixmap here for performance
    QMap<QString, QPixmap> m_icons;

    // Buttons hover
    QModelIndex m_historyButtonHover = QModelIndex();
    QModelIndex m_folderButtonHover = QModelIndex();

private:
    // UTILS //
    PaintParametersOld getPaintParameters(const QStyleOptionViewItem &option, RamObject *obj) const;
    void paintBG(QPainter *painter, PaintParametersOld *params) const;
    void paintTitle(RamObject *obj, QPainter *painter, PaintParametersOld *params) const;
    void paintTitle(QString title, QPainter *painter, PaintParametersOld *params) const;
    void paintButtons(RamObject *obj, QPainter *painter, PaintParametersOld *params, const QModelIndex &index) const;
    void paintDetails(QString details, QPainter *painter, PaintParametersOld *params) const;
    void paintDetails(RamObject *obj, QPainter *painter, PaintParametersOld *params) const;


    // Settings
    bool m_editable = false;
    bool m_comboBox = false;
    RamUser::UserRole m_editRole = RamUser::Admin;
    bool m_timeTracking = true;
    bool m_completionRatio = true;
    bool m_details = false;

    // Events
    bool m_historyButtonPressed = false;
    bool m_cellPressed = false;
    bool m_folderButtonPressed = false;
};

#endif // RAMOBJECTDELEGATEOLD_H
