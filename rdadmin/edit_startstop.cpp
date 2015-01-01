// edit_startstop.cpp
//
// Edit Rivendell Log Start/Stop Settings
//
//   (C) Copyright 2015 Fred Gleason <fredg@paravelsystems.com>
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License version 2 as
//   published by the Free Software Foundation.
//
//   This program is distributed in the hope that it will be useful,
//   but WITHOUT ANY WARRANTY; without even the implied warranty of
//   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//   GNU General Public License for more details.
//
//   You should have received a copy of the GNU General Public
//   License along with this program; if not, write to the Free Software
//   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//

#include <qpainter.h>

#include <rdlist_logs.h>

#include "edit_startstop.h"

EditStartStop::EditStartStop(RDStation *station,RDUser *user,QWidget *parent)
  : QWidget(parent)
{
  edit_station=station;
  edit_user=user;

  QFont small_font=QFont("Helvetica",12,QFont::Bold);
  small_font.setPixelSize(12);

  //
  // Exit Password
  //
  edit_passwd_changed=false;
  edit_exitpasswd_edit=new QLineEdit(this);
  edit_exitpasswd_edit->setEchoMode(QLineEdit::Password);
  edit_exitpasswd_edit->setText("******");
  edit_exitpasswd_label=
    new QLabel(edit_exitpasswd_edit,tr("Exit Password:"),this);
  edit_exitpasswd_label->setAlignment(AlignRight|AlignVCenter);
  connect(edit_exitpasswd_edit,SIGNAL(textChanged(const QString &)),
	  this,SLOT(passwordChangedData(const QString &)));

  //
  // Log Machine Selector
  //
  edit_logmachine_box=new QComboBox(this);
  edit_logmachine_box->insertItem(tr("Main Log"));
  for(unsigned i=1;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    edit_logmachine_box->insertItem(QString().sprintf("Aux %d Log",i));
  }
  connect(edit_logmachine_box,SIGNAL(activated(int)),
	  this,SLOT(logChangedData(int)));

  //
  // Startup Mode
  //
  edit_startmode_box=new QComboBox(this);
  edit_startmode_box->insertItem(tr("start with empty log"));
  edit_startmode_box->insertItem(tr("load previous log"));
  edit_startmode_box->insertItem(tr("load specified log"));
  edit_startmode_label=new QLabel(edit_exitpasswd_edit,tr("At Startup:"),this);
  edit_startmode_label->setAlignment(AlignRight|AlignVCenter);
  connect(edit_startmode_box,SIGNAL(activated(int)),
	  this,SLOT(modeChangedData(int)));

  //
  // Auto Restart Checkbox
  //
  edit_autorestart_box=new QCheckBox(this);
  edit_autorestart_label=
    new QLabel(edit_autorestart_box,
	       tr("Restart Log After Unclean Shutdown"),this);
  edit_autorestart_label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Startup Log
  //
  edit_startlog_edit=new QLineEdit(this);
  edit_startlog_label=new QLabel(edit_startlog_edit,tr("Log:"),this);
  edit_startlog_label->setAlignment(AlignRight|AlignVCenter);

  //
  //  Log Select Button
  //
  edit_startlog_button=new QPushButton(this);
  edit_startlog_button->setFont(small_font);
  edit_startlog_button->setText(tr("&Select"));
  connect(edit_startlog_button,SIGNAL(clicked()),this,SLOT(selectData()));
}


EditStartStop::~EditStartStop()
{
}


QSize EditStartStop::sizeHint() const
{
  return QSize(410,137);
}


QSizePolicy EditStartStop::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditStartStop::load(RDAirPlayConf *conf,RDLogModes *modes)
{
  edit_logmachine=0;
  for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    edit_startlog[i]=modes->logName(i);
    edit_startmode[i]=modes->startMode(i);
    edit_autorestart[i]=modes->autoRestart(i);
  }
  edit_startmode_box->setCurrentItem((int)edit_startmode[edit_logmachine]);
  edit_startlog_edit->setText(edit_startlog[edit_logmachine]);
  edit_autorestart_box->setChecked(edit_autorestart[edit_logmachine]);
  modeChangedData(edit_startmode[edit_logmachine]);
}


void EditStartStop::save(RDAirPlayConf *conf,RDLogModes *modes)
{
  if(edit_passwd_changed) {
    conf->setExitPassword(edit_exitpasswd_edit->text());
  }
  edit_startmode[edit_logmachine]=
    (RDLogModes::StartMode)edit_startmode_box->currentItem();
  edit_startlog[edit_logmachine]=edit_startlog_edit->text();
  edit_autorestart[edit_logmachine]=edit_autorestart_box->isChecked();
  for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    modes->setStartMode(i,edit_startmode[i]);
    modes->setLogName(i,edit_startlog[i]);
    modes->setAutoRestart(i,edit_autorestart[i]);
  }
}


void EditStartStop::selectData()
{
  QString logname=edit_startlog_edit->text();

  RDListLogs *ll=new RDListLogs(&logname,edit_station->name(),this,
                                "log",edit_user);
  if(ll->exec()==0) {
    edit_startlog_edit->setText(logname);
  }
  delete ll;
}


void EditStartStop::logChangedData(int lognum)
{
  edit_startmode[edit_logmachine]=
    (RDLogModes::StartMode)edit_startmode_box->currentItem();
  edit_startlog[edit_logmachine]=edit_startlog_edit->text();
  edit_autorestart[edit_logmachine]=edit_autorestart_box->isChecked();

  edit_logmachine=lognum;
  edit_startmode_box->setCurrentItem((int)edit_startmode[lognum]);
  edit_startlog_edit->setText(edit_startlog[lognum]);
  edit_autorestart_box->setChecked(edit_autorestart[lognum]);
  modeChangedData((int)edit_startmode[lognum]);
}


void EditStartStop::modeChangedData(int mode)
{
  edit_startlog_edit->setEnabled((RDLogModes::StartMode)mode==
				RDLogModes::StartSpecified);
  edit_startlog_label->setEnabled((RDLogModes::StartMode)mode==
				 RDLogModes::StartSpecified);
  edit_startlog_button->setEnabled((RDLogModes::StartMode)mode==
				 RDLogModes::StartSpecified);
  edit_autorestart_box->setDisabled((RDLogModes::StartMode)mode==
				   RDLogModes::StartEmpty);
  edit_autorestart_label->setDisabled((RDLogModes::StartMode)mode==
				     RDLogModes::StartEmpty);
}


void EditStartStop::passwordChangedData(const QString &str)
{
  edit_passwd_changed=true;
}


void EditStartStop::resizeEvent(QResizeEvent *e)
{
  edit_exitpasswd_label->setGeometry(0,0,85,20);
  edit_exitpasswd_edit->setGeometry(90,0,sizeHint().width()-905,20);
  edit_logmachine_box->setGeometry(35,25,100,20);
  edit_startmode_label->setGeometry(20,50,65,20);
  edit_startmode_box->setGeometry(90,50,240,20);
  edit_autorestart_label->setGeometry(115,75,250,15);
  edit_autorestart_box->setGeometry(95,75,15,15);
  edit_startlog_label->setGeometry(50,95,35,20);
  edit_startlog_edit->setGeometry(90,95,240,20);
  edit_startlog_button->setGeometry(340,93,50,24);
}


void EditStartStop::paintEvent(QPaintEvent *e)
{
  QPainter *p=new QPainter(this);
  p->setPen(black);
  p->drawRect(15,10,385,95);
  p->end();
  delete p;
}
