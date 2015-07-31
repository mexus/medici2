#ifndef GUI_WINDOWS_CALCULATOR_WINDOW
#define GUI_WINDOWS_CALCULATOR_WINDOW

#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>
#include <QListWidget>
#include <atomic>
#include <calculator/manager.h>
#include <widgets/progress_widget.h>
#include <helpers/cards_translations.h>

class CalculatorWindow : public QDialog {
    Q_OBJECT
public:
    CalculatorWindow(const CardsTranslations &, QWidget *parent = 0);
    ~CalculatorWindow();
    void Calculate(DeckSelectors &&, medici::PPatienceSelector &&);

protected:
    virtual void closeEvent(QCloseEvent *) override;

private:
    static constexpr std::size_t N = 36;
    void GenerateSeeds();
    void CreateObjects();
    void CreateLayout();

    MixersFactory mixersFactory;
    std::atomic_bool operationInProgress;
    calculator::Manager calculatorManager;
    const CardsTranslations &cardsTranslations;

    std::uint8_t threadsCount;
    QHBoxLayout *layout;
    QPushButton *interruptButton, *addThread, *removeThread;
    QVBoxLayout *progressBoxes;
    std::vector<ProgressWidget *> progressVector;
    QListWidget *foundDecks;

    void InterruptCalculation();
    void AddThread();
    void RemoveThread();

    void DisableButtons(bool disabled);

    template <class T, class... Args>
    void DefferedCommand(T t, Args &&... args);

    QTimer *updateProgressTimer;
    void ShowProgress();
    void PopulateParameters(const std::vector<calculator::ExecutionParameters> &);
    void PopulateDecks(const calculator::Manager::FoundVector &);
    void AddDeck(const std::vector<Card> &, const medici::PatienceInfo &);
};

#endif /* GUI_WINDOWS_CALCULATOR_WINDOW */
