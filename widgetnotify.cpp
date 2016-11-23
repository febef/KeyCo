#include "widgetnotify.h"

WidgetNotify::WidgetNotify(QWidget *parent) : QWidget(parent)
{
  initWidgetNotify();
}

void WidgetNotify::initAnimations()
{

 // FadeIn ...
  fadein = new QPropertyAnimation(this, "windowOpacity");
  fadein->setDuration(fadetime);
  fadein->setStartValue(-0.5);
  fadein->setEndValue(maxOpacity);

 // FadeOut ...r
  fadeout = new QPropertyAnimation(this, "windowOpacity");
  fadeout->setDuration(fadetime);
  fadeout->setStartValue(maxOpacity);
  fadeout->setEndValue(0);

 // MoveIn ...
  moveIn = new QPropertyAnimation(this, "geometry");
  moveIn->setDuration(movetime);
  moveIn->setEasingCurve(QEasingCurve::InOutBack);
 // MoveOut ...
  moveOut = new QPropertyAnimation(this, "geometry");
  moveOut->setDuration(movetime);
  moveOut->setEasingCurve(QEasingCurve::InOutBack);
 // MoveUp ...
  moveUp = new QPropertyAnimation(this, "geometry");
  moveUp->setDuration(movetime);
  moveUp->setEasingCurve(QEasingCurve::InOutBack);
 // moveto->easingCurve().setOvershoot(3)
 // moveto->easingCurve().setAmplitude(-100);
}

WidgetNotify::WidgetNotify(Notify notify, QWidget *parent) : QWidget(parent)
{
  data.body = notify.body;
  data.title = notify.title;

  initWidgetNotify();

}

void WidgetNotify::initWidgetNotify()
{
 // Config
  maxOpacity = 0.9;
  fadetime = 800;
  movetime = 700;

  QDesktopWidget DesktopWidget;
  QRect recPrimaryScreen = DesktopWidget.availableGeometry(DesktopWidget.primaryScreen());
  setStyleSheet("QWidget {background-color : black} QLabel{background-color : black;color : white}");
  setGeometry(recPrimaryScreen.width(),recPrimaryScreen.height()- (60+4), 250, 60);

  vbLayout = new QVBoxLayout();

  title = new QLabel();
  title->setGeometry(4,0, 16, 240);
  title->setMaximumHeight(22);
  title->setMargin(4);
  title->setText(data.title);


  body = new QLabel();
  body->setContentsMargins(8,0,0,4);
 // body->setMaximumHeight(30);
  body->setText(data.body);

  vbLayout->addWidget(title, 0, Qt::AlignTop);
  vbLayout->addWidget(body,1, Qt::AlignTop);

  vbLayout->setSpacing(0);
  vbLayout->setMargin(0);

  setLayout(vbLayout);
  this->setWindowFlags(Qt::WindowStaysOnTopHint);
  setWindowOpacity(0);
 // initialize
  initAnimations();

  show();
  qDebug() << "WidgetNotify Ready!";
}

void WidgetNotify::notifyIn()
{
  qDebug() << "in...";
  recPrimaryScreen = DesktopWidget.availableGeometry(DesktopWidget.primaryScreen());
  QRect wrec = geometry();
  moveIn->setStartValue(geometry());
  wrec.moveLeft(recPrimaryScreen.width()-(width()+4));
  moveIn->setEndValue(wrec);
  moveIn->start();
  fadein->start();
}

void WidgetNotify::notifyUp(int mov)
{
  qDebug() << "up...";
  recPrimaryScreen = geometry();
 // recPrimaryScreen.setTop(geometry().top()-((mov==-1 ? height() : mov) +4));
  recPrimaryScreen.moveTop(geometry().top()-((mov==-1 ? height() : mov) +4));
  moveUp->setStartValue(geometry());
  moveUp->setEndValue(recPrimaryScreen);
  moveUp->start();

}

void WidgetNotify::notifyOut()
{
  qDebug() << "out...";
  recPrimaryScreen = DesktopWidget.availableGeometry(DesktopWidget.primaryScreen());
  QRect wrec = geometry();
  moveOut->setStartValue(geometry());
  wrec.moveLeft(recPrimaryScreen.width());
  moveOut->setEndValue(wrec);
  moveOut->start();
  fadeout->start();
}

void WidgetNotify::notifyOutAndDelete(){

  qDebug() << "out...and delete";
  recPrimaryScreen = DesktopWidget.availableGeometry(DesktopWidget.primaryScreen());
  QRect wrec = geometry();
  moveOut->setStartValue(geometry());
  wrec.moveLeft(recPrimaryScreen.width());
  moveOut->setEndValue(wrec);
  moveOut->start();
  fadeout->start();

  connect(fadeout, SIGNAL(finished()) , this, SLOT(autoDelete()));
}

void WidgetNotify::notifyInUpOut(){

//  moveIn->connect(this->moveto, SIGNAL(finished()), this, SLOT(notifyOutAndDelete()));
  this->notifyUp();

}

void WidgetNotify::autoDelete(){



}

//QSize WidgetNotify::sizeHint() const
//{
 // return QSize(250, 60);
//}

void WidgetNotify::resizeEvent(QResizeEvent *  event)
{

  int radius_tl=5, radius_tr, radius_bl, radius_br;
  radius_tr = radius_bl = radius_br = radius_tl;

  QRegion region(0, 0, width(), height(), QRegion::Rectangle);

 // top left
  QRegion round (0, 0, 2*radius_tl, 2*radius_tl, QRegion::Ellipse);
  QRegion corner(0, 0, radius_tl, radius_tl, QRegion::Rectangle);
  region = region.subtracted(corner.subtracted(round));

 // top right
  round = QRegion(width()-2*radius_tr, 0, 2*radius_tr, 2*radius_tr, QRegion::Ellipse);
  corner = QRegion(width()-radius_tr, 0, radius_tr, radius_tr, QRegion::Rectangle);
  region = region.subtracted(corner.subtracted(round));

 // bottom right
  round = QRegion(width()-2*radius_br, height()-2*radius_br, 2*radius_br, 2*radius_br, QRegion::Ellipse);
  corner = QRegion(width()-radius_br, height()-radius_br, radius_br, radius_br, QRegion::Rectangle);
  region = region.subtracted(corner.subtracted(round));

 // bottom left
  round = QRegion(0, height()-2*radius_bl, 2*radius_bl, 2*radius_bl, QRegion::Ellipse);
  corner = QRegion(0, height()-radius_bl, radius_bl, radius_bl, QRegion::Rectangle);
  region = region.subtracted(corner.subtracted(round));

  setMask(region);

}
