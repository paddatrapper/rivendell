// edit_channelgpios.h
//
// Edit Rivendell Channel GPIO Settings
//
//   (C) Copyright 2013 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: edit_channelgpios.h,v 1.1.2.2 2013/03/13 15:18:05 cvs Exp $
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

#ifndef EDIT_CHANNELGPIOS_H
#define EDIT_CHANNELGPIOS_H

#include <qdialog.h>
#include <qspinbox.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcombobox.h>

#include <rdchannels.h>

class EditChannelGpios : public QDialog
{
 Q_OBJECT
 public:
  EditChannelGpios(RDChannels *chan,RDChannels::Channel type,QWidget *parent=0);
  QSize sizeHint() const;

 public slots:
  int exec(int num,int sub_num);

 protected:
  void resizeEvent(QResizeEvent *e);

 private slots:
  void startMatrixGpiChangedData(int n);
  void startMatrixGpoChangedData(int n);
  void stopMatrixGpiChangedData(int n);
  void stopMatrixGpoChangedData(int n);
  void okData();
  void cancelData();

 private:
  QLabel *edit_title_label;
  QLabel *edit_start_gpi_label;
  QSpinBox *edit_start_gpi_matrix_spin;
  QSpinBox *edit_start_gpi_line_spin;
  QLabel *edit_start_gpo_label;
  QSpinBox *edit_start_gpo_matrix_spin;
  QSpinBox *edit_start_gpo_line_spin;
  QLabel *edit_stop_gpi_label;
  QSpinBox *edit_stop_gpi_matrix_spin;
  QSpinBox *edit_stop_gpi_line_spin;
  QLabel *edit_stop_gpo_label;
  QSpinBox *edit_stop_gpo_matrix_spin;
  QSpinBox *edit_stop_gpo_line_spin;
  QLabel *edit_gpio_type_label;
  QComboBox *edit_gpio_type_box;
  QPushButton *edit_ok_button;
  QPushButton *edit_cancel_button;
  RDChannels *edit_channels;
  int edit_number;
  int edit_subnumber;
  RDChannels::Channel edit_channels_type;
};


#endif  // EDIT_CHANNELGPIOS

