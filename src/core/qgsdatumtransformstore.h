/***************************************************************************
    qgsdatumtransformstore.h
    ---------------------
    begin                : June 2014
    copyright            : (C) 2014 by Martin Dobias
    email                : wonder dot sk at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
#ifndef QGSDATUMTRANSFORMSTORE_H
#define QGSDATUMTRANSFORMSTORE_H

#include "qgis_core.h"
#include "qgscoordinatereferencesystem.h"

class QgsCoordinateTransform;
class QgsMapLayer;

class QDomElement;


/** \ingroup core
 * @brief The QgsDatumTransformStore class keeps track of datum transformations
 * as chosen by the user.
 *
 * @note added in 2.4
 */
class CORE_EXPORT QgsDatumTransformStore
{
  public:
    explicit QgsDatumTransformStore( const QgsCoordinateReferenceSystem& destCrs );

    void clear();

    void setDestinationCrs( const QgsCoordinateReferenceSystem& destCrs );

    void addEntry( const QString& layerId, const QString& srcAuthId, const QString& destAuthId, int srcDatumTransform, int destDatumTransform );

    bool hasEntryForLayer( QgsMapLayer* layer ) const;

    /** Will return transform from layer's CRS to current destination CRS.
     *  Will emit datumTransformInfoRequested signal if the layer has no entry.
     *  @returns transformation associated with layer, or an invalid QgsCoordinateTransform
     *  if no transform is associated with the layer
     */
    QgsCoordinateTransform transformation( QgsMapLayer* layer ) const;

    void readXml( const QDomNode& parentNode );

    void writeXml( QDomNode& parentNode, QDomDocument& theDoc ) const;

    struct CORE_EXPORT Entry
    {
      QString srcAuthId;
      QString destAuthId;
      int srcDatumTransform; //-1 if unknown or not specified
      int destDatumTransform;
    };

  protected:
    QgsCoordinateReferenceSystem mDestCRS;

    //! key = layer ID
    QHash< QString, Entry > mEntries;
};

#endif // QGSDATUMTRANSFORMSTORE_H
