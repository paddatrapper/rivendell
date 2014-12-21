// rdchannels.cpp
//
// Interface for storing/querying channel assignments.
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

#include "rdconf.h"
#include "rddb.h"
#include "rdescape_string.h"
#include "rdchannels.h"

RDChannels::RDChannels(const QString &station)
{
  chan_station=station;
}


int RDChannels::card(Channel chan,int num,int sub_num) const
{
  QVariant ret=ReadField("CARD",chan,num,sub_num);
  if(ret.isNull()) {
    return -1;
  }
  return ret.toInt();
}


void RDChannels::setCard(int card,Channel chan,int num,int sub_num) const
{
  WriteRow(card,"CARD",chan,num,sub_num);
}


int RDChannels::port(Channel chan,int num,int sub_num) const
{
  QVariant ret=ReadField("PORT",chan,num,sub_num);
  if(ret.isNull()) {
    return -1;
  }
  return ret.toInt();
}


void RDChannels::setPort(int port,Channel chan,int num,int sub_num) const
{
  WriteRow(port,"PORT",chan,num,sub_num);
}


QString RDChannels::rml(Channel chan,Action act,int num,int sub_num) const
{
  return ReadField(ActionString(act)+"_RML",chan,num,sub_num).toString();
}


void RDChannels::setRml(const QString &rml,Channel chan,Action act,int num,
			int sub_num) const
{
  WriteRow(rml,ActionString(act)+"_RML",chan,num,sub_num);
}


RDChannels::GpioType RDChannels::gpioType(RDChannels::Channel chan,int num,
					  int sub_num) const
{
  return (RDChannels::GpioType)ReadField("GPIO_TYPE",chan,num,sub_num).toInt();
}


void RDChannels::setGpioType(RDChannels::GpioType type,RDChannels::Channel chan,
			     int num,int sub_num) const
{
  WriteRow((int)type,"GPIO_TYPE",chan,num,sub_num);
}


int RDChannels::gpioMatrix(RDChannels::Action act,RDChannels::GpioDirection dir,
			   RDChannels::Channel chan,int num,int sub_num) const
{
  return ReadField(ActionString(act)+"_"+DirectionString(dir)+"_MATRIX",
		   chan,num,sub_num).toInt();
}


void RDChannels::setGpioMatrix(int matrix,RDChannels::Action act,
			       RDChannels::GpioDirection dir,
			       RDChannels::Channel chan,int num,
			       int sub_num) const
{
  WriteRow(matrix,ActionString(act)+"_"+DirectionString(dir)+"_MATRIX",
	   chan,num,sub_num);
}


int RDChannels::gpioLine(RDChannels::Action act,RDChannels::GpioDirection dir,
			 RDChannels::Channel chan,int num,int sub_num) const
{
  return ReadField(ActionString(act)+"_"+DirectionString(dir)+"_LINE",
		   chan,num,sub_num).toInt();
}


void RDChannels::setGpioLine(int line,RDChannels::Action act,
			     RDChannels::GpioDirection dir,
			     RDChannels::Channel chan,int num,
			     int sub_num) const
{
  WriteRow(line,ActionString(act)+"_"+DirectionString(dir)+"_LINE",
	   chan,num,sub_num);
}


unsigned RDChannels::cart(Action act,Channel chan,int num,int sub_num) const
{
  return ReadField(ActionString(act)+"_CART",chan,num,sub_num).toInt();
}


void RDChannels::setCart(unsigned cartnum,Action act,Channel chan,int num,
			 int sub_num) const
{
  WriteRow(cartnum,ActionString(act)+"_CART",chan,num,sub_num);
}


QVariant RDChannels::ReadField(const QString &param,RDChannels::Channel chan,
			       int num,int sub_num) const
{
  QString sql;
  RDSqlQuery *q;
  QVariant ret;

  sql=QString("select ")+param+" from CHANNELS where "+
    "(STATION_NAME=\""+RDEscapeString(chan_station)+"\")&&"+
    QString().sprintf("(CHANNEL=%d)&&",chan)+
    QString().sprintf("(NUMBER=%d)&&",num)+
    QString().sprintf("(SUB_NUMBER=%d)",sub_num);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    ret=q->value(0);
  }
  delete q;

  return ret;
}


void RDChannels::WriteRow(int value,const QString &param,Channel chan,int num,
			  int sub_num) const
{
  QString sql;
  RDSqlQuery *q;
  RDSqlQuery *q1;

  sql=QString("select ID from CHANNELS where ")+
    "(STATION_NAME=\""+RDEscapeString(chan_station)+"\")&&"+
    QString().sprintf("(CHANNEL=%d)&&",chan)+
    QString().sprintf("(NUMBER=%d)&&",num)+
    QString().sprintf("(SUB_NUMBER=%d)",sub_num);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    sql=QString("update CHANNELS set ")+param+QString().sprintf("=%d ",value)+
      QString().sprintf("where ID=%u",q->value(0).toUInt());
    q1=new RDSqlQuery(sql);
    delete q1;
  }
  else {
    sql=QString("insert into CHANNELS set ")+
      param+QString().sprintf("=%d,",value)+
      "STATION_NAME=\""+RDEscapeString(chan_station)+"\","+
      QString().sprintf("CHANNEL=%d,",chan)+
      QString().sprintf("NUMBER=%d,",num)+
      QString().sprintf("SUB_NUMBER=%d",sub_num);
    q1=new RDSqlQuery(sql);
    delete q1;
  }
  delete q;
}


void RDChannels::WriteRow(const QString &value,const QString &param,
			  Channel chan,int num,int sub_num) const
{
  QString sql;
  RDSqlQuery *q;
  RDSqlQuery *q1;

  sql=QString("select ID from CHANNELS where ")+
    "(STATION_NAME=\""+RDEscapeString(chan_station)+"\")&&"+
    QString().sprintf("(CHANNEL=%d)&&",chan)+
    QString().sprintf("(NUMBER=%d)&&",num)+
    QString().sprintf("(SUB_NUMBER=%d)",sub_num);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    sql=QString("update CHANNELS set ")+
      param+"=\""+RDEscapeString(value)+"\" "+
      QString().sprintf("where ID=%u",q->value(0).toUInt());
    q1=new RDSqlQuery(sql);
    delete q1;
  }
  else {
    sql=QString("insert into CHANNELS set ")+
      param+"=\""+RDEscapeString(value)+"\","+
      "STATION_NAME=\""+RDEscapeString(chan_station)+"\","+
      QString().sprintf("CHANNEL=%d,",chan)+
      QString().sprintf("NUMBER=%d,",num)+
      QString().sprintf("SUB_NUMBER=%d",sub_num);
    q1=new RDSqlQuery(sql);
    delete q1;
  }
  delete q;
}


QString RDChannels::DirectionString(RDChannels::GpioDirection dir) const
{
  QString ret;

  switch(dir) {
  case RDChannels::Gpi:
    ret="GPI";
    break;

  case RDChannels::Gpo:
    ret="GPO";
    break;
  }
  return ret;
}


QString RDChannels::ActionString(RDChannels::Action act) const
{
  QString ret;

  switch(act) {
  case RDChannels::Start:
    ret="START";
    break;

  case RDChannels::Stop:
    ret="STOP";
    break;
  }
  return ret;
}
