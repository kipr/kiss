#include "target_response.hpp"

using namespace Kiss::Target;

Response::Response()
{
	
}

Response::Response(quint64 id, const QString &type, const QVariant &data)
	: m_id(id),
	m_type(type),
	m_data(data)
{
}

const quint64 &Response::id() const
{
	return m_id;
}

const QString &Response::type() const
{
	return m_type;
}

const QVariant &Response::data() const
{
	return m_data;
}