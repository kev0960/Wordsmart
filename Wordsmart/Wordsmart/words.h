#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>

#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include "ui_wordsmart.h"
#include "HTMLParser.h"
#include <QtWidgets/QMainWindow>


using std::map;
using std::string;
using std::wstring;
using std::vector;

class WordInfo : public QObject {
	Q_OBJECT

	string word;
	vector<wstring> kr_definition;
	vector<wstring> en_definition;

	bool kr_def_parse_done, en_def_parse_done;

	// Number of times that the user has asked for the meaning of the word
	int frequency;

	QNetworkReply* response_kr;
	QNetworkReply* response_en;

	QNetworkAccessManager qnam;

	vector<wstring> split_example_sentence(wstring& s) {
		vector<wstring> v;

		int begin = 0;
		auto found = s.find(L';');
		while (found != wstring::npos) {
			v.push_back(s.substr(begin + 1, found - begin - 2));
			begin = found + 2;
			found = s.find(L';', found + 1);
		}
		return v;
	}

signals:
	// Signal when the definition of the word is fully fetched and parsed
	void def_found(string word);
	void kr_def_found();
	void en_def_found();

public:
	WordInfo(string word) : word(word), kr_def_parse_done(false), en_def_parse_done(false), QObject(Q_NULLPTR) {
		string url_kr_dic =
			"http://alldic.daum.net/search.do?q="
			+ word;

		string url_en_dic =
			"http://wordnetweb.princeton.edu/perl/webwn?s="
			+ word
			+ "&sub=Search+WordNet&o2=&o0=1&o8=1&o1=1&o7=&o5=&o9=&o6=&o3=&o4=&h=0000";

		QUrl search_url_kr(url_kr_dic.c_str());
		QUrl search_url_en(url_en_dic.c_str());

		response_kr = qnam.get(QNetworkRequest(search_url_kr));
		response_en = qnam.get(QNetworkRequest(search_url_en));

		connect(response_kr, &QNetworkReply::finished, this, &WordInfo::parse_kr_dic);
		connect(response_en, &QNetworkReply::finished, this, &WordInfo::parse_en_dic);

		connect(this, &WordInfo::kr_def_found, this, &WordInfo::parse_done);
		connect(this, &WordInfo::en_def_found, this, &WordInfo::parse_done);
	}
	void parse_done() {
		if (kr_def_parse_done && en_def_parse_done) {
			emit def_found(word);
		}
	}
	void parse_kr_dic() {
		kr_def_parse_done = true;

		// Network Error occured
		if (response_kr->error() != QNetworkReply::NoError) {
			emit kr_def_found();
			return;
		}

		/*
		
		DAUM English Dictionary Format

		<ul class="list_search">
			<li>
				<daum:word> { Meaning } </daum:word>
				<daum:word> { of } </daum:word>
				<daum:word> { the } </daum:word>
				<daum:word> { word } </daum:word> 

				** To parse this, we need to get each partial part of
				the meaning and concantenate it. 

			</li>
			<li>
			<daum:word> { Meaning of the word } </daum:word>
			</li>
		</ul>
		
		*/
		QString html = QString::fromUtf8(response_kr->readAll());
		wstring html_str = html.toStdWString();

		wstring key_class = L"<ul class=\"list_search\">";
		wstring end_tag = L"</ul>";

		auto found = html_str.find(key_class);
		auto end = html_str.find(end_tag, found + 1);

		if (found != wstring::npos) {
			wstring def = html_str.substr(found, end + 5 - found);

			found = def.find(L"<li>");
			while (found != wstring::npos) {
				end = def.find(L"</li>", found);

				wstring temp = def.substr(found, end + 5 - found);
				Parser::HTMLParser parser(temp);
				Parser::DOMTree::iterator itr = parser.DOM().begin();

				wstring definition;
				Parser::DOMTree::iterator temp_itr = itr;

				int from = found;
				for (; itr != parser.DOM().end(); ++itr) {
					if (itr->tag == L"daum:word") {
						definition += itr->inner_text;

						from = def.find(L"</daum:word>", from + 1);
						if (def[from + 12] == L' ') definition += L' ';
					}
				}

				if (definition.empty()) {
					itr = temp_itr;
					for (; itr != parser.DOM().end(); ++itr) {
						if (itr->tag == L"span" && itr->attr[L"class"] == L"txt_search") {
							definition = itr->inner_text;
						}
					}
				}

				kr_definition.push_back(definition);

				found = def.find(L"<li>", end + 1);
			}
		}
	}

	/*
	void parse_kr_dic() {
		kr_def_parse_done = true;

		// Network Error occured
		if (response_kr->error() != QNetworkReply::NoError) {
			emit kr_def_found();
			return;
		}

		QString html = QString::fromUtf8(response_kr->readAll());
		wstring html_str = html.toStdWString();

		

		NAVER English Dictionary has following format

		<dl class="list_e2">
			...
			<span class="fnt_k09"> { Noun Verb Adj etc. } </span>
			<span class="fnt_k05"> { Meaning of the word } </span>

			...
			<a href="..." class="fnt_k22 N=a....random string"> { Other Meanings } </a>
			<a href="..." class="fnt_k22 N=a....random string"> { Other Meanings } </a>
		</dl>


		
		wstring key_class = L"<dl class=\"list_e2\">";
		wstring end_tag = L"</dl>";

		auto found = html_str.find(key_class);
		auto end = html_str.find(end_tag, found);
		wstring def = html_str.substr(found, end + 5 - found);

		Parser::HTMLParser parser(def);
		Parser::DOMTree::iterator itr = parser.DOM().begin();

		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"span" && itr->attr[L"class"] == L"fnt_k09") {
				definition.push_back(itr->inner_text);
				break;
			}
		}

		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"span" && itr->attr[L"class"] == L"fnt_k05") {
				if (definition.size()) definition[0] += L"  " + itr->inner_text;
				else definition.push_back(itr->inner_text);

				break;
			}
		}

		// Additional definitions
		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"a") {
				if (itr->attr[L"class"].size() >= 7 && itr->attr[L"class"].substr(0, 7) == L"fnt_k22") {
					definition.push_back(itr->inner_text);
				}
			}
			if (itr->tag == L"dd") break;
		}

		emit kr_def_found();
	}
	*/

	void parse_en_dic() {
		en_def_parse_done = true;

		// Network Error occured
		if (response_en->error() != QNetworkReply::NoError) {
			emit en_def_found();
			return;
		}

		wstring html = QString::fromUtf8(response_en->readAll()).toStdWString();
		wstring key_tag = L"<li>";
		wstring end_tag = L"</li>";

		/*

		Wordnet Online Search Format

		<li>
			<a class="pos"> { n, v, adj ... } </a>
			<a class="...."> { synynoms } </a>
			<a class="...."> { synynoms } </a>
			...
			{ Definition of the word goes here }
			<i> { word usage in a sentence } </i>
		</li>

		*/

		int i = 0;
		auto found = html.find(key_tag);
		while (found != wstring::npos) {
			auto end = html.find(end_tag, found);

			Parser::HTMLParser parser(html.substr(found, end - found + 5));
			Parser::DOMTree::iterator itr = parser.DOM().begin();

			wstring html_def = L"<li style='margin-top:10px'>";

			for (; itr != parser.DOM().end(); ++itr) {
				if (itr->tag == L"a" && itr->attr[L"class"] == L"pos") {
					html_def += L"<span style='color:blue'>" + itr->inner_text + L" </span>";

					++itr;
					break;
				}
			}
			wstring def = parser.DOM().begin()->inner_text;
			for (int i = 0; i < def.size(); i++) {
				if (def[i] == L'(') {
					def = def.substr(i + 1);
					break;
				}
			}
			def.pop_back();

			html_def += def;

			int num_syn = 0;
			bool chk_example = false;
			for (; itr != parser.DOM().end(); ++itr) {
				if (itr->tag == L"a") {
					if (num_syn == 0) {
						html_def += L"<br/><span style='color:gray;padding-left:15px'>";
					}
					if (num_syn > 0) html_def += L",";

					html_def += L" " + itr->inner_text;
					num_syn++;
				}
				else if (itr->tag == L"i") {
					if (num_syn) html_def += L"</span>";
					vector<wstring> examples = split_example_sentence(itr->inner_text);
					if (examples.size()) html_def += L"<ul style='margin-top:15px'>";

					for (int i = 0; i < examples.size(); i++) {
						html_def += L"<li>" + examples[i] + L"</li>";
					}
					chk_example = true;

					if (examples.size()) html_def += L"</ul>";
					break;
				}
			}

			if (!chk_example && num_syn) html_def += L"</span>";

			html_def += L"</li>";
			en_definition.push_back(html_def);

			found = end;
			found = html.find(key_tag, found + 1);
			i++;
		}

		emit en_def_found();
	}


	WordInfo() : QObject(Q_NULLPTR) {

	}

	wstring get_kr_definition(int x) const {
		if (x < kr_definition.size()) {
			return kr_definition[x];
		}
		return L"";
	}

	wstring get_en_definition(int x) const {
		if (x < en_definition.size()) {
			return en_definition[x];
		}
		return L"";
	}


	int num_def() const {
		return kr_definition.size();
	}

	int num_en_def() const {
		return en_definition.size();
	}

	string get_word() const { return word;  }
	void inc_freqeuncy() { frequency++; }
	int get_frequency() { return frequency; }
};

class Words : public QObject {
	Q_OBJECT

	map<string, WordInfo> registered_words;

	// Found word 
	std::set<string> found_words;

	// convert a word into a standard format
	void normalize_word(string& word) {
		for (int i = 0; i < word.size(); i++) {
			word[i] = tolower(word[i]);
		}
	}

signals:
	void defFound(const WordInfo& word);

public:
	bool register_word(string word) {
		if (!is_word(word)) return false;

		normalize_word(word);

		found_words.insert(word);

		if (registered_words.find(word) == registered_words.end()) {
			registered_words.emplace(std::make_pair(word, word));

			// When the definition of this word is found, it callbacks to the word_is_found function
			connect(&registered_words[word], &WordInfo::def_found, this, &Words::word_is_found);
		}
		else {
			// If the word is already registered, we inc the freqeuncy
			registered_words[word].inc_freqeuncy();

			// emit the signal
			word_is_found(word);
		}

		return true;
	}

	void word_is_found(string word) {
		emit defFound(registered_words[word]);
	}

	// Check whether the word is in a correct format and fix it
	// To prevent unwanted datas to be registered into a word list, 
	// we assume that the user wanted words meet following conditions. 
	//
	// Copied data should be
	// 1. One continuous alphabet sequence
	// (If multiple words are found then the entire sequence will be discarded)
	// 2. Not having special characters
	// (But punctuation marks would be removed, like period, comman, parenthesis, etc. ) 

	bool is_word(string& word) {
		bool separate_found = false;

		for (int i = 0; i < word.size(); i++) {
			if (word[i] == ' ') separate_found = true;
			if (separate_found && isalpha(word[i])) {
				return false;
			}

			// Impossible characters (Used for URL)
			if (word[i] == '/') return false;
		}

		string mask = ",.\"':;-` <>(){}!?";
		for (int i = 0; i < word.size(); i++) {
			for (int j = 0; j < mask.size(); j++) {
				if (word[i] == mask[j]) {
					word.erase(i, 1);
					i--;
					break;
				}
			}
		}

		for (int i = 0; i < word.size(); i++) {
			if (!isalpha(word[i])) return false;
		}
		return true;
	}

	std::set<string>& get_word_list() {
		return found_words;
	}

	WordInfo& get_word_info(string w) {
		if (found_words.find(w) != found_words.end()) {
			return registered_words[w];
		}
	}

	Words() : QObject(Q_NULLPTR) {

	}
};