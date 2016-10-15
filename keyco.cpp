#include "keyco.h"

KeyCo::KeyCo(QWidget *parent)
  : QWidget(parent)
{
  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  trayIconMenu = new QMenu(this);
  trayIconMenu->addAction(quitAction);

  icon = new QIcon(":/heart.png");

  trayIcon = new QSystemTrayIcon(this);
  trayIcon->setIcon(QIcon(":/heart.png"));
  trayIcon->setContextMenu(trayIconMenu);
  trayIcon->show();

  mainNotify = new WidgetNotify();
  mainNotify->title->setText("<b> Keyco</b>");

  loadConfig();
}

KeyCo::~KeyCo()
{
  notify("chauu...");
  delete trayIcon;
  delete trayIconMenu;
  delete quitAction;
  delete icon;
}

void KeyCo::keyDown(UINT keyCode, wchar_t *buffer)
{
  if ((char)keyCode == cancelKey || command.count() > 5){
    recCommand = false;
    mainNotify->notifyOut();
  }
  //if (command.count() == 0 && (char)keyCode != commandKey) return;
  if((char)keyCode == commandKey ) {
    recCommand = true ; // !executeCommand();
    mainNotify->body->setText("");
    mainNotify->notifyIn();
    command = "";
  }

  if (recCommand){
    if ("\b" == (QString::fromUtf16((ushort*)buffer)))
      command = command.left(command.length()-1);
    else
      command = command + (QString::fromUtf16((ushort*)buffer));
    mainNotify->body->setText(command);
    executeCommand();
  }

  qDebug() << "COMAND: " << command;
}

void KeyCo::notify(QString msg,int live)
{


  // generar arreglo de notify
  // lansar en esta funcion un tiimer que llame a una funcion que
  // 1) si el arreglo de notify tiene elementos genere el primer wdgetnotify
  //    1.2) elimine el notyfy.
  //    1.3) llame al timer.
  // cuando una notificacion se va deve ser eliminado wl widget.

  for(int i=0; i <notifies.count(); i++ )
     notifies[i]->notifyUp();

  QRect ng;
  //trayIcon->showMessage(tr("KeyCo"), msg);
  notifies.append(new WidgetNotify());
  WidgetNotify *w = notifies[notifies.count() -1];
  w->body->setText(msg);
  w->title->setText("Keyco!");
  w->live = live;

  ng = w->geometry();
  ng.moveTop(ng.top() - ng.height() - 4);
  w->setGeometry(ng);

    w->notifyInUpOut();


}

void KeyCo::loadConfig()
{
  recCommand = false;

  QFile file(QApplication::applicationDirPath() + "\\keyco.json");
  QString strCfg;
  QJsonDocument cfg;
  QJsonObject cfgO;
  QJsonObject cmd;
  QJsonArray cmds;

  if (file.open(QIODevice::ReadOnly)){

    strCfg = file.readAll();
    cfg = QJsonDocument::fromJson(strCfg.toUtf8());
    cfgO = cfg.object();

    qDebug() << "cancel: " << cfgO["cancelKey"].toString().toInt() <<
                "\ncmd: " << cfgO["commandKey"].toString().toInt();

    cancelKey = (char) cfgO["cancelKey"].toString().toInt();
    commandKey = (char) cfgO["commandKey"].toString().toInt();

    cmds = cfgO["commands"].toArray();
    commands.clear();
    for (int i = 0; i < cmds.size(); ++i) {
      cmd = cmds[i].toObject();
      commands.insert(cmd["cmd"].toString(), cmd["code"].toString().toInt());
      qDebug() << cmd["cmd"].toString() << "  " << cmd["code"].toString().toInt();
    }

  }else{
    notify("no load config!");

    cancelKey = 27; // escape
    commandKey = 163; // right control
    commands.insert("A_", 0x09a4);
    commands.insert("N~", 0x00d1);
    commands.insert("n~", 0x00f1);
  }
}

bool KeyCo::executeCommand()
{
  //qDebug() << "exec< " << commands.value(command) << "\n";
  if (commands.value(command, 0) == 0) return false;

  recCommand = false;
  //qDebug() << "executed!";
  INPUT ip;
  ip.type = INPUT_KEYBOARD;
  ip.ki.time = 0;
  ip.ki.dwExtraInfo = 0;

  int count = command.count();

  for (int i=0; i < count; i++){
    ip.ki.dwFlags = KEYEVENTF_UNICODE;
    ip.ki.wVk = 8;
    ip.ki.wScan = 0; // This is a Bengali unicode character
    SendInput(1, &ip, sizeof(INPUT));

    // Release key
    ip.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
    SendInput(1, &ip, sizeof(INPUT));
  }

  // Press a unicode "key"
  ip.ki.dwFlags = KEYEVENTF_UNICODE;
  ip.ki.wVk = 0;
  ip.ki.wScan = commands[command]; // This is a Bengali unicode character
  SendInput(1, &ip, sizeof(INPUT));

  // Release key
  ip.ki.dwFlags = KEYEVENTF_UNICODE | KEYEVENTF_KEYUP;
  SendInput(1, &ip, sizeof(INPUT));
  mainNotify->notifyOut();
  return true;
}