#include "account-model.h"
#include "provider-helper.h"

//libaccounts-qt
#include <Accounts/Account>
#include <Accounts/Provider>

//libaccounts-ui
#include <AccountsUI/AccountsManagerSingleton>

class AccountModel::AccountModelPrivate
{
public:
    ~AccountModelPrivate()
    {
        qDeleteAll(accountsList);
    }

    QHash<int, QByteArray> headerData;
    Accounts::Manager *manager;
    QList<DisplayData *> accountsList;
};

AccountModel::AccountModel(QObject* parent)
    : QAbstractListModel(parent)
    , d_ptr(new AccountModelPrivate())
{
    Q_D(AccountModel);
    d->manager = AccountsUI::AccountsManager::instance();
    d->headerData.insert(AccountNameRole, "accountName");
    d->headerData.insert(AccountIconRole, "accountIcon" );
    d->headerData.insert(ProviderNameRole, "providerName");
    d->headerData.insert(ColumnCountRole, "columncount");
    setRoleNames(d->headerData);
    Accounts::AccountIdList idList = d->manager->accountList();
    foreach (Accounts::AccountId id, idList)
    {
        Accounts::Account *account = d->manager->account(id);
        d->accountsList.append(new DisplayData(account));
    }
}

AccountModel::~AccountModel()
{
}

int AccountModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const AccountModel);
    if (parent.isValid())
        return 0;
    return d->accountsList.length();
}

int AccountModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return ColumnCount;
}

QVariant AccountModel::data(const QModelIndex &index, int role) const
{
    Q_D(const AccountModel);
    if (!index.isValid())
        return QVariant();

    DisplayData *data = d->accountsList[index.row()];
    Accounts::Account *account = data->account;

    if (role == AccountNameRole ||
            (role == Qt::DisplayRole && index.column() == AccountNameColumn))
        return QVariant::fromValue(account->displayName());

    if (role == AccountIconRole ||
            (role == Qt::DisplayRole && index.column() == AccountIconColumn)) {
        if (data->accountIcon.isNull()) {
            Accounts::Provider provider =
                d->manager->provider(account->providerName());
            ProviderHelper helper(provider);
            data->accountIcon = helper.iconName();
        }
        return QVariant::fromValue(data->accountIcon);
    }

    if (role == ProviderNameRole ||
            (role == Qt::DisplayRole && index.column() == ProviderNameColumn)) {
        Accounts::Provider provider = d->manager->provider(account->providerName());
        if (provider.isValid()) {
            ProviderHelper helper(provider);
            QString providerName = helper.providerName();
            data->providerName = providerName;
        }
        return QVariant::fromValue(data->providerName);
    }
    return QVariant();
}

QVariant AccountModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    Q_D(const AccountModel);
    if (orientation != Qt::Horizontal) {
        return QVariant();
    }
    Columns column = static_cast<Columns>(section);

    if (role == Qt::DisplayRole) {
        if (section < d->headerData.size()) {
            return d->headerData.value(column);
        }
    }
    return QVariant();
}
