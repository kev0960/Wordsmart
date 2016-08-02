#include "wordsmart.h"
#include "words.h"

#include <QtWidgets/QApplication>
#include <QtGlobal>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	Wordsmart w;
	w.show();

	// Load the previous saved word list
	w.load_saved_wordlist();
	
	return a.exec();
}
