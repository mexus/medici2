#ifndef GUI_CALCULATOR_WINDOW
#define GUI_CALCULATOR_WINDOW

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QTextEdit>
#include <atomic>
#include <calculator/manager.h>
#include "progress_widget.h"
#include "cards_translations.h"

class CalculatorWindow : public QDialog {
    Q_OBJECT
public:
    CalculatorWindow(QWidget* parent = 0);
    ~CalculatorWindow();
    void Calculate(DeckSelectors&&);

protected:
    virtual void closeEvent(QCloseEvent*) override;

private:
    std::atomic_bool operationInProgress;
    calculator::Manager calculatorManager;
    CardsTranslations cardsTranslations;

    std::uint8_t threadsCount;
    QHBoxLayout *layout;
    QPushButton *interruptButton, *addThread, *removeThread;
    QVBoxLayout *progressBoxes;
    std::vector<ProgressWidget*> progressVector;
    QTextEdit *foundDecks;

    void InterruptCalculation();
    void AddThread();
    void RemoveThread();

    void DisableButtons(bool disabled);

    template<class T, class ...Args>
    void DefferedCommand(T t, Args&&... args);

    QTimer *updateProgressTimer;
    void ShowProgress();
    void PopulateParameters(const std::vector<calculator::Thread::RunParameters>&);
    void PopulateDecks(const calculator::Thread::FoundVector&);
    void AddDeck(const calculator::Thread::StandardDeck&, const medici::Patience::PatienceInfo&);

};

#endif /* GUI_CALCULATOR_WINDOW */
