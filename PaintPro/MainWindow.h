#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include "RenderArea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void fileOpen();

private:
	Ui::MainWindowClass ui;
	RenderArea renderArea;

};

#endif // MAINWINDOW_H
