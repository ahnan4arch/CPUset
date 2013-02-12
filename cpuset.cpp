/***************************************************************************
 *   Copyright (C) 2013 by Tomáš Frýda <fryda.tomas@gmail.com>             *
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

#include "cpuset.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <QProcess>
#include <QTimer>
#include <KLocale>

#include <plasma/theme.h>
#include <mypushbutton.h>
#include <plasma/widgets/slider.h>

CPUset::CPUset ( QObject *parent, const QVariantList &args )
    : Plasma::PopupApplet ( parent, args ),
      m_freq ( 0 ),
      m_gov ( 0 ),
      m_widget ( 0 )
{
    setPopupIcon ( KIcon ( "chronometer" ) );
    setBackgroundHints ( DefaultBackground );
    setHasConfigurationInterface ( false );
    //resize(200,200);
}


CPUset::~CPUset()
{
    if ( hasFailedToLaunch() )
    {
        // Do some cleanup here
    }
    else
    {
        // Save settings
    }
}

void CPUset::init()
{
    m_widget = new QGraphicsWidget ( this );
    m_widget->setMinimumSize ( 260,280 );

    MyPushButton * pb;
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout ( m_widget );
    layout->setOrientation ( Qt::Vertical );

    m_freq = new Plasma::Label ( m_widget );
    m_freq->setText ( "<b>Frequency:</b> " );
    layout->addItem ( m_freq );

    QProcess proc;
    proc.start ( "sh", QStringList() << "-c" << "LANG=C cpufreq-info | grep 'available frequency steps' | head -1 | cut -d\\: -f2 | tr -d ' ' | tr ',' '\\n' | sort -u | paste -s -d ','" );
    if ( !proc.waitForFinished() )
        return;  
    m_frequencies = proc.readAll().simplified().split ( ',' );
    Plasma::Slider * sld = new Plasma::Slider(this);
    sld->setRange(0, m_frequencies.count()-1);
    sld->setOrientation(Qt::Horizontal);
    connect(sld, SIGNAL(valueChanged(int)), this, SLOT(setFrequency(int)));
    layout->addItem(sld);

    m_gov = new Plasma::Label ( m_widget );
    m_gov->setText ( "<b>Governor:</b> " );
    layout->addItem ( m_gov );

    QProcess proc2;
    proc2.start ( "cpufreq-info", QStringList() <<"-g" );
    if ( !proc2.waitForFinished() )
        return;
    QList<QByteArray> lst;
    lst = proc2.readAll().simplified().split ( ' ' );
    foreach ( const QString &s, lst )
    {
        pb = new MyPushButton ( m_widget );
        pb->setText ( s );
        connect ( pb, SIGNAL ( pressed ( QString ) ), this, SLOT ( setGovernor ( QString ) ) );
        layout->addItem ( pb );
    }
    m_widget->setLayout ( layout );

    QTimer *timer = new QTimer ( this );
    connect ( timer, SIGNAL ( timeout() ), this, SLOT ( Refresh() ) );
    timer->start ( 1000 );
}

void CPUset::setFrequency ( int freq )
{
    if ( system ( ( "cpufreq-set -r -f " + QString(m_frequencies.at(freq)) ).toAscii() ) )
        m_freq->setText ( "<b>Frequency:</b> Error!" );
    else
        Refresh();
}

void CPUset::setGovernor ( QString gov )
{
    if ( system ( ( "cpufreq-set -r -g " + gov ).toAscii() ) )
        m_gov->setText ( "<b>Governor:</b> Error!" );
    else
        Refresh();
}

QGraphicsWidget* CPUset::graphicsWidget()
{
    Refresh ( true );
    return m_widget;
}

void CPUset::Refresh ( bool force )
{
    if ( isPopupShowing() || force )
    {
        //refresh
        QProcess gov;
        gov.start ( "cpufreq-info", QStringList() << "-p" );
        if ( !gov.waitForFinished() ) return;
        m_gov->setText ( QString ( "<b>Governor:</b> %1" ).arg ( QString ( gov.readAll().simplified().split ( ' ' ).back() ) ) );

        QProcess freq;
        freq.start ( "cpufreq-info", QStringList() << "-f" );
        if ( !freq.waitForFinished() ) return;
        m_freq->setText ( QString ( "<b>Frequency:</b> %1" ).arg ( QString ( freq.readAll().simplified() ) ) );
    }
}

#include "cpuset.moc"
