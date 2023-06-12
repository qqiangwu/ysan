#ifndef BYGONE_FRONTEND_BYWORKER_H_
#define BYGONE_FRONTEND_BYWORKER_H_

#include <QtCore/QThread>
#include <exception>
#include <string>

namespace Bygone {
class ByWorker : public QThread {
    Q_OBJECT

public:
    ByWorker(const std::string& config, const int leaderId, QObject* parent);

signals:
    void errorCatched(std::exception_ptr p);

    void scenarioLoaded();
    void gameInited();

private:
    virtual void run() override;

private:
    void initGame();

private:
    const std::string config_;
    const int leaderId_;
};
} // of namespace Bygone

#endif //! BYGONE_FRONTEND_BYWORKER_H_
