#ifndef GUI_SETTINGS_HELPER_H
#define GUI_SETTINGS_HELPER_H

#include <QSettings>

template<class T>
class SettingsHelper {
	static_assert(std::is_base_of<QSettings, T>::value, "T should be based on a QSettings");
public:
	SettingsHelper(const QString& prefix) : settings(new QSettings()), prefix(prefix) {}
	SettingsHelper(T& settings, const QString& prefix = QString()) : settings(settings), prefix(prefix) {}
	SettingsHelper(SettingsHelper<T>& helper, const QString& prefix) : SettingsHelper(helper.settings, helper.prefix + prefix) {}
	virtual ~SettingsHelper() = default;

	QVariant operator()(const QString& name) const {
		return settings.value(prefix + name);
	}

protected:
	T& settings;
	const QString prefix;
};
typedef SettingsHelper<const QSettings> SettingsGetHelper;

class SettingsSetHelper : public SettingsHelper<QSettings> {
public:
	template<typename... Args>
	SettingsSetHelper(Args&& ...args) : SettingsHelper(std::forward<Args>(args)...) {}

	void operator()(const QString& name, const QVariant& value) {
		settings.setValue(prefix + name, value);
	}
};

#endif /* GUI_SETTINGS_HELPER_H */
