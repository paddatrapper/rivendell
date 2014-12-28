// list_nownextplugins.cpp
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

#include <rdescape_string.h>

#include "edit_nownextplugin.h"
#include "list_nownextplugins.h"

ListNowNextPlugins::ListNowNextPlugins(QWidget *parent)
  : QWidget(parent)
{
  QFont font=QFont("Helvetica",12,QFont::Bold);
  font.setPixelSize(12);

  //
  // Plugin List
  //
  list_view=new RDListView(this);
  list_view->setItemMargin(5);
  list_view->addColumn(tr("Path"));
  list_view->setColumnAlignment(0,AlignLeft|AlignVCenter);
  list_view->addColumn(tr("Argument"));
  list_view->setColumnAlignment(1,AlignLeft|AlignVCenter);
  list_view->setAllColumnsShowFocus(true);
  /*
  connect(list_view,
	  SIGNAL(doubleClicked(QListViewItem *,const QPoint &,int)),
	  this,
	  SLOT(doubleClickedData(QListViewItem *,const QPoint &,int)));
  */
  list_add_button=new QPushButton(tr("Add"),this);
  list_add_button->setFont(font);
  connect(list_add_button,SIGNAL(clicked()),this,SLOT(addData()));

  list_edit_button=new QPushButton(tr("Edit"),this);
  list_edit_button->setFont(font);
  connect(list_edit_button,SIGNAL(clicked()),this,SLOT(editData()));

  list_delete_button=new QPushButton(tr("Delete"),this);
  list_delete_button->setFont(font);
  connect(list_delete_button,SIGNAL(clicked()),
	  this,SLOT(deleteData()));
}


ListNowNextPlugins::~ListNowNextPlugins()
{
}


QSize ListNowNextPlugins::sizeHint() const
{
  return QSize(380,155);
}


QSizePolicy ListNowNextPlugins::sizePolicy() const
{
  return QSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
}


void ListNowNextPlugins::load(const QString &station_name)
{
  QString sql;
  RDSqlQuery *q;
  RDListViewItem *item=NULL;

  sql=QString("select ID,PLUGIN_PATH,PLUGIN_ARG from NOWNEXT_PLUGINS where ")+
    "STATION_NAME=\""+RDEscapeString(station_name)+"\"";
  q=new RDSqlQuery(sql);
  while(q->next()) {
    item=new RDListViewItem(list_view);
    item->setId(q->value(0).toInt());
    item->setText(0,q->value(1).toString());
    item->setText(1,q->value(2).toString());
  }
  delete q;
}


void ListNowNextPlugins::save(const QString &station_name)
{
  QString sql;
  RDSqlQuery *q;
  RDListViewItem *item=NULL;

  sql=QString("delete from NOWNEXT_PLUGINS where ")+
    "STATION_NAME=\""+RDEscapeString(station_name)+"\"";
  q=new RDSqlQuery(sql);
  delete q;
  item=(RDListViewItem *)list_view->firstChild();
  while(item!=NULL) {
    sql=QString("insert into NOWNEXT_PLUGINS set ")+
      "STATION_NAME=\""+RDEscapeString(station_name)+"\","+
      "PLUGIN_PATH=\""+RDEscapeString(item->text(0))+"\","+
      "PLUGIN_ARG=\""+RDEscapeString(item->text(1))+"\"";
    q=new RDSqlQuery(sql);
    delete q;
    item=(RDListViewItem *)item->nextSibling();
  }
}


void ListNowNextPlugins::addData()
{
  QString path;
  QString arg;
  EditNowNextPlugin *d=new EditNowNextPlugin(&path,&arg,this);
  if(d->exec()==0) {
    RDListViewItem *item=new RDListViewItem(list_view);
    item->setId(-1);
    item->setText(0,path);
    item->setText(1,arg);
    list_view->ensureItemVisible(item);
  }
  delete d;
}


void ListNowNextPlugins::editData()
{
  RDListViewItem *item=
    (RDListViewItem *)list_view->selectedItem();
  if(item==NULL) {
    return;
  }
  QString path=item->text(0);
  QString arg=item->text(1);
  EditNowNextPlugin *d=new EditNowNextPlugin(&path,&arg,this);
  if(d->exec()==0) {
    item->setText(0,path);
    item->setText(1,arg);
  }
  delete d;  
}


void ListNowNextPlugins::deleteData()
{
  RDListViewItem *item=(RDListViewItem *)list_view->selectedItem();
  if(item==NULL) {
    return;
  }
  delete item;
}


void ListNowNextPlugins::resizeEvent(QResizeEvent *e)
{
  list_view->setGeometry(0,0,size().width(),125);
  list_add_button->setGeometry(size().width()-210,130,60,25);
  list_edit_button->setGeometry(size().width()-140,130,60,25);
  list_delete_button->setGeometry(size().width()-70,130,60,25);
}
