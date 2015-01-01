// rdlogmodes.cpp
//
// Log Mode Accessor Methods
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

#include <qobject.h>

#include <rddb.h>
#include <rdconf.h>
#include <rdlogmodes.h>
#include <rdescape_string.h>

#include "rdlogmodes.h"

RDLogModes::RDLogModes(const QString &station)
{
  air_station=station;
}


QString RDLogModes::station() const
{
  return air_station;
}


RDLogModes::OpMode RDLogModes::opMode(int lognum) const
{
  return (RDLogModes::OpMode)GetRow("OP_MODE",lognum).toInt();
}


void RDLogModes::setOpMode(int lognum,RDLogModes::OpMode mode) const
{
  SetRow("OP_MODE",lognum,(int)mode);
}


RDLogModes::OpMode RDLogModes::logStartMode(int lognum) const
{
  return (RDLogModes::OpMode)GetRow("START_MODE",lognum).toInt();
}


void RDLogModes::setLogStartMode(int lognum,RDLogModes::OpMode mode) const
{
  SetRow("START_MODE",lognum,(int)mode);
}


RDLogModes::StartMode RDLogModes::startMode(int lognum) const
{
  return (RDLogModes::StartMode)GetRow("CURRENT_START_MODE",lognum).toInt();
}


void RDLogModes::setStartMode(int lognum,RDLogModes::StartMode mode) const
{
  SetRow("CURRENT_START_MODE",lognum,(int)mode);
}


bool RDLogModes::autoRestart(int lognum) const
{
  return RDBool(GetRow("CURRENT_AUTO_RESTART",lognum).toString());
}


void RDLogModes::setAutoRestart(int lognum,bool state) const
{
  SetRow("CURRENT_AUTO_RESTART",lognum,RDYesNo(state));
}


QString RDLogModes::logName(int lognum) const
{
  return GetRow("CURRENT_LOG_NAME",lognum).toString();
}


void RDLogModes::setLogName(int lognum,const QString &name) const
{
  SetRow("CURRENT_LOG_NAME",lognum,name);
}


QString RDLogModes::currentLog(int lognum) const
{
  return GetRow("CURRENT_CURRENT_LOG",lognum).toString();
}


void RDLogModes::setCurrentLog(int lognum,const QString &name) const
{
  SetRow("CURRENT_CURRENT_LOG",lognum,name);
}


bool RDLogModes::logRunning(int lognum) const
{
  return RDBool(GetRow("CURRENT_RUNNING",lognum).toString());
}


void RDLogModes::setLogRunning(int lognum,bool state) const
{
  SetRow("CURRENT_RUNNING",lognum,RDYesNo(state));
}


int RDLogModes::logId(int lognum) const
{
  return GetRow("CURRENT_LOG_ID",lognum).toInt();
}


void RDLogModes::setLogId(int lognum,int id) const
{
  SetRow("CURRENT_LOG_ID",lognum,id);
}


int RDLogModes::logCurrentLine(int lognum) const
{
  return GetRow("CURRENT_LOG_LINE",lognum).toInt();
}


void RDLogModes::setLogCurrentLine(int lognum,int line) const
{
  SetRow("CURRENT_LOG_LINE",lognum,line);
}


unsigned RDLogModes::logNowCart(int lognum) const
{
  return GetRow("CURRENT_NOW_CART",lognum).toUInt();
}


void RDLogModes::setLogNowCart(int lognum,unsigned cartnum) const
{
  SetRow("CURRENT_NOW_CART",lognum,cartnum);
}


unsigned RDLogModes::logNextCart(int lognum) const
{
  return GetRow("CURRENT_NEXT_CART",lognum).toUInt();
}


void RDLogModes::setLogNextCart(int lognum,unsigned cartnum) const
{
  SetRow("CURRENT_NEXT_CART",lognum,cartnum);
}


QString RDLogModes::logModeText(RDLogModes::OpMode mode)
{
  QString ret=QObject::tr("Unknown");

  switch(mode) {
  case RDLogModes::LiveAssist:
    ret=QObject::tr("LiveAssist");
    break;

  case RDLogModes::Auto:
    ret=QObject::tr("Automatic");
    break;

  case RDLogModes::Manual:
    ret=QObject::tr("Manual");
    break;

  case RDLogModes::Previous:
    ret=QObject::tr("Previous");
    break;
  }

  return ret;
}


QVariant RDLogModes::GetRow(const QString &param,int lognum) const
{
  QString sql;
  RDSqlQuery *q;
  QVariant ret;

  sql="select "+param+" from LOG_MODES where "+
    "(STATION_NAME=\""+RDEscapeString(air_station)+"\")&&"+
    QString().sprintf("(MACHINE=%d)",lognum);
  q=new RDSqlQuery(sql);
  if(q->first()) {
    ret=q->value(0);
  }
  delete q;

  return ret;
}


void RDLogModes::SetRow(const QString &param,int lognum,int value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString("update LOG_MODES set ")+param+QString().sprintf("=%d ",value)+
    "where (STATION_NAME=\""+RDEscapeString(air_station)+"\")&&"+
    QString().sprintf("(MACHINE=%d)",lognum);
  q=new RDSqlQuery(sql);
  delete q;
}


void RDLogModes::SetRow(const QString &param,int lognum,unsigned value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString("update LOG_MODES set ")+param+QString().sprintf("=%u ",value)+
    "where (STATION_NAME=\""+RDEscapeString(air_station)+"\")&&"+
    QString().sprintf("(MACHINE=%d)",lognum);
  q=new RDSqlQuery(sql);
  delete q;
}


void RDLogModes::SetRow(const QString &param,int lognum,const QString &value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString("update LOG_MODES set ")+param+"=\""+RDEscapeString(value)+"\" "+
    "where (STATION_NAME=\""+RDEscapeString(air_station)+"\")&&"+
    QString().sprintf("(MACHINE=%d)",lognum);
  q=new RDSqlQuery(sql);
  delete q;
}
