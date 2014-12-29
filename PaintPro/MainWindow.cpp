#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	setCentralWidget(&renderArea);

	connect(ui.actionFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(ui.actionFileSave, SIGNAL(triggered()), this, SLOT(fileSave()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	connect(ui.actionPenWidth20, SIGNAL(triggered()), this, SLOT(penWidth(20)));
	connect(ui.actionPenWidth10, SIGNAL(triggered()), this, SLOT(penWidth(10)));
	connect(ui.actionPenWidth5, SIGNAL(triggered()), this, SLOT(penWidth(5)));
	connect(ui.actionPenWidth2, SIGNAL(triggered()), this, SLOT(penWidth(2)));
	connect(ui.actionPenWidth1, SIGNAL(triggered()), this, SLOT(penWidth(1)));

	this->adjustSize();
}

MainWindow::~MainWindow() {

}

void MainWindow::fileOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.setImage(filename);
	this->adjustSize();
}

void MainWindow::fileSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.saveImage(filename);
}

void MainWindow::penWidth(int width) {
	renderArea.setPenWidth(width);
}
