// edit_modecontrol.cpp
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

#include "edit_modecontrol.h"

EditModeControl::EditModeControl(QWidget *parent)
  : QWidget(parent)
{
  edit_current_lognum=0;

  //
  // Mode Control Style
  //
  edit_style_box=new QComboBox(this);
  connect(edit_style_box,SIGNAL(activated(int)),
	  this,SLOT(styleChangedData(int)));
  edit_style_label=new QLabel(edit_style_box,tr("Mode Control Style:"),this);
  edit_style_label->setAlignment(AlignRight|AlignVCenter);
  edit_style_box->insertItem(tr("Unified"));
  edit_style_box->insertItem(tr("Independent"));

  //
  // Log Selector
  //
  edit_log_box=new QComboBox(this);
  connect(edit_log_box,SIGNAL(activated(int)),this,SLOT(logChangedData(int)));
  edit_log_box->insertItem(tr("Main Log"));
  for(int i=1;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    edit_log_box->insertItem(tr("Aux Log")+QString().sprintf("%d",i));
  }

  //
  // Startup Mode
  //
  edit_mode_box=new QComboBox(this);
  edit_mode_label=new QLabel(edit_mode_box,tr("Startup Mode")+":",this);
  edit_mode_label->setAlignment(AlignRight|AlignVCenter);
  connect(edit_mode_box,SIGNAL(activated(int)),this,SLOT(modeChangedData(int)));
  edit_mode_box->insertItem(tr("Previous"));
  edit_mode_box->insertItem(tr("LiveAssist"));
  edit_mode_box->insertItem(tr("Automatic"));
  edit_mode_box->insertItem(tr("Manual"));
}


EditModeControl::~EditModeControl()
{
}


QSize EditModeControl::sizeHint() const
{
  return QSize(275,64);
}


QSizePolicy EditModeControl::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void EditModeControl::load(RDAirPlayConf *conf,RDLogModes *modes)
{
  edit_style_box->setCurrentItem(conf->opModeStyle());
  for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    edit_log_modes[i]=modes->logStartMode(i);
  }
  edit_current_lognum=0;
  edit_log_box->setCurrentItem(edit_current_lognum);
  edit_mode_box->setCurrentItem((int)edit_log_modes[edit_current_lognum]);
  styleChangedData(edit_style_box->currentItem());
}


void EditModeControl::save(RDAirPlayConf *conf,RDLogModes *modes)
{
  conf->
    setOpModeStyle((RDAirPlayConf::OpModeStyle)edit_style_box->currentItem());
  for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
    modes->setLogStartMode(i,edit_log_modes[i]);
  }
}


void EditModeControl::styleChangedData(int mode)
{
  switch((RDAirPlayConf::OpModeStyle)mode) {
  case RDAirPlayConf::Independent:
    edit_log_box->setEnabled(true);
    break;

  case RDAirPlayConf::Unified:
    edit_log_box->setDisabled(true);
    for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
      edit_log_modes[i]=edit_log_modes[edit_current_lognum];
    }
    break;
  }
}


void EditModeControl::logChangedData(int lognum)
{
  edit_log_modes[edit_current_lognum]=
    (RDLogModes::OpMode)edit_mode_box->currentItem();
  edit_mode_box->setCurrentItem(edit_log_modes[lognum]);
  edit_current_lognum=lognum;
}


void EditModeControl::modeChangedData(int mode)
{
  switch((RDAirPlayConf::OpModeStyle)edit_style_box->currentItem()) {
  case RDAirPlayConf::Independent:
    edit_log_modes[edit_current_lognum]=(RDLogModes::OpMode)mode;
    break;

  case RDAirPlayConf::Unified:
    for(int i=0;i<RDAIRPLAY_LOG_QUANTITY;i++) {
      edit_log_modes[i]=(RDLogModes::OpMode)mode;
    }
    break;
  }
}


void EditModeControl::resizeEvent(QResizeEvent *e)
{
  edit_style_label->setGeometry(0,0,120,20);
  edit_style_box->setGeometry(125,0,110,20);
  edit_log_box->setGeometry(20,22,110,20);
  edit_mode_label->setGeometry(0,44,120,20);
  edit_mode_box->setGeometry(125,44,110,20);
}
