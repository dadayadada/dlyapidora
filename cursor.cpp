#include "cursor.h"

QCursor CustomCursor::createHandCursor()
{
    QPixmap pixmap("C:/Users/Derya/Downloads/cur-Photoroom (1).png");
    pixmap = pixmap.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    return QCursor(pixmap, 0, 5);
}


