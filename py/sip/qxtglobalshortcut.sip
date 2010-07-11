%ModuleCode
#include "qxtglobalshortcut.h"
%End

class QxtGlobalShortcut : QObject
{

%TypeHeaderCode
#include "qxtglobalshortcut.h"
%End

public:
	explicit QxtGlobalShortcut(QObject* parent /TransferBack/ = 0);
	explicit QxtGlobalShortcut(const QKeySequence & shortcut, QObject* parent /TransferBack/ = 0);
	virtual ~QxtGlobalShortcut();
	QKeySequence shortcut() const;
	bool setShortcut(const QKeySequence & shortcut);
	bool isEnabled() const;

public slots:
       void setEnabled(bool enabled = true);
       void setDisabled(bool disabled = true);

signals:
	void activated();

};
