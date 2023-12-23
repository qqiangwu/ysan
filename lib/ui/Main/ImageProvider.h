#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QMap>
#include <QPixmap>
#include <QString>

namespace Bygone {
namespace Frontend {
    class ImageProvider {
    public:
        ImageProvider(const QString& root, QSize size);

    public:
        const QPixmap& person(int i);
        const QPixmap& event(const QString& i);

    private:
        QPixmap loadPerson_(int i);
        QPixmap loadEvent_(const QString& i);

    private:
        QString personPath_;
        QString eventPath_;
        QSize ratio_;
        QPixmap defaultPerson_;
        QMap<int, QPixmap> persons_;
        QMap<QString, QPixmap> events_;
    };
}
}

#endif // IMAGEPROVIDER_H
