#ifndef WORDSMART_H
#define WORDSMART_H

#include <QtWidgets/QMainWindow>
#include "ui_wordsmart.h"
#include "words.h"
#include "def_notify.h"
#include "download_words.h"

class Wordsmart : public QMainWindow
{
	Q_OBJECT

public:
	Wordsmart(QWidget *parent = 0);
	~Wordsmart();

	bool load_saved_wordlist();

private:
	Ui::WordsmartClass ui;

	void create_action();
	void clipboard_changed();

	// Show version of the program
	void show_version();

	// Flash cards
	void flash_cards();
	void flashcard_clicked(bool does_user_know);
	void show_flashcard();

	// Show Word List
	void word_list();
	void fetch_word_list();
	void delete_word();

	void word_is_found(const WordInfo& w);
	void word_clicked(QListWidgetItem *item);

	void word_list_clicked(QListWidgetItem* item);
	QClipboard* clipboard;

	WordListManager word_list_manager;
	Words my_words;
	string current_selected_word;

	// Because qttextbrowser does not support click event, we have to manually 
	// override it through installing custom eventfilter
	bool eventFilter(QObject* target, QEvent* event);

	// Desktop notification when the definition of the word is found
	Notify* notification;

	// Custom context menu for listwidgets
	void show_context_menu_2(const QPoint& pos);

	void rename_word_list();
	void delete_word_list();
	void merge_word_list();

	// Word Download
	DownloadWordList download_words;

	void show_download_words();
};


#endif // WORDSMART_H
