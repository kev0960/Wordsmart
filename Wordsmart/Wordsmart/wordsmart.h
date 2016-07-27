#ifndef WORDSMART_H
#define WORDSMART_H

#include <QtWidgets/QMainWindow>
#include "ui_wordsmart.h"
#include "words.h"

class Wordsmart : public QMainWindow
{
	Q_OBJECT

public:
	Wordsmart(QWidget *parent = 0);
	~Wordsmart();

	private slots:
	void show_version();

private:
	Ui::WordsmartClass ui;

	void create_action();
	void clipboard_changed();

	QClipboard* clipboard;

	Words my_words;
};


#endif // WORDSMART_H
