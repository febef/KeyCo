#ifndef KEYCO_H
#define KEYCO_H

#include <QWidget>
#include <QString>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QIcon>
#include <Windows.h>
#include <QDebug>
#include <QHash>
#include <QDir>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class KeyCo : public QWidget
{
  Q_OBJECT

public:
  KeyCo(QWidget *parent = 0);
  ~KeyCo();

  void keyDown(UINT keyCode, wchar_t *buffer);
  void notify(QString msg);

private:

  void loadConfig();
  bool executeCommand();

  QIcon *icon;
  QSystemTrayIcon *trayIcon;
  QMenu *trayIconMenu;
  QAction  *quitAction;
  QHash<QString, DWORD> commands;
  QString command;
  char commandKey;
  char cancelKey;
  bool recCommand;

};

#endif // KEYCO_H
