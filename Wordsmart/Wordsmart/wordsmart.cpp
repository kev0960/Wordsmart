#include "wordsmart.h"
#include <qmessagebox.h>
#include <QClipboard>
#include <qdebug.h>
#include <QInputDialog>
#include <QMessageBox>

// static variable initialize
QNetworkAccessManager WordInfo::qnam;

Wordsmart::Wordsmart(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	download_words.setup(&ui, &my_words, &word_list_manager);

	setWindowIcon(QIcon(":/Wordsmart/wordsmart.ico"));

	clipboard = QApplication::clipboard();
	connect(clipboard, &QClipboard::changed, this, &Wordsmart::clipboard_changed);
	create_action();

	connect(ui.listWidget, &QListWidget::itemPressed, this, &Wordsmart::word_clicked);
	connect(ui.listWidget_2, &QListWidget::itemPressed, this, &Wordsmart::word_list_clicked);

	ui.textBrowser->viewport()->installEventFilter(this);

	// Set custom right click behavior for list widgets
	ui.listWidget_2->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.listWidget_2, &QListWidget::customContextMenuRequested, this, &Wordsmart::show_context_menu_2);

	ui.listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui.listWidget, &QListWidget::customContextMenuRequested, this, &Wordsmart::show_context_menu);

	notification = new Notify;
}
void Wordsmart::show_context_menu_2(const QPoint& pos)
{
	// Handle global position
	QPoint globalPos = ui.listWidget_2->mapToGlobal(pos);

	// Create menu and insert some actions
	QMenu myMenu;
	myMenu.addAction("Delete List", this, &Wordsmart::delete_word_list);
	myMenu.addAction("Rename List", this, &Wordsmart::rename_word_list);
	myMenu.addAction("Merge List", this, &Wordsmart::merge_word_list);

	// Show context menu at handling position
	myMenu.exec(globalPos);
}
void Wordsmart::show_context_menu(const QPoint& pos)
{
	// Handle global position
	QPoint globalPos = ui.listWidget->mapToGlobal(pos);

	// Create menu and insert some actions
	QMenu myMenu;
	myMenu.addAction("Delete Word", this, &Wordsmart::delete_word);

	// Show context menu at handling position
	myMenu.exec(globalPos);
}
void Wordsmart::rename_word_list()
{
	if (word_list_manager.is_empty()) return;

	bool result;
	QString text = QInputDialog::getText(this,
		tr("Edit"),
		tr("Rename"),
		QLineEdit::Normal,
		tr(Util::wstr_to_str(word_list_manager.get_current_word_list().get_name()).c_str()),
		&result);

	if (result && !text.isEmpty()) {
		word_list_manager.get_current_word_list().set_name(text.toStdWString());
		fetch_word_list();
	}
}
void Wordsmart::delete_word_list()
{
	if (word_list_manager.is_empty()) return;

	auto current = word_list_manager.get_current_word_list();
	QMessageBox msgBox;
	msgBox.setText(
		tr("You are deleting Word List <")
		+ tr(Util::wstr_to_str(current.get_name()).c_str())
		+ tr(">")
	);

	msgBox.setInformativeText("Do you really want to delete it?");
	msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
	msgBox.setDefaultButton(QMessageBox::Cancel);
	int ret = msgBox.exec();

	if (ret == QMessageBox::Ok) {
		word_list_manager.delete_current_word_list();
		fetch_word_list();
	}
}
void Wordsmart::merge_word_list()
{
	if (word_list_manager.is_empty()) return;

	auto items = ui.listWidget_2->selectedItems();

	vector<int> selected;
	for (int i = 0; i < items.size(); i++) {
		selected.push_back(ui.listWidget_2->row(items[i]));
	}

	word_list_manager.merge_word_list(selected);

	for (int i = 1; i < items.size(); i++) {
		ui.listWidget_2->removeItemWidget(items[i]);
	}

	fetch_word_list();
}
bool Wordsmart::load_saved_wordlist()
{
	std::wifstream in("saved_wordlist.wrd");
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	in.imbue(loc);

	bool result = false;
	result = my_words.read_save_file(in, word_list_manager);

	fetch_word_list();
	return result;
}
bool Wordsmart::eventFilter(QObject* target, QEvent* event) {
	if (target == ui.textBrowser->viewport() && event->type() == QEvent::MouseButtonPress) {
		QMouseEvent *e = dynamic_cast<QMouseEvent *>(event);

		if (e->buttons() == Qt::RightButton) {
			flashcard_clicked(false);
		}
		else if (e->buttons() == Qt::LeftButton) {
			flashcard_clicked(true);
		}

		return true;
	}
	return QWidget::eventFilter(target, event);
}
void Wordsmart::show_flashcard() {
	if (word_list_manager.is_empty()) {
		string top_margin = std::to_string(ui.textBrowser->size().height() / 2 - 85);
		QString qs = "<br><p align=\"center\" style=\"font-size:50px;margin-top:";
		qs += top_margin.c_str();
		qs += "\">Your Flashcard is Empty!</p>";

		ui.textBrowser->setText(qs);
		return;
	}

	WordList& current_word_list = word_list_manager.get_current_word_list();

	if (current_word_list.all_words_memorized()) {
		string top_margin = std::to_string(ui.textBrowser->size().height() / 2 - 85);
		QString qs = "<br><p align=\"center\" style=\"font-size:50px;margin-top:";
		qs += top_margin.c_str();
		qs += "\">You have memorized every words in this list!</p>";

		ui.textBrowser->setText(qs);
		return;
	}

	if (current_word_list.is_showing_word()) {
		QString qs = "<div align=\"right\" style=\"font-size:small;vertical-align:top;";

		switch (current_word_list.memorize_count()) {
		case 0:
			qs += "color:red;\">";
			break;
		case 1:
			qs += "color:yellow;\">";
			break;
		default:
			qs += "color:green;\">";
			break;
		}
		qs += QString::fromWCharArray(L"¡Ü");


		string top_margin = std::to_string(ui.textBrowser->size().height() / 2 - 85);
		qs += "</div><span>";
		qs += ("  [" + std::to_string(current_word_list.get_current_index() + 1)
			+ "/" + std::to_string(current_word_list.get_words().size()) + "]").c_str();
		qs += "</span><br> < p align = \"center\" style=\"font-size:50px;margin-top:";
		qs += top_margin.c_str();
		qs += "\">";

		wstring word = current_word_list.get_word();
		word[0] = toupper(word[0]);

		qs += Util::wstr_to_str(word).c_str();
		qs += "</p>";
		// Set to show the word
		ui.textBrowser->setText(qs);
	}
	else {
		WordInfo* w = my_words.get_word_info(current_word_list.get_word());

		QString qs = "<h2 style='font-family:Verdana'> ";
		wstring word = w->get_word();
		word[0] = toupper(word[0]);
		qs += Util::wstr_to_str(word).c_str();
		qs += "</h2><span> ";

		for (int i = 0; i < w->num_def(); i++) {
			qs += std::to_string(i + 1).c_str();
			qs += ". ";
			qs += QString::fromWCharArray(w->get_kr_definition(i).c_str());
			qs += " ";
		}
		qs += "</span><ul>";

		for (int i = 0; i < w->num_en_def(); i++) {
			qs += QString::fromWCharArray(w->get_en_definition(i).c_str());
		}
		qs += "</ul>";
		ui.textBrowser->setText(qs);
	}
}
void Wordsmart::flashcard_clicked(bool does_user_know) {
	if (word_list_manager.is_empty()) return;

	WordList& current_word_list = word_list_manager.get_current_word_list();
	if (current_word_list.is_flashcard_empty()) {
		show_flashcard();
		return;
	}

	current_word_list.flip();

	if (current_word_list.is_showing_word()) {
		// User memorized the word
		if (does_user_know) {
			current_word_list.user_memorized();
		}
		else {
			current_word_list.user_forgot();
		}
		ui.statusBar->showMessage("");
		current_word_list.next_word();
	}
	else {
		ui.statusBar->showMessage("Left click - I know this word / Right click - I don't know this word");
	}
	show_flashcard();
}
void Wordsmart::clipboard_changed()
{
	//ui.textBrowser->setText("event happened");
	ui.textBrowser->setText(clipboard->text());
	if (clipboard->text().length() >= 30) return;

	my_words.register_word(clipboard->text().toStdWString());
	connect(&my_words, &Words::defFound, this, &Wordsmart::word_is_found);
}
void Wordsmart::show_version()
{
	QMessageBox msgBox;
	msgBox.setText(tr("Wordsmart v.0.0.1"));
	msgBox.exec();
}
void Wordsmart::create_action() {
	ui.actionVersion->setStatusTip(tr("Show the version of this program"));
	ui.actionFlash_Cards->setStatusTip(tr("Show the flash card of your word list"));
	ui.actionYour_Word_List->setStatusTip(tr("Show the list of your words"));

	connect(ui.actionVersion, &QAction::triggered, this, &Wordsmart::show_version);
	connect(ui.actionFlash_Cards, &QAction::triggered, this, &Wordsmart::flash_cards);
	connect(ui.actionYour_Word_List, &QAction::triggered, this, &Wordsmart::word_list);
	connect(ui.actionDownload_Online, &QAction::triggered, this, &Wordsmart::show_download_words);

}
void Wordsmart::show_download_words()
{
	ui.stackedWidget->setCurrentIndex(2);
}
void Wordsmart::flash_cards() {
	ui.stackedWidget->setCurrentIndex(0);
	show_flashcard();
}
void Wordsmart::fetch_word_list() {
	if (word_list_manager.is_empty()) {
		for (int i = 0; i < ui.listWidget->count(); i++) {
			// Remove the rest of the ListWidget
			ui.listWidget->takeItem(i);
			i--;
		}

		for (int i = 0; i < ui.listWidget_2->count(); i++) {
			ui.listWidget_2->takeItem(i);
			i--;
		}
		return;
	}

	// If current word list is not empty, fetch it
	auto& my_word_list = word_list_manager.get_current_word_list().get_words();
	auto itr = my_word_list.begin();

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
		if (item->text().toStdWString() != itr->first) {
			ui.listWidget->insertItem(i, Util::wstr_to_str(itr->first).c_str());
		}
		itr++;
	}

	// show List of word lists here
	auto& word_lists = word_list_manager.get_word_list();
	if (ui.listWidget_2->count() < word_lists.size()) {
		int num = word_lists.size() - ui.listWidget_2->count();
		while (num) {
			ui.listWidget_2->addItem("");
			num--;
		}
	}

	auto itr2 = word_lists.begin();
	for (int i = 0; i < ui.listWidget_2->count(); i++) {
		QListWidgetItem* item = ui.listWidget_2->item(i);

		if (itr2 == word_lists.end()) {
			// Remove the rest of the ListWidget
			ui.listWidget_2->takeItem(i);
			i--;
			continue;
		}
		if (item->text().toStdWString() != itr2->get_name()) {
			ui.listWidget_2->insertItem(i, Util::wstr_to_str(itr2->get_name()).c_str());
		}
		itr2++;
	}

	for (int i = 0; i < ui.listWidget_2->count(); i++) {
		ui.listWidget_2->item(i)->setTextColor(word_lists[i].get_color());
	}

}
void Wordsmart::word_list() {
	ui.stackedWidget->setCurrentIndex(1);
	fetch_word_list();
}
void Wordsmart::word_is_found(const WordInfo& w)
{
	notification->show_notification(w);
	word_list_manager.add_word(w.get_word());

	fetch_word_list();
}
void Wordsmart::word_clicked(QListWidgetItem *item)
{
	current_selected_word = item->text().toStdWString();
	WordInfo* w = my_words.get_word_info(current_selected_word);

	QString qs = "<h2> ";
	wstring word = w->get_word();
	word[0] = toupper(word[0]);
	qs += Util::wstr_to_str(word).c_str();
	qs += "</h2><span> ";

	for (int i = 0; i < w->num_def(); i++) {
		qs += std::to_string(i + 1).c_str();
		qs += ". ";
		qs += QString::fromWCharArray(w->get_kr_definition(i).c_str());
		qs += " ";
	}
	qs += "</span><ul>";

	for (int i = 0; i < w->num_en_def(); i++) {
		qs += QString::fromWCharArray(w->get_en_definition(i).c_str());
	}
	qs += "</ul>";
	ui.textBrowser_2->setText(qs);
}
void Wordsmart::word_list_clicked(QListWidgetItem* item) {
	// Select the current word list
	word_list_manager.select_word_list(item->listWidget()->row(item));

	// Refresh the words of current list
	fetch_word_list();
}
void Wordsmart::delete_word()
{
	if (!current_selected_word.size()) return;
	word_list_manager.delete_current_word(current_selected_word);

	fetch_word_list();
}
Wordsmart::~Wordsmart()
{
	// Sine MSVC does not have support on UTF-8 output, we have to 
	// manually specify it
	std::wofstream out("saved_wordlist.wrd");
	std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
	out.imbue(loc);

	my_words.write_save_file(out);
	word_list_manager.write_word_list(out);
}
