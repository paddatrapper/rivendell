// edit_panelchannels.cpp
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

#include <qsignalmapper.h>

#include <rdtextvalidator.h>

#include "edit_channelgpios.h"
#include "edit_panelchannels.h"

EditPanelChannels::EditPanelChannels(RDStation *cae_station,RDChannels *chans,
				     RDChannels::Channel type,QWidget *parent)
  : QWidget(parent)
{
  air_cae_station=cae_station;
  air_channels=chans;
  air_channel_type=type;

  QFont small_font=QFont("Helvetica",12,QFont::Bold);
  small_font.setPixelSize(12);

  //
  // Text Validator
  //
  RDTextValidator *validator=new RDTextValidator(this);

  QSignalMapper *mapper=new QSignalMapper(this);
  connect(mapper,SIGNAL(mapped(int)),this,SLOT(editGpiosData(int)));

  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    air_panel_card_label[i]=new QLabel(this);
    air_panel_card_label[i]->setFont(small_font);
    air_panel_card_label[i]->setGeometry(5,i*68,300,16);
    air_panel_card_sel[i]=new RDCardSelector(this);
    air_panel_card_sel[i]->setId(i);
    air_panel_card_sel[i]->setGeometry(0,18+i*68,120,117);
    air_panel_start_rml_edit[i]=new QLineEdit(this);
    air_panel_start_rml_edit[i]->setGeometry(190,18+i*68,95,19);
    air_panel_start_rml_edit[i]->setValidator(validator);
    air_panel_start_rml_label[i]=
      new QLabel(air_panel_start_rml_edit[i],tr("Start RML:"),this);
    air_panel_start_rml_label[i]->setGeometry(120,18+i*68,65,19);
    air_panel_start_rml_label[i]->setAlignment(AlignVCenter|AlignRight);
    air_panel_stop_rml_edit[i]=new QLineEdit(this);
    air_panel_stop_rml_edit[i]->setGeometry(190,39+i*68,95,19);
    air_panel_stop_rml_edit[i]->setValidator(validator);
    air_panel_stop_rml_label[i]=
      new QLabel(air_panel_start_rml_edit[i],tr("Stop RML:"),this);
    air_panel_stop_rml_label[i]->setGeometry(120,39+i*68,65,19);
    air_panel_stop_rml_label[i]->setAlignment(AlignVCenter|AlignRight);

    air_panel_channel_button[i]=new QPushButton(tr("Edit\nGPIOs"),this);
    air_panel_channel_button[i]->setGeometry(290,14+i*68,60,50);
    air_panel_channel_button[i]->setFont(small_font);
    mapper->setMapping(air_panel_channel_button[i],i);
    connect(air_panel_channel_button[i],SIGNAL(clicked()),mapper,SLOT(map()));
    connect(air_panel_card_sel[i],SIGNAL(settingsChanged(int,int,int)),
	    this,SLOT(audioSettingsChangedData(int,int,int)));
  }
  air_panel_card_label[0]->setText(tr("SoundPanel First Play Output"));
  air_panel_card_label[1]->setText(tr("SoundPanel Second Play Output"));
  air_panel_card_label[2]->setText(tr("SoundPanel Third Play Output"));
  air_panel_card_label[3]->setText(tr("SoundPanel Fourth Play Output"));
  air_panel_card_label[4]->
    setText(tr("SoundPanel Fifth and Later Play Output"));
}


EditPanelChannels::~EditPanelChannels()
{
  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    delete air_panel_card_label[i];
    delete air_panel_card_sel[i];
    delete air_panel_start_rml_edit[i];
    delete air_panel_stop_rml_edit[i];
    delete air_panel_channel_button[i];
  }
}


QSize EditPanelChannels::sizeHint() const
{
  return QSize(350,337);
}


void EditPanelChannels::load()
{
  if(air_cae_station->scanned()) {
    for(int i=0;i<PANEL_MAX_OUTPUTS;i++) {
      air_panel_card_sel[i]->setMaxCards(air_cae_station->cards());
      for(int j=0;j<air_panel_card_sel[i]->maxCards();j++) {
	air_panel_card_sel[i]->setMaxPorts(j,air_cae_station->cardOutputs(j));
      }
    }
  }
  else {
    for(int i=0;i<PANEL_MAX_OUTPUTS;i++) {
      air_panel_card_sel[i]->setDisabled(true);
    }
  }
  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    air_panel_card_sel[i]->
      setCard(air_channels->card(RDChannels::AirplayButtonOutput,i));
    air_panel_card_sel[i]->
      setPort(air_channels->port(RDChannels::AirplayButtonOutput,i));
    air_panel_start_rml_edit[i]->setText(air_channels->rml(air_channel_type,RDChannels::Start,i));
    air_panel_stop_rml_edit[i]->setText(air_channels->rml(air_channel_type,RDChannels::Stop,i));
    audioSettingsChangedData(i,air_panel_card_sel[i]->card(),
			     air_panel_card_sel[i]->port());
  }
}


void EditPanelChannels::save()
{
  for(unsigned i=0;i<PANEL_MAX_OUTPUTS;i++) {
    air_channels->setRml(air_panel_start_rml_edit[i]->text(),air_channel_type,RDChannels::Start,i);
    air_channels->setRml(air_panel_stop_rml_edit[i]->text(),air_channel_type,RDChannels::Stop,i);
    air_channels->setCard(air_panel_card_sel[i]->card(),air_channel_type,i);
    air_channels->setPort(air_panel_card_sel[i]->port(),air_channel_type,i);
    if(air_panel_card_sel[i]->isDisabled()) {
      air_channels->setGpioMatrix(-1,RDChannels::Start,RDChannels::Gpi,air_channel_type,i);
      air_channels->setGpioMatrix(-1,RDChannels::Start,RDChannels::Gpo,air_channel_type,i);
      air_channels->setGpioMatrix(-1,RDChannels::Stop,RDChannels::Gpi,air_channel_type,i);
      air_channels->setGpioMatrix(-1,RDChannels::Stop,RDChannels::Gpo,air_channel_type,i);
    }
  }
}


QSizePolicy EditPanelChannels::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}
  

void EditPanelChannels::audioSettingsChangedData(int id,int card,int port)
{
  if(air_panel_channel_button[id]!=NULL) {
    bool state=air_panel_card_sel[id]->isDisabled();
    air_panel_start_rml_label[id]->setDisabled(state);
    air_panel_start_rml_edit[id]->setDisabled(state);
    air_panel_stop_rml_label[id]->setDisabled(state);
    air_panel_stop_rml_edit[id]->setDisabled(state);
    air_panel_channel_button[id]->
      setDisabled(air_panel_card_sel[id]->isDisabled());
  }
}


void EditPanelChannels::editGpiosData(int num)
{
  EditChannelGpios *d=new EditChannelGpios(air_channels,air_channel_type,this);
  d->exec(num,0);
  delete d;
}
