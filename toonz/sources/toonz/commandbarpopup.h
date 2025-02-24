#pragma once

#ifndef COMMANDBARPOPUP_H
#define COMMANDBARPOPUP_H

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QAction>
#include <QCheckBox>

#include "toonzqt/dvdialog.h"
#include "tfilepath.h"

class QXmlStreamReader;
class QXmlStreamWriter;

//=============================================================================
// CommandBarTree
//-----------------------------------------------------------------------------

class CommandBarTree final : public QTreeWidget {
  Q_OBJECT

  void loadMenuTree(const TFilePath& fp);
  void loadMenuRecursive(QXmlStreamReader& reader, QTreeWidgetItem* parentItem);
  void saveMenuRecursive(QXmlStreamWriter& writer, QTreeWidgetItem* parentItem);

public:
  CommandBarTree(TFilePath& path, TFilePath& defaultPath, QWidget* parent = 0);
  void saveMenuTree(TFilePath& path);

protected:
  bool dropMimeData(QTreeWidgetItem* parent, int index, const QMimeData* data,
                    Qt::DropAction action) override;
  QStringList mimeTypes() const override;
  void contextMenuEvent(QContextMenuEvent* event) override;
protected slots:
  void removeItem();
};

//=============================================================================
// CommandBarListTree
//-----------------------------------------------------------------------------

class CommandBarListTree final : public QTreeWidget {
  Q_OBJECT

  void addFolder(const QString& title, int commandType,
                 QTreeWidgetItem* parentFolder = 0);

public:
  CommandBarListTree(QWidget* parent = 0);

  void searchItems(const QString& searchWord = QString());

public slots:
  void onItemClicked(const QModelIndex&);

private:
  void displayAll(QTreeWidgetItem* item);
  void hideAll(QTreeWidgetItem* item);

protected:
  void mousePressEvent(QMouseEvent*) override;
};

//=============================================================================
// CommandBarPopup
//-----------------------------------------------------------------------------

class CommandBarPopup final : public DVGui::Dialog {
  Q_OBJECT
  CommandBarListTree* m_commandListTree;
  CommandBarTree* m_menuBarTree;
  QCheckBox* m_saveAsDefaultCB;
  TFilePath m_path, m_defaultPath;

public:
  CommandBarPopup(QString barId, bool isQuickToolbar = false);
protected slots:
  void onOkPressed();
  void onSearchTextChanged(const QString& text);
};

#endif