#ifndef BYGONE_UI_SUPPORT_UTILITY_H_
#define BYGONE_UI_SUPPORT_UTILITY_H_

#include <QtGui/QImage>
#include <QtWidgets/QWidget>

#include <string>

namespace Bygone {
inline void setBackground(QWidget* w, const std::string& path)
{
    auto pal = w->palette();

    pal.setBrush(w->backgroundRole(), QBrush(QImage(path.c_str())));

    w->setPalette(pal);
    w->setAutoFillBackground(true);
}
} // of namespace Bygone

#endif //! BYGONE_UI_SUPPORT_UTILITY_H_
