#ifndef QDEBUGTOLOG_H
#define QDEBUGTOLOG_H

#include <iostream>
#include <cstdlib>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>

namespace UpData {
	using namespace std;
	//��־����:����ں���ΪuseTxtLog();
	QMutex mutex;
	QString timePoint;
	void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg);
	/*
	* ������������ں�����ͨ����main������ʼ�����øú������滻QDebug()�������txt��־�ļ��С�
	* ʹ�÷�ʽ����main�����е���UpData::useTxtlog();��pro�ļ��н�QDeToTxtlog.pri�ļ�include����
	*/
	void useTxtLog()
	{
		UpData::timePoint = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
		qInstallMessageHandler(UpData::LogMsgOutput);
	}
	/*�������������������ʽ����Ϣ������
	ʹ�÷�������qInstallMessageHandler�����øûص�����*/
	void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
	{
		UpData::mutex.lock();
		cout << msg.toStdString() << endl;
		QByteArray localMsg = msg.toLocal8Bit();
		QString log;
		switch (type) {
		case QtDebugMsg:
			log.append(QString("Debug File: %1 %2 Line: %3 Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg));
			break;
		case QtInfoMsg:
			log.append(QString("Info: %1 %2 %3 %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
			break;
		case QtWarningMsg:
			log.append(QString("Warning: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
			break;
		case QtCriticalMsg:
			log.append(QString("Critical: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
			break;
		case QtFatalMsg:
			log.append(QString("Fatal: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
			abort();
		}

		QFile file;
		QString path = QString("log%1.lgt").arg(timePoint);
		file.setFileName(path);
		if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
		{
			QString erinfo = file.errorString();
			cout << erinfo.toStdString() << endl;
			return;
		}
		QTextStream out(&file);
		out << "\n\r" << log;
		file.close();
		UpData::mutex.unlock();
	}
}

#endif // QDEBUGTOLOG_H
