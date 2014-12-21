// edit_logchannels.h
//
// Edit Log Machine channel assignments
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

#ifndef EDIT_LOGCHANNELS_H
#define EDIT_LOGCHANNELS_H

#include <qwidget.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qpushbutton.h>

#include <rd.h>
#include <rdcardselector.h>
#include <rdairplay_conf.h>
#include <rdstation.h>
#include <rdchannels.h>

#include "edit_channelgpios.h"

class EditLogChannels : public QWidget
{
 Q_OBJECT
 public:
  EditLogChannels(RDAirPlayConf *conf,RDStation *cae_station,RDChannels *chans,
		    QWidget *parent=0);
  ~EditLogChannels();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void save();

 private slots:
  void logQuantityChangedData(int value);
  void logChangedData(int value);
  void audioSettingsChangedData(int id,int card,int port);
  void editGpiosData(int sub_num);

 protected:
  void resizeEvent(QResizeEvent *e);
  
 private:
  void LoadMachine(int mach);
  void SaveMachine(int mach);
  RDAirPlayConf *air_rdairplay_conf;
  RDStation *air_cae_station;
  RDChannels *air_channels;
  QLabel *air_log_quan_label;
  QSpinBox *air_log_quan_spin;
  QComboBox *air_log_box;
  QLabel *air_log_label[2];
  RDCardSelector *air_log_card_sel[2];
  QLabel *air_log_start_rml_label[2];
  QLineEdit *air_log_start_rml_edit[2];
  QLabel *air_log_stop_rml_label[2];
  QLineEdit *air_log_stop_rml_edit[2];
  QPushButton *air_log_channel_button[2];
  EditChannelGpios *air_gpios_dialog;
  int air_loaded_mach;
};


#endif  // EDIT_LOGCHANNELS_H

