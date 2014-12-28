// edit_rdpanel.cpp
//
// Edit an RDPanel Configuration
//
//   (C) Copyright 2002-2007 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_rdpanel.cpp,v 1.9.8.2 2013/12/23 18:35:15 cvs Exp $
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
#include <qpainter.h>
#include <qfiledialog.h>

#include <rddb.h>
#include <rd.h>
#include <rdtextvalidator.h>
#include <rdlist_logs.h>
#include <rdsound_panel.h>

#include <edit_rdpanel.h>

EditRDPanel::EditRDPanel(RDStation *station,RDStation *cae_station,
			 RDChannels *chans,QWidget *parent)
  : QDialog(parent,"",true)
{
  QString sql;
  RDSqlQuery *q;

  //
  // Fix the Window Size
  //
  setMinimumWidth(sizeHint().width());
  setMaximumWidth(sizeHint().width());
  setMinimumHeight(sizeHint().height());
  setMaximumHeight(sizeHint().height());

  air_conf=new RDAirPlayConf(station->name(),"RDPANEL");
  air_channels=chans;

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
  // Text Validator
  //
  RDTextValidator *validator=new RDTextValidator(this);

  //
  // Dialog Name
  //
  setCaption(tr("RDPanel config for ")+station->name());

  //
  // Channel Assignments Section
  //
  QLabel *label=new QLabel(tr("Channel Assignments"),this);
  label->setFont(big_font);
  label->setGeometry(10,10,200,16);

  //
  // Sound Panel First Play Output
  //
  label=new QLabel(tr("SoundPanel First Play Output"),this);
  label->setFont(small_font);
//  label->setGeometry(395,32,300,16);
  label->setGeometry(20,32,300,16);
  air_card_sel[0]=new RDCardSelector(this);
  air_card_sel[0]->setGeometry(15,50,120,117);
  air_start_rml_edit[0]=new QLineEdit(this);
  air_start_rml_edit[0]->setGeometry(205,50,160,19);
  air_start_rml_edit[0]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[0],tr("Start RML:"),this);
  label->setGeometry(135,50,65,19);
  label->setAlignment(AlignVCenter|AlignRight);
  air_stop_rml_edit[0]=new QLineEdit(this);
  air_stop_rml_edit[0]->setGeometry(205,71,160,19);
  air_stop_rml_edit[0]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[0],tr("Stop RML:"),this);
  label->setGeometry(135,71,65,19);
  label->setAlignment(AlignVCenter|AlignRight);

  //
  // Sound Panel Second Play Output
  //
  label=new QLabel(tr("SoundPanel Second Play Output"),this);
  label->setFont(small_font);
  label->setGeometry(20,100,300,16);
  air_card_sel[1]=new RDCardSelector(this);
  air_card_sel[1]->setGeometry(15,118,120,117);
  air_start_rml_edit[1]=new QLineEdit(this);
  air_start_rml_edit[1]->setGeometry(205,118,160,19);
  air_start_rml_edit[1]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[1],tr("Start RML:"),this);
  label->setGeometry(135,118,65,19);
  label->setAlignment(AlignVCenter|AlignRight);
  air_stop_rml_edit[1]=new QLineEdit(this);
  air_stop_rml_edit[1]->setGeometry(205,139,160,19);
  air_stop_rml_edit[1]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[1],tr("Stop RML:"),this);
  label->setGeometry(135,139,65,19);
  label->setAlignment(AlignVCenter|AlignRight);

  //
  // Sound Panel Third Play Output
  //
  label=new QLabel(tr("SoundPanel Third Play Output"),this);
  label->setFont(small_font);
  label->setGeometry(20,168,300,16);
  air_card_sel[2]=new RDCardSelector(this);
  air_card_sel[2]->setGeometry(15,186,120,117);
  air_start_rml_edit[2]=new QLineEdit(this);
  air_start_rml_edit[2]->setGeometry(205,186,160,19);
  air_start_rml_edit[2]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[2],tr("Start RML:"),this);
  label->setGeometry(135,186,65,19);
  label->setAlignment(AlignVCenter|AlignRight);
  air_stop_rml_edit[2]=new QLineEdit(this);
  air_stop_rml_edit[2]->setGeometry(205,207,160,19);
  air_stop_rml_edit[2]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[2],tr("Stop RML:"),this);
  label->setGeometry(135,207,65,19);
  label->setAlignment(AlignVCenter|AlignRight);

  //
  // Sound Panel Fourth Play Output
  //
  label=new QLabel(tr("SoundPanel Fourth Play Output"),this);
  label->setFont(small_font);
  label->setGeometry(20,236,300,16);
  air_card_sel[3]=new RDCardSelector(this);
  air_card_sel[3]->setGeometry(15,254,120,117);
  air_start_rml_edit[3]=new QLineEdit(this);
  air_start_rml_edit[3]->setGeometry(205,254,160,19);
  air_start_rml_edit[3]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[3],tr("Start RML:"),this);
  label->setGeometry(135,254,65,19);
  label->setAlignment(AlignVCenter|AlignRight);
  air_stop_rml_edit[3]=new QLineEdit(this);
  air_stop_rml_edit[3]->setGeometry(205,275,160,19);
  air_stop_rml_edit[3]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[3],tr("Stop RML:"),this);
  label->setGeometry(135,275,65,19);
  label->setAlignment(AlignVCenter|AlignRight);

  //
  // Sound Panel Fifth Play Output
  //
  label=new QLabel(tr("SoundPanel Fifth and Later Play Output"),
		   this,"globals_label");
  label->setFont(small_font);
  label->setGeometry(20,304,300,16);
  air_card_sel[4]=new RDCardSelector(this);
  air_card_sel[4]->setGeometry(15,322,120,117);
  air_start_rml_edit[4]=new QLineEdit(this);
  air_start_rml_edit[4]->setGeometry(205,322,160,19);
  air_start_rml_edit[4]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[4],tr("Start RML:"),this);
  label->setGeometry(135,322,65,19);
  label->setAlignment(AlignVCenter|AlignRight);
  air_stop_rml_edit[4]=new QLineEdit(this);
  air_stop_rml_edit[4]->setGeometry(205,343,160,19);
  air_stop_rml_edit[4]->setValidator(validator);
  label=new QLabel(air_start_rml_edit[4],tr("Stop RML:"),this);
  label->setGeometry(135,343,65,19);
  label->setAlignment(AlignVCenter|AlignRight);

  //
  // Display Settings Section
  //
  label=new QLabel(tr("Display Settings"),this);
  label->setFont(big_font);
  label->setGeometry(20,378,200,16);

  //
  // Skin Path
  //
  air_skin_edit=new QLineEdit(this);
  air_skin_edit->setGeometry(135,400,180,20);
  label=new QLabel(air_skin_edit,tr("Background Image:"),this);
  label->setGeometry(10,400,115,20);
  label->setAlignment(AlignRight|AlignVCenter);
  QPushButton *button=new QPushButton(tr("Select"),this);
  button->setGeometry(320,398,50,25);
  connect(button,SIGNAL(clicked()),this,SLOT(selectSkinData()));

  //
  // Sound Panel Section
  //
  label=new QLabel(tr("Sound Panel Settings"),this);
  label->setFont(big_font);
//  label->setGeometry(430,179,200,16);
  label->setGeometry(430,10,200,16);

  //
  // # of Station Panels
  //
  air_station_box=new QSpinBox(this);
  air_station_box->setGeometry(510,35,50,20);
  air_station_box->setRange(0,MAX_PANELS);
  air_station_box->setSpecialValueText(tr("None"));
  air_station_label=new QLabel(air_station_box,tr("Host Panels:"),this);
  air_station_label->setGeometry(405,35,100,20);
  air_station_label->setAlignment(AlignRight|AlignVCenter);

  //
  // # of User Panels
  //
  air_user_box=new QSpinBox(this);
  air_user_box->setGeometry(510,57,50,20);
  air_user_box->setRange(0,MAX_PANELS);
  air_user_box->setSpecialValueText(tr("None"));
  air_user_label=new QLabel(air_user_box,tr("User Panels:"),this);
  air_user_label->setGeometry(405,57,100,20);
  air_user_label->setAlignment(AlignRight|AlignVCenter);

  //
  // Flash Active Button
  //
  air_flash_box=new QCheckBox(this);
  air_flash_box->setGeometry(435,85,15,15);
  label=new QLabel(air_flash_box,tr("Flash Active Buttons"),this);
  label->setGeometry(455,85,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Pause Panel Button
  //
  air_panel_pause_box=new QCheckBox(this);
  air_panel_pause_box->setGeometry(435,107,15,15);
  label=new QLabel(air_panel_pause_box,tr("Enable Button Pausing"),this);
  label->setGeometry(455,107,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Clear Cart Filter
  //
  air_clearfilter_box=new QCheckBox(this);
  air_clearfilter_box->setGeometry(435,129,15,15);
  label=new QLabel(air_clearfilter_box,tr("Clear Cart Search Filter"),this);
  label->setGeometry(455,129,150,15);
  label->setAlignment(AlignLeft|AlignVCenter);

  //
  // Default Service
  //
  air_defaultsvc_box=new QComboBox(this);
  air_defaultsvc_box->setGeometry(520,151,100,20);
  label=new QLabel(air_defaultsvc_box,tr("Default Service:"),this);
  label->setGeometry(385,151,130,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  // Label Template
  //
  air_label_template_edit=new QLineEdit(this);
  air_label_template_edit->setGeometry(520,173,sizeHint().width()-530,20);
  label=new QLabel(air_label_template_edit,tr("Label Template:"),this);
  label->setGeometry(415,173,100,20);
  label->setAlignment(AlignRight|AlignVCenter);

  //
  //  Ok Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-180,sizeHint().height()-60,80,50);
  button->setDefault(true);
  button->setFont(small_font);
  button->setText(tr("&OK"));
  connect(button,SIGNAL(clicked()),this,SLOT(okData()));

  //
  //  Cancel Button
  //
  button=new QPushButton(this);
  button->setGeometry(sizeHint().width()-90,sizeHint().height()-60,80,50);
  button->setFont(small_font);
  button->setText(tr("&Cancel"));
  connect(button,SIGNAL(clicked()),this,SLOT(cancelData()));

  //
  // Populate Fields
  //
  if(station->scanned()) {
    for(int i=0;i<PANEL_MAX_OUTPUTS;i++) {
      air_card_sel[i]->setMaxCards(cae_station->cards());
      for(int j=0;j<air_card_sel[i]->maxCards();j++) {
	air_card_sel[i]->setMaxPorts(j,cae_station->cardOutputs(j));
      }
    }
  }
  else {
    QMessageBox::information(this,tr("No Audio Configuration Data"),
			    tr("Channel assignments will not be available for this host, as audio resource data\nhas not yet been generated.  Please start the Rivendell daemons on this host\n(by executing, as user 'root',  the command \"/etc/init.d/rivendell start\")\nin order to populate the audio resources database."));
    for(int i=0;i<PANEL_MAX_OUTPUTS;i++) {
      air_card_sel[i]->setDisabled(true);
    }
  }
  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    air_card_sel[i]->
      setCard(air_channels->card(RDChannels::PanelButtonOutput,i));
    air_card_sel[i]->
      setPort(air_channels->port(RDChannels::PanelButtonOutput,i));
    air_start_rml_edit[i]->
      setText(air_channels->rml(RDChannels::PanelButtonOutput,
				RDChannels::Start,i));
    air_stop_rml_edit[i]->
      setText(air_channels->rml(RDChannels::PanelButtonOutput,
				RDChannels::Stop,i));
  }
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
  air_skin_edit->setText(air_conf->skinPath());
  air_station_box->setValue(air_conf->panels(RDAirPlayConf::StationPanel));
  air_user_box->setValue(air_conf->panels(RDAirPlayConf::UserPanel));
  air_clearfilter_box->setChecked(air_conf->clearFilter());
  air_flash_box->setChecked(air_conf->flashPanel());
  air_panel_pause_box->setChecked(air_conf->panelPauseEnabled());
  air_label_template_edit->setText(air_conf->buttonLabelTemplate());
}


EditRDPanel::~EditRDPanel()
{
}


QSize EditRDPanel::sizeHint() const
{
  return QSize(630,496);
} 


QSizePolicy EditRDPanel::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditRDPanel::selectSkinData()
{
 QString filename=air_skin_edit->text();
  filename=QFileDialog::getOpenFileName(filename,RD_IMAGE_FILE_FILTER,this,"",
					tr("Select Image File"));
  if(!filename.isNull()) {
    air_skin_edit->setText(filename);
  }
}


void EditRDPanel::okData()
{
  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    air_channels->
      setCard(air_card_sel[i]->card(),RDChannels::PanelButtonOutput,i);
    air_channels->
      setPort(air_card_sel[i]->port(),RDChannels::PanelButtonOutput,i);
    air_channels->
      setRml(air_start_rml_edit[i]->text(),RDChannels::PanelButtonOutput,
	     RDChannels::Start,i);
    air_channels->
      setRml(air_stop_rml_edit[i]->text(),RDChannels::PanelButtonOutput,
	     RDChannels::Stop,i);
  }
  air_conf->setSkinPath(air_skin_edit->text());
  if(air_defaultsvc_box->currentItem()==0) {
    air_conf->setDefaultSvc("");
  }
  else {
    air_conf->setDefaultSvc(air_defaultsvc_box->currentText());
  }
  air_conf->setPanels(RDAirPlayConf::StationPanel,air_station_box->value());
  air_conf->setPanels(RDAirPlayConf::UserPanel,air_user_box->value());
  air_conf->setClearFilter(air_clearfilter_box->isChecked());
  air_conf->setFlashPanel(air_flash_box->isChecked());
  air_conf->setPanelPauseEnabled(air_panel_pause_box->isChecked());
  air_conf->setButtonLabelTemplate(air_label_template_edit->text());

  done(0);
}


void EditRDPanel::cancelData()
{
  done(1);
}
