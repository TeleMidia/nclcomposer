#ifndef QNCG_H
#define QNCG_H

class Qncg
{
public:
    enum EntityType {
        Node,
        Edge,
        Interface,

        NoType
    };

    enum ResizeType {
        Top,
        TopRight,
        Right,
        BottomRight,
        Bottom,
        BottomLeft,
        Left,
        TopLeft,

        NoResizeType
    };
};

#endif // QNCG_H
