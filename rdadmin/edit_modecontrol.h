// edit_modecontrol.h
//
// Edit Rivendell Log Modes
//
//   (C) Copyright 2014 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef EDIT_MODECONTROL_H
#define EDIT_MODECONTROL_H

#include <qcombobox.h>
#include <qlabel.h>
#include <qwidget.h>

#include <rdairplay_conf.h>
#include <rd.h>
#include <rdlogmodes.h>

class EditModeControl : public QWidget
{
  Q_OBJECT
 public:
  EditModeControl(QWidget *parent=0);
  ~EditModeControl();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void load(RDAirPlayConf *conf,RDLogModes *modes);
  void save(RDAirPlayConf *conf,RDLogModes *modes);

 private slots:
  void styleChangedData(int mode);
  void logChangedData(int lognum);
  void modeChangedData(int mode);

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  QLabel *edit_style_label;
  QComboBox *edit_style_box;
  QComboBox *edit_log_box;
  QLabel *edit_mode_label;
  QComboBox *edit_mode_box;
  RDLogModes::OpMode edit_log_modes[RDAIRPLAY_LOG_QUANTITY];
  int edit_current_lognum;
};


#endif  // EDIT_MODECONTROL_H
