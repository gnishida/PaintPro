#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QScrollArea>
#include "RenderArea.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

public slots:
	void fileOpen();
	void fileSave();
	void penWidth(int width);

private:
	Ui::MainWindowClass ui;
	QScrollArea scrollArea;
	RenderArea renderArea;

};

#endif // MAINWINDOW_H
