// rdairplay_conf.h
//
// Abstract RDAirPlay Configuration
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
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

#ifndef RDAIRPLAY_CONF_H
#define RDAIRPLAY_CONF_H

#include <qsqldatabase.h>
#include <qhostaddress.h>

#include <rdlog_line.h>         


class RDAirPlayConf
{
 public:
  enum TimeMode {TwelveHour=0,TwentyFourHour=1};
  enum OpModeStyle {Unified=0,Independent=1};
  enum ActionMode {Normal=0,AddFrom=1,AddTo=2,DeleteFrom=3,MoveFrom=4,MoveTo=5,
		   CopyFrom=6,CopyTo=7,Audition=8};
  enum PieEndPoint {CartEnd=0,CartTransition=1};
  enum BarAction {NoAction=0,StartNext=1};
  enum TrafficAction {TrafficStart=1,TrafficStop=2,TrafficFinish=3,
		      TrafficPause=4,TrafficMacro=5};
  enum PanelType {StationPanel=0,UserPanel=1};
  enum ExitCode {ExitClean=0,ExitDirty=1};
  RDAirPlayConf(const QString &station,const QString &tablename);
  QString station() const;
  int segueLength() const;
  void setSegueLength(int len) const;
  int transLength() const;
  void setTransLength(int len) const;
  RDAirPlayConf::OpModeStyle opModeStyle() const;
  void setOpModeStyle(RDAirPlayConf::OpModeStyle style) const;
  unsigned logQuantity() const;
  void setLogQuantity(unsigned quan) const;
  int pieCountLength() const;
  void setPieCountLength(int len) const;
  RDAirPlayConf::PieEndPoint pieEndPoint() const;
  void setPieEndPoint(RDAirPlayConf::PieEndPoint point) const;
  bool checkTimesync() const;
  void setCheckTimesync(bool state) const;
  int panels(RDAirPlayConf::PanelType type) const;
  void setPanels(RDAirPlayConf::PanelType type,int quan) const;
  bool clearFilter() const;
  void setClearFilter(bool state) const;
  RDLogLine::TransType defaultTransType() const;
  void setDefaultTransType(RDLogLine::TransType type) const;
  RDAirPlayConf::BarAction barAction() const;
  void setBarAction(RDAirPlayConf::BarAction action) const;
  bool flashPanel() const;
  void setFlashPanel(bool state) const;
  bool panelPauseEnabled() const;
  void setPanelPauseEnabled(bool state) const;
  QString buttonLabelTemplate() const;
  void setButtonLabelTemplate(const QString &str) const;
  bool pauseEnabled() const;
  void setPauseEnabled(bool state) const;
  QString defaultSvc() const;
  void setDefaultSvc(const QString &svcname) const;
  QString titleTemplate() const;
  void setTitleTemplate(const QString &str);
  QString artistTemplate() const;
  void setArtistTemplate(const QString &str);
  QString outcueTemplate() const;
  void setOutcueTemplate(const QString &str);
  QString descriptionTemplate() const;
  void setDescriptionTemplate(const QString &str);
  bool hourSelectorEnabled() const;
  void setHourSelectorEnabled(bool state) const;
  RDAirPlayConf::ExitCode exitCode() const;
  void setExitCode(RDAirPlayConf::ExitCode code) const;
  bool exitPasswordValid(const QString &passwd) const;
  void setExitPassword(const QString &passwd) const;
  QString skinPath() const;
  void setSkinPath(const QString &path) const;
  bool showCounters() const;
  void setShowCounters(bool state) const;
  int auditionPreroll() const;
  void setAuditionPreroll(int msecs) const;

 private:
  void SetRow(const QString &param,int value) const;
  void SetRow(const QString &param,unsigned value) const;
  void SetRow(const QString &param,const QString &value) const;
  QString air_station;
  unsigned air_id;
  QString air_tablename;
};


#endif  // RDAIRPLAY_CONF_H
