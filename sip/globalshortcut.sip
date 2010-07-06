
class QxtGlobalShortcut : QObject
{

%TypeHeaderCode
#include "qxtglobalshortcut.h"
%End

public:
	QxtGlobalShortcut(QObject* parent = 0);
	QxtGlobalShortcut(const QKeySequence& shortcut, QObject* parent = 0);
	~QxtGlobalShortcut();
	QKeySequence shortcut() const;
	bool setShortcut(const QKeySequence& shortcut);
	bool isEnabled() const;

public slots:
       void setEnabled(bool enabled = true);
       void setDisabled(bool disabled = true);

signals:
	void activated();


};