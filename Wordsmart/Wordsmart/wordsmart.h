#ifndef WORDSMART_H
#define WORDSMART_H

#include <QtWidgets/QMainWindow>
#include "ui_wordsmart.h"
#include "words.h"
#include "def_notify.h"

class FlashCard
{
	vector<string> word_list;
	vector<bool> memorized;

	int current_word;
	bool flashcard_showing_word;

public:
	FlashCard():  flashcard_showing_word(true), current_word(0) {

	}

	void set_word_list(std::set<string>& new_word_list) {
		word_list.resize(new_word_list.size());

		int i = 0;
		for (std::set<string>::iterator itr = new_word_list.begin(); itr != new_word_list.end(); itr++, i ++) {
			word_list[i] = *itr;
		}

		memorized.resize(word_list.size(), false);
	}

	void flip() {
		flashcard_showing_word = !flashcard_showing_word;
	}

	void user_memorized() {
		memorized[current_word] = true;
	}

	// When it goes to the last word, it comes back to the first one
	bool next_word() {
		if (current_word >= word_list.size() - 1) {
			current_word = 0;
			return false;
		}
		else {
			current_word++;
		}
		return true;
	}

	string get_word() {
		return word_list[current_word];
	}

	bool is_showing_word() {
		return flashcard_showing_word;
	}
};
class Wordsmart : public QMainWindow
{
	Q_OBJECT

public:
	Wordsmart(QWidget *parent = 0);
	~Wordsmart();

private:
	Ui::WordsmartClass ui;

	void create_action();
	void clipboard_changed();

	// Show version of the program
	void show_version();

	// Flash cards
	FlashCard my_flash_card;
	void flash_cards();
	void flashcard_clicked(int x);
	void show_flashcard();

	// Show Word List
	void word_list();
	void fetch_word_list();

	void word_is_found(const WordInfo& w);
	void list_word_clicked(QListWidgetItem *item);

	QClipboard* clipboard;

	Words my_words;

	// Because qttextbrowser does not support click event, we have to manually 
	// override it through installing custom eventfilter
	bool eventFilter(QObject* target, QEvent* event);

	// Desktop notification when the definition of the word is found
	Notify* notification;
};


#endif // WORDSMART_H
