#include "CommonDefinitions.h"
#include "Server.h"

#define LOG_TAG "MAIN"
#include "Logger.h"

#include <signal.h>
#include <unistd.h>
#include <QCoreApplication>

static Server sServer(QT_SOCKET);

void sigHandler(int sig)
{
    switch (sig) {
        case SIGINT:
            sServer.stop();
            exit(0);
            break;
        default:
            LOG_INFO << "Unsupported signal catched" << std::endl;
    }
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    signal(SIGINT, sigHandler);

    ServiceDescriptor desc;
    desc.serviceAddress = "127.0.0.1";
    desc.servicePort = 44332;

    if (!sServer.start(desc)) {
        LOG_ERR << "Unable to start server" << std::endl;
        return 1;
    }

    return app.exec();
}
