#include "progress.h"

#include <QGridLayout>

Progress::Progress(std::uint8_t threadNumber, QWidget *parent) : QGroupBox(tr("Thread #") + QString::number(threadNumber), parent)
{
    auto layout = new QGridLayout();
    layout->addWidget(new QLabel(tr("Total decks:")),      0, 0);
    totalDecks = new QLabel("0");
    layout->addWidget(totalDecks,                          0, 1);

    layout->addWidget(new QLabel(tr("Suitable decks:")), 1, 0);
    suitableDecks = new QLabel("0");
    layout->addWidget(suitableDecks,                     1, 1);

    layout->addWidget(new QLabel(tr("Running time:")),     2, 0);
    runningTime = new QLabel("0");
    layout->addWidget(runningTime,                         2, 1);

    setLayout(layout);
}

void Progress::Set(const calculator::Thread::RunParameters& params) {
    totalDecks->setText(QString::number(params.checkedDecks));
    suitableDecks->setText(QString::number(params.suitableDecks));
    auto seconds = std::chrono::duration_cast<std::chrono::duration<float>>(params.runningTime).count();;
    runningTime->setText(QString::number(seconds) + tr("s.", "seconds abbrevation"));
}

