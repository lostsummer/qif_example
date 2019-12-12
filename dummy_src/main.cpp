#include "QtGuiApplication1.h"
#include <QtWidgets/QApplication>
#include <QProcess>
#include <QDebug>

static bool checkUpdates()
{
	QProcess process;
	process.start("maintenancetool --checkupdates");

	// Wait until the update tool is finished
	process.waitForFinished();

	if (process.error() != QProcess::UnknownError)
	{
		qDebug() << "Error checking for updates";
		return false;
	}

	// Read the output
	QByteArray data = process.readAllStandardOutput();

	// No output means no updates available
	// Note that the exit code will also be 1, but we don't use that
	// Also note that we should parse the output instead of just checking if it is empty if we want specific update info
	if (data.isEmpty())
	{
		qDebug() << "No updates available";
		return false;
	}

	// Call the maintenance tool binary
	// Note: we start it detached because this application need to close for the update
	QStringList args("--updater");
	bool success = QProcess::startDetached("maintenancetool", args);

	// Close the application
	qApp->closeAllWindows();
	return true;
}

int main(int argc, char *argv[])
{
	checkUpdates();
	QApplication a(argc, argv);
	QtGuiApplication1 w;
	w.show();
	return a.exec();
}


