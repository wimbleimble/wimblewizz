#include "ProjectWizardWindow.h"
#include "ui_ProjectWizardWindow.h"

#include <iostream>
#include <QDir>
#include <QString>
#include <QFIleDialog>
#include <QFile>
#include <QTextStream>
#include <QDirIterator>

ProjectWizardWindow::ProjectWizardWindow(QWidget *parent) :
	QDialog{ parent },
	ui{ new Ui::ProjectWizardWindow },
	projectDirectory{ QDir::currentPath() },
	useQt{ false }
{
	ui->setupUi(this);
	updateDirectoryView();

	/*Connect up buttons.*/
	connect(ui->dialogButtons, &QDialogButtonBox::accepted,
			this, &ProjectWizardWindow::generateFiles);
	connect(ui->browseButton, &QPushButton::clicked,
			this, &ProjectWizardWindow::browse);
	connect(ui->qtInput, &QCheckBox::clicked,
			this, &ProjectWizardWindow::updateQt);
	
	/*Connect up text inputs.*/
	connect(ui->projectNameInput, &QLineEdit::textChanged,
			this, &ProjectWizardWindow::updateProjectName);
	connect(ui->projectDescriptionInput, &QPlainTextEdit::textChanged,
			this, &ProjectWizardWindow::updateProjectDescription);
}

ProjectWizardWindow::~ProjectWizardWindow()
{
	delete ui;
}

void ProjectWizardWindow::generateFiles()
{
	if(!createDirectoryStructure())
		std::cout << "Failed to create project directories.\n";
	if(!fillDirectoryStructure())
		std::cout << "Failed to create project template files.\n";
	
}

const QString ProjectWizardWindow::unwrapQt(const QString& input)
{
	QString ret{ input };
	if(useQt)
		ret.replace("\\/?qt[|", "").replace("|]", "");
	else
		for(int index{ ret.indexOf("\\/?qt[|") }; index != -1; index = ret.indexOf("\\/?qt[|"))
		{
			int endIndex{ ret.indexOf("|]", index) };
			int blockLength{ (endIndex - index) + 2 };
			ret.remove(index, blockLength);
		}
	return ret;
}

const QString ProjectWizardWindow::substituteMeta(const QString& input)
{
	QString ret{ input };
	while(ret.indexOf("\\/PROJECT_NAME\\/") != -1)
		ret.replace("\\/PROJECT_NAME\\/", projectName);
	while(ret.indexOf("\\/PROJECT_DESCRIPTION\\/") != -1)
		ret.replace("\\/PROJECT_DESCRIPTION\\/", projectDescription);
	return ret;
}

void ProjectWizardWindow::browse()
{
	QString path{QFileDialog::getExistingDirectory(this, "Select Directory", projectDirectory.absolutePath())};
	setProjectDirectory(QDir{ path });
}

bool ProjectWizardWindow::createDirectoryStructure()
{
	QDirIterator it{ "template", QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories };
	while(it.hasNext())
	{
		it.next();
		if(!projectDirectory.mkpath(it.fileName()))
			return false;
		std::cout << it.fileName().toStdString() << '\n';
	}
	return true;
}

bool ProjectWizardWindow::fillDirectoryStructure()
{
	QDirIterator it{ "template", QDir::Files, QDirIterator::Subdirectories };
	while(it.hasNext())
	{
		const QString name{ it.next() };
		QFile inFile{ name };
		if(!inFile.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			std::cout << "Could not open template file.\"" << name.toStdString() << "\".\n";
			return false;
		}
		std::cout << "Opened "<< name.toStdString() << " successfully.\n";
		QTextStream istream{ &inFile };
		QString rawContent{ istream.readAll() };
		QString processed{ unwrapQt(substituteMeta(rawContent)) };
		inFile.close();
		QFile outFile{ projectDirectory.absolutePath() + name.mid(8) };
		if(!outFile.open(QIODevice::WriteOnly | QIODevice::Text))
		{
			std::cout << "Could not open write file.\n";
			return false;
		}
		QTextStream ostream{ &outFile };
		ostream << processed;
		outFile.close();
	}
	
	return true;
}


/*Setters.*/
void ProjectWizardWindow::setProjectDirectory(const QDir& directory)
{
	projectDirectory = directory;
	updateDirectoryView();
}

/*View update functions.*/
void ProjectWizardWindow::updateDirectoryView()
{
	ui->pathInput->setText(projectDirectory.absolutePath());
}

void ProjectWizardWindow::updateProjectName()
{
	projectName = ui->projectNameInput->text();
}

void ProjectWizardWindow::updateProjectDescription()
{
	projectDescription = ui->projectDescriptionInput->toPlainText();
}

void ProjectWizardWindow::updateQt()
{
	useQt = ui->qtInput->isChecked();
}