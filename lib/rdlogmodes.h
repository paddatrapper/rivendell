// rdlogmodes.h
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

#ifndef RDLOGMODES_H
#define RDLOGMODES_H

#include <qsqldatabase.h>

class RDLogModes
{
 public:
  enum OpMode {Previous=0,LiveAssist=1,Auto=2,Manual=3};
  enum StartMode {StartEmpty=0,StartPrevious=1,StartSpecified=2};
  RDLogModes(const QString &station);
  QString station() const;
  RDLogModes::OpMode opMode(int lognum) const;
  void setOpMode(int lognum,RDLogModes::OpMode mode) const;
  RDLogModes::OpMode logStartMode(int lognum) const;
  void setLogStartMode(int lognum,RDLogModes::OpMode mode) const;
  RDLogModes::StartMode startMode(int lognum) const;
  void setStartMode(int lognum,RDLogModes::StartMode mode) const;
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
  static QString logModeText(RDLogModes::OpMode mode);

 private:
  QVariant GetRow(const QString &param,int lognum) const;
  void SetRow(const QString &param,int lognum,int value) const;
  void SetRow(const QString &param,int lognum,unsigned value) const;
  void SetRow(const QString &param,int lognum,const QString &value) const;
  QString air_station;
};


#endif  // RDLOGMODES_H
