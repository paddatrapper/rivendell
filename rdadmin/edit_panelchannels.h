// edit_panelchannels.h
//
// Edit SoundPanel channel assignments
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

#ifndef EDIT_PANELCHANNELS_H
#define EDIT_PANELCHANNELS_H

#include <qwidget.h>
#include <qsqldatabase.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

#include <rd.h>
#include <rdcardselector.h>
#include <rdstation.h>
#include <rdchannels.h>
#include <rdsound_panel.h>

class EditPanelChannels : public QWidget
{
 Q_OBJECT
 public:
  EditPanelChannels(RDStation *cae_station,RDChannels *chans,
		    RDChannels::Channel type,QWidget *parent=0);
  ~EditPanelChannels();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void load();
  void save();

 private slots:
  void audioSettingsChangedData(int id,int card,int port);
  void editGpiosData(int num);
  
 private:
  RDStation *air_cae_station;
  RDChannels *air_channels;
  RDChannels::Channel air_channel_type;
  QLabel *air_panel_card_label[PANEL_MAX_OUTPUTS];
  RDCardSelector *air_panel_card_sel[PANEL_MAX_OUTPUTS];
  QLabel *air_panel_start_rml_label[PANEL_MAX_OUTPUTS];
  QLineEdit *air_panel_start_rml_edit[PANEL_MAX_OUTPUTS];
  QLabel *air_panel_stop_rml_label[PANEL_MAX_OUTPUTS];
  QLineEdit *air_panel_stop_rml_edit[PANEL_MAX_OUTPUTS];
  QPushButton *air_panel_channel_button[PANEL_MAX_OUTPUTS];
};


#endif  // EDIT_PANELCHANNELS_H

