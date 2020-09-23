#ifndef PROJECTWIZARDWINDOW_H
#define PROJECTWIZARDWINDOW_H

#include <QDialog>
#include <QString>
#include <QDir>


namespace Ui {
class ProjectWizardWindow;
}

class ProjectWizardWindow : public QDialog
{
	Q_OBJECT

public:
	explicit ProjectWizardWindow(QWidget *parent = nullptr);
	~ProjectWizardWindow();

private:
	Ui::ProjectWizardWindow *ui;

	QString projectName;
	QString projectDescription;
	bool useQt;
	QDir projectDirectory;

	void setProjectDirectory(const QDir& directory);

	void generateFiles();
	void browse();
	void updateDirectoryView(); 	//called whenever target directory is changed.
	void updateProjectName();		//called whenever projectName is changed.
	void updateProjectDescription();//called whenever projectDescription is changed.
	void updateQt();				//called when Qt checkbox changed.

	//pre-processing functions

	const QString unwrapQt(const QString& input);
	const QString substituteMeta(const QString& input);

	//file creation functions

	bool createDirectoryStructure();
	bool fillDirectoryStructure();
};

#endif // PROJECTWIZARDWINDOW_H
