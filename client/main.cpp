#include "CommonDefinitions.h"
#include "Client.h"

#define LOG_TAG "MAIN"
#include "Logger.h"

#include <signal.h>
#include <unistd.h>
#include <QCoreApplication>

static Client sClient(QT_SOCKET);

void sigHandler(int sig)
{
    switch (sig)
    {
        case SIGINT:
            sClient.disconnect();
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

    if (!sClient.connect(desc))
    {
        LOG_ERR << "Unable to connect to server" << std::endl;
        return 1;
    }

    return app.exec();
}
