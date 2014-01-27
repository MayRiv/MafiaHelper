#ifndef CONSTANTS_H
#define CONSTANTS_H
#ifdef Q_OS_WIN
int MIN_HEIGHT               = 50;
int MIN_WARNING_BUTTON_WIDTH = 50;
int MIN_COMBOBOX_WIDTH       = 50;

#endif
#ifdef Q_OS_ANDROID
int MIN_WARNING_BUTTON_WIDTH = 50;
int MIN_COMBOBOX_WIDTH       = 100;
int MIN_HEIGHT               = 100;
#endif
#endif // CONSTANTS_H
