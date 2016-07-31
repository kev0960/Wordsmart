#pragma once

#include <QDialog>
#include <QDesktopWidget>
#include <QMouseEvent>
#include "ui_def_notify.h"
#include "words.h"
class Notify : public QDialog
{
	Q_OBJECT

public:
	Notify(QWidget *parent = 0);
	~Notify();

	void show_notification(const WordInfo& w);
	void close_notify();

signals:
	void closed();

private:
	Ui::Dialog *ui;
};