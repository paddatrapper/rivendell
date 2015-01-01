// edit_logchannels.cpp
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

#include <qsignalmapper.h>

#include <rdtextvalidator.h>

#include "edit_logchannels.h"

EditLogChannels::EditLogChannels(RDAirPlayConf *conf,RDStation *cae_station,
				 RDChannels *chans,QWidget *parent)
  : QWidget(parent)
{
  air_loaded_mach=-1;
  air_rdairplay_conf=conf;
  air_cae_station=cae_station;
  air_channels=chans;

  //
  // Fonts
  //
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Log Quantity
  //
  air_log_quan_label=new QLabel(tr("Visible Log Machines")+":",this);
  air_log_quan_label->setFont(label_font);
  air_log_quan_label->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
  air_log_quan_spin=new QSpinBox(this);
  air_log_quan_spin->setRange(1,RDAIRPLAY_LOG_QUANTITY);

  //
  // Log Selector
  //
  air_log_box=new QComboBox(this);
  air_log_box->insertItem(tr("Main Log"));
  for(int i=1;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    air_log_box->insertItem(tr("Aux Log")+QString().sprintf(" %u",i));
  }
  connect(air_log_box,SIGNAL(activated(int)),this,SLOT(logChangedData(int)));

  //
  // Text Validator
  //
  RDTextValidator *validator=new RDTextValidator(this);

  QSignalMapper *mapper=new QSignalMapper(this);
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(editGpiosData(int)));
  for(int i=0;i<2;i++) {
    air_log_label[i]=new QLabel(tr("Output")+QString().sprintf(" %d",i+1),this);
    air_log_label[i]->setFont(label_font);
    air_log_card_sel[i]=new RDCardSelector(this);
    air_log_card_sel[i]->setId(i);
    air_log_start_rml_edit[i]=new QLineEdit(this);
    air_log_start_rml_edit[i]->setValidator(validator);
    air_log_start_rml_label[i]=
      new QLabel(air_log_start_rml_edit[i],tr("Start RML:"),this);
    air_log_start_rml_label[i]->setAlignment(AlignVCenter|AlignRight);
    air_log_stop_rml_edit[i]=new QLineEdit(this);
    air_log_stop_rml_edit[i]->setValidator(validator);
    air_log_stop_rml_label[i]=
      new QLabel(air_log_start_rml_edit[i],tr("Stop RML:"),this);
    air_log_stop_rml_label[i]->setAlignment(AlignVCenter|AlignRight);
    
    air_log_channel_button[i]=new QPushButton(tr("Edit\nGPIOs"),this);
    air_log_channel_button[i]->setFont(label_font);
    mapper->setMapping(air_log_channel_button[i],i);
    connect(air_log_channel_button[i],SIGNAL(clicked()),mapper,SLOT(map()));
    connect(air_log_card_sel[i],SIGNAL(settingsChanged(int,int,int)),
	    this,SLOT(audioSettingsChangedData(int,int,int)));
  }
  air_gpios_dialog=
    new EditChannelGpios(air_channels,RDChannels::AirplayLogOutput,this);

  if(air_cae_station->scanned()) {
    for(int i=0;i<2;i++) {
      air_log_card_sel[i]->setMaxCards(air_cae_station->cards());
      for(int j=0;j<air_log_card_sel[i]->maxCards();j++) {
	air_log_card_sel[i]->setMaxPorts(j,air_cae_station->cardOutputs(j));
      }
    }
  }
  else {
    for(int i=0;i<2;i++) {
      air_log_card_sel[i]->setDisabled(true);
    }
  }

  air_log_quan_spin->setValue(conf->logQuantity());

  LoadMachine(0);
}


EditLogChannels::~EditLogChannels()
{
  /*
  delete air_log_quan_label;
  delete air_log_quan_spin;
  delete air_log_box;
  delete air_log_card_sel;
  delete air_log_start_rml_edit;
  delete air_log_start_rml_label;
  delete air_log_stop_rml_edit;
  delete air_log_stop_rml_label;
  delete air_log_channel_button;
  */
}


QSize EditLogChannels::sizeHint() const
{
  return QSize(350,182);
}


QSizePolicy EditLogChannels::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditLogChannels::save()
{
  air_rdairplay_conf->setLogQuantity(air_log_quan_spin->value());
  SaveMachine(air_log_box->currentItem());
}


void EditLogChannels::logChangedData(int value)
{
  if(air_loaded_mach>=0) {
    SaveMachine(air_loaded_mach);
  }
  LoadMachine(value);
  air_loaded_mach=value;
}


void EditLogChannels::audioSettingsChangedData(int id,int card,int port)
{
  air_log_start_rml_edit[id]->setEnabled(air_log_card_sel[id]->isActive());
  air_log_start_rml_label[id]->setEnabled(air_log_card_sel[id]->isActive());
  air_log_stop_rml_edit[id]->setEnabled(air_log_card_sel[id]->isActive());
  air_log_stop_rml_label[id]->setEnabled(air_log_card_sel[id]->isActive());
  air_log_channel_button[id]->setEnabled(air_log_card_sel[id]->isActive());
}


void EditLogChannels::editGpiosData(int sub_num)
{
  air_gpios_dialog->exec(air_log_box->currentItem(),sub_num);
}


void EditLogChannels::resizeEvent(QResizeEvent *e)
{
  air_log_quan_label->setGeometry(0,0,130,20);
  air_log_quan_spin->setGeometry(135,0,50,20);
  air_log_box->setGeometry(20,25,110,20);

  for(int i=0;i<2;i++) {
    air_log_label[i]->setGeometry(10,50+i*68,200,20);
    air_log_card_sel[i]->setGeometry(0,68+i*68,120,117);
    air_log_start_rml_edit[i]->setGeometry(190,68+i*68,95,19);
    air_log_start_rml_label[i]->setGeometry(120,68+i*68,65,19);
    air_log_stop_rml_edit[i]->setGeometry(190,89+i*68,95,19);
    air_log_stop_rml_label[i]->setGeometry(120,89+i*68,65,19);
    air_log_channel_button[i]->setGeometry(290,64+i*68,60,50);
  }
}


void EditLogChannels::LoadMachine(int mach)
{
  for(int i=0;i<2;i++) {
    air_log_card_sel[i]->
      setCard(air_channels->card(RDChannels::AirplayLogOutput,mach,i));
    air_log_card_sel[i]->
      setPort(air_channels->port(RDChannels::AirplayLogOutput,mach,i));
    
    air_log_start_rml_edit[i]->
      setText(air_channels->rml(RDChannels::AirplayLogOutput,
				RDChannels::Start,mach,i));
    air_log_stop_rml_edit[i]->
      setText(air_channels->rml(RDChannels::AirplayLogOutput,
				RDChannels::Stop,mach,i));
    audioSettingsChangedData(i,air_log_card_sel[i]->card(),
			     air_log_card_sel[i]->port());
  }
}


void EditLogChannels::SaveMachine(int mach)
{
  for(int i=0;i<2;i++) {
    air_channels->
      setCard(air_log_card_sel[i]->card(),RDChannels::AirplayLogOutput,mach,i);
    air_channels->
      setPort(air_log_card_sel[i]->port(),RDChannels::AirplayLogOutput,mach,i);

    air_channels->
      setRml(air_log_start_rml_edit[i]->text(),RDChannels::AirplayLogOutput,
	     RDChannels::Start,mach,i);
    air_channels->
      setRml(air_log_stop_rml_edit[i]->text(),RDChannels::AirplayLogOutput,
	     RDChannels::Stop,mach,i);
  }
}
