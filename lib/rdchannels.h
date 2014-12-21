// rdchannels.h
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

#ifndef RDCHANNELS_H
#define RDCHANNELS_H

#include <qstring.h>
#include <qvariant.h>

class RDChannels
{
 public:
  enum Channel {CueOutput=0,
		LibraryInput=1,LibraryOutput=2,
		LogEditInput=3,LogEditOutput=4,
		CartSlotInput=5,CartSlotOutput=6,
		CatchInput=7,CatchOutput=8,CatchMonitor=9,
		PanelButtonOutput=10,
		AirplayButtonOutput=11,AirplayLogOutput=12,
		LastChannel=13};
  enum Action {Start=0,Stop=1};
  enum GpioDirection {Gpi=0,Gpo=1};
  enum GpioType {EdgeGpio=0,LevelGpio=1};
  RDChannels(const QString &station);
  int card(Channel chan,int num=0,int sub_num=0) const;
  void setCard(int card,Channel chan,int num=0,int sub_num=0) const;
  int port(Channel chan,int num=0,int sub_num=0) const;
  void setPort(int port,Channel chan,int num=0,int sub_num=0) const;
  QString rml(Channel chan,Action act,int num=0,int sub_num=0) const;
  void setRml(const QString &rml,Channel chan,Action act,int num=0,
	      int sub_num=0) const;
  GpioType gpioType(Channel chan,int num=0,int sub_num=0) const;
  void setGpioType(GpioType type,Channel chan,int num=0,int sub_num=0) const;
  int gpioMatrix(Action act,GpioDirection dir,Channel chan,int num=0,
		 int sub_num=0) const;
  void setGpioMatrix(int matrix,Action act,GpioDirection dir,Channel chan,
		     int num=0,int sub_num=0) const;
  int gpioLine(Action act,GpioDirection dir,Channel chan,int num=0,
	       int sub_num=0) const;
  void setGpioLine(int line,Action act,GpioDirection dir,Channel chan,
		   int num=0,int sub_num=0) const;
  unsigned cart(Action act,Channel chan,int num=0,int sub_num=0) const;
  void setCart(unsigned cartnum,Action act,Channel chan,int num=0,
	       int sub_num=0) const;

 private:
  QVariant ReadField(const QString &param,Channel chan,int num,
		     int sub_num) const;
  void WriteRow(int value,const QString &param,Channel chan,int num,
		int sub_num) const;
  void WriteRow(const QString &value,const QString &param,Channel chan,int num,
		int sub_num) const;
  QString DirectionString(GpioDirection dir) const;
  QString ActionString(Action act) const;
  QString chan_station;
};


#endif  // RDCHANNELS
