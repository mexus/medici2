#include "progress_widget.h"

#include <QGridLayout>

ProgressWidget::ProgressWidget(std::uint8_t threadNumber, QWidget* parent)
        : QGroupBox(tr("Thread #") + QString::number(threadNumber), parent) {
    CreateObjects();
    CreateLayout();
}

void ProgressWidget::CreateObjects() {
    totalDecks = new QLabel("0");
    suitableDecks = new QLabel("0");
    runningTime = new QLabel("0");
}

void ProgressWidget::CreateLayout() {
    auto layout = new QGridLayout();
    layout->addWidget(new QLabel(tr("Total decks:")), 0, 0);
    layout->addWidget(totalDecks, 0, 1);

    layout->addWidget(new QLabel(tr("Suitable decks:")), 1, 0);
    layout->addWidget(suitableDecks, 1, 1);

    layout->addWidget(new QLabel(tr("Running time:")), 2, 0);
    layout->addWidget(runningTime, 2, 1);

    layout->setSizeConstraint(QLayout::SetFixedSize);
    setLayout(layout);
}

void ProgressWidget::Set(const calculator::ExecutionParameters& params) {
    using namespace std::chrono;
    totalDecks->setText(QString::number(params.checkedDecks));
    suitableDecks->setText(QString::number(params.suitableDecks));
    auto seconds = duration_cast<duration<float>>(params.runningTime).count();
    ;
    runningTime->setText(QString::number(seconds) + tr("s.", "seconds abbrevation"));
}
