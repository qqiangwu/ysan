#include <Main/ImageProvider.h>
#include <QFile>
#include <cassert>

namespace Fr = Bygone::Frontend;

constexpr auto default_person = 0;

Fr::ImageProvider::ImageProvider(const QString& root, QSize s)
    : personPath_(root + "/Person/%1.jpg")
    , eventPath_(root + "/Event/%1.jpg")
    , ratio_(s)
{
    //! load default person
    defaultPerson_ = loadPerson_(default_person);

    assert(!defaultPerson_.isNull());
}

const QPixmap& Fr::ImageProvider::person(int i)
{
    auto iter = persons_.constFind(i);
    if (iter == persons_.constEnd()) {
        if (!QFile::exists(personPath_.arg(i))) {
            return defaultPerson_;
        }

        iter = persons_.insert(i, loadPerson_(i));
    }

    return *iter;
}

const QPixmap& Fr::ImageProvider::event(const QString& i)
{
    auto iter = events_.constFind(i);
    if (iter == events_.constEnd()) {
        iter = events_.insert(i, loadEvent_(i));
    }

    return *iter;
}

QPixmap Fr::ImageProvider::loadPerson_(int i) { return QPixmap(personPath_.arg(i)).scaled(ratio_); }

QPixmap Fr::ImageProvider::loadEvent_(const QString& i) { return QPixmap(eventPath_.arg(i)).scaled(ratio_); }
