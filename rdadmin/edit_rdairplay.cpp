// edit_rdairplay.cpp
//
// Edit an RDAirPlay Configuration
//
//   (C) Copyright 2002-2004 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_rdairplay.cpp,v 1.53.6.9 2014/02/11 23:46:26 cvs Exp $
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

#include <qdialog.h>
#include <qstring.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qlistbox.h>
#include <qtextedit.h>
#include <qpainter.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qfiledialog.h>

#include <rd.h>
#include <rddb.h>
#include <rdtextvalidator.h>
#include <rdlist_logs.h>
#include <globals.h>

#include <edit_rdairplay.h>
#include <edit_hotkeys.h>
#include <edit_channelgpios.h>

EditRDAirPlay::EditRDAirPlay(RDStation *station,RDStation *cae_station,
			     RDChannels *chans,QWidget *parent)
  : QDialog(parent,"",true)
{
  QString sql;
  RDSqlQuery *q;

  air_station=station;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  air_conf=new RDAirPlayConf(station->name(),"RDAIRPLAY");
  air_channels=chans;
  air_modes=new RDLogModes(station->name());

  //
  // Create Fonts
  //
  QFont unit_font=QFont("Helvetica",12,QFont::Normal);
  unit_font.setPixelSize(12);
  QFont small_font=QFont("Helvetica",12,QFont::Bold);
  small_font.setPixelSize(12);
  QFont big_font=QFont("Helvetica",14,QFont::Bold);
  big_font.setPixelSize(14);

  //
  // Dialog Name
  //
  setCaption(tr("RDAirPlay config for ")+station->name());

  //
  // Channel Assignments Section
  //
  QLabel *label=new QLabel(tr("Channel Assignments"),this);
  label->setFont(big_font);
  label->setGeometry(10,10,200,16);

  //
  // Log Machine Channel Assignments
  //
  air_edit_logchannels=new EditLogChannels(air_conf,cae_station,chans,this);
  air_edit_logchannels->
    setGeometry(25,32,air_edit_logchannels->sizeHint().width(),
		air_edit_logchannels->sizeHint().height());

  //
  // RLM Plugins
  //
  label=new QLabel(tr("Rivendell Loadable Modules"),this);
  label->setFont(big_font);
  label->setGeometry(20,220,200,20);
  air_list_plugins=new ListNowNextPlugins(this);
  air_list_plugins->setGeometry(10,240,air_list_plugins->sizeHint().width(),
				air_list_plugins->sizeHint().height());
  air_list_plugins->load(air_station->name());

  //
  // Start/Stop Section
  //
  label=new QLabel(tr("Start/Stop Settings"),this);
  label->setFont(big_font);
  label->setGeometry(10,401,200,16);

  air_start_stop=new EditStartStop(admin_station,admin_user,this);
  air_start_stop->setGeometry(10,424,air_start_stop->sizeHint().width(),
			      air_start_stop->sizeHint().height());

  //
  // HotKeys Configuration Button
  //
  QPushButton *button=new QPushButton(this);
  button->setGeometry(10,550,180,50);
  button->setFont(small_font);
  button->setText(tr("Configure Hot Keys"));
  connect(button,SIGNAL(clicked()),this,SLOT(editHotKeys()));

  //
  // SoundPanel Channel Assignments
  //
  air_edit_panelchannels=
    new EditPanelChannels(cae_station,chans,RDChannels::AirplayButtonOutput,
			  this);
  air_edit_panelchannels->
    setGeometry(400,32,air_edit_panelchannels->sizeHint().width(),
		air_edit_panelchannels->sizeHint().height());

  //
  // Main Log Play Section
  //
  label=new QLabel("Log Settings",this);
  label->setFont(big_font);
  label->setGeometry(805,10,200,16);

  //
  // Segue Length
  //
  air_segue_edit=new QLineEdit(this);
  air_segue_edit->setGeometry(895,32,50,20);
  air_segue_label=new QLabel(air_segue_edit,tr("Manual Segue:"),this);
  air_segue_label->setGeometry(790,32,100,20);
  air_segue_label->setAlignment(AlignRight|AlignVCenter);
  air_segue_unit=new QLabel(air_segue_edit,tr("msecs"),this);
  air_segue_unit->setGeometry(950,32,40,20);
  air_segue_unit->setAlignment(AlignLeft|AlignVCenter);
  
  //
  // Forced Transition Length
  //
  air_trans_edit=new QLineEdit(this);
  air_trans_edit->setGeometry(895,54,50,20);
  air_trans_label=new QLabel(air_trans_edit,tr("Forced Segue:"),this);
  air_trans_label->setGeometry(790,54,100,20);
  air_trans_label->setAlignment(AlignRight|AlignVCenter);
  air_trans_unit=new QLabel(air_trans_edit,tr("msecs"),this);
  air_trans_unit->setGeometry(950,54,40,20);
  air_trans_unit->setAlignment(AlignLeft|AlignVCenter);
  
  //
  // Pie Countdown Length
  //
  air_piecount_box=new QSpinBox(this);
  air_piecount_box->setGeometry(895,76,50,20);
  air_piecount_box->setRange(0,60);
  air_piecount_label=new QLabel(air_piecount_box,tr("Pie Counts Last:"),this);
  air_piecount_label->setGeometry(785,76,105,20);
  air_piecount_label->setAlignment(AlignRight|AlignVCenter);
  air_piecount_unit=new QLabel(tr("secs"),this);
  air_piecount_unit->setGeometry(950,76,40,20);
  air_piecount_unit->setAlignment(AlignLeft|AlignVCenter);

  //
  // Pie Countdown To
  //
  air_countto_box=new QComboBox(this);
  air_countto_box->setGeometry(895,98,100,20);
  air_countto_label=new QLabel(air_countto_box,tr("Pie Counts To:"),this);
  air_countto_label->setGeometry(785,98,105,20);
  air_countto_label->setAlignment(AlignRight|AlignVCenter);
  air_countto_box->insertItem(tr("Cart End"));
  air_countto_box->insertItem(tr("Transition"));

  //
  // Default Transition Type
  //
  air_default_transtype_box=new QComboBox(this);
  air_default_transtype_box->setGeometry(895,120,100,20);
  label=new QLabel(air_default_transtype_box,tr("Default Trans. Type:"),this);
  label->setGeometry(760,120,130,20);
  label->setAlignment(AlignRight|AlignVCenter);
  air_default_transtype_box->insertItem(tr("Play"));
  air_default_transtype_box->insertItem(tr("Segue"));
  air_default_transtype_box->insertItem(tr("Stop"));

  //
  // Default Service
  //
  air_defaultsvc_box=new QComboBox(this);
  air_defaultsvc_box->setGeometry(895,142,100,20);
  label=new QLabel(air_defaultsvc_box,tr("Default Service:"),this);
  label->setGeometry(760,142,130,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Sound Panel Section
  //
  label=new QLabel(tr("Sound Panel Settings"),this);
  label->setFont(big_font);
  label->setGeometry(805,179,200,16);

  //
  // # of Station Panels
  //
  air_station_box=new QSpinBox(this);
  air_station_box->setGeometry(895,204,50,20);
  air_station_box->setRange(0,MAX_PANELS);
  air_station_box->setSpecialValueText(tr("None"));
  air_station_label=new QLabel(air_station_box,tr("Host Panels:"),this);
  air_station_label->setGeometry(790,204,100,20);
  air_station_label->setAlignment(AlignRight|AlignVCenter);

  //
  // # of User Panels
  //
  air_user_box=new QSpinBox(this);
  air_user_box->setGeometry(895,226,50,20);
  air_user_box->setRange(0,MAX_PANELS);
  air_user_box->setSpecialValueText(tr("None"));
  air_user_label=new QLabel(air_user_box,tr("User Panels:"),this);
  air_user_label->setGeometry(790,226,100,20);
  air_user_label->setAlignment(AlignRight|AlignVCenter);

  //
  // Flash Active Button
  //
  air_flash_box=new QCheckBox(this);
  air_flash_box->setGeometry(810,254,15,15);
  label=new QLabel(air_flash_box,tr("Flash Active Buttons"),this);
  label->setGeometry(830,254,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Pause Panel Button
  //
  air_panel_pause_box=new QCheckBox(this);
  air_panel_pause_box->setGeometry(810,276,15,15);
  label=new QLabel(air_panel_pause_box,tr("Enable Button Pausing"),this);
  label->setGeometry(830,276,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Label Template
  //
  air_label_template_edit=new QLineEdit(this);
  air_label_template_edit->setGeometry(895,298,sizeHint().width()-910,20);
  label=new QLabel(air_label_template_edit,tr("Label Template:"),this);
  label->setGeometry(790,298,100,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Miscellaneous Section
  //
  label=new QLabel(tr("Miscellaneous Settings"),this);
  label->setFont(big_font);
  label->setGeometry(805,330,200,16);

  //
  // Check Timesync
  //
  air_timesync_box=new QCheckBox(this);
  air_timesync_box->setGeometry(810,356,15,15);
  air_timesync_label=new QLabel(air_timesync_box,tr("Check TimeSync"),this);
  air_timesync_label->setGeometry(830,356,100,15);
  air_timesync_label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Show Log Buttons
  //
  air_auxlog_box[0]=new QCheckBox(this);
  air_auxlog_box[0]->setGeometry(810,378,15,15);
  label=new QLabel(air_auxlog_box[0],tr("Show Auxlog 1 Button"),this);
  label->setGeometry(830,378,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  air_auxlog_box[1]=new QCheckBox(this);
  air_auxlog_box[1]->setGeometry(810,400,15,15);
  label=new QLabel(air_auxlog_box[1],tr("Show Auxlog 2 Button"),this);
  label->setGeometry(830,400,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Clear Cart Filter
  //
  air_clearfilter_box=new QCheckBox(this);
  air_clearfilter_box->setGeometry(810,422,15,15);
  label=new QLabel(air_clearfilter_box,tr("Clear Cart Search Filter"),this);
  label->setGeometry(830,422,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Pause Enable Checkbox
  //
  air_pause_box=new QCheckBox(this);
  air_pause_box->setGeometry(810,444,15,15);
  label=new QLabel(air_pause_box,tr("Enable Paused Events"),this);
  label->setGeometry(830,444,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Show Extra Counters/Buttons
  //
  air_show_counters_box=new QCheckBox(this);
  air_show_counters_box->setGeometry(810,466,15,15);
  label=
    new QLabel(air_show_counters_box,tr("Show Extra Buttons/Counters"),this);
  label->setGeometry(830,466,170,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Show Hour Selector
  //
  air_hour_selector_box=new QCheckBox(this);
  air_hour_selector_box->setGeometry(810,488,15,15);
  label=new QLabel(air_hour_selector_box,tr("Show Hour Selector"),this);
  label->setGeometry(830,488,170,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Audition Preroll
  //
  air_audition_preroll_spin=new QSpinBox(this);
  air_audition_preroll_spin->setGeometry(895,507,45,20);
  air_audition_preroll_spin->setRange(1,60);
  air_audition_preroll_label=
    new QLabel(air_audition_preroll_spin,tr("Audition Preroll:"),this);
  air_audition_preroll_label->setGeometry(800,510,90,15);
  air_audition_preroll_label->setAlignment(AlignRight|AlignVCenter);
  air_audition_preroll_unit=new QLabel(tr("secs"),this);
  air_audition_preroll_unit->setGeometry(945,510,100,15);
  air_audition_preroll_unit->setAlignment(AlignLeft|AlignVCenter);

  //
  // Space Bar Action
  //
  air_bar_group=new QButtonGroup(1,Qt::Vertical,tr("Space Bar Action"),this);
  air_bar_group->setGeometry(805,532,sizeHint().width()-815,55);
  new QRadioButton(tr("None"),air_bar_group);
  new QRadioButton(tr("Start Next"),air_bar_group);

  //
  // Display Settings Section
  //
  label=new QLabel(tr("Display Settings"),this,"globals_label");
  label->setFont(big_font);
  label->setGeometry(435,381,200,16);

  //
  // Skin Path
  //
  air_skin_edit=new QLineEdit(this);
  air_skin_edit->setGeometry(555,403,180,20);
  label=new QLabel(air_skin_edit,tr("Background Image:"),this);
  label->setGeometry(435,403,115,20);
  label->setAlignment(AlignRight|AlignVCenter);
  button=new QPushButton(tr("Select"),this);
  button->setGeometry(745,400,50,25);
  connect(button,SIGNAL(clicked()),this,SLOT(selectSkinData()));

  //
  // Title Template
  //
  air_title_template_edit=new QLineEdit(this);
  air_title_template_edit->setGeometry(555,425,180,20);
  label=new QLabel(air_label_template_edit,tr("Title Template:"),this);
  label->setGeometry(430,425,120,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Artist Template
  //
  air_artist_template_edit=new QLineEdit(this);
  air_artist_template_edit->setGeometry(555,447,180,20);
  label=new QLabel(air_label_template_edit,tr("Artist Template:"),this);
  label->setGeometry(430,447,120,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Outcue Template
  //
  air_outcue_template_edit=new QLineEdit(this);
  air_outcue_template_edit->setGeometry(555,469,180,20);
  label=new QLabel(air_label_template_edit,tr("Outcue Template:"),this);
  label->setGeometry(430,469,120,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Description Template
  //
  air_description_template_edit=new QLineEdit(this);
  air_description_template_edit->setGeometry(555,491,180,20);
  label=new QLabel(air_label_template_edit,tr("Description Template:"),this);
  label->setGeometry(430,491,120,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Log Mode Control Section
  //
  label=new QLabel(tr("Log Mode Control"),this);
  label->setFont(big_font);
  label->setGeometry(435,530,200,16);

  //
  // Mode Control
  //
  air_mode_control=new EditModeControl(this);
  air_mode_control->setGeometry(435,550,air_mode_control->sizeHint().width(),
				air_mode_control->sizeHint().height());

  //
  //  Ok Button
  //
  button=new QPushButton(this,"ok_button");
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(small_font);
  button->setText(tr("&OK"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this,"cancel_button");
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(small_font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Populate Fields
  //
  if(!cae_station->scanned()) {
    air_edit_logchannels->setDisabled(true);
    QMessageBox::information(this,tr("No Audio Configuration Data"),
			     tr("Channel assignments will not be available for this host as audio resource data\nhas not yet been generated.  Please start the Rivendell daemons on the host\nconfigured to run the CAE service in order to populate the audio resources database."));
  }
  air_edit_panelchannels->load();
  air_segue_edit->setText(QString().sprintf("%d",air_conf->segueLength()));
  air_trans_edit->setText(QString().sprintf("%d",air_conf->transLength()));
  air_piecount_box->setValue(air_conf->pieCountLength()/1000);
  air_countto_box->setCurrentItem(air_conf->pieEndPoint());
  air_default_transtype_box->setCurrentItem(air_conf->defaultTransType());
  air_defaultsvc_box->insertItem(tr("[none]"));
  QString defaultsvc=air_conf->defaultSvc();
  sql=QString().sprintf("select SERVICE_NAME from SERVICE_PERMS \
                         where STATION_NAME=\"%s\"",
			(const char *)air_conf->station());
  q=new RDSqlQuery(sql);
  while(q->next()) {
    air_defaultsvc_box->insertItem(q->value(0).toString());
    if(defaultsvc==q->value(0).toString()) {
      air_defaultsvc_box->setCurrentItem(air_defaultsvc_box->count()-1);
    }
  }
  delete q;
  air_station_box->setValue(air_conf->panels(RDAirPlayConf::StationPanel));
  air_user_box->setValue(air_conf->panels(RDAirPlayConf::UserPanel));
  air_timesync_box->setChecked(air_conf->checkTimesync());
  air_clearfilter_box->setChecked(air_conf->clearFilter());
  air_bar_group->setButton((int)air_conf->barAction());
  air_flash_box->setChecked(air_conf->flashPanel());
  air_panel_pause_box->setChecked(air_conf->panelPauseEnabled());
  air_label_template_edit->setText(air_conf->buttonLabelTemplate());
  air_pause_box->setChecked(air_conf->pauseEnabled());
  air_show_counters_box->setChecked(air_conf->showCounters());
  air_hour_selector_box->setChecked(air_conf->hourSelectorEnabled());
  air_audition_preroll_spin->setValue(air_conf->auditionPreroll()/1000);
  air_title_template_edit->setText(air_conf->titleTemplate());
  air_artist_template_edit->setText(air_conf->artistTemplate());
  air_outcue_template_edit->setText(air_conf->outcueTemplate());
  air_description_template_edit->setText(air_conf->descriptionTemplate());
  air_mode_control->load(air_conf,air_modes);
  air_start_stop->load(air_conf,air_modes);
  air_skin_edit->setText(air_conf->skinPath());
}


EditRDAirPlay::~EditRDAirPlay()
{
}


QSize EditRDAirPlay::sizeHint() const
{
  return QSize(1010,660);
} 


QSizePolicy EditRDAirPlay::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditRDAirPlay::editHotKeys()
{
  QString module_name = QString("airplay");
  EditHotkeys *edit_hotkeys=new EditHotkeys(air_conf->station(),
		                (const char *)module_name,
				this,"hotkeys");
  edit_hotkeys->exec();
  delete edit_hotkeys;
}


void EditRDAirPlay::selectSkinData()
{
  QString filename=air_skin_edit->text();
  filename=QFileDialog::getOpenFileName(filename,RD_IMAGE_FILE_FILTER,this,"",
					tr("Select Image File"));
  if(!filename.isNull()) {
    air_skin_edit->setText(filename);
  }
}


void EditRDAirPlay::okData()
{
  bool ok=false;
  int segue=air_segue_edit->text().toInt(&ok);
  if(!ok) {
    QMessageBox::warning(this,tr("Data Error"),tr("Invalid Segue Length!"));
    return;
  }
  int trans=air_trans_edit->text().toInt(&ok);
  if(!ok) {
    QMessageBox::warning(this,tr("Data Error"),
			 tr("Invalid Forced Segue Length!"));
    return;
  }
  air_edit_logchannels->save();
  air_list_plugins->save(air_station->name());
  air_edit_panelchannels->save();
  air_conf->setSegueLength(segue);
  air_conf->setTransLength(trans);
  air_conf->setPieCountLength(air_piecount_box->value()*1000);
  air_conf->
    setPieEndPoint((RDAirPlayConf::PieEndPoint)air_countto_box->currentItem());
  air_conf->setDefaultTransType((RDLogLine::TransType)
				air_default_transtype_box->currentItem());
  if(air_defaultsvc_box->currentItem()==0) {
    air_conf->setDefaultSvc("");
  }
  else {
    air_conf->setDefaultSvc(air_defaultsvc_box->currentText());
  }
  air_conf->setPanels(RDAirPlayConf::StationPanel,air_station_box->value());
  air_conf->setPanels(RDAirPlayConf::UserPanel,air_user_box->value());
  air_conf->setCheckTimesync(air_timesync_box->isChecked());
  air_conf->setClearFilter(air_clearfilter_box->isChecked());
  air_conf->
    setBarAction((RDAirPlayConf::BarAction)air_bar_group->selectedId());
  air_conf->setFlashPanel(air_flash_box->isChecked());
  air_conf->setPanelPauseEnabled(air_panel_pause_box->isChecked());
  air_conf->setButtonLabelTemplate(air_label_template_edit->text());
  air_conf->setPauseEnabled(air_pause_box->isChecked());
  air_conf->setShowCounters(air_show_counters_box->isChecked());
  air_conf->setHourSelectorEnabled(air_hour_selector_box->isChecked());
  air_conf->setAuditionPreroll(air_audition_preroll_spin->value()*1000);
  air_conf->setTitleTemplate(air_title_template_edit->text());
  air_conf->setArtistTemplate(air_artist_template_edit->text());
  air_conf->setOutcueTemplate(air_outcue_template_edit->text());
  air_conf->setDescriptionTemplate(air_description_template_edit->text());
  air_mode_control->save(air_conf,air_modes);
  air_start_stop->save(air_conf,air_modes);
  air_conf->setSkinPath(air_skin_edit->text());
  done(0);
}


void EditRDAirPlay::cancelData()
{
  done(1);
}
