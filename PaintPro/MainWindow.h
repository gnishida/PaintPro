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
	void onFileOpen();
	void onFileSave();
	void onFileLoadBackground();
	void onPenWidth(int width);
	void onZoom(int factor);
	void onPeekBackground();

private:
	Ui::MainWindowClass ui;
	QScrollArea scrollArea;
	RenderArea renderArea;

};

#endif // MAINWINDOW_H
