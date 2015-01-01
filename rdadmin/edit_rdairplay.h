// edit_rdairplay.h
//
// Edit an RDAirPlay Configuration
//
//   (C) Copyright 2002-2010 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_rdairplay.h,v 1.29.6.5 2014/02/10 20:45:10 cvs Exp $
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

#ifndef EDIT_RDAIRPLAY_H
#define EDIT_RDAIRPLAY_H

#include <qdialog.h>
#include <qsqldatabase.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>

#include <rd.h>
#include <rdairplay_conf.h>
#include <rdcardselector.h>
#include <rdstation.h>
#include <rdchannels.h>
#include <rdlogmodes.h>
#include <rdsound_panel.h>

#include "edit_logchannels.h"
#include "edit_modecontrol.h"
#include "edit_panelchannels.h"
#include "edit_startstop.h"
#include "list_nownextplugins.h"

/*
 * Application Settings
 */
#define LOG_PLAY_PORTS 2
#define MAX_MANUAL_SEGUE 10

class EditRDAirPlay : public QDialog
{
 Q_OBJECT
 public:
  EditRDAirPlay(RDStation *station,RDStation *cae_station,RDChannels *chans,
		QWidget *parent=0);
  ~EditRDAirPlay();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  
 private slots:
  void editHotKeys();
  void selectSkinData();
  void okData();
  void cancelData();

 private:
  RDStation *air_station;
  RDAirPlayConf *air_conf;
  RDLogModes *air_modes;
  RDChannels *air_channels;
  EditLogChannels *air_edit_logchannels;
  EditPanelChannels *air_edit_panelchannels;
  ListNowNextPlugins *air_list_plugins;
  EditModeControl *air_mode_control;
  EditStartStop *air_start_stop;
  QLabel *air_segue_label;
  QLineEdit *air_segue_edit;
  QLabel *air_trans_label;
  QLineEdit *air_trans_edit;
  QLabel *air_trans_unit;
  QLabel *air_piecount_label;
  QSpinBox *air_piecount_box;
  QLabel *air_piecount_unit;
  QLabel *air_countto_label;
  QComboBox *air_countto_box;
  QLabel *air_segue_unit;
  QLabel *air_station_label;
  QSpinBox *air_station_box;
  QLabel *air_user_label;
  QSpinBox *air_user_box;
  QLabel *air_timesync_label;
  QCheckBox *air_timesync_box;
  QLabel *air_aux1_label;
  QSpinBox *air_aux1_box;
  QLabel *air_aux2_label;
  QSpinBox *air_aux2_box;
  QCheckBox *air_auxlog_box[2];
  QCheckBox *air_clearfilter_box;
  QButtonGroup *air_bar_group;
  QCheckBox *air_flash_box;
  QCheckBox *air_panel_pause_box;
  QCheckBox *air_show_counters_box;
  QLabel *air_audition_preroll_label;
  QLabel *air_audition_preroll_unit;
  QSpinBox *air_audition_preroll_spin;
  QLineEdit *air_label_template_edit;
  QCheckBox *air_pause_box;
  QCheckBox *air_hour_selector_box;
  QComboBox *air_default_transtype_box;
  QComboBox *air_defaultsvc_box;
  QLineEdit *air_skin_edit;
  QLineEdit *air_title_template_edit;
  QLineEdit *air_artist_template_edit;
  QLineEdit *air_outcue_template_edit;
  QLineEdit *air_description_template_edit;
};


#endif  // EDIT_RDAIRPLAY_H

