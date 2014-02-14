#ifndef CONSTANTS_H
#define CONSTANTS_H
#ifdef Q_OS_WIN
//int MIN_HEIGHT               = 47;
#define MIN_HEIGHT 47
#define MIN_WARNING_BUTTON_WIDTH  50
#define  MIN_COMBOBOX_WIDTH        50

#endif
#ifdef Q_OS_ANDROID
#define MIN_WARNING_BUTTON_WIDTH  50
#define MIN_COMBOBOX_WIDTH        100
#define MIN_HEIGHT                100
#endif
#endif // CONSTANTS_H
