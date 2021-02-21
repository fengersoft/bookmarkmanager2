#include "sqlitedao.h"
SqliteDao* SqliteDao::m_instance = nullptr;
SqliteDao* SqliteDao::instance()
{
    if (m_instance == nullptr) {
        m_instance = new SqliteDao(nullptr);
    }
    return m_instance;
}

void SqliteDao::freeInstance()
{
    if (m_instance != nullptr) {
        delete m_instance;
    }
}

SqliteDao::SqliteDao(QObject* parent)
    : QObject(parent)
{

    m_sqliteWrapper = new SqliteWrapper(this);
    m_sqliteWrapper->setFileName(QApplication::applicationDirPath() + "/data/data.db");
    m_sqliteWrapper->setDbName("data");
    m_sqliteWrapper->open();
}

SqliteWrapper* SqliteDao::sqliteWrapper() const
{
    return m_sqliteWrapper;
}

SqliteDao* sqliteDao()
{
    return SqliteDao::instance();
}
