#ifndef TRANSACTIONCONTROL_H
#define TRANSACTIONCONTROL_H

#include <QThread>

namespace composer {
namespace core {
namespace module {
    class TransactionControl : public QThread {
        public:
            TransactionControl();
        protected:
            void run();
    };
}
}
}
#endif // TRANSACTIONCONTROL_H
