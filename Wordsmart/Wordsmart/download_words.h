#pragma once

#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <exception>
#include <fstream>


#include <QtWidgets/QMainWindow>
#include "HTMLParser.h"
#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QMessageBox>
#include "ui_wordsmart.h"
#include "words.h"

class DownloadWordList : public QObject
{
	Q_OBJECT

private:
	struct wordlist_content {
		wstring name;
		vector<wstring> word_list_name;
		vector<vector<wstring>> words;

		wordlist_content(wstring name) : name(name) {}
	};

	QNetworkAccessManager qnam;
	QNetworkReply* response_wordlist;
	QNetworkReply* response_words;

	// Download the words from the web
	vector<QNetworkReply*> word_response;
	vector<WordInfo*> word_infos;
	wstring donwloading_wordlist_name;

	// Words that are currently downloading
	vector<wstring> current_download_word;

	// Words that are candidates for downloading
	vector<wstring> downloading_words;

	int total_download;
	int num_download_done;

	vector<bool> download_done;
	bool download_in_progress;

	vector<wordlist_content> down_avail;

	int current_title;
	int word_list_index;

	Ui::WordsmartClass* ui;
	Words* words;
	WordListManager* words_manager;

signals:
	void kr_def_found(int index);
	void en_def_found(int index);
	void def_found(int index);

public:
	DownloadWordList() : download_in_progress(false) {
	}

	// Query the server
	void query_wordsmart_server()
	{
		QUrl server_url("http://104.199.179.40/all-word-list");
		response_wordlist = qnam.get(QNetworkRequest(server_url));
		connect(response_wordlist, &QNetworkReply::finished, this, &DownloadWordList::server_response_ready);
	}
	void setup(Ui::WordsmartClass* _ui, Words* _words, WordListManager* _manager) 
	{
		ui = _ui;
		words = _words;
		words_manager = _manager;

		query_wordsmart_server();

		connect(ui->listWidget_3, &QListWidget::itemPressed, this, &DownloadWordList::on_title_click);
		connect(ui->listWidget_4, &QListWidget::itemPressed, this, &DownloadWordList::on_wordlist_click);

		ui->listWidget_4->setContextMenuPolicy(Qt::CustomContextMenu);
		connect(ui->listWidget_4, &QListWidget::customContextMenuRequested, this, &DownloadWordList::custom_context_menu);

		connect(this, &DownloadWordList::kr_def_found, this, &DownloadWordList::download_word_done);
		connect(this, &DownloadWordList::en_def_found, this, &DownloadWordList::download_word_done);
	}
	void custom_context_menu(const QPoint& pos)
	{
		// Handle global position
		QPoint globalPos = ui->listWidget_4->mapToGlobal(pos);

		// Create menu and insert some actions
		QMenu myMenu;
		myMenu.addAction("Add to my list", this, &DownloadWordList::start_download_word_list);

		// Show context menu at handling position
		myMenu.exec(globalPos);
	}

	void server_response_ready() {
		// When the server is down
		if (response_wordlist->error() != QNetworkReply::NoError) {
			return; 
		}

		wstring html = QString::fromUtf8(response_wordlist->readAll()).toStdWString();
		
		Parser::HTMLParser parser(html);
		auto itr = parser.DOM().begin();

		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"wordlist") {
				down_avail.push_back(wordlist_content(itr->attr[L"name"]));
			}
			else if (itr->tag == L"list") {
				down_avail.back().word_list_name.push_back(itr->attr[L"name"]);
				down_avail.back().words.push_back(vector<wstring>());
			}
		}

		draw_title_list();
	}
	void on_title_click(QListWidgetItem *item)
	{
		current_title = ui->listWidget_3->row(item);
		draw_word_list();
	}
	void draw_title_list()
	{
		auto itr = down_avail.begin();
		if (ui->listWidget_3->count() < down_avail.size()) {
			int num = down_avail.size() - ui->listWidget_3->count();
			while (num) {
				ui->listWidget_3->addItem("");
				num--;
			}
		}
		for (int i = 0; i < ui->listWidget_3->count(); i++) {
			QListWidgetItem* item = ui->listWidget_3->item(i);
			if (itr == down_avail.end()) {
				// Remove the rest of the ListWidget
				ui->listWidget_3->takeItem(i);
				i--;
				continue;
			}
			if (item->text().toStdWString() != itr->name) {
				ui->listWidget_3->insertItem(i, Util::wstr_to_str(itr->name).c_str());
			}
			itr++;
		}
	}
	void draw_word_list()
	{
		auto itr = down_avail[current_title].word_list_name.begin();
		if (ui->listWidget_4->count() < down_avail.size()) {
			int num = down_avail.size() - ui->listWidget_4->count();
			while (num) {
				ui->listWidget_4->addItem("");
				num--;
			}
		}
		for (int i = 0; i < ui->listWidget_4->count(); i++) {
			QListWidgetItem* item = ui->listWidget_4->item(i);
			if (itr == down_avail[current_title].word_list_name.end()) {
				// Remove the rest of the ListWidget
				ui->listWidget_4->takeItem(i);
				i--;
				continue;
			}
			if (item->text().toStdWString() != *itr) {
				ui->listWidget_4->insertItem(i, Util::wstr_to_str(*itr).c_str());
			}
			itr++;
		}
	}
	void on_wordlist_click(QListWidgetItem *item)
	{
		word_list_index = ui->listWidget_4->row(item);
		query_words();
	}

	void query_words()
	{
		string url = "http://104.199.179.40/word-list/" + Util::wstr_to_str(down_avail[current_title].name)
			+ "/" + Util::wstr_to_str(down_avail[current_title].word_list_name[word_list_index]);
		
		QUrl server_url(url.c_str());

		response_words = qnam.get(QNetworkRequest(server_url));
		connect(response_words, &QNetworkReply::finished, this, &DownloadWordList::receive_word_list);
	}

	void receive_word_list()
	{
		// When the server is down
		if (response_words->error() != QNetworkReply::NoError) {
			return;
		}

		wstring html = QString::fromUtf8(response_words->readAll()).toStdWString();
		std::wstringstream ws(html);

		wstring word;
		down_avail[current_title].words[word_list_index].clear();

		while (std::getline(ws, word)) {
			for (int i = 0; i < word.size(); i++) {
				if (word[i] == '\r') {
					word.erase(word.begin() + i); i--;
				}
			}
			down_avail[current_title].words[word_list_index].push_back(word);
		}

		auto itr = down_avail[current_title].words[word_list_index].begin();
		if (ui->listWidget_5->count() < down_avail.size()) {
			int num = down_avail.size() - ui->listWidget_5->count();
			while (num) {
				ui->listWidget_5->addItem("");
				num--;
			}
		}
		for (int i = 0; i < ui->listWidget_5->count(); i++) {
			QListWidgetItem* item = ui->listWidget_5->item(i);
			if (itr == down_avail[current_title].words[word_list_index].end()) {
				// Remove the rest of the ListWidget
				ui->listWidget_5->takeItem(i);
				i--;
				continue;
			}
			if (item->text().toStdWString() != *itr) {
				ui->listWidget_5->insertItem(i, Util::wstr_to_str(*itr).c_str());
			}
			itr++;
		}
	}

	// Max download at one time = 3 words; 
	// (limit on QNetworkAccessManager) 
	void start_download_word_list()
	{
		// Cannot start the download when the download is in progress
		if (download_in_progress) {
			QMessageBox msg_box;
			msg_box.setText("Sorry! Wait for other downloads to be completed");  
			msg_box.exec();
			return;
		}

		download_in_progress = true;

		const int max_download = 3;
		num_download_done = 0;

		word_response.resize(max_download * 2);
		download_done.resize(max_download);

		for (int i = 0; i < download_done.size(); i++) download_done[i] = false;

		downloading_words = down_avail[current_title].words[word_list_index];
		donwloading_wordlist_name = down_avail[current_title].word_list_name[word_list_index];

		current_download_word.resize(max_download);
		word_infos.resize(max_download);

		for (int i = 0; i < (max_download > downloading_words.size() ? downloading_words.size() : max_download); i++) {
			word_infos[i] = new WordInfo();
			word_infos[i]->set_word(downloading_words[i]);

			current_download_word[i] = downloading_words[i];
			download_word(i);

			total_download = i + 1;
		}
	}

	void download_word(int index)
	{
		string word = Util::wstr_to_str(current_download_word[index]);

		string s_kr_url = "http://alldic.daum.net/search.do?q=" + word;
		string s_en_url = "http://wordnetweb.princeton.edu/perl/webwn?s="
			+ word
			+ "&sub=Search+WordNet&o2=&o0=1&o8=1&o1=1&o7=&o5=&o9=&o6=&o3=&o4=&h=0000";

		QUrl kr_url(s_kr_url.c_str());
		QUrl en_url(s_en_url.c_str());

		word_response[index * 2] = qnam.get(QNetworkRequest(kr_url));
		word_response[index * 2 + 1] = qnam.get(QNetworkRequest(en_url));

		connect(word_response[index * 2], &QNetworkReply::finished, this, &DownloadWordList::parse_kr);
		connect(word_response[index * 2 + 1], &QNetworkReply::finished, this, &DownloadWordList::parse_en);
	}
	void parse_kr()
	{
		auto send = sender();
		int index = -1;

		for (int i = 0; i < word_response.size(); i++) {
			if (word_response[i] == send) {
				index = i; break;
			}
		}

		if (word_response[index]->error() != QNetworkReply::NoError) {
			emit kr_def_found(index);
			return;
		}

		wstring html = QString::fromUtf8(word_response[index]->readAll()).toStdWString();
		word_infos[index / 2]->parse_kr(html);

		emit kr_def_found(index);
	}
	void parse_en()
	{
		auto send = sender();
		int index = -1;

		for (int i = 0; i < word_response.size(); i++) {
			if (word_response[i] == send) {
				index = i; break;
			}
		}

		if (word_response[index]->error() != QNetworkReply::NoError) {
			emit en_def_found(index);
			return;
		}
		
		wstring html = QString::fromUtf8(word_response[index]->readAll()).toStdWString();
		word_infos[index / 2]->parse_en(html);

		emit en_def_found(index);
	}
	void download_word_done(int idx)
	{
		int index = idx / 2;
		if (download_done[index]) {
			bool result = words->register_word(
				current_download_word[index],
				word_infos[index]
			);

			num_download_done++;

			string status_msg = "[" + Util::wstr_to_str(current_download_word[index])
				+ "] is fetched (" + std::to_string(num_download_done)
				+ "/" + std::to_string(downloading_words.size()) + ")";

			ui->statusBar->showMessage(status_msg.c_str());

			if (total_download < downloading_words.size()) {
				current_download_word[index] = downloading_words[total_download];
				word_infos[index] = new WordInfo();
				word_infos[index]->set_word(downloading_words[total_download]);

				disconnect(word_response[index * 2], 0, 0, 0);
				disconnect(word_response[index * 2 + 1], 0, 0, 0);

				word_response[index * 2]->deleteLater();
				word_response[index * 2 + 1]->deleteLater();

				download_done[index] = false;
				download_word(index);

				total_download++;
			}
			else if(num_download_done == downloading_words.size()) {
				// When the download is done, add it to my word list
				WordList new_word_list(donwloading_wordlist_name);
				for (int i = 0; i < downloading_words.size(); i++) {
					new_word_list.add_word(downloading_words[i]);
				}
				words_manager->add_word_list(new_word_list);
				download_in_progress = false; 

				ui->statusBar->showMessage("Download complete!");

				std::wofstream out("saved_wordlist.wrd");

				// Sine MSVC does not have support on UTF-8 output, we have to 
				// manually specify it
				std::locale loc(std::locale::classic(), new std::codecvt_utf8<wchar_t>);
				out.imbue(loc);
				words->write_save_file(out);
				words_manager->write_word_list(out);
			}
		}
		else {
			download_done[index] = true;
		}

	}
};