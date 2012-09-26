#include "qstringlistmodelforqtcomponent.h"

QStringListModelForQtComponent::QStringListModelForQtComponent(const QByteArray &displayRoleName, QObject *parent)
    : QStringListModel(parent)
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::DisplayRole, displayRoleName);
    setRoleNames(roleNames);
}

void
QStringListModelForQtComponent::setStringList(const QStringList &strings)
{
    if (!stringList().isEmpty())
    {
        emit stringsReset();
    }
    QStringListModel::setStringList(strings);
    QStringList::const_iterator i = strings.constBegin();
    for (; i != strings.constEnd(); ++i) {
        QString s = *i;
        emit stringAdded(s);
    }
}
