#include <QApplication>
#include "ProjectWizardWindow.h"

int main(int argc, char* argv[])
{
	QApplication app{ argc, argv };
	ProjectWizardWindow window{};
	window.show();
	return app.exec();
}