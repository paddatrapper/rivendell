// rdairplay_conf.h
//
// Abstract RDAirPlay Configuration
//
//   (C) Copyright 2002-2003 Fred Gleason <fredg@paravelsystems.com>
//
//      $Id: rdairplay_conf.h,v 1.36.8.5 2014/02/10 20:45:09 cvs Exp $
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
  enum OpMode {Previous=0,LiveAssist=1,Auto=2,Manual=3};
  enum ActionMode {Normal=0,AddFrom=1,AddTo=2,DeleteFrom=3,MoveFrom=4,MoveTo=5,
		   CopyFrom=6,CopyTo=7,Audition=8};
  enum PieEndPoint {CartEnd=0,CartTransition=1};
  enum BarAction {NoAction=0,StartNext=1};
  enum TrafficAction {TrafficStart=1,TrafficStop=2,TrafficFinish=3,
		      TrafficPause=4,TrafficMacro=5};
  enum PanelType {StationPanel=0,UserPanel=1};
  enum ExitCode {ExitClean=0,ExitDirty=1};
  enum StartMode {StartEmpty=0,StartPrevious=1,StartSpecified=2};
  RDAirPlayConf(const QString &station,const QString &tablename);
  QString station() const;
  int segueLength() const;
  void setSegueLength(int len) const;
  int transLength() const;
  void setTransLength(int len) const;
  RDAirPlayConf::OpModeStyle opModeStyle() const;
  void setOpModeStyle(RDAirPlayConf::OpModeStyle style) const;
  RDAirPlayConf::OpMode opMode(int mach) const;
  void setOpMode(int mach,RDAirPlayConf::OpMode mode) const;
  RDAirPlayConf::OpMode logStartMode(int mach) const;
  void setLogStartMode(int mach,RDAirPlayConf::OpMode mode) const;
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
  bool showAuxButton(int auxbutton) const;
  void setShowAuxButton(int auxbutton,bool state) const;
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
  QHostAddress udpAddress(int logno) const;
  void setUdpAddress(int logno,QHostAddress addr) const;
  Q_UINT16 udpPort(int logno) const;
  void setUdpPort(int logno,Q_UINT16 port) const;
  QString udpString(int logno) const;
  void setUdpString(int logno,const QString &str) const;
  QString logRml(int logno) const;
  void setLogRml(int logno,const QString &str) const;
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
  RDAirPlayConf::StartMode startMode(int lognum) const;
  void setStartMode(int lognum,RDAirPlayConf::StartMode mode) const;
  bool autoRestart(int lognum) const;
  void setAutoRestart(int lognum,bool state) const;
  QString logName(int lognum) const;
  void setLogName(int lognum,const QString &name) const;
  QString currentLog(int lognum) const;
  void setCurrentLog(int lognum,const QString &name) const;
  bool logRunning(int lognum) const;
  void setLogRunning(int lognum,bool state) const;
  int logId(int lognum) const;
  void setLogId(int lognum,int id) const;
  int logCurrentLine(int lognum) const;
  void setLogCurrentLine(int lognum,int line) const;
  unsigned logNowCart(int lognum) const;
  void setLogNowCart(int lognum,unsigned cartnum) const;
  unsigned logNextCart(int lognum) const;
  void setLogNextCart(int lognum,unsigned cartnum) const;
  //  static QString channelText(RDAirPlayConf::Channel chan);
  static QString logModeText(RDAirPlayConf::OpMode mode);

 private:
  RDAirPlayConf::OpMode GetLogMode(const QString &param,int mach) const;
  void SetLogMode(const QString &param,int mach,
		  RDAirPlayConf::OpMode mode) const;
  void SetRow(const QString &param,int value) const;
  void SetRow(const QString &param,unsigned value) const;
  void SetRow(const QString &param,const QString &value) const;
  QString air_station;
  unsigned air_id;
  QString air_tablename;
};


#endif  // RDAIRPLAY_CONF_H
