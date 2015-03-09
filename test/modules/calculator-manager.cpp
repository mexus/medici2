#include "calculator-manager.h"
#include <calculator/manager.h>
#include "operators.h"

logxx::Log TestCalculatorManager::cLog("TestCalculatorManager");

using namespace calculator;

TestCalculatorManager::TestCalculatorManager() : TestFW("calculator-manager") {
}

bool TestCalculatorManager::Tests() {
	return TestLaunch() && TestIncrease() && TestDecrease() && TestInterrupt();
}

bool TestCalculatorManager::TestLaunch() {
	S_LOG("TestLaunch");
	DeckSelectors selectors;
	Thread::StandardMixer mixer;

	Manager manager;
	static const std::size_t testThreadsCount = 2;
	manager.Launch(testThreadsCount, std::move(selectors), mixer);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	auto allParams = manager.GetRunParameters();
	if (allParams.size() != testThreadsCount) {
		log(logxx::error) << "There should be " << testThreadsCount << " threads, but only " << allParams.size() << " detected" << logxx::endl;
		return false;
	}

	for (auto& params : allParams) {
		log(logxx::debug) << "Params: " << params << logxx::endl;
		if (params.checkedDecks == 0 || params.suitableDecks == 0)
			return false;
	}

	return true;
}

bool TestCalculatorManager::TestIncrease() {
	S_LOG("TestIncrease");
	DeckSelectors selectors;
	Thread::StandardMixer mixer;

	Manager manager;
	static const std::size_t testThreadsCount = 2;
	manager.Launch(testThreadsCount, std::move(selectors), mixer);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	manager.IncreaseThreads();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	auto allParams = manager.GetRunParameters();
	if (allParams.size() != testThreadsCount + 1) {
		log(logxx::error) << "There should be " << testThreadsCount + 1 << " threads, but only " << allParams.size() << " detected" << logxx::endl;
		return false;
	}

	for (auto& params : allParams) {
		log(logxx::debug) << "Params: " << params << logxx::endl;
		if (params.checkedDecks == 0 || params.suitableDecks == 0)
			return false;
	}

	return true;
}

bool TestCalculatorManager::TestDecrease() {
	S_LOG("TestDecrease");
	DeckSelectors selectors;
	Thread::StandardMixer mixer;

	Manager manager;
	static const std::size_t testThreadsCount = 2;
	manager.Launch(testThreadsCount, std::move(selectors), mixer);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	manager.DecreaseThreads();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	auto allParams = manager.GetRunParameters();
	if (allParams.size() != testThreadsCount - 1) {
		log(logxx::error) << "There should be " << testThreadsCount - 1 << " threads, but only " << allParams.size() << " detected" << logxx::endl;
		return false;
	}

	for (auto& params : allParams) {
		log(logxx::debug) << "Params: " << params << logxx::endl;
		if (params.checkedDecks == 0 || params.suitableDecks == 0)
			return false;
	}

	return true;
}

bool TestCalculatorManager::TestInterrupt() {
	S_LOG("TestInterrupt");
	DeckSelectors selectors;
	Thread::StandardMixer mixer;

	Manager manager;
	static const std::size_t testThreadsCount = 2;
	manager.Launch(testThreadsCount, std::move(selectors), mixer);

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	manager.Interrupt();

	std::this_thread::sleep_for(std::chrono::milliseconds(500));

	auto allParams = manager.GetRunParameters();
	if (!allParams.empty()) {
		log(logxx::error) << "There should be no threads, but " << allParams.size() << " detected" << logxx::endl;
		return false;
	}
	return true;
}

