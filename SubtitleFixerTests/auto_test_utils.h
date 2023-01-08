#ifndef AUTO_TEST_UTILS_H
#define AUTO_TEST_UTILS_H

#include <QDebug>
#include <QString>

inline void noMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg){ }

#endif // AUTO_TEST_UTILS_H
