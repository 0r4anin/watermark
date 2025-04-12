#ifndef WATERMARK_H
#define WATERMARK_H

#include <QLabel>

#include <QApplication>
#include <QLabel>
#include <QPainter>
#include <QPixmap>
#include <QScreen>
#include <QWindow>

class WaterMark : public QLabel
{
    Q_OBJECT

public:
    WaterMark( QWidget *parent = nullptr);
    ~WaterMark();
    void initialize(const QString &path, double scale, int alpha);
    void preventFocus();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void closeEvent(QCloseEvent *event) override;
};
#endif // WATERMARK_H
