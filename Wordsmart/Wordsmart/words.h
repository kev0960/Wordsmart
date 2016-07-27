#pragma once
#include <vector>
#include <string>
#include <map>
#include <utility>

#include <QtNetwork>
#include <QUrl>
#include <QNetworkAccessManager>
#include "ui_wordsmart.h"
#include <QtWidgets/QMainWindow>
#include <ParserDom.h>

using std::map;
using std::string;

class WordInfo : public QObject {
	string meaning;

	// Number of times that the user has asked for the meaning of the word
	int frequency;

	QNetworkReply* response; 
	QNetworkAccessManager qnam;

public:
	WordInfo(string word) : QObject(Q_NULLPTR) {
		string url = 
			"http://wordnetweb.princeton.edu/perl/webwn?s="
			+ word
			+ "&sub=Search+WordNet&o2=&o0=1&o8=1&o1=1&o7=&o5=&o9=&o6=&o3=&o4=&h=0000";

		QUrl search_url(url.c_str());

		response = qnam.get(QNetworkRequest(search_url));
		connect(response, &QNetworkReply::finished, this, &WordInfo::finished_fetch);
	}

	void finished_fetch() {
		// Network Error occured
		if (response->error() != QNetworkReply::NoError) return; 

		QString html = QString::fromUtf8(response->readAll());
		htmlcxx::HTML::ParserDom parser;
		tree<htmlcxx::HTML::Node> dom = parser.parseTree(html.toStdString());
	}
	WordInfo() : QObject(Q_NULLPTR) {

	}

	void inc_freqeuncy() { frequency++; }
	int get_frequency() { return frequency;  }
};

class Words {
	map<string, WordInfo> registered_words;
	
public:
	bool register_word(string word) {
		if (!is_word(word)) return false;

		if (registered_words.find(word) == registered_words.end()) {
			registered_words.emplace(std::make_pair(word, word));
		}
		else {
			// If the word is already registered, we inc the freqeuncy
			registered_words[word].inc_freqeuncy();
		}

		return true;
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
};