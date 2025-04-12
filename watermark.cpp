#include "watermark.h"
#include <QMouseEvent>
#include <QPixmap>
#include <QBitmap>
#ifdef Q_OS_WIN
#include <windows.h>
#else
#include <QX11Info>
#include <X11/Xlib.h>
#endif


WaterMark::WaterMark(QWidget *parent)
    : QLabel(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::WindowStaysOnTopHint |
                   Qt::ToolTip |  // Используем ToolTip вместо Tool
                   Qt::X11BypassWindowManagerHint |
                   Qt::WindowDoesNotAcceptFocus);

    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_ShowWithoutActivating);
    setAttribute(Qt::WA_X11DoNotAcceptFocus);
}

WaterMark::~WaterMark() {}

void WaterMark::initialize(const QString &path, double scale, int alpha)
{
    QPixmap pixmap(path);
    pixmap = pixmap.scaled(pixmap.width()/scale, pixmap.height()/scale);
    setPixmap(pixmap);
    setMask(pixmap.mask());
    adjustSize();

    setWindowOpacity(alpha/100.0);
    // setMouseTracking(true);
}

void WaterMark::preventFocus()
{
#ifdef Q_OS_WIN
    HWND hwnd = (HWND)winId();
    LONG_PTR style = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
    SetWindowLongPtr(hwnd, GWL_EXSTYLE, style | WS_EX_NOACTIVATE);
#else
    if (QX11Info::isPlatformX11()) {
        Display* display = QX11Info::display();
        Window window = winId();

        // Устанавливаем атрибуты окна X11
        XWMHints hints;
        hints.flags = InputHint;
        hints.input = False;  // Окно не будет принимать фокус ввода

        XSetWMHints(display, window, &hints);

        // Дополнительные настройки для некоторых оконных менеджеров
        Atom net_wm_state_skip_taskbar = XInternAtom(display, "_NET_WM_STATE_SKIP_TASKBAR", False);
        Atom net_wm_state_skip_pager = XInternAtom(display, "_NET_WM_STATE_SKIP_PAGER", False);

        XChangeProperty(display, window,
                        XInternAtom(display, "_NET_WM_STATE", False),
                        XA_ATOM, 32, PropModeAppend,
                        (unsigned char *)&net_wm_state_skip_taskbar, 1);

        XChangeProperty(display, window,
                        XInternAtom(display, "_NET_WM_STATE", False),
                        XA_ATOM, 32, PropModeAppend,
                        (unsigned char *)&net_wm_state_skip_pager, 1);
    }
#endif
}

void WaterMark::mousePressEvent(QMouseEvent *event) {
    event->ignore();
}

void WaterMark::mouseMoveEvent(QMouseEvent *event) {
    event->ignore();
}

void WaterMark::mouseReleaseEvent(QMouseEvent *event) {
    event->ignore();
}

void WaterMark::closeEvent(QCloseEvent *event)
{
    event->ignore();
}

