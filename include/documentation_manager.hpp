#ifndef _DOCUMENTATION_MANAGER_HPP_
#define _DOCUMENTATION_MANAGER_HPP_

#include "singleton.hpp"

#include <QString>
#include <QList>
#include <QMap>

namespace kiss
{
    class DocumentationSource
    {
    public:
        DocumentationSource(const QString &path);
        
        QMap<QString, QString> metadata() const;
        const QString &path() const;
        
        bool operator ==(const DocumentationSource &rhs) const;
        
    private:
        void lazyInitMetadata() const;
        
        QString _path;
        mutable QMap<QString, QString> _metadata;
    };

    class DocumentationManager : public Singleton<DocumentationManager>
    {
    public:
        DocumentationManager();

        void addDocumentationSource(const DocumentationSource &documentationSource);
        void removeDocumentationSource(const DocumentationSource &documentationSource);
        const QList<DocumentationSource> &documentationSources() const;

    private:
        static QString documentationPath();
        void loadDefaultDocumentationSources();

        QList<DocumentationSource> _documentationSources;
    };
}

#endif
