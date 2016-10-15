#include "notify.h"

Notify::Notify(int id=-1, QString title="", QString body="", int timeLive)
  : id(id), title(title), body(body), timeLive(timeLive)
{

}

Notify::Notify()
{
  id=-1;
  title = body ="";
  timeLive = 1000;
}

Notify::Notify(int id)
  : id(id)
{
  title = body ="";
  timeLive = 1000;
}
