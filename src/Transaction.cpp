#include "Transaction.h"

using namespace TransactionType;

Transaction::Transaction(const QString& name, int type) : m_name(name), m_type(type) {}

const QString& Transaction::name() const { return m_name; }
const int Transaction::type() const { return m_type; }

const QVariantList& Transaction::data() const { return m_data; }

QString Transaction::successMessage() const { return m_name + ": success!"; }
QString Transaction::failureMessage() const { return m_name + ": FAILURE!"; }

Unknown::Unknown() : Transaction("Unknown", Transaction::Unknown) {}

CreateProject::CreateProject(const QString& name) : Transaction(name + " (create project)", Transaction::CreateProject)
{
	m_data = QVariantList() << QVariant(name);
}

MakeDirectory::MakeDirectory(const QString& path) : Transaction(path + " (mkdir)", Transaction::MakeDirectory)
{
	m_data = QVariantList() << QVariant(path);
}

SendToDirectory::SendToDirectory(const QString& path, const QString& directory)
	 : Transaction(path + " -> " + directory, Transaction::SendToDirectory)
{
	m_data = QVariantList() << QVariant(path) << directory;
}

CompileFile::CompileFile(const QString& path, const QString& options)
	: Transaction(path + " (compile with options:" + options + ")", Transaction::CompileFile)
{
	
}

RunFile::RunFile(const QString& path, const QString& options)
	: Transaction(path + " (run with options:" + options + ")", Transaction::RunFile)
{
	
}
