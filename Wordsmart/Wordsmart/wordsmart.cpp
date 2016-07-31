#include "wordsmart.h"
#include <qmessagebox.h>
#include <QClipboard>
#include <qdebug.h>

Wordsmart::Wordsmart(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::changed, this, &Wordsmart::clipboard_changed);
	create_action();

	connect(ui.listWidget, &QListWidget::itemPressed, this, &Wordsmart::list_word_clicked);
	
	ui.textBrowser->viewport()->installEventFilter(this);

	notification = new Notify;
}
bool Wordsmart::eventFilter(QObject* target, QEvent* event) {
	
	if (target == ui.textBrowser->viewport() && event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);

		int x = e->x();
		ui.textBrowser->setText(std::to_string(e->x()).c_str());
		flashcard_clicked(x);

		return true;
	}
	return QWidget::eventFilter(target, event);
}
void Wordsmart::show_flashcard() {
	ui.textBrowser->document()->setDefaultStyleSheet("p{margin-top:30px}");
	if (my_flash_card.is_showing_word()) {
		QString qs = "<p style='padding-top:30px;color:blue;'>";
		qs += my_flash_card.get_word().c_str();
		qs += "</p>";

		// Set to show the word
		ui.textBrowser->setHtml(qs);
		//ui.textBrowser->setAlignment(Qt::AlignCenter);
	}
	else {
		WordInfo& w = my_words.get_word_info(my_flash_card.get_word());

		QString qs = "<h2 style='font-family:Verdana'> ";
		string word = w.get_word().c_str();
		word[0] = toupper(word[0]);
		qs += word.c_str();
		qs += "</h2><span> ";

		for (int i = 0; i < w.num_def(); i++) {
			qs += std::to_string(i + 1).c_str();
			qs += ". ";
			qs += QString::fromWCharArray(w.get_kr_definition(i).c_str());
			qs += " ";
		}
		qs += "</span><ul>";

		for (int i = 0; i < w.num_en_def(); i++) {
			qs += QString::fromWCharArray(w.get_en_definition(i).c_str());
		}
		qs += "</ul>";
		ui.textBrowser->setText(qs);
	}
}
void Wordsmart::flashcard_clicked(int x) {
	my_flash_card.flip();

	if (my_flash_card.is_showing_word()) {
		// User memorized the word
		if (x > ui.textBrowser->size().width() / 2) {
			my_flash_card.user_memorized();
		}

		my_flash_card.next_word();
	}
	show_flashcard();
}
void Wordsmart::clipboard_changed()
{
	ui.textBrowser->setText(clipboard->text());
	my_words.register_word(clipboard->text().toStdString());
	connect(&my_words, &Words::defFound, this, &Wordsmart::word_is_found);
}
void Wordsmart::show_version()
{
	QMessageBox msgBox;
	msgBox.setText(tr("Wordsmart v.0.0.1"));
	msgBox.exec();
}
void Wordsmart::create_action() {
	ui.actionVersion->setStatusTip(tr("Show me the version of this program"));
	ui.actionFlash_Cards->setStatusTip(tr("Show flash card of your word list"));
	ui.actionYour_Word_List->setStatusTip(tr("Show list of your words"));

	connect(ui.actionVersion, &QAction::triggered, this, &Wordsmart::show_version);
	connect(ui.actionFlash_Cards, &QAction::triggered, this, &Wordsmart::flash_cards);
	connect(ui.actionYour_Word_List, &QAction::triggered, this, &Wordsmart::word_list);
}
void Wordsmart::flash_cards() {
	ui.stackedWidget->setCurrentIndex(0);
	show_flashcard();
}
void Wordsmart::fetch_word_list() {
	std::set<string>& my_word_list = my_words.get_word_list();
	std::set<string>::iterator itr = my_word_list.begin();

	if (ui.listWidget->count() < my_word_list.size()) {
		int num = my_word_list.size() - ui.listWidget->count();
		while (num) {
			ui.listWidget->addItem("");
			num--;
		}
	}
	for (int i = 0; i < ui.listWidget->count(); i++) {
		QListWidgetItem* item = ui.listWidget->item(i);
		if (itr == my_word_list.end()) {
			// Remove the rest of the ListWidget
			ui.listWidget->takeItem(i);
			i--;
			continue;
		}
		if (item->text().toStdString() != *itr) {
			ui.listWidget->insertItem(i, (*itr).c_str());
		}
		itr++;
	}
}
void Wordsmart::word_list() {
	ui.stackedWidget->setCurrentIndex(1);
	fetch_word_list();
}
void Wordsmart::word_is_found(const WordInfo& w)
{	
	notification->show_notification(w);
	fetch_word_list();
	my_flash_card.set_word_list(my_words.get_word_list());
}
void Wordsmart::list_word_clicked(QListWidgetItem *item) 
{
	WordInfo& w = my_words.get_word_info(item->text().toStdString());

	QString qs = "<h2 style='font-family:Verdana'> ";
	string word = w.get_word().c_str();
	word[0] = toupper(word[0]);
	qs += word.c_str();
	qs += "</h2><span> ";

	for (int i = 0; i < w.num_def(); i++) {
		qs += std::to_string(i + 1).c_str();
		qs += ". ";
		qs += QString::fromWCharArray(w.get_kr_definition(i).c_str());
		qs += " ";
	}
	qs += "</span><ul>";

	for (int i = 0; i < w.num_en_def(); i++) {
		qs += QString::fromWCharArray(w.get_en_definition(i).c_str());
	}
	qs += "</ul>";
	ui.textBrowser_2->setText(qs);
}
Wordsmart::~Wordsmart()
{

}
