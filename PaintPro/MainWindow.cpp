#include "MainWindow.h"
#include <QFileDialog>
#include <QSignalMapper>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
	ui.setupUi(this);

	setCentralWidget(&scrollArea);
	scrollArea.setWidget(&renderArea);
	scrollArea.setAlignment(Qt::AlignCenter);

	connect(ui.actionFileOpen, SIGNAL(triggered()), this, SLOT(onFileOpen()));
	connect(ui.actionFileSave, SIGNAL(triggered()), this, SLOT(onFileSave()));
	connect(ui.actionFileLoadBackground, SIGNAL(triggered()), this, SLOT(onFileLoadBackground()));
	connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(close()));

	QSignalMapper* signalMapper = new QSignalMapper(this);
	connect(ui.actionPenWidth20, SIGNAL(triggered()), signalMapper, SLOT(map()));
	connect(ui.actionPenWidth10, SIGNAL(triggered()), signalMapper, SLOT(map()));
	connect(ui.actionPenWidth5, SIGNAL(triggered()), signalMapper, SLOT(map()));
	connect(ui.actionPenWidth2, SIGNAL(triggered()), signalMapper, SLOT(map()));
	connect(ui.actionPenWidth1, SIGNAL(triggered()), signalMapper, SLOT(map()));

	signalMapper -> setMapping (ui.actionPenWidth20, 20);
	signalMapper -> setMapping (ui.actionPenWidth10, 10);
	signalMapper -> setMapping (ui.actionPenWidth5, 5);
	signalMapper -> setMapping (ui.actionPenWidth2, 2);
	signalMapper -> setMapping (ui.actionPenWidth1, 1);

	connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(onPenWidth(int))) ;

	connect(ui.actionPeekBackground, SIGNAL(triggered()), this, SLOT(onPeekBackground())) ;

	scrollArea.adjustSize();
}

MainWindow::~MainWindow() {
}

void MainWindow::onFileOpen() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.setImage(filename);
	//scrollArea.adjustSize();
}

void MainWindow::onFileSave() {
	QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.saveImage(filename);
}

void MainWindow::onFileLoadBackground() {
	QString filename = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Image files (*.jpg)"));
	if (filename.isEmpty()) return;

	renderArea.setBgImage(filename);
	//scrollArea.adjustSize();
}

void MainWindow::onPenWidth(int width) {
	renderArea.setPenWidth(width);
}

void MainWindow::onPeekBackground() {
	renderArea.setLayerFlipped(ui.actionPeekBackground->isChecked());
}