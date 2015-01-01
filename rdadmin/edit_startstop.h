// edit_startstop.h
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

#ifndef EDIT_STARTSTOP_H
#define EDIT_STARTSTOP_H

#include <qcheckbox.h>
#include <qcombobox.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qpushbutton.h>
#include <qwidget.h>

#include <rdairplay_conf.h>
#include <rd.h>
#include <rdlogmodes.h>
#include <rdstation.h>
#include <rduser.h>

class EditStartStop : public QWidget
{
  Q_OBJECT
 public:
  EditStartStop(RDStation *station,RDUser *user,QWidget *parent=0);
  ~EditStartStop();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void load(RDAirPlayConf *conf,RDLogModes *modes);
  void save(RDAirPlayConf *conf,RDLogModes *modes);

 private slots:
  void selectData();
  void logChangedData(int lognum);
  void modeChangedData(int mode);
  void passwordChangedData(const QString &str);

 protected:
  void resizeEvent(QResizeEvent *e);
  void paintEvent(QPaintEvent *e);

 private:
  QLabel *edit_exitpasswd_label;
  QLineEdit *edit_exitpasswd_edit;
  bool edit_passwd_changed;
  int edit_logmachine;
  QComboBox *edit_logmachine_box;
  QLabel *edit_startmode_label;
  QComboBox *edit_startmode_box;
  RDLogModes::StartMode edit_startmode[RDAIRPLAY_LOG_QUANTITY];
  QLineEdit *edit_startlog_edit;
  QLabel *edit_startlog_label;
  QPushButton *edit_startlog_button;
  QString edit_startlog[RDAIRPLAY_LOG_QUANTITY];
  QCheckBox *edit_autorestart_box;
  bool edit_autorestart[RDAIRPLAY_LOG_QUANTITY];
  QLabel *edit_autorestart_label;
  RDStation *edit_station;
  RDUser *edit_user;
};


#endif  // EDIT_MODECONTROL_H
