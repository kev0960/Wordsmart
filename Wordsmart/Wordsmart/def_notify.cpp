#include "def_notify.h"

Notify::Notify(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
	ui->setupUi(this);

	setAttribute(Qt::WA_ShowWithoutActivating);
	setWindowFlags(
		Qt::Window | // Add if popup doesn't show up
		Qt::FramelessWindowHint | // No window border
		Qt::WindowDoesNotAcceptFocus | // No focus
		Qt::WindowStaysOnTopHint // Always on top
	);

	ui->textBrowser->viewport()->installEventFilter(this);
	setWindowOpacity(0.8);
}
void Notify::timerEvent(QTimerEvent* event) 
{
	if (timer_id == event->timerId()) {
		close_notify();
	}
}
bool Notify::eventFilter(QObject* target, QEvent* event) 
{
	if (target == ui->textBrowser->viewport() && event->type() == QEvent::MouseButtonPress) {
		close_notify();
		return true;
	}
	return QWidget::eventFilter(target, event);
}
Notify::~Notify() {
	delete ui;
}

void Notify::show_notification(const WordInfo& w)
{
	timer_id = startTimer(1000 * 8);

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
	ui->textBrowser->setText(qs);

	// resize the widget, as text label may be larger than the previous size 
	setGeometry(QStyle::alignedRect(
		Qt::RightToLeft,
		Qt::AlignBottom,
		size(),
		qApp->desktop()->availableGeometry()));

	show();
}
void Notify::close_notify() {
	emit closed();

	close();
}