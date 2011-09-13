#ifndef QNCG_H
#define QNCG_H

class Qncg
{
public:
    enum EntityType {
        Node,
        Edge,
        Interface
    };

    enum ResizeType {
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft
    };
};

#endif // QNCG_H
