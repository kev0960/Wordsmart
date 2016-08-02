#pragma once
#include <vector>
#include <string>
#include <map>
#include <set>
#include <utility>
#include <exception>
#include <fstream>

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

	std::wstring str_to_wstr(string& s) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(s);
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
	WordInfo(const WordInfo& w) {
		word = w.word;
		kr_definition = w.kr_definition;
		en_definition = w.en_definition;
	}
	WordInfo(string& word, vector<wstring>& kr_definition, vector<wstring>& en_definition)
		: word(word), kr_definition(kr_definition), en_definition(en_definition)
	{

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
		
		DAUM English Dictionary Response Format

		<ul class="list_search">
			<li>
				<daum:word> { Meaning } </daum:word>
				<daum:word> { of } </daum:word>
				<daum:word> { the } </daum:word>
				<daum:word> { word } </daum:word> 

				** To parse this, we need to get each partial part of
				the meaning and concantenate it. 

				** sometimes the response does not contain <daum:word> but instead
				just a simple <span class="text_search">

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
					if (itr->tag == L"span" && itr->attr[L"class"] == L"txt_search") {
						wstring inside = itr->inner_html;
						// strip off tags
						int x = inside.find(L"<");
						int y = inside.find(L">");

						while (x != wstring::npos) {
							inside.erase(x, y - x + 1);

							x = inside.find(L"<");
							y = inside.find(L">");
						}
						kr_definition.push_back(inside);
						break;
					}
				}

				found = def.find(L"<li>", end + 1);
			}
		}
	}
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

	/*

	File output format

	<wordlist>
		<word word="{ Word }">
			<kr_def> { Korean Definition } </kr_def>
			<en_def> { English Definition } </en_def>
			<mem_date result="{ Mem count }"> { Date when last memorized } </mem_date>
		</word>
	</wordlist>

	*/

	bool write_file(std::wofstream& o) {
		if (!o) return false;

		o << L"<word word=\"" << str_to_wstr(word) << L"\">";
		for (int i = 0; i < kr_definition.size(); i++) {
			o << L"<kr_def>" << kr_definition[i] << L"</kr_def>";
		}
		for (int i = 0; i < en_definition.size(); i++) {
			o << L"<en_def>" << en_definition[i] << L"</en_def>";
		}
		o << "</word>\n";
		return true;
	}
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

	string wstr_to_str(wstring& wstr) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(wstr);
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

	WordInfo get_word_info(string w) {
		if (found_words.find(w) != found_words.end()) {
			return registered_words[w];
		}
		return WordInfo("");
	}

	bool write_save_file(std::wofstream& out) {
		if (!out) return false;

		out << L"<wordlist>";
		for (auto itr = found_words.begin(); itr != found_words.end(); itr++) {
			registered_words[*itr].write_file(out);
		}
		out << L"</wordlist>";

		return true;
	}
	bool delete_word(string word) {
		if (registered_words.find(word) != registered_words.end()) {
			registered_words.erase(word);
			found_words.erase(word);
			return true;
		}
		return false;
	}
	bool read_save_file(std::wifstream& in) {
		if (!in) return false;

		wstring file_data; 
		wstring temp;
		while (getline(in, temp)) {
			file_data += temp;
		}

		Parser::HTMLParser parser(file_data);
		Parser::DOMTree::iterator itr = parser.DOM().begin();

		wstring current_word; 
		vector<wstring> kr_definition, en_definition;

		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"word") {
				if (current_word.size()) {
					// Register the previous current_word
					string s_word = wstr_to_str(current_word);
					found_words.insert(s_word);
					registered_words.emplace(
						s_word, 
						WordInfo(s_word, kr_definition, en_definition));

					kr_definition.clear();
					en_definition.clear();
				}
				current_word = itr->attr[L"word"];
			}
			else if (itr->tag == L"kr_def") {
				kr_definition.push_back(itr->inner_html);
			}
			else if (itr->tag == L"en_def") {
				en_definition.push_back(itr->inner_html);
			}
		}

		if (current_word.size()) {
			// Register the previous current_word
			string s_word = wstr_to_str(current_word);
			found_words.insert(s_word);
			registered_words.emplace(s_word, WordInfo(s_word, kr_definition, en_definition));
		}

		return true;
	}

	Words() : QObject(Q_NULLPTR) {

	}
};