/*
 * MPlayer backend for the Phonon library
 * Copyright (C) 2006-2008  Ricardo Villalba <rvm@escomposlinux.org>
 * Copyright (C) 2007-2010  Tanguy Krotoff <tkrotoff@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MEDIACONTROL_MYPROCESS_H
#define MEDIACONTROL_MYPROCESS_H

#include <QtCore/QProcess>


namespace MediaControl2
{

/**
 * MyProcess is a specialized QProcess designed to properly work with MPlayer.
 *
 * It can split the MPlayer status line into lines.
 */
class MyProcess : public QProcess {
	Q_OBJECT
public:

	MyProcess(QObject * parent);

	virtual ~MyProcess();

        /** ����true ��ʾmplayer������. */
	bool isRunning() const;

        /** ��������. */
	void start(const QString & program, const QStringList & arguments);

signals:

        /** ����ȡ��һ�������ķ��������� */
	void lineAvailable(const QString & line);

private slots:

        /** mplayer������ȡ */
	void readStdout();

        /** ���̽��������*/
	void finished(int exitCode, QProcess::ExitStatus exitStatus);

	void error(QProcess::ProcessError error);

private:

	/**
	 * Return true if it's possible to read an entire line.
	 *
	 * @param output process output
	 * @param from specifies the position to begin.
	 */
        int canReadLine(const QByteArray & output, int from = 0);

	/** Called from readStdOut() to do all the work. */
	void genericRead(const QByteArray & output);

	QByteArray _remainingOutput;
};

}	//Namespace Phonon::MPlayer

#endif	//MEDIACONTROL_MYPROCESS_H
