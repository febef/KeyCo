#ifndef NOTIFY_H
#define NOTIFY_H

#include <QObject>
#include <QString>
#include <QDateTime>

class Notify
{

public:
  Notify();
  Notify(int id);
  Notify(int id, QString title, QString body, int timeLive=1000);

  QString title;
  QString body;
  QDateTime launchDateTime;
  int timeLive;
  int id;

};

#endif // NOTIFY_H