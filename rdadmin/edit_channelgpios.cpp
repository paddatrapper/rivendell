// edit_channelgpios.cpp
//
// Edit Rivendell Channel GPIO Settings
//
//   (C) Copyright 2013-2014 Fred Gleason <fredg@paravelsystems.com>
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

#include <rd.h>

#include "edit_channelgpios.h"

EditChannelGpios::EditChannelGpios(RDChannels *chans,
				   RDChannels::Channel type,QWidget *parent)
  : QDialog(parent,"",true)
{
  edit_channels=chans;
  edit_channels_type=type;

  setCaption(QString("RDAdmin - ")+tr("Edit Channel GPIOs"));

  //
  // Fonts
  //
  QFont title_font("helvetica",14,QFont::Bold);
  title_font.setPixelSize(14);
  QFont label_font("helvetica",12,QFont::Bold);
  label_font.setPixelSize(12);

  //
  // Title
  //
  edit_title_label=new QLabel("GPIO Settings",this);
  edit_title_label->setFont(title_font);
  edit_title_label->setAlignment(Qt::AlignCenter);

  //
  // Start GPI
  //
  edit_start_gpi_label=new QLabel(tr("Start GPI:"),this);
  edit_start_gpi_label->setFont(label_font);
  edit_start_gpi_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_start_gpi_matrix_spin=new QSpinBox(this);
  edit_start_gpi_matrix_spin->setRange(-1,MAX_MATRICES);
  edit_start_gpi_matrix_spin->setSpecialValueText(tr("None"));
  connect(edit_start_gpi_matrix_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(startMatrixGpiChangedData(int)));

  edit_start_gpi_line_spin=new QSpinBox(this);
  edit_start_gpi_line_spin->setRange(1,MAX_GPIO_PINS);
  if(type!=RDChannels::AirplayLogOutput) {
    edit_start_gpi_label->setDisabled(true);
    edit_start_gpi_matrix_spin->setDisabled(true);
    edit_start_gpi_line_spin->setDisabled(true);
  }

  //
  // Start GPO
  //
  edit_start_gpo_label=new QLabel(tr("Start GPO:"),this);
  edit_start_gpo_label->setFont(label_font);
  edit_start_gpo_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_start_gpo_matrix_spin=new QSpinBox(this);
  edit_start_gpo_matrix_spin->setRange(-1,MAX_MATRICES);
  edit_start_gpo_matrix_spin->setSpecialValueText(tr("None"));
  connect(edit_start_gpo_matrix_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(startMatrixGpoChangedData(int)));

  edit_start_gpo_line_spin=new QSpinBox(this);
  edit_start_gpo_line_spin->setRange(1,MAX_GPIO_PINS);
  /*
  if(type!=RDChannels::AirplayLogOutput) {
    edit_start_gpo_label->setDisabled(true);
    edit_start_gpo_matrix_spin->setDisabled(true);
    edit_start_gpo_line_spin->setDisabled(true);
  }
  */

  //
  // Stop GPI
  //
  edit_stop_gpi_label=new QLabel(tr("Stop GPI:"),this);
  edit_stop_gpi_label->setFont(label_font);
  edit_stop_gpi_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_stop_gpi_matrix_spin=new QSpinBox(this);
  edit_stop_gpi_matrix_spin->setRange(-1,MAX_MATRICES);
  edit_stop_gpi_matrix_spin->setSpecialValueText(tr("None"));
  connect(edit_stop_gpi_matrix_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(stopMatrixGpiChangedData(int)));

  edit_stop_gpi_line_spin=new QSpinBox(this);
  edit_stop_gpi_line_spin->setRange(1,MAX_GPIO_PINS);

  //
  // Stop GPO
  //
  edit_stop_gpo_label=new QLabel(tr("Stop GPO:"),this);
  edit_stop_gpo_label->setFont(label_font);
  edit_stop_gpo_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_stop_gpo_matrix_spin=new QSpinBox(this);
  edit_stop_gpo_matrix_spin->setRange(-1,MAX_MATRICES);
  edit_stop_gpo_matrix_spin->setSpecialValueText(tr("None"));
  connect(edit_stop_gpo_matrix_spin,SIGNAL(valueChanged(int)),
	  this,SLOT(stopMatrixGpoChangedData(int)));

  edit_stop_gpo_line_spin=new QSpinBox(this);
  edit_stop_gpo_line_spin->setRange(1,MAX_GPIO_PINS);

  //
  // Signaling Type
  //
  edit_gpio_type_label=new QLabel(tr("Signalling Type:"),this);
  edit_gpio_type_label->setFont(label_font);
  edit_gpio_type_label->setAlignment(Qt::AlignVCenter|Qt::AlignRight);

  edit_gpio_type_box=new QComboBox(this);
  edit_gpio_type_box->insertItem(tr("Edge"));
  edit_gpio_type_box->insertItem(tr("Level"));

  //
  // Buttons
  //
  edit_ok_button=new QPushButton(tr("OK"),this);
  edit_ok_button->setFont(label_font);
  connect(edit_ok_button,SIGNAL(clicked()),this,SLOT(okData()));

  edit_cancel_button=new QPushButton(tr("Cancel"),this);
  edit_cancel_button->setFont(label_font);
  connect(edit_cancel_button,SIGNAL(clicked()),this,SLOT(cancelData()));
}


QSize EditChannelGpios::sizeHint() const
{
  return QSize(300,227);
}


int EditChannelGpios::exec(int num,int sub_num)
{
  edit_number=num;
  edit_subnumber=sub_num;

  //
  // Load Values
  //
  if(edit_channels_type==RDChannels::AirplayLogOutput) {
    edit_start_gpi_matrix_spin->
      setValue(edit_channels->gpioMatrix(RDChannels::Start,RDChannels::Gpi,
					 edit_channels_type,num,sub_num));
    startMatrixGpiChangedData(edit_start_gpi_matrix_spin->value());
    edit_start_gpi_line_spin->
      setValue(edit_channels->gpioLine(RDChannels::Start,RDChannels::Gpi,
				     edit_channels_type,num,sub_num));
  }

  edit_start_gpo_matrix_spin->
    setValue(edit_channels->gpioMatrix(RDChannels::Start,RDChannels::Gpo,
				       edit_channels_type,num,sub_num));
  startMatrixGpoChangedData(edit_start_gpo_matrix_spin->value());
  edit_start_gpo_line_spin->
    setValue(edit_channels->gpioLine(RDChannels::Start,RDChannels::Gpo,
				     edit_channels_type,num,sub_num));

  edit_stop_gpi_matrix_spin->
    setValue(edit_channels->gpioMatrix(RDChannels::Stop,RDChannels::Gpi,
				       edit_channels_type,num,sub_num));
  stopMatrixGpiChangedData(edit_stop_gpi_matrix_spin->value());
  edit_stop_gpi_line_spin->
    setValue(edit_channels->gpioLine(RDChannels::Stop,RDChannels::Gpi,
				     edit_channels_type,num,sub_num));

  edit_stop_gpo_matrix_spin->
    setValue(edit_channels->gpioMatrix(RDChannels::Stop,RDChannels::Gpo,
				       edit_channels_type,num,sub_num));
  stopMatrixGpoChangedData(edit_stop_gpo_matrix_spin->value());
  edit_stop_gpo_line_spin->
    setValue(edit_channels->gpioLine(RDChannels::Stop,RDChannels::Gpo,
				     edit_channels_type,num,sub_num));

  edit_gpio_type_box->
    setCurrentItem(edit_channels->gpioType(edit_channels_type,num,sub_num));

  return QDialog::exec();
}


void EditChannelGpios::resizeEvent(QResizeEvent *e)
{
  edit_title_label->setGeometry(10,10,size().width()-20,20);

  edit_start_gpi_label->setGeometry(10,35,120,20);
  edit_start_gpi_matrix_spin->setGeometry(135,35,60,20);
  edit_start_gpi_line_spin->setGeometry(205,35,60,20);

  edit_start_gpo_label->setGeometry(10,57,120,20);
  edit_start_gpo_matrix_spin->setGeometry(135,57,60,20);
  edit_start_gpo_line_spin->setGeometry(205,57,60,20);

  edit_stop_gpi_label->setGeometry(10,79,120,20);
  edit_stop_gpi_matrix_spin->setGeometry(135,79,60,20);
  edit_stop_gpi_line_spin->setGeometry(205,79,60,20);

  edit_stop_gpo_label->setGeometry(10,101,120,20);
  edit_stop_gpo_matrix_spin->setGeometry(135,101,60,20);
  edit_stop_gpo_line_spin->setGeometry(205,101,60,20);

  edit_gpio_type_label->setGeometry(10,128,120,20);
  edit_gpio_type_box->setGeometry(135,128,80,20);

  edit_ok_button->setGeometry(size().width()-180,size().height()-60,80,50);
  edit_cancel_button->setGeometry(size().width()-90,size().height()-60,80,50);
}


void EditChannelGpios::startMatrixGpiChangedData(int n)
{
  edit_start_gpi_line_spin->setEnabled(n>=0);
}


void EditChannelGpios::startMatrixGpoChangedData(int n)
{
  edit_start_gpo_line_spin->setEnabled(n>=0);
}


void EditChannelGpios::stopMatrixGpiChangedData(int n)
{
  edit_stop_gpi_line_spin->setEnabled(n>=0);
}


void EditChannelGpios::stopMatrixGpoChangedData(int n)
{
  edit_stop_gpo_line_spin->setEnabled(n>=0);
}


void EditChannelGpios::okData()
{
  edit_channels->
    setGpioMatrix(edit_start_gpi_matrix_spin->value(),RDChannels::Start,
		  RDChannels::Gpi,edit_channels_type,edit_number,
		  edit_subnumber);
  edit_channels->
    setGpioLine(edit_start_gpi_line_spin->value(),RDChannels::Start,
		RDChannels::Gpi,edit_channels_type,edit_number,
		edit_subnumber);

  edit_channels->
    setGpioMatrix(edit_start_gpo_matrix_spin->value(),RDChannels::Start,
		  RDChannels::Gpo,edit_channels_type,edit_number,
		  edit_subnumber);
  edit_channels->
    setGpioLine(edit_start_gpo_line_spin->value(),RDChannels::Start,
		RDChannels::Gpo,edit_channels_type,edit_number,
		edit_subnumber);

  edit_channels->
    setGpioMatrix(edit_stop_gpi_matrix_spin->value(),RDChannels::Stop,
		  RDChannels::Gpi,edit_channels_type,edit_number,
		  edit_subnumber);
  edit_channels->
    setGpioLine(edit_stop_gpi_line_spin->value(),RDChannels::Stop,
		RDChannels::Gpi,edit_channels_type,edit_number,
		edit_subnumber);

  edit_channels->
    setGpioMatrix(edit_stop_gpo_matrix_spin->value(),RDChannels::Stop,
		  RDChannels::Gpo,edit_channels_type,edit_number,
		  edit_subnumber);
  edit_channels->
    setGpioLine(edit_stop_gpo_line_spin->value(),RDChannels::Stop,
		RDChannels::Gpo,edit_channels_type,edit_number,
		edit_subnumber);

  edit_channels->
    setGpioType((RDChannels::GpioType)edit_gpio_type_box->currentItem(),
		edit_channels_type,edit_number,edit_subnumber);

  done(0);
}


void EditChannelGpios::cancelData()
{
  done(-1);
}
