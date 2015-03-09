#ifndef GUI_PROGRESS_H
#define GUI_PROGRESS_H

#include <QGroupBox>
#include <QLabel>

#include <calculator/thread.h>

class Progress : public QGroupBox {
	Q_OBJECT
public:
	Progress(std::uint8_t threadNumber, QWidget *parent = 0);
	void Set(const calculator::Thread::RunParameters&);
private:
	QLabel *totalDecks, *suitableDecks, *runningTime;
};

#endif /* GUI_PROGRESS_H */
