#ifndef NCLREGION_H
#define NCLREGION_H


#include <QRect>
#include <QWidget>


class NCLRegion : public QRect
{
public:
    NCLRegion (QString  id, NCLRegion *parent = 0);
    NCLRegion (QString  id, int width, int height, NCLRegion *parent = 0);

    NCLRegion * getChildrenWhithPoint (QPoint point);
    NCLRegion * getParent () const { return parent; }

    void setParent (NCLRegion * parent);
    void setChild (NCLRegion *child) { children.append (child); }
    void notifyChildren (int desX, int desY, bool flag);
    void modify (int desX, int desY, bool flag);
    void setId (QString Id) { this->Id = Id; }
    void setRelativeWidth (int limit);
    void setRelativeHeight (int limit);
    void setRelativeTop (int limit);
    void setRelativeLeft (int limit);
    void adjustChildren ();
    void autoAdjust ();
    void clearChildrenList ();
    void removeChild (NCLRegion * region) { children.removeAll (region); }


    QString getId () const { return Id; }
    QString title () const;
    QList <NCLRegion*> getChildren () const { return children; }

    int zIndex () const;
    double rightAdjusted () const;
    double bottomAdjusted () const;
    double topAdjusted () const;
    double leftAdjusted () const;
    double widthAdjusted () const;
    double heightAdjusted () const;
    double getRelativeWidth () const { return relativeWidth; }
    double getRelativeHeight () const { return relativeHeight; }
    double getRelativeTop () const { return relativeTop; }
    double getRelativeLeft () const { return relativeLeft; }



private:
    QRect scrennResolution;
    QString Id;
    QString Title;
    int ZIndex;
    double Top;
    double Left;
    double Height;
    double Width;
    int Right;
    int Bottom;
    int oldLeft;
    int oldTop;
    int oldWidth;
    int oldHeight;
    NCLRegion *parent;
    QList <NCLRegion *> children;

    int canvasWidth;
    int canvasHeight;

    int relativeTop;
    int relativeLeft;
    int relativeWidth;
    int relativeHeight;
};

#endif // NCLREGION_H
