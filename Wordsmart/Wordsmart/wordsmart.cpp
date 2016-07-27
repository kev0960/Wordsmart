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
}
void Wordsmart::clipboard_changed()
{
	qDebug() << "clipboard is changed! \n";
	qDebug() << clipboard->text();

	ui.textBrowser->setText(clipboard->text());
	my_words.register_word(clipboard->text().toStdString());
}
void Wordsmart::show_version()
{
	QMessageBox msgBox;
	msgBox.setText(tr("Wordsmart v.0.0.1"));
	msgBox.exec();
}
void Wordsmart::create_action() {
	ui.actionVersion->setStatusTip(tr("Show me the version of this program"));
	connect(ui.actionVersion, &QAction::triggered, this, &Wordsmart::show_version);

}
Wordsmart::~Wordsmart()
{

}
