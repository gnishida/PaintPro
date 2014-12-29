#include "MainWindow.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	setCentralWidget(&renderArea);

	connect(ui.actionFileOpen, SIGNAL(triggered()), this, SLOT(fileOpen()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));
}

MainWindow::~MainWindow() {

}

void MainWindow::fileOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.setImage(filename);
}