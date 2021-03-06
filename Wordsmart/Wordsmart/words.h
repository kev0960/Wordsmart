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

template <typename T>
inline T max(T a, T b)
{
	return a < b ? b : a;
}

namespace Util {
	inline string wstr_to_str(wstring ws) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.to_bytes(ws);
	}
	inline wstring str_to_wstr(string s) {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		return converter.from_bytes(s);
	}
	inline long long wstr_to_long_long(wstring s) {
		std::wstringstream ws(s); 
		long long x; ws >> x; return x;
	}
}

class Memorize {
	vector<time_t> memorized;
	vector<int> score;

public:
	Memorize() {}
	Memorize(vector<time_t>& m, vector<int>& s) : memorized(m), score(s) {

	}
	Memorize(const Memorize& m) : memorized(m.memorized), score(m.score) {}

	void set_memorization(int sc) {
		memorized.push_back(time(0));
		score.push_back(sc);
	}

	// If user have successfully memorized for the last 3 cases,
	// the user confirms to have fully memorized (and don't show)
	
	// Even if the user passes the last 3 cases, we should check whether
	// the date difference is more than 1 week. If more than 1 week has passed,
	// we should present word in a flashcard. 
	
	// Finally, if the user has managed to pass last 3 checks for 3 weeks, 
	// the word will be registered as a permanent memorization.
	bool present_word() {
		const int min_passed_test = 3;
		const int min_score = 3;

		// Check from the back
		int cnt = 0;
		for (int i = score.size() - 1; i >= max(0, static_cast<int>(score.size()) - min_passed_test); i--) {
			if (score[i] >= min_score) cnt++;
		}

		if (cnt < min_passed_test) return true;

		int i = score.size() - 1;
		while (i >= 0) {
			if (score[i] < min_passed_test) break;
			i--;
		}
		if (i < 0) i= 0;

		if (memorized[i] <= time(0) - 60 * 60 * 24 * 7 * 3) { 
			return false;
		}
		return true;
	}
	bool recent_memorized() {
		const int min_score = 3;
		if (!memorized.size()) return false; 
		time_t last = memorized.back();

		// If you have memorized the word less than 2 hours ago,
		// do not present in a flashcard temporarily
		if (time(0) - last <= 60 * 60 * 2 && score.back() >= min_score) {
			return true;
		}
		return false; 
	}
	string last_visited() {
		if (!score.size()) return "";
		struct tm* time_info = localtime(&memorized.back());

		char buf[80];
		strftime(buf, 80, "%Y-%m-%d", time_info);
		return buf;
	}

	void add_memory(time_t mem, int sc) {
		memorized.push_back(mem);
		score.push_back(sc);
	}

	wstring stringify() {
		wstring s;
		for (int i = 0; i < memorized.size(); i++) {
			s += L"<mem time=\"";
			s += std::to_wstring(static_cast<unsigned long long>(memorized[i]));
			s += L"\" score=\"";
			s += std::to_wstring(static_cast<unsigned long long>(score[i]));
			s += L"\"></mem>";
		}
		return s;
	}

	void add_score(int sc) {
		if (!memorized.size()) {
			memorized.push_back(time(0));
			score.push_back(sc);
			return;
		}

		time_t now = time(0);

		// If the difference between last visit is less than 2 hours,
		// we consider that the user is continuing his flashcard practice
		if (now - memorized.back() < 2 * 3600) {
			score.back() += sc;
		}
		else {
			memorized.push_back(time(0));
			score.push_back(sc);
		}
	}

	bool user_forget() {
		if (!memorized.size()) {
			return false;
		}

		score.back() = 0;
		return true;
	}

	int current_score() {
		if (!memorized.size()) {
			return 0;
		}

		time_t now = time(0);

		// If the difference between last visit is less than 2 hours,
		// we consider that the user is continuing his flashcard practice
		if (now - memorized.back() < 2 * 3600) {
			return score.back();
		}
		else {
			return 0;
		}
	}
};

class WordList {
	vector<std::pair<wstring, Memorize>> words;
	wstring name;

	int current_word;
	bool flashcard_showing_word; 

public:
	WordList(wstring name) 
		: name(name), flashcard_showing_word(true), current_word(0) {

	}
	WordList(const WordList& w) 
		: words(w.words), name(w.name), current_word(w.current_word),
		flashcard_showing_word(w.flashcard_showing_word) {

	}

	vector<std::pair<wstring, Memorize>> get_words() {
		return words;
	}

	bool add_word(wstring s) {
		for (int i = 0; i < words.size(); i++) {
			if (words[i].first == s) return false; 
		}
		words.push_back(std::make_pair(s, Memorize()));

		if (current_word == -1) {
			current_word = words.size() - 1;
		}

		return true;
	}
	bool add_word(wstring s, Memorize& mem) {
		for (int i = 0; i < words.size(); i++) {
			if (words[i].first == s) return false;
		}
		words.push_back(std::make_pair(s, mem));

		if (current_word == -1) {
			current_word = words.size() - 1;
		}

		return true;
	}

	bool delete_word(wstring word) {
		for (int i = 0; i < words.size(); i++) {
			if (words[i].first == word) {
				words.erase(words.begin() + i);

				if (current_word > words.size()) current_word = words.size() - 1;
				return true;
			}
		}
		return false; 
	}
	bool set_memorization(wstring word, int sc)
	{
		for (int i = 0; i < words.size(); i++) {
			if (words[i].first == word) {
				words[i].second.set_memorization(sc);
				return true;
			}
		}
		return false;
	}

	bool merge(WordList& w) {
		for (int i = 0; i < w.words.size(); i++) {
			bool duplicate = false;
			for (int j = 0; j < words.size(); j++) {
				if (w.words[i].first == words[j].first) {
					duplicate = true;
					break;
				}
			}

			if (!duplicate) {
				add_word(w.words[i].first, w.words[i].second);
			}
		}
		return true;
	}

	wstring get_name() {
		return name;
	}
	
	void set_name(wstring& n) {
		name = n;
	}

	void flip() {
		flashcard_showing_word = !flashcard_showing_word;
	}

	void user_memorized() {
		if (current_word != -1) {
			words[current_word].second.add_score(1);
		}
	}
	void user_forgot() {
		if (current_word != -1) {
			words[current_word].second.user_forget();
		}
	}

	int memorize_count() {
		if (current_word != -1) {
			return words[current_word].second.current_score();
		}
		return -1;
	}

	// When it goes to the last word, it comes back to the first one
	bool next_word() {
		int previous_word = current_word;
		if (current_word == -1) return false;

		do {
			current_word++; 
			if (current_word == words.size()) {
				current_word = 0;
			}
			
			// If all words are memorized so when there is nothing to present
			// it returns false. 
			if (current_word == previous_word && words[current_word].second.recent_memorized()) {
				current_word = -1;
				return false; 
			}
		} while (words[current_word].second.recent_memorized());
		return true;
	}

	wstring get_word() {
		if (current_word != -1) {
			return words[current_word].first;
		}
		return L"";
	}

	bool all_words_memorized() {
		if (current_word == -1) return true;
		else {
			for (int i = 0; i < words.size(); i++) {
				if (!words[i].second.recent_memorized()) return false;
			}
		}
		return true; 
	}
	bool is_showing_word() {
		return flashcard_showing_word;
	}

	bool is_flashcard_empty() {
		return words.empty();
	}

	int get_current_index() {
		return current_word;
	}
	// Red :: Most of the words are not memorized (0 ~ 50%)
	// Yellow : More than half of the words are memorized (50% ~ 90%)
	// Green : Most of the words are memorized (90 ~ 99%)
	// Blue : All words are memorized
	QColor get_color() {
		int total = words.size(); 
		if (total == 0) return Qt::green;

		for (int i = 0; i < words.size(); i++) {
			if (words[i].second.present_word()) total--;
		}

		float percent = static_cast<float>(total) / words.size();

		if (percent <= 0.5) {
			return Qt::red;
		}
		else if (percent <= 0.9) {
			return Qt::yellow;
		}
		else if (percent == 1.0f) {
			return Qt::blue;
		}
		else {
			return Qt::green;
		}
	}
};
class WordListManager {
	vector<WordList> word_lists;
	int current_word_list;

public:
	WordListManager() : current_word_list(0) {

	}

	vector<WordList>& get_word_list() {
		return word_lists;
	}

	WordList& get_current_word_list() {
		if (current_word_list >= word_lists.size()) {
			current_word_list = word_lists.size() - 1;
		}
		return word_lists[current_word_list];
	}

	bool is_empty() {
		return !word_lists.size();
	}

	// Merge word lists into one word list
	// (It all merges into the least index word list)
	void merge_word_list(vector<int> selected) {
		std::sort(selected.begin(), selected.end());

		WordList& w = word_lists[selected[0]];
		for (int i = selected.size() - 1; i > 0; i --) {
			w.merge(word_lists[selected[i]]);
			word_lists.erase(word_lists.begin() + selected[i]);
		}
	}

	vector<wstring> get_word_list_names() {
		vector<wstring> list; 
		for (int i = 0; i < word_lists.size(); i++) {
			list.push_back(word_lists[i].get_name());
		}
		return list;
	}
	bool select_word_list(int index) {
		if (index < word_lists.size()) {
			current_word_list = index;
			return true;
		}
		return false; 
	}
	// Returns true if the word is added to the previous word list
	// Returns false if a new word list is created and the word is added to it
	bool add_word(wstring word) {
		time_t t = time(0);
		struct tm* time_info = localtime(&t);

		char buf[80];
		strftime(buf, 80, "%Y-%m-%d", time_info);

		wstring today = Util::str_to_wstr(string(buf));
		// Today's word list
		if (word_lists.size() && word_lists.back().get_name() == today) {
			word_lists.back().add_word(word);
			return true;
		}
		else {
			word_lists.push_back(WordList(today));
			word_lists.back().add_word(word);
			return false; 
		}
	}
	void add_word_list(WordList& w) {
		word_lists.push_back(w);
	}

	void write_word_list(std::wofstream& out) {
		for (int i = 0; i < word_lists.size(); i++) {
			out << L"<wordlist name=\"" + word_lists[i].get_name() + L"\">";

			auto words = word_lists[i].get_words();
			for (int j = 0; j < words.size(); j++) {
				out << L"<word word=\""
					<< words[j].first
					<< "\">"
					<< words[j].second.stringify() << "</word>";
			}
			out << L"</wordlist>";
		}
	}

	bool delete_current_word(wstring word) {
		if (current_word_list < word_lists.size()) {
			return word_lists[current_word_list].delete_word(word);
		}
		return false; 
	}

	bool delete_current_word_list() {
		if (current_word_list < word_lists.size()) {
			word_lists.erase(word_lists.begin() + current_word_list);
			return true;
		}
		return false; 
		return false;
	}
};
class WordInfo : public QObject {
	Q_OBJECT

	wstring word;
	vector<wstring> kr_definition;
	vector<wstring> en_definition;

	bool kr_def_parse_done, en_def_parse_done;

	// Number of times that the user has asked for the meaning of the word
	int frequency;

	QNetworkReply* response_kr;
	QNetworkReply* response_en;

	static QNetworkAccessManager qnam;

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
	void def_found(wstring word);
	void kr_def_found();
	void en_def_found();

public:
	WordInfo(wstring word) : word(word), kr_def_parse_done(false), en_def_parse_done(false), QObject(Q_NULLPTR) {
		if (word == L"") return;

		string url_kr_dic =
			"http://alldic.daum.net/search.do?q="
			+ Util::wstr_to_str(word);

		string url_en_dic =
			"http://wordnetweb.princeton.edu/perl/webwn?s="
			+ Util::wstr_to_str(word)
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
	WordInfo(wstring word, vector<wstring>& kr_definition, vector<wstring>& en_definition)
		: word(word), kr_definition(kr_definition), en_definition(en_definition)
	{

	}

	WordInfo& operator= (WordInfo&& w) 
	{
		word = w.word;
		kr_definition = w.kr_definition;
		en_definition = w.en_definition;

		return *this;
	}

	void parse_done() {
		if (kr_def_parse_done && en_def_parse_done) {
			emit def_found(word);
		}
	}
	void parse_kr(wstring& html_str) 
	{
		/*

		DAUM English Dictionary Response Format

		<ul class="list_search">
		<li>
		<span text_search="">
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
						wstring inside = itr->inner_html();
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
	void parse_kr_dic() {
		kr_def_parse_done = true;

		// Network Error occured
		if (response_kr->error() != QNetworkReply::NoError) {
			emit kr_def_found();
			return;
		}

		response_kr->close();
		QString html = QString::fromUtf8(response_kr->readAll());
		wstring html_str = html.toStdWString();

		parse_kr(html_str);

		emit kr_def_found();
	}

	void parse_en(wstring& html) {
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
						html_def += L"<br/><span style='color:red;padding-left:15px'>";
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
	}
	void parse_en_dic() {
		en_def_parse_done = true;

		// Network Error occured
		if (response_en->error() != QNetworkReply::NoError) {
			emit en_def_found();
			return;
		}

		response_en->close();

		wstring html = QString::fromUtf8(response_en->readAll()).toStdWString();
		parse_en(html);

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

	wstring get_word() const { return word;  }
	void set_word(wstring s) { word = s; }

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

		o << L"<word word=\"" << word << L"\">";
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

private:
	map<wstring, WordInfo*> registered_words;

	// Found word 
	std::set<wstring> found_words;

	// convert a word into a standard format
	void normalize_word(wstring& word) {
		for (int i = 0; i < word.size(); i++) {
			word[i] = tolower(word[i]);
		}
	}

signals:
	void defFound(const WordInfo& word);

public:
	bool register_word(wstring word) {
		if (!is_word(word)) return false;

		normalize_word(word);

		found_words.insert(word);

		if (registered_words.find(word) == registered_words.end()) {
			WordInfo* word_info = new WordInfo(word);
			registered_words[word] = word_info;

			// When the definition of this word is found, it callbacks to the word_is_found function
			connect(registered_words[word], &WordInfo::def_found, this, &Words::word_is_found);
		}
		else {
			// If the word is already registered, we inc the freqeuncy
			registered_words[word]->inc_freqeuncy();

			// emit the signal
			word_is_found(word);
		}

		return true;
	}
	bool register_word(wstring word, WordInfo* word_info) {
		if (registered_words.find(word) == registered_words.end()) {
			registered_words[word] = word_info;
			found_words.insert(word);
			return true;
		}
		return false;
	}

	void word_is_found(wstring word) {
		emit defFound(*registered_words[word]);
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

	bool is_word(wstring& word) {
		bool separate_found = false;
		if (word.size() >= 30) return false; 

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

	std::set<wstring>& get_word_list() {
		return found_words;
	}

	WordInfo* get_word_info(wstring w) {
		if (found_words.find(w) != found_words.end()) {
			return registered_words[w];
		}
		return nullptr;
	}

	bool write_save_file(std::wofstream& out) {
		if (!out) return false;

		out << L"<words>";
		for (auto itr = found_words.begin(); itr != found_words.end(); itr++) {
			registered_words[*itr]->write_file(out);
		}
		out << L"</words>";

		return true;
	}
	bool delete_word(wstring word) {
		if (registered_words.find(word) != registered_words.end()) {
			registered_words.erase(word);
			found_words.erase(word);
			return true;
		}
		return false;
	}
	bool read_save_file(std::wifstream& in, WordListManager& word_list_manager) {
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
					wstring s_word = current_word;
					found_words.insert(s_word);

					registered_words[s_word] = new WordInfo(s_word, kr_definition, en_definition);
					
					kr_definition.clear();
					en_definition.clear();
				}
				current_word = itr->attr[L"word"];
			}
			else if (itr->tag == L"kr_def") {
				kr_definition.push_back(itr->inner_html());
			}
			else if (itr->tag == L"en_def") {
				en_definition.push_back(itr->inner_html());
			}
			else if (itr->tag == L"wordlist") {
				break;
			}
		}

		if (current_word.size()) {
			// Register the previous current_word
			wstring s_word = current_word;
			found_words.insert(s_word);
			registered_words[s_word] = new WordInfo(s_word, kr_definition, en_definition);
		}

		// Now read the word list
		wstring word_list_name;
		wstring word;

		WordList word_list(L"");
		Memorize m;

		for (; itr != parser.DOM().end(); ++itr) {
			if (itr->tag == L"wordlist") {
				if (word_list_name != L"") {
					if (word != L"") {
						word_list.add_word(word, m);
					}
					word_list_manager.add_word_list(word_list);

					// Clear 
					word_list = WordList(L"");
					m = Memorize();
					word = L"";
				}
				word_list_name = itr->attr[L"name"];
				word_list.set_name(word_list_name);
			}
			else if (itr->tag == L"word") {
				// Add the previous mem and score to the word list
				if (word != L"") {
					word_list.add_word(word, m);
				}

				word = itr->attr[L"word"];
				m = Memorize();
			}
			else if (itr->tag == L"mem") {
				m.add_memory(
					Util::wstr_to_long_long(itr->attr[L"time"]),
					Util::wstr_to_long_long(itr->attr[L"score"])
				);
			}
		}
		
		if (word_list_name != L"") {
			if (word != L"") {
				word_list.add_word(word, m);
			}
			word_list_manager.add_word_list(word_list);

			// Clear 
			word_list = WordList(L"");
			m = Memorize();
			word = L"";
		}
		return true;
	}

	Words() : QObject(Q_NULLPTR) {

	}
};