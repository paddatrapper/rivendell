// rdairplay_conf.cpp
//
// Abstract an RDAirPlay Configuration.
//
//   (C) Copyright 2002-2014 Fred Gleason <fredg@paravelsystems.com>
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
#include <rdairplay_conf.h>
#include <rdescape_string.h>

RDAirPlayConf::RDAirPlayConf(const QString &station,const QString &tablename)
{
  RDSqlQuery *q;
  QString sql;

  air_station=station;
  air_tablename=tablename;

  sql=QString().
    sprintf("select ID from %s where STATION=\"%s\"",
	    (const char *)air_tablename,
	    (const char *)RDEscapeString(air_station));
  q=new RDSqlQuery(sql);
  if(!q->first()) {
    delete q;
    sql=QString().
      sprintf("insert into %s set STATION=\"%s\"",
	      (const char *)air_tablename,
	      (const char *)RDEscapeString(air_station));
    q=new RDSqlQuery(sql);
    delete q;
    sql=QString().
      sprintf("select ID from %s where STATION=\"%s\"",
	      (const char *)air_tablename,
	      (const char *)RDEscapeString(air_station));
    q=new RDSqlQuery(sql);
    q->first();
  }
  air_id=q->value(0).toUInt();
  delete q;
}


QString RDAirPlayConf::station() const
{
  return air_station;
}


int RDAirPlayConf::segueLength() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"SEGUE_LENGTH").toInt();
}


void RDAirPlayConf::setSegueLength(int len) const
{
  SetRow("SEGUE_LENGTH",len);
}


int RDAirPlayConf::transLength() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"TRANS_LENGTH").toInt();
}


void RDAirPlayConf::setTransLength(int len) const
{
  SetRow("TRANS_LENGTH",len);
}


RDAirPlayConf::OpModeStyle RDAirPlayConf::opModeStyle() const
{
  return (RDAirPlayConf::OpModeStyle)
    RDGetSqlValue(air_tablename,"ID",air_id,"LOG_MODE_STYLE").toInt();
}


void RDAirPlayConf::setOpModeStyle(RDAirPlayConf::OpModeStyle style) const
{
  SetRow("LOG_MODE_STYLE",(int)style);
}


unsigned RDAirPlayConf::logQuantity() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"LOG_QUANTITY").toInt();
}


void RDAirPlayConf::setLogQuantity(unsigned quan) const
{
  SetRow("LOG_QUANTITY",quan);
}


int RDAirPlayConf::pieCountLength() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"PIE_COUNT_LENGTH").toInt();
}


void RDAirPlayConf::setPieCountLength(int len) const
{
  SetRow("PIE_COUNT_LENGTH",len);
}


RDAirPlayConf::PieEndPoint RDAirPlayConf::pieEndPoint() const
{
  return (RDAirPlayConf::PieEndPoint)
    RDGetSqlValue(air_tablename,"ID",air_id,"PIE_COUNT_ENDPOINT").toInt();
}


void RDAirPlayConf::setPieEndPoint(RDAirPlayConf::PieEndPoint point) const
{
  SetRow("PIE_COUNT_ENDPOINT",(int)point);
}


bool RDAirPlayConf::checkTimesync() const
{
  return RDBool(RDGetSqlValue(air_tablename,"ID",air_id,
			    "CHECK_TIMESYNC").toString());
}


void RDAirPlayConf::setCheckTimesync(bool state) const
{
  SetRow("CHECK_TIMESYNC",RDYesNo(state));
}


int RDAirPlayConf::panels(RDAirPlayConf::PanelType type) const
{
  switch(type) {
      case RDAirPlayConf::StationPanel:
	return RDGetSqlValue(air_tablename,"ID",air_id,"STATION_PANELS").toInt();

      case RDAirPlayConf::UserPanel:
	return RDGetSqlValue(air_tablename,"ID",air_id,"USER_PANELS").toInt();
  }
  return 0;
}


void RDAirPlayConf::setPanels(RDAirPlayConf::PanelType type,int quan) const
{
  switch(type) {
      case RDAirPlayConf::StationPanel:
	SetRow("STATION_PANELS",quan);
	break;

      case RDAirPlayConf::UserPanel:
	SetRow("USER_PANELS",quan);
	break;
  }
}


bool RDAirPlayConf::showAuxButton(int auxbutton) const
{
  return RDBool(RDGetSqlValue(air_tablename,"ID",
	    air_id,QString().sprintf("SHOW_AUX_%d",auxbutton+1)).toString());
}


void RDAirPlayConf::setShowAuxButton(int auxbutton,bool state) const
{
  SetRow(QString().sprintf("SHOW_AUX_%d",auxbutton+1),RDYesNo(state));
}


bool RDAirPlayConf::clearFilter() const
{
  return 
    RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"CLEAR_FILTER").toString());
}


void RDAirPlayConf::setClearFilter(bool state) const
{
  SetRow("CLEAR_FILTER",RDYesNo(state));
}


RDLogLine::TransType RDAirPlayConf::defaultTransType() const
{
  return (RDLogLine::TransType)
    RDGetSqlValue(air_tablename,"ID",air_id,"DEFAULT_TRANS_TYPE").toInt();
}


void RDAirPlayConf::setDefaultTransType(RDLogLine::TransType type) const
{
  SetRow("DEFAULT_TRANS_TYPE",(int)type);
}


RDAirPlayConf::BarAction RDAirPlayConf::barAction() const
{
  return (RDAirPlayConf::BarAction)
    RDGetSqlValue(air_tablename,"ID",air_id,"BAR_ACTION").toUInt();
}


void RDAirPlayConf::setBarAction(RDAirPlayConf::BarAction action) const
{
  SetRow("BAR_ACTION",(int)action);
}


bool RDAirPlayConf::flashPanel() const
{
  return 
    RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"FLASH_PANEL").toString());
}


void RDAirPlayConf::setFlashPanel(bool state) const
{
  SetRow("FLASH_PANEL",RDYesNo(state));
}


bool RDAirPlayConf::panelPauseEnabled() const
{
  return RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"PANEL_PAUSE_ENABLED").
	       toString());
}


void RDAirPlayConf::setPanelPauseEnabled(bool state) const
{
  SetRow("PANEL_PAUSE_ENABLED",RDYesNo(state));
}


QString RDAirPlayConf::buttonLabelTemplate() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"BUTTON_LABEL_TEMPLATE").
    toString();
}


void RDAirPlayConf::setButtonLabelTemplate(const QString &str) const
{
  SetRow("BUTTON_LABEL_TEMPLATE",str);
}


bool RDAirPlayConf::pauseEnabled() const
{
  return 
    RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"PAUSE_ENABLED").toString());
}


void RDAirPlayConf::setPauseEnabled(bool state) const
{
  SetRow("PAUSE_ENABLED",RDYesNo(state));
}


QString RDAirPlayConf::defaultSvc() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"DEFAULT_SERVICE").toString();
}


void RDAirPlayConf::setDefaultSvc(const QString &svcname) const
{
  SetRow("DEFAULT_SERVICE",svcname);
}


bool RDAirPlayConf::hourSelectorEnabled() const
{
  return 
    RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"HOUR_SELECTOR_ENABLED").
	   toString());
}


void RDAirPlayConf::setHourSelectorEnabled(bool state) const
{
  SetRow("HOUR_SELECTOR_ENABLED",RDYesNo(state));
}


QString RDAirPlayConf::titleTemplate() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"TITLE_TEMPLATE").
    toString();
}


void RDAirPlayConf::setTitleTemplate(const QString &str)
{
  SetRow("TITLE_TEMPLATE",str);
}


QString RDAirPlayConf::artistTemplate() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"ARTIST_TEMPLATE").
    toString();
}


void RDAirPlayConf::setArtistTemplate(const QString &str)
{
  SetRow("ARTIST_TEMPLATE",str);
}


QString RDAirPlayConf::outcueTemplate() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"OUTCUE_TEMPLATE").
    toString();
}


void RDAirPlayConf::setOutcueTemplate(const QString &str)
{
  SetRow("OUTCUE_TEMPLATE",str);
}


QString RDAirPlayConf::descriptionTemplate() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"DESCRIPTION_TEMPLATE").
    toString();
}


void RDAirPlayConf::setDescriptionTemplate(const QString &str)
{
  SetRow("DESCRIPTION_TEMPLATE",str);
}


RDAirPlayConf::ExitCode RDAirPlayConf::exitCode() const
{
  return (RDAirPlayConf::ExitCode)
    RDGetSqlValue(air_tablename,"ID",air_id,"EXIT_CODE").toInt();
}


void RDAirPlayConf::setExitCode(RDAirPlayConf::ExitCode code) const
{
  SetRow("EXIT_CODE",(int)code);
}


bool RDAirPlayConf::exitPasswordValid(const QString &passwd) const
{
  QString sql;
  RDSqlQuery *q;

  sql=QString().sprintf("select EXIT_PASSWORD from %s where \
                         STATION=\"%s\" && EXIT_PASSWORD=PASSWORD(\"%s\")",
			(const char *)air_tablename,
			(const char *)air_station,
			(const char *)passwd);
  q=new RDSqlQuery(sql);
  if(q->size()>0) {
    delete q;
    return true;
  }
  delete q;
  return false;
}


void RDAirPlayConf::setExitPassword(const QString &passwd) const
{
  QString sql;
  RDSqlQuery *q;

  sql=QString().sprintf ("update %s set EXIT_PASSWORD=PASSWORD(\"%s\") \
                          where STATION=\"%s\"",(const char *)air_tablename,
			 (const char *)passwd,
			 (const char *)air_station);
  q=new RDSqlQuery(sql);
  delete q;
}


QString RDAirPlayConf::skinPath() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"SKIN_PATH").toString();
}


void RDAirPlayConf::setSkinPath(const QString &path) const
{
  SetRow("SKIN_PATH",path);
}


bool RDAirPlayConf::showCounters() const
{
  return RDBool(RDGetSqlValue(air_tablename,"ID",air_id,"SHOW_COUNTERS").
		toString());
}


void RDAirPlayConf::setShowCounters(bool state) const
{
  SetRow("SHOW_COUNTERS",RDYesNo(state));
}


int RDAirPlayConf::auditionPreroll() const
{
  return RDGetSqlValue(air_tablename,"ID",air_id,"AUDITION_PREROLL").toInt();
}


void RDAirPlayConf::setAuditionPreroll(int msecs) const
{
  SetRow("AUDITION_PREROLL",msecs);
}


void RDAirPlayConf::SetRow(const QString &param,int value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().
    sprintf("UPDATE %s SET %s=%d WHERE STATION=\"%s\"",
	    (const char *)air_tablename,
	    (const char *)param,
	    value,
	    (const char *)RDEscapeString(air_station));
  q=new RDSqlQuery(sql);
  delete q;
}


void RDAirPlayConf::SetRow(const QString &param,unsigned value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().
    sprintf("UPDATE %s SET %s=%u WHERE STATION=\"%s\"",
	    (const char *)air_tablename,
	    (const char *)param,
	    value,
	    (const char *)RDEscapeString(air_station));
  q=new RDSqlQuery(sql);
  delete q;
}


void RDAirPlayConf::SetRow(const QString &param,const QString &value) const
{
  RDSqlQuery *q;
  QString sql;

  sql=QString().
    sprintf("UPDATE %s SET %s=\"%s\" WHERE STATION=\"%s\"",
	    (const char *)air_tablename,
	    (const char *)RDEscapeString(param),
	    (const char *)RDEscapeString(value),
	    (const char *)RDEscapeString(air_station));
  q=new RDSqlQuery(sql);
  delete q;
}
