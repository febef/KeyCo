#ifndef WIDGETNOTIFY_H
#define WIDGETNOTIFY_H

#include <QDesktopWidget>

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>

#include <QPropertyAnimation>
#include <QRect>
#include <QPoint>

//#include <Qsize>
#include <QRegion>

#include "Notify.h"

#include <QDebug>

typedef enum {out=0, in=1, moving} myWgtState;

class WidgetNotify : public QWidget
{
  Q_OBJECT
public:
  WidgetNotify(QWidget *parent = 0);
  WidgetNotify(Notify notify,QWidget *parent = 0);

  void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
//  QSize sizeHint() const Q_DECL_OVERRIDE;

  void initWidgetNotify();
  void initAnimations();

  Notify data;

  QLabel *title, *body;
  QVBoxLayout *vbLayout;

  myWgtState state;
  QDesktopWidget DesktopWidget;
  QRect recPrimaryScreen;
  qreal maxOpacity;
  int fadetime, movetime;
  int live;
  QPropertyAnimation *moveIn, *moveOut, *moveUp, *fadein, *fadeout;



 signals:

 public slots:
  void notifyInUp();
  void notifyInUpOut();
  void notifyIn();
  void notifyOut();
  void notifyOutAndDelete();
  void notifyUp(int mov=-1);
  void autoDelete();
};

#endif // WIDGETNOTIFY_H
