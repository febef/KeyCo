#ifndef KEYCO_H
#define KEYCO_H

#include <Windows.h>

#include <QAction>
#include <QApplication>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHash>
#include <QIcon>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMenu>
#include <QString>
#include <QList>
#include <QSystemTrayIcon>
#include <QWidget>

#include "notify.h"
#include "widgetnotify.h"

class KeyCo : public QWidget
{
  Q_OBJECT

public:
  KeyCo(QWidget *parent = 0);
  ~KeyCo();

  void keyDown(UINT keyCode, wchar_t *buffer);
  void notify(QString msg, int live=3);

private:

  void loadConfig();
  bool executeCommand();

  QAction  *quitAction;
  QHash<QString, DWORD> commands;
  QIcon *icon;
  QMenu *trayIconMenu;
  QString command;
  QSystemTrayIcon *trayIcon;

  bool recCommand;
  char cancelKey;
  char commandKey;

  WidgetNotify *mainNotify;
  QList<WidgetNotify *> notifies;

};

#endif // KEYCO_H
