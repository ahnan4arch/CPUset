/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                 *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

// Here we avoid loading the header multiple times
#ifndef CPUSET_HEADER
#define CPUSET_HEADER

#include <KIcon>
// We need the Plasma Applet headers
#include <Plasma/PopupApplet>
#include <Plasma/Label>

#include <QGraphicsLinearLayout>
class QSizeF;

// Define our plasma Applet
class CPUset : public Plasma::PopupApplet
{
  Q_OBJECT
public:
  // Basic Create/Destroy
  CPUset ( QObject *parent, const QVariantList &args );
  ~CPUset();

  void init();
  QGraphicsWidget *graphicsWidget ();

public slots:
  void setFrequency ( QString freq );
  void setGovernor ( QString gov );

private:
  Plasma::Label * m_freq;
  Plasma::Label * m_gov;
  QGraphicsWidget * m_widget;
private slots:
  void Refresh ( bool force = false );
};

K_EXPORT_PLASMA_APPLET ( cpuset, CPUset )
// This is the command that links your applet to the .desktop file
//K_EXPORT_PLASMA_APPLET ( cpuset, CPUset )
#endif
