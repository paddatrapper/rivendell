// list_nownextplugins.h
//
// List Rivendell RLM Configurations
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

#ifndef LIST_NOWNEXTPLUGINS_H
#define LIST_NOWNEXTPLUGINS_H

#include <qpixmap.h>
#include <qradiobutton.h>
#include <qpushbutton.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qwidget.h>

#include <rdlistview.h>
#include <rdlistviewitem.h>
#include <rddb.h>

class ListNowNextPlugins : public QWidget
{
  Q_OBJECT
 public:
  ListNowNextPlugins(QWidget *parent=0);
  ~ListNowNextPlugins();
  QSize sizeHint() const;
  QSizePolicy sizePolicy() const;
  void load(const QString &station_name);
  void save(const QString &station_name);

 private slots:
  void addData();
  void editData();
  void deleteData();

 protected:
  void resizeEvent(QResizeEvent *e);

 private:
  RDListView *list_view;
  QPushButton *list_add_button;
  QPushButton *list_edit_button;
  QPushButton *list_delete_button;
};


#endif  // LIST_NOWNEXTPLUGINS_H
