#ifndef QGSRASTERITERATOR_H
#define QGSRASTERITERATOR_H

#include "qgsrectangle.h"
#include <QMap>

class QgsMapToPixel;
class QgsRasterInterface;
class QgsRasterProjector;
struct QgsRasterViewPort;

class CORE_EXPORT QgsRasterIterator
{
  public:
    //Stores information about reading of a raster band. Columns and rows are in unsampled coordinates
    struct RasterPartInfo
    {
      int currentCol;
      int currentRow;
      int nCols;
      int nRows;
      void* data; //data (can be in oversampled/undersampled resolution)
      QgsRasterProjector* prj; //raster projector (or 0 if no reprojection is done)
    };

    QgsRasterIterator( QgsRasterInterface* input );
    ~QgsRasterIterator();

    /**Start reading of raster band. Raster data can then be retrieved by calling readNextRasterPart until it returns false.
      @param bandNumer number of raster band to read
      @param viewPort describes raster position on screen
     */
    void startRasterRead( int bandNumber, int nCols, int nRows, const QgsRectangle& extent );

    /**Fetches next part of raster data
       @param nCols number of columns on output device
       @param nRows number of rows on output device
       @param nColsRaster number of raster columns (different to nCols if oversamplingX != 1.0)
       @param nRowsRaster number of raster rows (different to nRows if oversamplingY != 0)*/
    bool readNextRasterPart( int bandNumber,
                             int& nCols, int& nRows,
                             void** rasterData,
                             int& topLeftCol, int& topLeftRow );

    void stopRasterRead( int bandNumber );

    const QgsRasterInterface* input() const { return mInput; }

    void setMaximumTileWidth( int w ) { mMaximumTileWidth = w; }
    int maximumTileWidth() const { return mMaximumTileWidth; }

    void setMaximumTileHeight( int h ) { mMaximumTileHeight = h; }
    int maximumTileHeight() const { return mMaximumTileHeight; }

  private:
    QgsRasterInterface* mInput;
    QMap<int, RasterPartInfo> mRasterPartInfos;
    QgsRectangle mExtent;

    int mMaximumTileWidth;
    int mMaximumTileHeight;

    /**Remove part into and release memory*/
    void removePartInfo( int bandNumber );
};

#endif // QGSRASTERITERATOR_H