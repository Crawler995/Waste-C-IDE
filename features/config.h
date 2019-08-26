#ifndef CONFIG_H
#define CONFIG_H

#include <QApplication>
#include <QDesktopWidget>

#define SCREEN_WIDTH QApplication::desktop()->screenGeometry().width()
#define SCREEN_HEIGHT QApplication::desktop()->screenGeometry().height()

#endif // CONFIG_H
